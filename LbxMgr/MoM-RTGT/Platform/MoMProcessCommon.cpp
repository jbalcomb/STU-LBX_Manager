// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <fstream>
#include <iomanip>
#include <iostream>
#include <regex>
#include <string.h>

#include "MoMProcess.h"
#include "MoMUtility.h"

namespace MoM
{

MoMProcess::MoMProcess(void) :
    m_hCreatedProcess(NULL),
    m_hProcess(NULL),
    m_lpBaseAddress(0),
    m_dwBaseAddressSize(0),
    m_dwOffsetSegment0(0),
    m_dwOffsetCode(0),
    m_dwOffsetDatasegment(0),
    m_dataSegmentAndUp(),
    m_exeFilepath(),
    m_verbose(false)
{
}

MoMProcess::~MoMProcess(void)
{
    close();
}

void MoMProcess::close() throw()
{
    m_dataSegmentAndUp.clear();

    m_lpBaseAddress = 0;
    m_dwBaseAddressSize = 0;
    m_dwOffsetSegment0 = 0;
    m_dwOffsetCode = 0;
    m_dwOffsetDatasegment = 0;

    closeProcess();

    m_exeFilepath.clear();
}

void MoMProcess::constructExeFilepath()
{
    const char* pszBaseFilename = 0;
    switch (getOffset_DS_Code())
    {
    case gOFFSET_WIZARDS_DSEG_CODE: pszBaseFilename = "WIZARDS.EXE"; break;
    case gOFFSET_MAGIC_DSEG_CODE:   pszBaseFilename = "MAGIC.EXE"; break;
    default:                        break;
    }

    if (0 == pszBaseFilename)
    {
        std::cout << "Unrecognized Offset_DS_Code()=" << getOffset_DS_Code() << ". Clear .exe filepath" << std::endl;
        m_exeFilepath.clear();
        return;
    }

    auto processFileDirectory = std::regex_replace(m_processFileName, std::regex(R"([^/\\]+$)"), "");
    if (!std::regex_search(m_exeFilepath, std::regex(R"(^(/|\\|\w:))")))
    {
        // Relative path or empty: append to processFileDirectory (which may also be empty)
        m_exeFilepath = processFileDirectory + m_exeFilepath;
    }

    if (!m_exeFilepath.empty() && m_exeFilepath[m_exeFilepath.size() - 1] != '/'
            && m_exeFilepath[m_exeFilepath.size() - 1] != '\\')
    {
        m_exeFilepath += "/";
    }
    m_exeFilepath += pszBaseFilename;
    std::cout << "Set .exe filepath to '" << m_exeFilepath << "'" << std::endl;
}

bool MoMProcess::findSEG0(const std::vector<uint8_t>& data)
{
    size_t offset_DS_Code = 0;
    if ((m_dwOffsetDatasegment >= gOFFSET_WIZARDS_DSEG_CODE * gPARAGRAPH_SIZE) 
        && (gCS_SIGNATURE_BYTE == data[ m_dwOffsetDatasegment - gOFFSET_WIZARDS_DSEG_CODE * gPARAGRAPH_SIZE ]))
    {
        offset_DS_Code = gOFFSET_WIZARDS_DSEG_CODE;
    }
    else if ((m_dwOffsetDatasegment >= gOFFSET_MAGIC_DSEG_CODE * gPARAGRAPH_SIZE) 
        && (gCS_SIGNATURE_BYTE == data[ m_dwOffsetDatasegment - gOFFSET_MAGIC_DSEG_CODE * gPARAGRAPH_SIZE ]))
    {
        offset_DS_Code = gOFFSET_MAGIC_DSEG_CODE;
    }
    else
    {
        return false;
    }

    m_dwOffsetCode = m_dwOffsetDatasegment - offset_DS_Code * gPARAGRAPH_SIZE;

    uint8_t* ptrCode = (uint8_t*)&data[m_dwOffsetCode];

    // Check Borland CS signature code: BA ?? ?? 2E 89 16 C4 02
    if (0 != memcmp(&ptrCode[3], gCS_SIGNATURE_SEQUENCE, sizeof(gCS_SIGNATURE_SEQUENCE)))
    {
        return false;
    }

    size_t offsetDStoSegment0 = *(uint16_t*)&ptrCode[1];
    std::cout << "Found MoM Data Segment (DS_SEG0) Offset as " << std::hex << offsetDStoSegment0 << ":0" << std::dec << std::endl;
    std::cout << "Hex sequence was:" << std::hex << std::setfill('0');
    for (size_t i = 0; i < 8; ++i)
    {
        std::cout << ' ' << std::setw(2) << (unsigned)ptrCode[i];
    }
    std::cout << std::dec << std::endl;
    offsetDStoSegment0 *= gPARAGRAPH_SIZE;

    if (m_dwOffsetDatasegment < offsetDStoSegment0)
    {
        return false;
    }

    m_dwOffsetSegment0 = m_dwOffsetDatasegment - offsetDStoSegment0;

    return true;
}

bool MoMProcess::findSignatures(size_t baseAddress, const std::vector<uint8_t>& data)
{
    bool ok = true;

    size_t indexDOSBoxExe = findStringInBuffer(gDOSBOX_EXE_MATCH, data);
    if (indexDOSBoxExe < data.size())
    {
        std::cout << "Found '" << gDOSBOX_EXE_MATCH << "' in BaseAddress 0x" << std::hex << baseAddress
                  << " with size 0x"<< data.size() << std::dec << std::endl;
        const auto* pathDOSBoxExe = data.data() + indexDOSBoxExe;
        while ((pathDOSBoxExe >= data.data()) && (*pathDOSBoxExe >= 0x20) && (*pathDOSBoxExe <= 0x7E) && (*pathDOSBoxExe != '"'))
        {
            --pathDOSBoxExe;
        }
        ++pathDOSBoxExe;
        m_processFileName = std::string((const char*)pathDOSBoxExe,
                                        (const char*)(data.data() + indexDOSBoxExe +  ARRAYSIZE(gDOSBOX_EXE_MATCH)));
        std::cout << "Resulting processFileName='" << m_processFileName << "'" << std::endl;
    }

    // Find the gLOCAL_DIRECTORY, but do not accept '%s' as a result (which is in the dosbox executable itself)
    std::string strLocalDirectoryKey(gLOCAL_DIRECTORY);
    size_t indexLocalDirectory = findStringInBuffer(strLocalDirectoryKey, data);
    if (indexLocalDirectory < data.size())
    {
        const char* localDirectory = (const char*)&data[indexLocalDirectory + strLocalDirectoryKey.size()];
        if (strlen(localDirectory) > 2)
        {
            std::cout << "Found '" << gLOCAL_DIRECTORY << "' in BaseAddress 0x" << std::hex << baseAddress
                      << " with size 0x"<< data.size() << std::dec << std::endl;
            std::cout << "Result of '" << gLOCAL_DIRECTORY << "' is '" << localDirectory
                      << "' at offset 0x" << std::hex << indexLocalDirectory << std::dec << std::endl;

            // Local directory is only the directory that was mounted.
            // Search again immediately after for a second occurence of the found local directory.
            // That will be the full path, consisting of the local directory and the long directory name path.

            m_exeFilepath = localDirectory;   // Default to the local directory
            size_t indexAfterLocalDirectory = (size_t)(localDirectory + strlen(localDirectory) - (const char*)&data[0]);
            if (indexAfterLocalDirectory < data.size())
            {
                size_t relativeIndexExeFilepath = findStringInBuffer(localDirectory, &data[indexAfterLocalDirectory], data.size() - indexAfterLocalDirectory);
                size_t indexExeFilepath = indexAfterLocalDirectory + relativeIndexExeFilepath;
                if (indexExeFilepath < data.size())
                {
                    m_exeFilepath = (const char*)&data[indexExeFilepath];
                    std::cout << "Found '" << localDirectory << "' again afterward at offset 0x" << std::hex << indexExeFilepath << std::dec << std::endl;
                    std::cout << "Resulting exe path is '" << m_exeFilepath << "'" << std::endl;
                }
            }
        }
    }

    // Find the gDATASEGMENT_IDENTIFIER and fill in the related fields
    size_t indexDatasegmentIdentifier = findStringInBuffer(std::string(gDATASEGMENT_IDENTIFIER, sizeof(gDATASEGMENT_IDENTIFIER)), data);
    if (indexDatasegmentIdentifier < data.size())
    {
        std::cout << "Found MoM BaseAddress 0x" << std::hex << baseAddress << " with size 0x"<< data.size() << std::dec << std::endl;
        m_lpBaseAddress = (uint8_t*)baseAddress;
        m_dwBaseAddressSize = data.size();
        m_dwOffsetDatasegment = indexDatasegmentIdentifier;
        std::cout << "MoM Data Segment (DS) Identifier (size " << ARRAYSIZE(gDATASEGMENT_IDENTIFIER)
                  << ") is at offset 0x" << std::hex << indexDatasegmentIdentifier << std::dec << std::endl;

        // Check and fill the related fields
        ok = findSEG0(data);
    }

    return ok;
}

bool MoMProcess::registerResults(bool ok)
{
    if (0 == m_lpBaseAddress || 0 == m_dwOffsetDatasegment)
    {
        std::cout << "Could not find DATASEGMENT_IDENTIFIER (size " << ARRAYSIZE(gDATASEGMENT_IDENTIFIER) << ")" << std::endl;
        ok = false;
    }
    if (m_exeFilepath.empty())
    {
        std::cout << "Could not find LOCAL_DIRECTORY" << std::endl;
        // Do not treat as failure
    }

    if (ok)
    {
        ok = readData();
    }

    if (ok)
    {
        constructExeFilepath();
    }
    return ok;
}

bool MoMProcess::readData()
{
    if (m_dwOffsetDatasegment >= m_dwBaseAddressSize)
        return false;
    size_t size = m_dwBaseAddressSize - m_dwOffsetDatasegment;
    m_dataSegmentAndUp.resize(size);
    return readData(&m_dataSegmentAndUp[0], size);
}

bool MoMProcess::readData(void *pointer, size_t size)
{
    if (NULL == m_hProcess)
        return false;
    if (0 == m_lpBaseAddress)
        return false;
    if (0 == m_dwOffsetDatasegment)
        return false;
    if (m_dwOffsetDatasegment >= m_dwBaseAddressSize)
        return false;
    if ((uint8_t*)pointer < &m_dataSegmentAndUp[0])
        return false;
    if ((uint8_t*)pointer + size > &m_dataSegmentAndUp[0] + m_dataSegmentAndUp.size())
        return false;

    size_t dsegOffset = (size_t)((uint8_t*)pointer - &m_dataSegmentAndUp[0]);

    bool ok = readProcessData(m_hProcess, m_lpBaseAddress + m_dwOffsetDatasegment + dsegOffset, size, &m_dataSegmentAndUp[dsegOffset]);

    // Check if we're still the same executable
    if (ok)
    {
        size_t sizeSignature = 4;
        std::vector<uint8_t> signature(sizeSignature);
        ok = readProcessData(m_hProcess, m_lpBaseAddress + m_dwOffsetCode, sizeSignature, &signature[0]);
        if (ok)
        {
            ok = (gCS_SIGNATURE_BYTE == signature[0]);
        }
        if (ok)
        {
            size_t offsetDStoSegment0 = (m_dwOffsetDatasegment - m_dwOffsetSegment0) / MoM::gPARAGRAPH_SIZE;
            ok = (*(uint16_t*)&signature[1] == offsetDStoSegment0);
        }
    }

    return ok;
}

bool MoMProcess::load(const char *filename)
{
    std::ifstream ifs(filename, std::ios_base::binary);

    bool ok = ifs.good();

    // TODO: Try and get updating the MoM memory to work (now DOSBox plunks out of existance)
    //       For now: break the connection with a quick hack
    if (ok)
    {
        m_lpBaseAddress = NULL;
    }

    // TODO: Check mismatch in size
    if (ok)
    {
        ifs.read(reinterpret_cast<char*>(&m_dataSegmentAndUp[0]), m_dataSegmentAndUp.size());
        ok = ifs.good();
    }

//    if (ok)
//    {
//        ok = writeData(&m_dataSegmentAndUp[0], m_dataSegmentAndUp.size());
//    }

    return ok;
}

bool MoMProcess::save(const char *filename)
{
    std::ofstream ofs(filename, std::ios_base::binary);

    ofs.write(reinterpret_cast<const char*>(&m_dataSegmentAndUp[0]), m_dataSegmentAndUp.size());

    return ofs.good();
}

}
