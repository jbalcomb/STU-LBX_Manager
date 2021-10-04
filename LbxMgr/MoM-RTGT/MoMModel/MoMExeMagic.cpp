// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "MoMExeMagic.h"

namespace MoM
{

static const size_t gSIZE_MAGIC_EXE = 303264;

static const size_t gOFFSET_REGULAR_SEGMENT_ENTRIES = (0x280 + 0x214C) * gPARAGRAPH_SIZE + 0x00C0; // seg038:00C0
static const size_t gNR_REGULAR_SEGMENTS = 38;
static const size_t gOFFSET_STUB_SEGMENT_ENTRIES = (0x280 + 0x214C) * gPARAGRAPH_SIZE + 0x0240;    // seg038:0240
static const size_t gNR_STUB_SEGMENTS = 14;
static const size_t gFIRST_STUB_NR = 48;

MoMExeMagic::MoMExeMagic() :
    MoMExeBase()
{
}

void MoMExeMagic::close() throw()
{
    MoMExeBase::close();
}

bool MoMExeMagic::load(const std::string& filename)
{
    setOverlayParameters(gOFFSET_REGULAR_SEGMENT_ENTRIES, gNR_REGULAR_SEGMENTS, gOFFSET_STUB_SEGMENT_ENTRIES, gNR_STUB_SEGMENTS, gFIRST_STUB_NR);

    bool ok = MoMExeBase::load(filename);

    return ok;

}

}

