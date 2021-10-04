// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#pragma once

#include "MoMLbxBase.h"
#include "MoMTemplate.h"

namespace MoM
{

inline Spell_Data* getLbxSpellData(MoM::MoMLbxBase& lbx)
{
    const auto* data = lbx.getRecord(0);
    auto size = lbx.getRecordSize(0);
    if (data == nullptr)
        return nullptr;
    auto* spellData = (Spell_Data*)(data + 4);
    if ((uint8_t*)spellData + eSpell_MAX * sizeof(Spell_Data) > data + size)
        return nullptr;
    return spellData;
}

}
