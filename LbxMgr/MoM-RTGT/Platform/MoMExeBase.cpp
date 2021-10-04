// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <fstream>

#include "MoMExeBase.h"

namespace MoM
{

namespace
{
const uint8_t gCS_SIGNATURE_BYTE = '\xBA';
}

MoMExeBase::MoMExeBase() :
    m_filename(),
    m_contents(),
    m_Stubs(),
    m_EXE_Header(0),
    m_EXE_Reloc(0),
    m_Load_Module(0),
    m_RegularSegments(0),
    m_StubSegments(0),
    m_DataSegment(0),
    m_BorlandIdentifier(0),
    m_StackSegment(0),
    m_Overlays(0),
    m_offsetRegularSegmentEntries(0),
    m_nrRegularSegments(0),
    m_offsetStubSegmentEntries(0),
    m_nrStubSegments(0),
    m_firstStubNr(0)
{
}

void MoMExeBase::close() throw()
{
    m_filename.clear();
    m_contents.clear();
    m_Stubs.clear();

    m_EXE_Header = 0;
    m_EXE_Reloc = 0;
    m_Load_Module = 0;
    m_RegularSegments = 0;
    m_StubSegments = 0;
    m_DataSegment = 0;
    m_BorlandIdentifier = 0;
    m_StackSegment = 0;
    m_Overlays = 0;

    // Do not reset CONFIG members
//    m_offsetStubSegmentEntries = 0;
//    m_nrStubSegments = 0;
//    m_firstStubNr = 0;
}

bool MoMExeBase::convertDsegOffset_to_ExeOffset(uint16_t dsegOffset, size_t& exeOffset) const
{
    exeOffset = 0;

    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    exeOffset = (m_DataSegment - pContents) + dsegOffset;

    return true;
}

bool MoMExeBase::convertExeOffset_to_DsegOffset(size_t exeOffset, uint16_t& dsegOffset) const
{
    dsegOffset = 0;

    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    if (pContents + exeOffset < m_DataSegment)
        return false;
    if (pContents + exeOffset >= m_DataSegment + 0x10000)
        return false;

    dsegOffset  = exeOffset - (m_DataSegment - pContents);

    return true;
}

bool MoMExeBase::convertExeOffset_to_OvlOffset(size_t exeOffset, size_t& ovlNr, uint16_t& ovlOffset) const
{
    ovlNr = 0;
    ovlOffset = 0;

    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    for (size_t i = 0; i < m_nrStubSegments; ++i)
    {
        size_t offset = (m_Overlays - pContents) + m_Stubs[i]->fileoff;
        if ((exeOffset >= offset) && (exeOffset < offset + m_Stubs[i]->codesize))
        {
            // Found: set output parameters and return true
            ovlNr = m_firstStubNr + i;
            ovlOffset = exeOffset - offset;
            return true;
        }
    }

    // Not found: return false
    return false;
}

bool MoMExeBase::convertOvlOffset_to_ExeOffset(size_t ovlNr, uint16_t ovlOffset, size_t& exeOffset) const
{
    exeOffset = 0;

    if (ovlNr < m_firstStubNr)
        return false;
    if (ovlNr >= m_firstStubNr + m_nrStubSegments)
        return false;
    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    size_t stubNr = ovlNr - m_firstStubNr;

    exeOffset = (m_Overlays - pContents) + m_Stubs[stubNr]->fileoff + ovlOffset;

    return true;
}

bool MoMExeBase::convertExeOffset_to_SegOffset(size_t exeOffset, size_t &segNr, uint16_t &segOffset) const
{
    segNr = 0;
    segOffset = 0;

    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    for (size_t i = 0; i < m_nrRegularSegments; ++i)
    {
        size_t offsetSegment = (m_Load_Module - pContents) + m_RegularSegments[i].base * gPARAGRAPH_SIZE + m_RegularSegments[i].offset;
        if ((exeOffset >= offsetSegment) && (exeOffset < offsetSegment + m_RegularSegments[i].size))
        {
            // Found: set output parameters and return true
            segNr = i;
            segOffset = exeOffset - offsetSegment + m_RegularSegments[i].offset;
            return true;
        }
    }

    // Not found: return false
    return false;
}

bool MoMExeBase::convertSegOffset_to_ExeOffset(size_t segNr, uint16_t segOffset, size_t &exeOffset) const
{
    exeOffset = 0;

    if (segNr >= m_nrRegularSegments)
        return false;
    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    exeOffset = (m_Load_Module - pContents) + m_RegularSegments[segNr].base * gPARAGRAPH_SIZE + segOffset;

    return true;
}

bool MoMExeBase::convertExeOffset_to_StubOffset(size_t exeOffset, size_t &stubNr, uint16_t &stubOffset) const
{
    stubNr = 0;
    stubOffset = 0;

    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    for (size_t i = 0; i < m_nrStubSegments; ++i)
    {
        size_t offsetSegment = (m_Load_Module - pContents) + m_StubSegments[i].base * gPARAGRAPH_SIZE + m_StubSegments[i].offset;
        if ((exeOffset >= offsetSegment) && (exeOffset < offsetSegment + m_StubSegments[i].size))
        {
            // Found: set output parameters and return true
            stubNr = m_firstStubNr + i;
            stubOffset = exeOffset - offsetSegment + m_StubSegments[i].offset;
            return true;
        }
    }

    // Not found: return false
    return false;
}

bool MoMExeBase::convertStubOffset_to_ExeOffset(size_t stubNr, uint16_t stubOffset, size_t &exeOffset) const
{
    exeOffset = 0;

    if (stubNr < m_firstStubNr)
        return false;
    size_t index = stubNr - m_firstStubNr;
    if (index >= m_nrStubSegments)
        return false;
    if (m_contents.empty())
        return false;
    const uint8_t* pContents = (const uint8_t*)&m_contents[0];

    exeOffset = (m_Load_Module - pContents) + m_StubSegments[index].base * gPARAGRAPH_SIZE + stubOffset;

    return true;
}

uint8_t *MoMExeBase::getSegment(size_t segNr)
{
    if (0 == m_RegularSegments)
        return 0;
    if (segNr >= m_nrRegularSegments)
        return 0;
    uint8_t* ptr = m_Load_Module + (m_RegularSegments[segNr].base - m_RegularSegments[0].base) * gPARAGRAPH_SIZE - m_RegularSegments[segNr].offset;

    return ptr;
}

BorlandStub *MoMExeBase::getStub(size_t stubNr)
{
    if (0 == m_StubSegments)
        return 0;
    if (stubNr >= m_nrStubSegments)
        return 0;
    BorlandStub* ptr = m_Stubs[stubNr];

    return ptr;
}

uint8_t* MoMExeBase::getOverlay(size_t ovlNr)
{
    if (0 == m_Overlays)
        return 0;
    if (ovlNr < m_firstStubNr)
        return 0;
    if (ovlNr >= m_firstStubNr + m_nrStubSegments)
        return 0;

    size_t index = ovlNr - m_firstStubNr;

    if (index >= m_Stubs.size())
        return 0;

    uint8_t* ptr = m_Overlays + m_Stubs[index]->fileoff;
    // TODO: in range?

    return ptr;
}

bool MoMExeBase::load(const std::string& filename)
{
    std::ifstream ifs(filename.c_str(), (std::ios_base::in | std::ios_base::binary));

    if (!ifs)
    {
        std::cout << "Could not open file '"<< filename << "'" << std::endl;
        return false;
    }

    // Determine and check size
    ifs.seekg(0, std::ios_base::end);
    size_t size = ifs.tellg();
    ifs.seekg(0, std::ios_base::beg);
    if (size < sizeof(EXE_Header))
    {
        std::cout << "File '"<< filename << "' is not large enough (size = " << size << ")" << std::endl;
        return false;
    }

    close();

    m_contents.resize(size);

    uint8_t* pContents = (uint8_t*)&m_contents[0];

    if (!ifs.read((char*)pContents, m_contents.size()))
    {
        std::cout << "Could not (fully) read file '"<< filename << "'" << std::endl;
        return false;
    }

    // TODO: Signature "MZ" check
    // TODO: Range checks
    m_EXE_Header        = (EXE_Header*)pContents;
    m_EXE_Reloc         = (EXE_Reloc*)(pContents + m_EXE_Header->relocation_table_offset_bytes);
    m_Load_Module       = (uint8_t*)(pContents + m_EXE_Header->load_module_offset_paragraphs * gPARAGRAPH_SIZE);
    m_StackSegment      = (uint8_t*)(pContents + m_EXE_Header->ss_offset_in_load_module_paragraphs * gPARAGRAPH_SIZE);
    m_Overlays          = (uint8_t*)(pContents + m_EXE_Header->blocks_in_file * gBLOCK_SIZE + gPARAGRAPH_SIZE);
    if (0 != m_EXE_Header->bytes_in_last_block)
    {
        m_Overlays -= (512 - m_EXE_Header->bytes_in_last_block);
    }

    if (gCS_SIGNATURE_BYTE != m_Load_Module[0])
    {
        std::cout << "File is not a Borland EXE because the CS Signature 0x" << std::hex << gCS_SIGNATURE_BYTE << std::dec << " is missing" << std::endl;
        return false;
    }

    m_DataSegment       = (uint8_t*)(m_Load_Module + *(uint16_t*)&m_Load_Module[1] * gPARAGRAPH_SIZE);
    m_BorlandIdentifier = (char*)(m_DataSegment + 4);

    initializeStubs();

    m_filename = filename;

    return true;
}

bool MoMExeBase::save(const std::string& filename)
{
    if (m_contents.empty())
        return false;

    std::ofstream ofs(filename.c_str(), std::ios_base::out | std::ios_base::binary);
    if (!ofs)
    {
        std::cout << "Could not open file '"<< filename << "' for writing" << std::endl;
        return false;
    }

    if (!ofs.write((const char*)&m_contents[0], m_contents.size()))
    {
        std::cout << "Could not write .EXE data (fully) to file '"<< filename << "'" << std::endl;
        return false;
    }

    m_filename = filename;

    return true;
}

bool MoMExeBase::writeData(const void* ptr, const void* pNewValue, size_t size)
{
    if (0 == getExeContents())
        return false;
    uint8_t* ptrByte = (uint8_t*)ptr;
    if (ptrByte < getExeContents())
        return false;
    if (ptrByte + size > getExeContents() + getExeSize())
        return false;

    // TODO: Compare old value with new value BEFORE commit
    //       What to do if the memory has changed? Perhaps significantly?
    //       - DOSBox restarted
    //       - MoM MAGIC.EXE or WIZARDS.EXE restarted
    //       - Content of memory reordered, such as the order of the Units
    //       Presumably we want to warn the user of this and allow him to overrule

    // TODO: Give warning with confirm to modify the file
    // TODO: Make backup of original if not already done so
    //       Let's call it "MAGIC.EXE_TweakerBackup"
    std::fstream::pos_type pos = (std::fstream::pos_type)(ptrByte - getExeContents());
    std::fstream fs(m_filename.c_str(), std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!fs)
    {
        std::cout << "WARN: Could not open file for writing '" << m_filename << "'" << std::endl;
        return false;
    }
    else if (!fs.seekp(pos))
    {
        std::cout << "WARN: Could not seek to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename << "'" << std::endl;
        return false;
    }
    else if (!fs.write((const char*)pNewValue, size))
    {
        std::cout << "WARN: Could not write data to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename << "'" << std::endl;
        return false;
    }
    else
    {
        std::cout << "INFO: " << size << " bytes written to pos 0x" << std::hex << pos << std::dec << " in file '" << m_filename << "'" << std::endl;
    }

    return true;
}

size_t MoMExeBase::getOffsetLoadModule() const
{
    if (m_contents.empty())
        return 0;
    return (m_Load_Module - &m_contents[0]);
}

size_t MoMExeBase::getOffsetDataSegment() const
{
    if (m_contents.empty())
        return 0;
    return (m_DataSegment - &m_contents[0]);
}

void MoMExeBase::setOverlayParameters(size_t offsetRegularSegmentEntries, size_t nrRegularSegments, size_t offsetStubSegmentEntries, size_t nrStubSegments, size_t firstStubNr)
{
    m_offsetRegularSegmentEntries = offsetRegularSegmentEntries;
    m_nrRegularSegments = nrRegularSegments;
    m_offsetStubSegmentEntries = offsetStubSegmentEntries;
    m_nrStubSegments = nrStubSegments;
    m_firstStubNr = firstStubNr;

    initializeStubs();
}

void MoMExeBase::initializeStubs()
{
    if (m_contents.empty())
        return;

    uint8_t* pContents = (uint8_t*)&m_contents[0];

    if (0 != m_offsetRegularSegmentEntries)
    {
        m_RegularSegments      = (BorlandSegmentEntry*)(pContents + m_offsetRegularSegmentEntries);
    }
    if (0 != m_offsetStubSegmentEntries)
    {
        m_StubSegments      = (BorlandSegmentEntry*)(pContents + m_offsetStubSegmentEntries);
        m_Stubs.resize(m_nrStubSegments);
        for (size_t i = 0; i < m_nrStubSegments; ++i)
        {
            // TODO: in range?
            m_Stubs[i] = (BorlandStub*)(m_Load_Module + m_StubSegments[i].base * gPARAGRAPH_SIZE + m_StubSegments[i].offset);
        }
    }
}

}

