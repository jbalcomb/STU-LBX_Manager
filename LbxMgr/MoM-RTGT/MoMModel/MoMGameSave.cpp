// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <ctype.h>      // tolower()
#include <fstream>
#include <sstream>
#include <string>
#include <string.h>     // memset(), strncpy(), strrchr()

#include "MoMController.h"
#include "MoMGenerated.h"
#include "MoMLbxBase.h"
#include "MoMUtility.h"
#include "MoMExeWizards.h"

#include "MoMGameSave.h"

namespace MoM {

namespace {

const std::string gLowercaseMagicPrefix = "magic";
const std::string gLowercaseWizardsPrefix = "wizards";

std::string lowercase_filetitle(const std::string& filename)
{
    std::string filetitle;
    size_t pos = filename.find_last_of("/\\");
    if (std::string::npos != pos)
    {
        filetitle = filename.substr(pos + 1);
    }
    else
    {
        filetitle = filename;
    }

    return lowercase(filetitle);
}

std::string lowercase_extension(const std::string& filename)
{
    std::string lwr_filetitle = lowercase_filetitle(filename);
    std::string ext;
    size_t pos = lwr_filetitle.find_last_of('.');
    if (std::string::npos != pos)
    {
        ext = lwr_filetitle.substr(pos);
    }
    return ext;
}

}

bool MoMGameSave::commitData(void* ptr, const void* pNewValue, size_t size)
{
    setErrorString("");
    // TODO: Check memory pointer + range
    memcpy(ptr, pNewValue, size);
    return true;
}

std::string MoMGameSave::getGameDirectory() const
{
    return m_GameDirectory;
}

std::string MoMGameSave::getSources() const
{
    std::string sources;
    if (!m_GameDirectory.empty())
    {
        sources += m_GameDirectory + " - ";
    }
    if (!m_filename_SaveGame.empty())
    {
        sources += " " + m_filename_SaveGame;
        if (sizeof(SaveGame) == m_size_SaveGame)
        {
            sources += " (MoM)";
        }
        else if (sizeof(SaveGame_CasterOfMagic) == m_size_SaveGame)
        {
            sources += " (CoM)";
        }
        else if (m_size_SaveGame < sizeof(SaveGame_CasterOfMagic))
        {
            sources += " (CoM-)";
        }
        else if (m_size_SaveGame > sizeof(SaveGame_CasterOfMagic))
        {
            sources += " (CoM+)";
        }
    }
    if (!m_filename_MagicExe.empty())
    {
        sources += " " + m_filename_MagicExe;
    }
    if (!m_filename_WizardsExe.empty())
    {
        sources += " " + m_filename_WizardsExe;
    }
    if (0 != m_BuilddatLbx.get())
    {
        sources += " " + m_BuilddatLbx->getFilename();
    }
    if (0 != m_SpelldatLbx.get())
    {
        sources += " " + m_SpelldatLbx->getFilename();
    }
    if (0 != m_TerrstatLbx.get())
    {
        sources += " " + m_TerrstatLbx->getFilename();
    }
    if (sources.empty())
    {
        sources = "MASTER OF MAGIC - Real-Time Game Tweaker";
    }
    return sources;
}

uint8_t* MoMGameSave::getMagicOverlay(size_t ovlNr)
{
    uint8_t* ptr = 0;

    if (0 != m_MagicExe.get())
    {
        ptr = m_MagicExe->getOverlay(ovlNr);
    }

    return ptr;
}

uint8_t* MoMGameSave::getWizardsOverlay(size_t ovlNr)
{
    uint8_t* ptr = 0;

    if (0 != m_WizardsExe.get())
    {
        ptr = m_WizardsExe->getOverlay(ovlNr);
    }

    return ptr;
}

bool MoMGameSave::load(const char* filename)
{
    assert(0 != filename);
    setErrorString("");
    bool ok = true;

    std::string lower_filetitle = lowercase_filetitle(filename);
    std::string ext = lowercase_extension(filename);
    if (".gam" == ext)
    {
        std::ifstream ifs(filename, std::ios::in | std::ios::binary);
        if (!ifs)
        {
            setErrorString("Could not open file '" + toStr(filename) + "' for reading");
            std::cout << errorString() << std::endl;
            return false;
        }

        ifs.seekg(0, std::ios::end);
        m_size_SaveGame = ifs.tellg();
        ifs.seekg(0, std::ios::beg);
        std::cout << "Size save file=" << m_size_SaveGame << std::endl;
        std::cout << "sizeof(SaveGame)=" << sizeof(SaveGame) << std::endl;
        std::cout << "sizeof(SaveGame_CasterOfMagic)=" << sizeof(SaveGame_CasterOfMagic) << std::endl;

        if (m_size_SaveGame < sizeof(SaveGame))
        {
            setErrorString("Save game is too small in file '" + toStr(filename) + "'. Not loaded");
            std::cout << errorString() << std::endl;
            return false;
        }

        size_t sizeBuffer = m_size_SaveGame;
        if (m_size_SaveGame > sizeof(SaveGame_CasterOfMagic))
        {
            std::cout << "Treating savefile as CasterOfMagic although file is too big" << std::endl;
        }
        else if (m_size_SaveGame == sizeof(SaveGame_CasterOfMagic))
        {
            std::cout << "Treating savefile as CasterOfMagic" << std::endl;
        }
        else if (m_size_SaveGame > sizeof(SaveGame))
        {
            std::cout << "Treating savefile as Vanilla although file is too big" << std::endl;
        }
        else
        {
            std::cout << "Treating savefile as vanilla" << std::endl;
        }

        m_SaveGame = std::vector<uint8_t>(sizeBuffer, 0);
        if (!ifs.read((char*)m_SaveGame.data(), m_size_SaveGame))
        {
            setErrorString("Could not read " + toStr(m_size_SaveGame) + " bytes from file '"
                           + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }
        m_filename_SaveGame = filename;
    }
    else if ("builddat.lbx" == lower_filetitle)
    {
        m_BuilddatLbx.reset(new MoMLbxBase);
        ok = loadLbx(filename, m_BuilddatLbx.get());
    }
    else if ("itemdata.lbx" == lower_filetitle)
    {
        m_ItemDataLbx.reset(new MoMLbxBase);
        ok = loadLbx(filename, m_ItemDataLbx.get());
    }
    else if ("itempow.lbx" == lower_filetitle)
    {
        m_ItemPowLbx.reset(new MoMLbxBase);
        ok = loadLbx(filename, m_ItemPowLbx.get());
    }
    else if ("spelldat.lbx" == lower_filetitle)
    {
        m_SpelldatLbx.reset(new MoMLbxBase);
        ok = loadLbx(filename, m_SpelldatLbx.get());
    }
    else if ("terrstat.lbx" == lower_filetitle)
    {
        m_TerrstatLbx.reset(new MoMLbxBase);
        ok = loadLbx(filename, m_TerrstatLbx.get());
    }
    else if ((lower_filetitle.substr(0, gLowercaseMagicPrefix.size()) == gLowercaseMagicPrefix) && (".exe" == ext))
    {
        m_MagicExe.reset(new MoMExeMagic);
        if (!m_MagicExe->load(filename))
        {
            setErrorString("Could not (fully) read file '" + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }

        m_filename_MagicExe = filename;
    }
    else if ((lower_filetitle.substr(0, gLowercaseWizardsPrefix.size()) == gLowercaseWizardsPrefix) && (".exe" == ext))
    {
        m_WizardsExe.reset(new MoMExeWizards);
        if (!m_WizardsExe->load(filename))
        {
            setErrorString("Could not (fully) read file '" + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }

        m_filename_WizardsExe = filename;
    }
    else
    {
        setErrorString("Filename pattern unrecognized of '" + toStr(filename) + "'");
        std::cout << errorString() << std::endl;
        return false;
    }

    // Use last succesfully loaded file to identify the game directory
    m_GameDirectory = dirFromFilepath(filename);

    return ok;
}

bool MoMGameSave::loadLbx(const std::string &filename, MoMLbxBase *lbxBase)
{
    assert(0 != lbxBase);
    lbxBase->close();
    bool ok = lbxBase->load(filename);
    if (!ok)
    {
        setErrorString("Could not (fully) read file '" + toStr(filename) + "'");
        std::cout << errorString() << std::endl;
    }
    return ok;
}

bool MoMGameSave::save(const char* filename)
{
    assert(0 != filename);
    setErrorString("");
    bool ok = true;

    std::string lower_filetitle = lowercase_filetitle(filename);
    std::string ext = lowercase_extension(filename);
    if (".gam" == ext)
    {
        if (0 == m_size_SaveGame)
        {
            setErrorString("Cannot save to '" + toStr(filename) + "' because no SAVEn.GAM was loaded");
            std::cout << errorString() << std::endl;
            return false;
        }

        std::ofstream ofs(filename, std::ios_base::out | std::ios_base::binary);
        if (!ofs)
        {
            setErrorString("Could not open file '" + toStr(filename) + "' for writing");
            std::cout << errorString() << std::endl;
            return false;
        }

        if (!ofs.write((const char*)m_SaveGame.data(), m_size_SaveGame))
        {
            setErrorString("Could not write " + toStr(m_size_SaveGame) + " bytes of SAVEn.GAM data to file '"
                           + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }

        m_filename_SaveGame = filename;
    }
    else if ((lower_filetitle.substr(0, gLowercaseMagicPrefix.size()) == gLowercaseMagicPrefix) && (".exe" == ext))
    {
        if (0 == m_MagicExe.get())
        {
            setErrorString("Cannot save to '" + toStr(filename) + "' because no MAGIC.EXE was loaded");
            std::cout << errorString() << std::endl;
            return false;
        }

        if (!m_MagicExe->save(filename))
        {
            setErrorString("Could not write MAGIC.EXE data (fully) to file '" + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }

        m_filename_WizardsExe = filename;
    }
    else if ((lower_filetitle.substr(0, gLowercaseWizardsPrefix.size()) == gLowercaseWizardsPrefix) && (".exe" == ext))
    {
        if (0 == m_WizardsExe.get())
        {
            setErrorString("Cannot save to '" + toStr(filename) + "' because no WIZARDS.EXE was loaded");
            std::cout << errorString() << std::endl;
            return false;
        }

        if (!m_WizardsExe->save(filename))
        {
            setErrorString("Could not write WIZARDS.EXE data (fully) to file '" + toStr(filename) + "'");
            std::cout << errorString() << std::endl;
            return false;
        }

        m_filename_WizardsExe = filename;
    }
    else if ("builddat.lbx" == lower_filetitle)
    {
        ok = saveLbx(lower_filetitle, m_BuilddatLbx.get(), filename);
    }
    else if ("itemdata.lbx" == lower_filetitle)
    {
        ok = saveLbx(lower_filetitle, m_ItemDataLbx.get(), filename);
    }
    else if ("itempow.lbx" == lower_filetitle)
    {
        ok = saveLbx(lower_filetitle, m_ItemPowLbx.get(), filename);
    }
    else if ("spelldat.lbx" == lower_filetitle)
    {
        ok = saveLbx(lower_filetitle, m_SpelldatLbx.get(), filename);
    }
    else if ("terrstat.lbx" == lower_filetitle)
    {
        ok = saveLbx(lower_filetitle, m_TerrstatLbx.get(), filename);
    }
    else
    {
        setErrorString("Could not recognize filename pattern to save to file '" + toStr(filename) + "'");
        std::cout << errorString() << std::endl;
        return false;
    }

    return ok;
}

bool MoMGameSave::saveLbx(const std::string& lbxTitle, MoMLbxBase *lbxBase, const std::string &filename)
{
    bool ok = true;
    if (0 == lbxBase)
    {
        setErrorString("Cannot save to '" + filename + "' because no " + lbxTitle + " was loaded");
        std::cout << errorString() << std::endl;
        ok = false;
    }
    if (ok && !lbxBase->save(filename))
    {
        setErrorString("Could not write " + lbxTitle + " data (fully) to file '" + filename + "'");
        std::cout << errorString() << std::endl;
        ok = false;
    }
    return ok;
}

}
