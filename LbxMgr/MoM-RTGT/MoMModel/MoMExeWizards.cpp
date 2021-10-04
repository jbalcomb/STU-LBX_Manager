// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "MoMUtility.h"

#include "MoMExeWizards.h"

namespace MoM
{

static const size_t gOFFSET_REGULAR_SEGMENT_ENTRIES = (0x2A0 + 0x2369) * gPARAGRAPH_SIZE + 0x01A0; // seg040:01A0
static const size_t gNR_REGULAR_SEGMENTS = 40;
static const size_t gOFFSET_STUB_SEGMENT_ENTRIES = (0x2A0 + 0x2369) * gPARAGRAPH_SIZE + 0x0330;    // seg040:0330
static const size_t gNR_STUB_SEGMENTS = 115;
static const size_t gFIRST_STUB_NR = 50;

static SpellSaveEntry gSpellSaveTableV131[] =
{
    { 0x1C82, 0 },
    { 0x1C89, 0x1C8D },
    { 0x1C91, 0x1C96 },
    { 0x1C9A, 0x1C9F },
    { 0x1CA3, 0x1CA8 },
    { 0x1CAC, 0x1CB1 },
    { 0x1CB5, 0x1CBA },
    { 0x1CBE, 0x1CC3 },
    { 0x1CC7, 0x1CCC },
    { 0x1CD0, 0x1CD5 },
    { 0x1CD9, 0 },
    { 0x1CE1, 0x1CE6 },
    { 0x1CEA, 0x1CEF },
    { 0x1CF3, 0 },
    { 0x1CFA, 0 },
    { 0x1D01, 0x1D05 },
    { 0x1D09, 0x1D0D },
    { 0x1D11, 0x1D15 },
    { 0x1D19, 0x1D1D },
    { 0x1D21, 0x1D25 },
};

static SpellSaveEntry gSpellSaveTableV140j[] =
{
    { 0x1C82, 0x1C86 },
    { 0x1C8A, 0x1C8E },
    { 0x1C92, 0x1C97 },
    { 0x1C9B, 0x1CA0 },
    { 0x1CA4, 0x1CA9 },
    { 0x1CAD, 0x1CB2 },
    { 0x1CB6, 0x1CBB },
    { 0x1CBF, 0x1CC4 },
    { 0x1CC8, 0x1CCD },
    { 0x1CD1, 0x1CD6 },
    { 0x1CDA, 0x1CDF },
    { 0x1CE3, 0x1CE8 },
    { 0x1CEC, 0x1CF1 },
    { 0x1CF5, 0x1CF9 },
    { 0x1CFD, 0x1D01 },
    { 0x1D05, 0x1D09 },
    { 0x1D0D, 0x1D11 },
    { 0x1D15, 0x1D19 },
    { 0x1D1D, 0x1D21 },
    { 0x1D25, 0x1D29 },
};

MoMExeWizards::MoMExeWizards() :
    MoMExeBase()
{
}

void MoMExeWizards::close() throw()
{
    MoMExeBase::close();
}

size_t MoMExeWizards::getNrSpellSaves() const
{
    return ARRAYSIZE(gSpellSaveTableV131);
}

SpellSaveEntry MoMExeWizards::getSpellSave(size_t nr)
{
    const MoMDataSegment* pDataSegment = (const MoMDataSegment*)getDataSegment();
    if ((0 != pDataSegment) && (&pDataSegment->m_Copyright_and_Version[34] >= std::string("v1.40j")))
    {
        return gSpellSaveTableV140j[nr];
    }
    else
    {
        return gSpellSaveTableV131[nr];
    }
}

bool MoMExeWizards::load(const std::string& filename)
{
    setOverlayParameters(gOFFSET_REGULAR_SEGMENT_ENTRIES, gNR_REGULAR_SEGMENTS, gOFFSET_STUB_SEGMENT_ENTRIES, gNR_STUB_SEGMENTS, gFIRST_STUB_NR);

    bool ok = MoMExeBase::load(filename);

    return ok;
}

}

