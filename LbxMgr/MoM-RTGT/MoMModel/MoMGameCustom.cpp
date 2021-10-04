// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <fstream>
#include <iostream>
#include <string.h> // memcmp()

#include "MoMUtility.h"
#include "MoMExeMagic.h"

#include "MoMGameCustom.h"

namespace MoM {

namespace {

const size_t gMAX_NAME_SIZE = 256;

}

MoMGameCustom::MoMGameCustom(void)
: m_process(),
  m_MagicExe(),
  m_filename_MagicExe()
{
    assert(1 == sizeof(eSkill) && "Check enum-size (g++ flag -fshort-enums)");
    assert(0x04C8 == sizeof(Wizard) && "Check pack-struct (g++ flag -fpack-struct)");
}

MoMGameCustom::~MoMGameCustom(void)
{
    closeGame();
}

void MoMGameCustom::closeGame() throw()
{
    try
    {
        m_process.reset();
        m_MagicExe.reset();
        m_filename_MagicExe.clear();
    }
    catch (...)
    {
        // Make sure this cleanup function does not throw
    }
}

bool MoMGameCustom::commitData(void* ptr, const void* pNewValue, size_t size)
{
    if (0 == m_process.get())
        return false;

    bool ok = true;

    // TODO: Compare old value with new value BEFORE commit
    //       What to do if the memory has changed? Perhaps significantly?
    //       - DOSBox restarted
    //       - MoM MAGIC.EXE or WIZARDS.EXE restarted
    //       - Content of memory reordered, such as the order of the Units
    //       Presumably we want to warn the user of this and allow him to overrule
    if ((0 != m_MagicExe.get()) && m_MagicExe->writeData(ptr, pNewValue, size))
    {
        memcpy(ptr, pNewValue, size);
    }
    else
    {
        memcpy(ptr, pNewValue, size);
        ok = m_process->writeData(ptr, size);
    }

    return ok;
}

bool MoMGameCustom::openGame(std::unique_ptr<MoMProcess>& momProcess)
{
    if (0 == momProcess.get())
        return false;
    if (MoM::gOFFSET_MAGIC_DSEG_CODE != momProcess->getOffset_DS_Code())
        return false;

    closeGame();

    // Transfer ownership
    m_process.swap(momProcess);

    m_filename_MagicExe = m_process->getExeFilepath();

    if (m_filename_MagicExe.empty())
    {
        // TODO: errorhandling??
        return true;
    }

    m_MagicExe.reset( new MoMExeMagic );

    if (!m_MagicExe->load(m_filename_MagicExe))
    {
        std::cout << "Could not load file '"<< m_filename_MagicExe << "'" << std::endl;
        return true;
    }

    return true;
}

std::string MoMGameCustom::getGameDirectory() const
{
    std::string dir;
    if (0 != m_process.get())
    {
        dir = dirFromFilepath(m_process->getExeFilepath());
    }
    return dir;
}

WizardsExe_Game_Data* MoMGameCustom::getGameData_WizardsExe()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data, 1);
}

Game_Settings* MoMGameCustom::getGameSettings()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Settings, 1);
}

Hero_Stats_Initializer *MoMGameCustom::getList_Hero_Stats_Initializer()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(pMoMDataSegment->m_Hero_Stats_Initializers, gMAX_HERO_TYPES);
}

MoMMagicDataSegment* MoMGameCustom::getMagicDataSegment()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    // TODO: Restore pointer validation
    //return validateStaticPointer(pDataSegment, 1);
    return pDataSegment;
}

const char* MoMGameCustom::getNameByOffset(DS_Offset offset)
{
    if (0 == m_process.get())
        return 0;
    char* namebuffer = (char*)m_process->getDatasegmentData();
    if (0 == validateStaticPointer(namebuffer, gMAX_NAME_SIZE))
        return 0;
    // Check if there is a terminating zero in range
    for (size_t i = 0; i < gMAX_NAME_SIZE; ++i)
    {
        if (namebuffer[i] == '\0')
        {
            return namebuffer + offset;
        }
    }
    // No terminating zero found
    return 0;
}

uint16_t* MoMGameCustom::getNumber_of_Wizards()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(&pMoMDataSegment->m_Game_Data.m_Number_of_Wizards, 1);
}

uint8_t* MoMGameCustom::getMagicOverlay(size_t ovlNr)
{
    if (0 == m_MagicExe.get())
        return 0;

    uint8_t* ptr = m_MagicExe->getOverlay(ovlNr);

    return ptr;
}

Wizard* MoMGameCustom::getWizards()
{
    if (0 == m_process.get())
        return 0;
    MoMMagicDataSegment* pMoMDataSegment = (MoMMagicDataSegment*)m_process->getDatasegmentData();
    return validateStaticPointer(pMoMDataSegment->m_Wizards, MoM::gMAX_WIZARD_RECORDS);
}

}
