// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <cassert>
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

#include "MoMGameBase.h"

namespace MoM {

static_assert(1 == sizeof(eHero_TypeCode), "Check enum-size (g++ flag -fshort-enums)");
static_assert(1 == sizeof(eSkill), "Check enum-size (BETTER_ENUM?)");
static_assert(0x04C8 == sizeof(Wizard), "Check pack-struct (g++ flag -fpack-struct)");

MoMGameBase::~MoMGameBase() = default;

void MoMGameBase::findUnitsAtLocation(const MoMLocation& location, std::vector<int>& units)
{
    units.clear();

    for (int unitNr = 0; unitNr < getNrUnits(); ++unitNr)
    {
        const Unit* unit = getUnit(unitNr);
        if (0 == unit)
            break;

        if (location.m_XPos == unit->m_XPos
            && location.m_YPos == unit->m_YPos
            && location.m_Plane == unit->m_Plane)
        {
            units.push_back(unitNr);
        }
    }
}

void MoMGameBase::getHeroSlotTypes(eHero_TypeCode heroTypeCode, eSlot_Type16 heroSlotTypes[3])
{
    if (toUInt(heroTypeCode) <= toUInt(HEROTYPE_Wizard))
    {
        heroSlotTypes[0] = static_cast<eSlot_Type16>(1 + heroTypeCode);
        if (HEROTYPE_Wizard == heroTypeCode)
        {
            heroSlotTypes[1] = SLOT16_Amulet;
        }
        else
        {
            heroSlotTypes[1] = SLOT16_Armor_Shield;
        }
        heroSlotTypes[2] = SLOT16_Amulet;
    }
    else
    {
        unsigned slotCode = static_cast<unsigned>(heroTypeCode);
        slotCode -= 6;
        heroSlotTypes[0] = static_cast<eSlot_Type16>(1 + slotCode % 6);
        slotCode /= 6;
        heroSlotTypes[1] = static_cast<eSlot_Type16>(1 + slotCode % 6);
        slotCode /= 6;
        heroSlotTypes[2] = static_cast<eSlot_Type16>(1 + slotCode % 6);
    }
}

eHero_TypeCode MoMGameBase::getHeroTypeCode(eSlot_Type16 slotSword, eSlot_Type16 slotShield, eSlot_Type16 slotRing)
{
    unsigned codeSword = static_cast<unsigned>(slotSword) - 1;
    unsigned codeShield = static_cast<unsigned>(slotShield) - 1;
    unsigned codeRing = static_cast<unsigned>(slotRing) - 1;
    unsigned slotCode = 6 + codeSword + 6 *(codeShield + 6 * codeRing);
    return static_cast<MoM::eHero_TypeCode>(slotCode);
}

Hired_Hero *MoMGameBase::getHiredHero(ePlayer playerNr, eUnit_Type unitType)
{
    Hired_Hero* value = 0;
    Wizard* wizard = getWizard(playerNr);
    if (0 == wizard)
        return 0;
    for (size_t slotNr = 0; (slotNr < gMAX_HIRED_HEROES) && (0 == value); ++slotNr)
    {
        Hired_Hero* hh = &wizard->m_Heroes_hired_by_wizard[slotNr];
        Unit* unit = getUnit(hh->m_Unit_Nr);
        if ((0 != unit) && (unit->m_Unit_Type == unitType))
        {
            value = hh;
        }
    }
    return value;
}

ItemDataLbx* MoMGameBase::getItemDataLbx(int itemDataNr)
{
    ItemDataLbx* value = 0;

    (void)getNrItemDataLbx();   // Trigger loading of file
    if (m_ItemDataLbx->getSubRecordSize(0) == sizeof(*value))
    {
        std::vector<uint8_t> data;
        value = (ItemDataLbx*)m_ItemDataLbx->getSubRecord(0, itemDataNr);
    }

    return value;
}

ItemPowLbx *MoMGameBase::getItemPowLbx(int itemPowNr)
{
    ItemPowLbx* value = 0;

    (void)getNrItemPowLbx();   // Trigger loading of file
    if (m_ItemPowLbx->getSubRecordSize(0) == sizeof(*value))
    {
        std::vector<uint8_t> data;
        value = (ItemPowLbx*)m_ItemPowLbx->getSubRecord(0, itemPowNr);
    }

    return value;
}

uint8_t* MoMGameBase::getLevelBonusesHero()
{
    uint8_t* ovl = getWizardsOverlay(116);
    if (0 == ovl)
        return 0;
    return &ovl[0x1DA2];
}

uint8_t* MoMGameBase::getLevelBonusesUnit()
{
    uint8_t* ovl = getWizardsOverlay(116);
    if (0 == ovl)
        return 0;
    return &ovl[0x1D1B];
}

int MoMGameBase::getNrItemDataLbx()
{
    if (0 == m_ItemDataLbx.get())
    {
        m_ItemDataLbx.reset(new MoMLbxBase);
        // Only try to load once and ignore result.
        // We'll check if it's there when we use it.
        if (m_ItemDataLbx->load(this->getGameDirectory() + "/" + "ITEMDATA.LBX"))
        {
            // error
        }
    }
    return m_ItemDataLbx->getNrSubRecords(0);
}

int MoMGameBase::getNrItemPowLbx()
{
    if (0 == m_ItemPowLbx.get())
    {
        m_ItemPowLbx.reset(new MoMLbxBase);
        // Only try to load once and ignore result.
        // We'll check if it's there when we use it.
        if (m_ItemPowLbx->load(this->getGameDirectory() + "/" + "ITEMPOW.LBX"))
        {
            // error
        }
    }
    return m_ItemPowLbx->getNrSubRecords(0);
}

// TODO: Replace all references in other modules to race names by getRaceName()
std::string MoMGameBase::getRaceName(eRace race)
{
	std::string raceName;
 	const MoMDataSegment* pMoMDataSegment = getDataSegment();
    if ((0 != pMoMDataSegment) && (static_cast<unsigned>(race) < gMAX_RACES))
	{
		const char* ptrName = getNameByOffset(pMoMDataSegment->m_Race_Data[race].m_PtrName);
		if (0 != ptrName)
		{
			raceName = ptrName;
		}
	}
	if (raceName.empty())
	{
		raceName = toStr(race);
		// Strip Capital prefix
		size_t pos = 0;
		while ((pos < raceName.size()) && isupper(raceName[pos]))
		{
			pos++;
		}
		if ((pos >= 2) && (pos + 1 < raceName.size()))
		{
			raceName = raceName.substr(pos + 1);
		}
		// Strip trailing numbers
		pos = raceName.find('(');
        if ((std::string::npos != pos) && (pos > 1))
		{
            raceName = raceName.substr(0, pos - 1);
		}
		// Replace underscores by spaces
        raceName = replaceUnderscoresBySpaces(raceName);
	}
    return raceName;
}

eRealm_Type MoMGameBase::getRealmRace(eRace race) const
{
    eRealm_Type value = REALM_None;
    switch (race)
    {
    // Note: different order than eRealm_Type
    case MoM::RACE_Nature:     value = REALM_Nature;    break;
    case MoM::RACE_Sorcery:    value = REALM_Sorcery;   break;
    case MoM::RACE_Chaos:      value = REALM_Chaos;     break;
    case MoM::RACE_Life:       value = REALM_Life;      break;
    case MoM::RACE_Death:      value = REALM_Death;     break;
    case MoM::RACE_Arcane:     value = REALM_Arcane;    break;
    default:                   value = REALM_None;      break;
    }
    return value;
}

eRealm_Type MoMGameBase::getRealmRangedType(eRanged_Type rangedType) const
{
    eRealm_Type value = REALM_None;
    switch (rangedType)
    {
    case RANGED_None:
    case RANGED_Rock:
    case RANGED_Arrow:
    case RANGED_Bullet:                     value = REALM_None;     break;

    case RANGED_Nature_Icebolt:
    case RANGED_Nature_Priest:
    case RANGED_Nature_Shimmer:
    case RANGED_Nature_Green:               value = REALM_Nature;   break;

    case RANGED_Sorcery_Illusion:           value = REALM_Sorcery;  break;

    case RANGED_Chaos_Lightning:            value = REALM_Chaos;    break;
    case RANGED_Chaos_Fireball:             value = REALM_Chaos;    break;
    case RANGED_Chaos_Deathbolt:            value = REALM_Chaos;    break;
    case RANGED_Chaos_Drow:                 value = REALM_Chaos;    break;

    case RANGED_Thrown_Weapons:             value = REALM_None;     break;

    case RANGED_Fire_Breath:
    case RANGED_Lightning_Breath:           value = REALM_Chaos;    break;

    case RANGED_Stoning_Gaze:               value = REALM_Nature;   break;
    case RANGED_Multiple_Gaze:              value = REALM_Chaos;    break;     // Doom gaze
    case RANGED_Death_Gaze:                 value = REALM_Death;    break;

    default:                                value = REALM_None;     break;
    }
    return value;
}

eRealm_Type MoMGameBase::getRealmSpecialAttack(eUnitAbility specialAttack) const
{
    eRealm_Type value = REALM_None;
    switch (specialAttack)
    {
    case UNITABILITY_Poison_attack:         value = REALM_None;     break;

    case UNITABILITY_Stoning_Touch:         value = REALM_Nature;   break;

    case UNITABILITY_Illusionary_attack:    value = REALM_Sorcery;  break;

    case UNITABILITY_Destruction:           value = REALM_Chaos;    break;

    case UNITABILITY_Dispel_Evil:           value = REALM_Life;     break;

    case UNITABILITY_Life_Stealing:
    case UNITABILITY_Death_Touch:           value = REALM_Death;    break;

    case UNITABILITY_Armor_Piercing:
    case UNITABILITY_First_Strike:
    case UNITABILITY_Automatic_Damage:
    case UNITABILITY_No_effect05:
    case UNITABILITY_Power_Drain:
    case UNITABILITY_Ball_COMBAT:
    case UNITABILITY_Simultaneous_Damage_COMBAT:
    case UNITABILITY_Eldritch_Weapon_COMBAT:
    case UNITABILITY_Warp_Lightning_COMBAT:
    default:                                value = REALM_None;     break;
    }
    return value;
}

}
