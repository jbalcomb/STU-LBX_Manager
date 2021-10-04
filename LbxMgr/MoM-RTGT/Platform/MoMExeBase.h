// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMEXEBASE_H
#define MOMEXEBASE_H

#include <vector>

#include "MoMCommon.h"

namespace MoM {

class MoMExeBase
{
public:
    MoMExeBase();
    virtual ~MoMExeBase()
    {
        close();
    }

    virtual void close() throw();

    bool convertExeOffset_to_DsegOffset(size_t exeOffset, uint16_t& dsegOffset) const;
    bool convertDsegOffset_to_ExeOffset(uint16_t dsegOffset, size_t& exeOffset) const;
    bool convertExeOffset_to_OvlOffset(size_t exeOffset, size_t& ovlNr, uint16_t& ovlOffset) const;
    bool convertOvlOffset_to_ExeOffset(size_t ovlNr, uint16_t ovlOffset, size_t& exeOffset) const;
    bool convertExeOffset_to_SegOffset(size_t exeOffset, size_t& segNr, uint16_t& segOffset) const;
    bool convertSegOffset_to_ExeOffset(size_t segNr, uint16_t segOffset, size_t& exeOffset) const;
    bool convertExeOffset_to_StubOffset(size_t exeOffset, size_t& stubNr, uint16_t& stubOffset) const;
    bool convertStubOffset_to_ExeOffset(size_t stubNr, uint16_t stubOffset, size_t& exeOffset) const;

    virtual bool load(const std::string& filename);

    virtual bool save(const std::string& filename);

    bool writeData(const void* ptr, const void* pNewValue, size_t size);

    EXE_Header* getExeHeader()
    {
        return m_EXE_Header;
    }

    std::string getFilename() const
    {
        return m_filename;
    }

    uint8_t* getDataSegment()
    {
        return m_DataSegment;
    }

    size_t getOffsetLoadModule() const;
    size_t getOffsetDataSegment() const;

    uint8_t* getSegment(size_t segNr);

    BorlandStub* getStub(size_t stubNr);

    uint8_t* getOverlay(size_t ovlNr);

    uint8_t* getExeContents()
    {
        if (m_contents.empty())
            return 0;
        return &m_contents[0];
    }

    size_t getExeSize() const
    {
        return m_contents.size();
    }

protected:
    void setOverlayParameters(size_t offsetRegularSegmentEntries, size_t nrRegularSegments, size_t offsetStubSegmentEntries, size_t nrStubSegments, size_t firstStubNr);

private:
    void initializeStubs();

    // NOT IMPLEMENTED
    MoMExeBase(const MoMExeBase& rhs);
    MoMExeBase& operator=(const MoMExeBase& rhs);

    // STATUS

    std::string             m_filename;
    std::vector<uint8_t>    m_contents;
    std::vector<BorlandStub*>    m_Stubs;

    EXE_Header*             m_EXE_Header;
    EXE_Reloc*              m_EXE_Reloc;
    uint8_t*                m_Load_Module;
    BorlandSegmentEntry*    m_RegularSegments;
    BorlandSegmentEntry*    m_StubSegments;
    uint8_t*                m_DataSegment;
    char*                   m_BorlandIdentifier;
    uint8_t*                m_StackSegment;
    uint8_t*                m_Overlays;

    // CONFIG

    size_t                  m_offsetRegularSegmentEntries;
    size_t                  m_nrRegularSegments;
    size_t                  m_offsetStubSegmentEntries;
    size_t                  m_nrStubSegments;
    size_t                  m_firstStubNr;
};

}

#endif
