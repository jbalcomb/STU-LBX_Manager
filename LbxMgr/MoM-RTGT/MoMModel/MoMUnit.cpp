// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <cmath>
#include <cstring>

#include "MoMGameBase.h"
#include "MoMGenerated.h"
#include "MoMUtility.h"

#include "MoMUnit.h"

namespace MoM
{

MoMUnit::MoMUnit() : m_game()
{
    zeroFields();
}

MoMUnit::MoMUnit(MoMGameBase *game) : m_game(game)
{
    zeroFields();
}

MoMUnit::MoMUnit(MoMGameBase *game, Battle_Unit *battleUnit) : m_game(game)
{
    zeroFields();
    changeUnit(battleUnit);
}

MoMUnit::MoMUnit(MoMGameBase *game, eUnit_Type unitTypeNr) : m_game(game)
{
    zeroFields();
    changeUnit(unitTypeNr);
}

MoMUnit::MoMUnit(MoMGameBase *game, Hero_stats *heroStats) : m_game(game)
{
    zeroFields();
    changeUnit(heroStats);
}

MoMUnit::MoMUnit(MoMGameBase *game, Hero_Stats_Initializer *heroStatsInitializer) : m_game(game)
{
    zeroFields();
    changeUnit(heroStatsInitializer);
}

MoMUnit::MoMUnit(MoMGameBase *game, Hired_Hero *hiredHero) : m_game(game)
{
    zeroFields();
    changeUnit(hiredHero);
}

MoMUnit::MoMUnit(MoMGameBase *game, Unit_Type_Data *unitType) : m_game(game)
{
    zeroFields();
    changeUnit(unitType);
}

MoMUnit::MoMUnit(MoMGameBase *game, Unit *unit) : m_game(game)
{
    zeroFields();
    changeUnit(unit);
}

void MoMUnit::zeroFields()
{
    m_battleUnit = 0;
    m_heroStats = 0;
    m_heroStatsInitializer = 0;
    m_hiredHero = 0;
    m_unitType = 0;
    m_unit = 0;
    m_bonuses = BaseAttributes();
    m_dnSpells = BaseAttributes();
    m_penalties = BaseAttributes();
    m_upAbilities = BaseAttributes();
    m_upItems = BaseAttributes();
    m_upLevel = BaseAttributes();
    m_upSpells = BaseAttributes();
    m_upWeaponType = BaseAttributes();
}

MoMUnit::~MoMUnit()
{
}

MoMUnit::MoMUnit(const MoMUnit& rhs)
{
    copyMemberData(rhs);
}

MoMUnit& MoMUnit::operator=(const MoMUnit& rhs)
{
    if (this != &rhs)
    {
        // Release_All_Resources_Of( this );
        // Not applicable

        // Base::operator=( rhs );
        // Not applicable

        // Copy_Member_Data( this, rhs );
        copyMemberData(rhs);
    }
    return *this;
}

void MoMUnit::copyMemberData(const MoMUnit& rhs)
{
    m_game = rhs.m_game;

    m_battleUnit = rhs.m_battleUnit;
    m_heroStats = rhs.m_heroStats;
    m_heroStatsInitializer = rhs.m_heroStatsInitializer;
    m_hiredHero = rhs.m_hiredHero;
    m_unit = rhs.m_unit;
    m_unitType = rhs.m_unitType;
    m_bonuses = rhs.m_bonuses;
    m_dnSpells = rhs.m_dnSpells;
    m_penalties = rhs.m_penalties;
    m_upAbilities = rhs.m_upAbilities;
    m_upItems = rhs.m_upItems;
    m_upLevel = rhs.m_upLevel;
    m_upSpells = rhs.m_upSpells;
    m_upWeaponType = rhs.m_upWeaponType;
}

void MoMUnit::close()
{
    zeroFields();
}

void MoMUnit::changeUnit(Battle_Unit *battleUnit)
{
    close();

    m_battleUnit = battleUnit;

    if (0 != m_game)
    {
        if (0 != battleUnit)
        {
            m_unit = m_game->getUnit(battleUnit->m_unitNr);
        }

        if (0 != m_unit)
        {
            m_heroStats = m_game->getHeroStats(battleUnit->m_Owner, m_unit->m_Unit_Type);
            m_hiredHero = m_game->getHiredHero(m_unit);
            m_unitType = m_game->getUnitTypeData(m_unit->m_Unit_Type);
        }

        applyEffects();
    }
}

void MoMUnit::changeUnit(eUnit_Type unitTypeNr)
{
    close();

    if (0 != m_game)
    {
//        m_battleUnit = m_game->getBattle_Units();
        // TODO: playerNr is YOU if you want to summon, or otherwise it is unknown
        m_heroStats = m_game->getHeroStats(MoM::PLAYER_YOU, unitTypeNr);
//        m_heroStatsInitializer = 0;
//        m_hiredHero = 0;
        m_unitType = m_game->getUnitTypeData(unitTypeNr);

        applyEffects();
    }
}

void MoMUnit::changeUnit(Hired_Hero *hiredHero)
{
    close();

    m_hiredHero = hiredHero;

    if (0 != m_game)
    {
        ePlayer playerNr = m_game->getPlayerNr(m_hiredHero);

        if (0 != hiredHero)
        {
            m_unit = m_game->getUnit(hiredHero->m_Unit_Nr);
        }
        if (0 != m_unit)
        {
            m_unitType = m_game->getUnitTypeData(m_unit->m_Unit_Type);
            m_heroStats = m_game->getHeroStats(playerNr, m_unit->m_Unit_Type);
        }
//        m_battleUnit = m_game->getBattle_Units();

        applyEffects();
    }
}

void MoMUnit::changeUnit(Hero_stats *heroStats)
{
    close();

    m_heroStats = heroStats;

    if (0 != m_game)
    {
        MoM::ePlayer playerNr = m_game->getPlayerNr(m_heroStats);
        MoM::eUnit_Type unitTypeNr = m_game->getUnitTypeNr(m_heroStats);
        m_unitType = m_game->getUnitTypeData(unitTypeNr);

        for (unsigned slotNr = 0; slotNr < gMAX_HIRED_HEROES; ++slotNr)
        {
            Hired_Hero* hiredHero = m_game->getHiredHero(playerNr, slotNr);
            if (0 == hiredHero)
                continue;
            Unit* unit = m_game->getUnit(hiredHero->m_Unit_Nr);
            if (0 == unit)
                continue;
            if (unit->m_Unit_Type == unitTypeNr)
            {
                m_hiredHero = hiredHero;
                m_unit = unit;
            }
        }
//        m_battleUnit = m_game->getBattle_Units();

        applyEffects();
    }
}

void MoMUnit::changeUnit(Hero_Stats_Initializer *heroStatsInitializer)
{
    close();

    m_heroStatsInitializer = heroStatsInitializer;

    if (0 != m_game)
    {
        applyEffects();
    }
}

void MoMUnit::changeUnit(Unit_Type_Data* unitType)
{
    close();

    m_unitType = unitType;

    if (0 != m_game)
    {
        MoM::eUnit_Type unitTypeNr = m_game->getUnitTypeNr(unitType);

//        m_battleUnit = m_game->getBattle_Units();
        m_heroStats = m_game->getHeroStats(MoM::PLAYER_YOU, unitTypeNr);
//        m_heroStatsInitializer = 0;
//        m_hiredHero = 0;

        applyEffects();
    }
}

void MoMUnit::changeUnit(Unit* unit)
{
    close();

    m_unit = unit;

    if (0 != m_game)
    {
        MoM::eUnit_Type unitTypeNr = (eUnit_Type)-1;
        if (0 != m_unit)
        {
            unitTypeNr = m_unit->m_Unit_Type;
        }

//        m_battleUnit = m_game->getBattle_Units();
        m_heroStats = m_game->getHeroStats(MoM::PLAYER_YOU, unitTypeNr);
        m_hiredHero = m_game->getHiredHero(unit);
        m_unitType = m_game->getUnitTypeData(unitTypeNr);

        applyEffects();
    }
}


Battle_Unit MoMUnit::getBattleUnit() const
{
    Battle_Unit value;
    memset(&value, '\0', sizeof(value));
    if (0 != m_battleUnit)
    {
        value = *m_battleUnit;
    }
    return value;
}
Hero_stats MoMUnit::getHeroStats() const
{
    Hero_stats value;
    memset(&value, '\0', sizeof(value));
    if (0 != m_heroStats)
    {
        value = *m_heroStats;
    }
    return value;
}
Hero_Stats_Initializer MoMUnit::getHeroStatsInitializer() const
{
    Hero_Stats_Initializer value;
    memset(&value, '\0', sizeof(value));
    if (0 != m_heroStatsInitializer)
    {
        value = *m_heroStatsInitializer;
    }
    return value;
}
Hired_Hero MoMUnit::getHiredHero() const
{
    Hired_Hero value;
    memset(&value, '\0', sizeof(value));
    if (0 != m_hiredHero)
    {
        value = *m_hiredHero;
    }
    return value;
}
Unit MoMUnit::getUnitInGame() const
{
    Unit value;
    memset(&value, '\0', sizeof(value));
    value.m_Hero_Slot_Number = -1;
    if (0 != m_unit)
    {
        value = *m_unit;
    }
    return value;
}
Unit_Type_Data MoMUnit::getUnitTypeData() const
{
    Unit_Type_Data value;
    memset(&value, '\0', sizeof(value));
    if (0 != m_unitType)
    {
        value = *m_unitType;
    }
    return value;
}



//
// OTHER
//

int MoMUnit::calcGoldUpkeep() const
{
    if (getUnitInGame().m_Weapon_Mutation.s.Undead)
    {
        return 0;
    }

    int goldUpkeep = 0;
    if (isNormal())
    {
        goldUpkeep = getUnitTypeData().m_Upkeep;
    }
    else if (isHero() && !hasHeroAbility(HEROABILITY_Noble))
    {
        goldUpkeep = getUnitTypeData().m_Upkeep;
    }

    // TODO: Computer players get a reduction depending on difficulty

    return goldUpkeep;
}

int MoMUnit::calcManaUpkeep() const
{
    int manaUpkeep = 0;
    if (isSummoned() || (getUnitTypeNr() == UNITTYPE_Chosen))
    {
        manaUpkeep = getUnitTypeData().m_Upkeep;
    }

    if (getUnitInGame().m_Weapon_Mutation.s.Undead)
    {
        manaUpkeep *= 2;
    }

    int enchantmentsUpkeep = 0;
    MoM::Upkeep_Enchantments* upkeepEnchantments = 0;
    if (m_game)
    {
        if (0 != m_game->getDataSegment())
        {
            upkeepEnchantments = &m_game->getDataSegment()->m_Upkeep_Enchantments;
        }
    }
    MOM_FOREACH(eUnitEnchantment, unitEnchantment, eUnitEnchantment_MAX)
    {
        if ((0 != upkeepEnchantments) && hasUnitEnchantment(unitEnchantment))
        {
            enchantmentsUpkeep += (&upkeepEnchantments->Immolation)[unitEnchantment];
        }
    }

    manaUpkeep += enchantmentsUpkeep;

    // TODO: Computer players get a reduction depending on difficulty

    return manaUpkeep;
}

MoMUnit::BaseAttributes MoMUnit::getActualAttributes() const
{
    BaseAttributes base = getBaseAttributes();
    BaseAttributes bonus = m_bonuses;
//    BaseAttributes penalty = m_penalties;
    BaseAttributes actual = base;
    bonus.addBonus(m_upAbilities);
    bonus.addBonus(m_upLevel);

    if (base.melee != 0)        actual.melee        += bonus.melee;
    if (base.defense != 0)      actual.defense      += bonus.defense;
    if (base.ranged != 0)       actual.ranged       += bonus.ranged;
    if (base.resistance != 0)   actual.resistance   += bonus.resistance;
    if (base.hitpoints != 0)    actual.hitpoints    += bonus.hitpoints;
    actual.toHitMelee   += bonus.toHitMelee;
    actual.toHitRanged  += bonus.toHitRanged;
    actual.toDefend     += bonus.toDefend;
    actual.moves        += bonus.moves;

    // TODO:
//    if (0 != m_unit)
//    {
//        actual.moves = m_unit->m_Moves_Total / 2.0;
//    }

    return actual;
}

MoMUnit::BaseAttributes MoMUnit::getBaseAttributes() const
{
    BaseAttributes base;

    if (0 != m_unitType)
    {
        base.melee = m_unitType->m_Melee;
		base.defense = m_unitType->m_Defense;
        base.ranged = m_unitType->m_Ranged;
        base.resistance = m_unitType->m_Resistance;
        base.hitpoints = m_unitType->m_Hitpoints;
        base.toHitMelee = m_unitType->m_To_Hit;
        base.toHitRanged = m_unitType->m_To_Hit;
        base.toDefend = 0;
        base.moves = m_unitType->m_MoveHalves / 2.0;
    }

    return base;
}

MoMUnit::BaseAttributes MoMUnit::getBonusAttributes() const
{
    return m_bonuses;
}

std::string MoMUnit::getDisplayName() const
{
    std::string raceName = getRaceName();
    std::string unitName = getUnitName();

    std::string name = raceName + " " + unitName;

    return name;
}

int MoMUnit::getGazeModifier() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Gaze_Modifier;
    }
    return value;
}

std::string MoMUnit::getHeroName() const
{
    std::string name;
    if (0 != m_hiredHero)
    {
        name = m_hiredHero->m_Hero_name;
    }
    return name;
}

MoMUnit::ListSpells MoMUnit::getHeroSpells() const
{
    ListSpells value;
    if (0 != m_heroStats)
    {
        for(size_t i = 0; i < ARRAYSIZE(m_heroStats->m_Spell); ++i)
        {
            if (m_heroStats->m_Spell[i] != SPELL_None)
            {
                value.push_back(m_heroStats->m_Spell[i]);
            }
        }
    }
    else if (0 != m_heroStatsInitializer)
    {
        for(size_t i = 0; i < ARRAYSIZE(m_heroStatsInitializer->m_Spell); ++i)
        {
            if (m_heroStatsInitializer->m_Spell[i] != SPELL16_None)
            {
                value.push_back((eSpell)m_heroStatsInitializer->m_Spell[i]);
            }
        }
    }
    else
    {
        // Nothing to do
    }
    return value;
}

eHero_TypeCode MoMUnit::getHeroTypeCode() const
{
    eHero_TypeCode value = (eHero_TypeCode)-1;
    if ((0 != m_unitType) && isHero())
    {
        value = m_unitType->m_Building2_or_HeroType;
    }
    return value;
}

//MoMUnit::MapSpecials MoMUnit::getItemEffects() const
//{
//    MapSpecials mapSpecials;

//    for (int itemSlotNr = 0; toUInt(itemSlotNr) < gMAX_ITEMSLOTS; ++itemSlotNr)
//    {
//        if ((0 == m_hiredHero) || (0 == m_game))
//            break;
//        MoM::Item* item = m_game->getItem(m_hiredHero->m_Items_In_Slot[itemSlotNr]);
//        if (0 == item)
//            continue;

//        // TODO
////        ADDMFIELDFEATURE(mapSpecials, item, Item_Name);
////        ADDMFIELDFEATURE(mapSpecials, item, Icon);
////        ADDMFIELDFEATURE(mapSpecials, item, Slot_Required);
////        ADDMFIELDFEATURE(mapSpecials, item, Item_Type);
////        ADDMFIELDFEATURE(mapSpecials, item, Cost);

////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, Attack);
////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, To_Hit);
////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, Defense);
////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, Movement_in_halves);
////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, Resistance);
////        ADDFIELDFEATURE(mapSpecials, (int)item->m_Bonuses, Spell_Points);
////        ADDFIELDFEATURE(mapSpecials, -(int)item->m_Bonuses, Spell_Save);

////        ADDMFIELDFEATURE(mapSpecials, item, Spell_Number_Charged);
////        ADDMFIELDFEATURE(mapSpecials, item, Number_Of_Charges);

//    }

//    return mapSpecials;
//}

Item* MoMUnit::getSlotItem(int itemSlotNr) const
{
    Item* item = 0;
    if ((0 != m_hiredHero) && (0 != m_game))
    {
        int itemNr = m_hiredHero->m_Items_In_Slot[itemSlotNr];
        item = m_game->getItem(itemNr);
    }
    return item;
}

eSlot_Type16 MoMUnit::getSlotType(int itemSlotNr) const
{
    eSlot_Type16 value = (eSlot_Type16)0;
    if ((0 != m_hiredHero) && (toUInt(itemSlotNr) < gMAX_ITEMSLOTS))
    {
        value = m_hiredHero->m_Slot_Types[itemSlotNr];
    }
    else if ((0 != m_unitType) && isHero() && (0 != m_game))
    {
        // Retrieve the slot types
        eSlot_Type16 heroSlotTypes[3];
        m_game->getHeroSlotTypes(m_unitType->m_Building2_or_HeroType, heroSlotTypes);
        if (toUInt(itemSlotNr) < 3)
        {
            value = heroSlotTypes[itemSlotNr];
        }
    }
    return value;
}

int MoMUnit::getLevel() const
{
    int value = 0;
    if (0 != m_unit)
    {
        value = 1 + m_unit->m_Level;
    }
    else if (0 != m_heroStats)
    {
        value = static_cast<int16_t>(m_heroStats->m_Level_Status);
		if (m_heroStats->m_Level_Status == HEROLEVELSTATUS_Active_in_Wizards_army)
		{
			value = 0;
		}
        else if (value < 0)
        {
            value = -value;
        }
        else
        {
            value++;
        }
    }
    return value;
}

std::string MoMUnit::getLevelName() const
{
    std::string name;
    MoMDataSegment* dataSegment = 0;
    int level = getLevel();
    if ((0 != m_game) && (level > 0) && (0 != (dataSegment = m_game->getDataSegment())))
    {
        level--;
        if (isHero() && (toUInt(level) < ARRAYSIZE(dataSegment->m_Offsets_HeroLevelNames)))
        {
            name = m_game->getNameByOffset(dataSegment->m_Offsets_HeroLevelNames[level]);
        }
        else if (toUInt(level) < ARRAYSIZE(dataSegment->m_Offsets_UnitLevelNames))
        {
            name = m_game->getNameByOffset(dataSegment->m_Offsets_UnitLevelNames[level]);
        }
    }
    return name;
}

// TODO: Eliminate using getActualAttributes() instead
double MoMUnit::getMoves() const
{
    double value = 0;
    if (0 != m_unit)
    {
        value = m_unit->m_Moves_Total / 2.0;
    }
    else if (0 != m_unitType)
    {
        value = m_unitType->m_MoveHalves / 2.0;
    }
    return value;
}

int MoMUnit::getCastingSkillBase() const
{
    int value = 0;
    if (0 != m_heroStats)
    {
        value = m_heroStats->m_Hero_Casting_Skill;
    }
    else if (0 != m_heroStatsInitializer)
    {
        value = m_heroStatsInitializer->m_Hero_Casting_Skill;
    }
    else if (hasUnitAbility(UNITABILITY_Caster_40_MP))
    {
        value = 40;
    }
    else if (hasUnitAbility(UNITABILITY_Caster_20_MP))
    {
        value = 20;
    }
    else
    {
    }
    return value;
}

int MoMUnit::getCastingSkillTotal() const
{
    int value = getCastingSkillBase();
    if (isHero() && (getLevel() > 0))
    {
        value = static_cast<int>(getLevel() * (1 + getCastingSkillBase()) * 5 / 2);
    }
    return value;
}

int MoMUnit::getConstructionSkill() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Construction;
    }
    return value;
}

int MoMUnit::getCost() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Cost;
    }
    return value;
}

int MoMUnit::getCurFigures() const
{
    int value = 0;
    if (0 != m_battleUnit)
    {
        value = m_battleUnit->m_Current_figures;
    }
    else if (0 != m_unitType)
    {
        // TODO: process damage
        value = m_unitType->m_Nr_Figures;
    }
    else
    {
    }
    return value;
}

int MoMUnit::getDamage() const
{
    int value = 0;
    if (0 != m_battleUnit)
    {
        value = (m_battleUnit->m_Max_figures - m_battleUnit->m_Current_figures) * m_battleUnit->m_Hitpoints_per_Figure 
                + m_battleUnit->m_top_figure_damage; 
    }
    else if (0 != m_unit)
    {
        value = m_unit->m_Damage;
    }
    return value;
}

int MoMUnit::getHeroAbility(eHeroAbility heroAbility) const
{
    int bonus = 0;
    int level = getLevel();
    switch (heroAbility)
    {
    case HEROABILITY_Agility:          { bonus = level; break; }
    case HEROABILITY_Agility_X:        { bonus = static_cast<int>(level * 3 / 2); break; }
    case HEROABILITY_Arcane_Power:     { bonus = level; break; }
    case HEROABILITY_Arcane_Power_X:   { bonus = static_cast<int>(level * 3 / 2); break; }
    case HEROABILITY_Armsmaster:       { bonus = 2 * level; break; }
    case HEROABILITY_Armsmaster_X:     { bonus = static_cast<int>(2 * level * 3 / 2); break; }
    case HEROABILITY_Blademaster:      { bonus = static_cast<int>(level / 2); break; }
    case HEROABILITY_Blademaster_X:    { bonus = static_cast<int>(level * 3 / 4); break; }
    case HEROABILITY_Constitution:     { bonus = level; break; }
    case HEROABILITY_Constitution_X:   { bonus = static_cast<int>(level * 3 / 2); break; }
    case HEROABILITY_Leadership:       { bonus = static_cast<int>(level / 3); break; }
    case HEROABILITY_Leadership_X:     { bonus = static_cast<int>(level / 2); break; }
    case HEROABILITY_Legendary:        { bonus = 3 * level; break; }
    case HEROABILITY_Legendary_X:      { bonus = static_cast<int>(3 * level * 3 / 2); break; }
    case HEROABILITY_Lucky:            { break; }
    case HEROABILITY_Might:            { bonus = level; break; }
    case HEROABILITY_Might_X:          { bonus = static_cast<int>(level * 3 / 2); break; }
    case HEROABILITY_Noble:            { bonus = 10; break; }
    case HEROABILITY_Prayermaster:     { bonus = level; break; }
    case HEROABILITY_Prayermaster_X:   { bonus = static_cast<int>(level * 3 / 2); break; }
    case HEROABILITY_Sage:             { bonus = 3 * level; break; }
    case HEROABILITY_Sage_X:           { bonus = static_cast<int>(3 * level * 3 / 2); break; }
    default:                           { break; }
    }
    return bonus;
}

int MoMUnit::getMaxFigures() const
{
    int value = 0;
    if (0 != m_battleUnit)
    {
        value = m_battleUnit->m_Max_figures;
    }
    else if (0 != m_unitType)
    {
        value = m_unitType->m_Nr_Figures;
    }
    return value;
}

ePlayer MoMUnit::getOwner() const
{
    ePlayer value = (ePlayer)-1;
    if (0 != m_unit)
    {
        value = m_unit->m_Owner;
    }
    else if (0 != m_battleUnit)
    {
        value = m_battleUnit->m_Owner;
    }
    else
    {
    }
    return value;
}

MoMUnit::BaseAttributes MoMUnit::getPenaltyAttributes() const
{
    return m_penalties;
}

eRace MoMUnit::getRace() const
{
    eRace value = (eRace)0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Race_Code;
    }
    return value;
}

std::string MoMUnit::getRaceName() const
{
    std::string name;
    if ((0 != m_game) && (0 != m_unitType))
    {
        name = m_game->getRaceName(m_unitType->m_Race_Code);
    }
    return name;
}

int MoMUnit::getMaxRangedShots() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Ranged_Shots;
    }
    return value;
}

eRanged_Type MoMUnit::getRangedType() const
{
    eRanged_Type value = MoM::RANGED_None;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Ranged_Type;
    }
    return value;
}

MoMUnit::ListBuildings MoMUnit::getRequiredBuildings() const
{
    ListBuildings value;

    if ((0 != m_unitType) && isNormal())
    {
        if ((toUInt(m_unitType->m_Building1Required_or_PortraitLbxIndex) >= BUILDING_Barracks)
            && (toUInt(m_unitType->m_Building1Required_or_PortraitLbxIndex) < eBuilding_array_MAX))
        {
            value.push_back((eBuilding)m_unitType->m_Building1Required_or_PortraitLbxIndex);
        }
        if ((toUInt(m_unitType->m_Building2_or_HeroType) >= BUILDING_Barracks)
            && (toUInt(m_unitType->m_Building2_or_HeroType) < eBuilding_array_MAX))
        {
            value.push_back((eBuilding)m_unitType->m_Building2_or_HeroType);
        }
    }
    return value;
}

int MoMUnit::getScouting() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Scouting;
    }
    return value;
}

int MoMUnit::getTransportCapacity() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Transport_Capacity;
    }
    return value;
}

int MoMUnit::getUnitAbility(eUnitAbility unitAbility) const
{
    int bonus = 0;
    if (0 == m_unitType)
        return 0;

    switch (unitAbility)
    {
    // Positive bonuses
    case UNITABILITY_Holy_Bonus:
    case UNITABILITY_Resistance_to_All:
    case UNITABILITY_Poison_attack:
        bonus = m_unitType->m_Gaze_Modifier;
        break;

    case UNITABILITY_Immolation:
        bonus = 4;
        break;

    // Penalties
    case UNITABILITY_Life_Stealing:
    case UNITABILITY_Stoning_Touch:
    case UNITABILITY_Death_Touch:
    case UNITABILITY_Power_Drain:
    case UNITABILITY_Dispel_Evil:
        bonus = m_unitType->m_Gaze_Modifier;
        break;

    default:
        break;
    }
    return bonus;
}

std::string MoMUnit::getUnitName() const
{
    std::string name;
    if ((0 != m_unitType) && (0 != m_game))
    {
        const char* ptrName = m_game->getNameByOffset(m_unitType->m_PtrName);
        if (0 != ptrName)
        {
            name = ptrName;
        }
    }
    return name;
}

eUnit_Type MoMUnit::getUnitTypeNr() const
{
    eUnit_Type value = (MoM::eUnit_Type)-1;
    if ((0 != m_unit))
    {
        value = m_unit->m_Unit_Type;
    }
    else if ((0 != m_unitType) && (0 != m_game))
    {
        MoM::MoMGameBase* game = const_cast<MoM::MoMGameBase*>(m_game);
        value = game->getUnitTypeNr(m_unitType);
    }
    else if ((0 != m_heroStatsInitializer) && (0 != m_game))
    {
        MoM::MoMGameBase* game = const_cast<MoM::MoMGameBase*>(m_game);
        value = game->getUnitTypeNr(m_heroStatsInitializer);
    }
    else
    {
    }
    return value;
}

int MoMUnit::getUpkeep() const
{
    int value = 0;
    if (0 != m_unitType)
    {
        value = m_unitType->m_Upkeep;
    }
    return value;
}

eWeaponType MoMUnit::getWeaponType() const
{
    eWeaponType value = MoM::WEAPON_normal;
    if (0 != m_unit)
    {
        value = static_cast<eWeaponType>(m_unit->m_Weapon_Mutation.s.Weapon_Type);
    }
    return value;
}

int MoMUnit::getXP() const
{
    int value = 0;
    if (0 != m_unit)
    {
        value = m_unit->m_Experience;
    }
    return value;
}

bool MoMUnit::hasFireBreath() const
{
    return ((getRangedType() == MoM::RANGED_Fire_Breath) || hasMutation(UNITMUTATION_Chaos_Channels_Fiery_Breath));
}

bool MoMUnit::hasIllusionaryAttack() const
{
    return (hasUnitAbility(UNITABILITY_Illusionary_attack) || hasItemPower(ITEMPOWER_Phantasmal));
}

bool MoMUnit::hasImmolation() const
{
    return (hasUnitEnchantment(UNITENCHANTMENT_Immolation) || hasUnitAbility(UNITABILITY_Immolation));
}

bool MoMUnit::hasLightningBreath() const
{
    return ((getRangedType() == MoM::RANGED_Lightning_Breath) /*|| hasItemPower(ITEMPOWER_Lightning)*/);
}

bool MoMUnit::hasMagicalBreathAttack() const
{
    bool value = (hasFireBreath() || hasLightningBreath());
    return value;
}

bool MoMUnit::hasMagicalGazeAttack() const
{
    eRanged_Type rangedType = getRangedType();
    bool value = ((rangedType == MoM::RANGED_Stoning_Gaze)
                  || (rangedType == MoM::RANGED_Multiple_Gaze)
                  || (rangedType == MoM::RANGED_Death_Gaze));
    return value;
}

bool MoMUnit::hasMagicalRangedAttack() const
{
    eRanged_Type rangedType = getRangedType();
    bool value = ((rangedType >= MoM::RANGED_Chaos_Lightning)
                  && (rangedType < MoM::RANGED_Thrown_Weapons));
    return value;
}

bool MoMUnit::hasMissileRangedAttack() const
{
    eRanged_Type rangedType = getRangedType();
    bool value = ((rangedType == MoM::RANGED_Arrow)
                  || (rangedType == MoM::RANGED_Bullet)
                  || (rangedType == MoM::RANGED_Rock));
    return value;
}

bool MoMUnit::hasPhysicalRangedAttack() const
{
    bool value = (hasMissileRangedAttack() || hasThrownRangedAttack());
    return value;
}

bool MoMUnit::hasThrownRangedAttack() const
{
    eRanged_Type rangedType = getRangedType();
    bool value = (rangedType == MoM::RANGED_Thrown_Weapons);
    return value;
}

bool MoMUnit::hasCombatEnchantment(eCombatEnchantment combatEnchantment) const
{
    bool value = false;
    if (m_battleUnit != 0)
    {
        value = (1 & (m_battleUnit->m_Flags_Combat_Enchantment.bits >> combatEnchantment));
    }
    return value;
}

bool MoMUnit::hasHeroAbility(eHeroAbility heroAbility) const
{
    bool value = false;
    if (m_heroStats != 0)
    {
        value = (1 & (m_heroStats->m_Hero_Abilities.bits >> heroAbility));
    }
    else if (m_heroStatsInitializer != 0)
    {
        value = (1 & (m_heroStatsInitializer->m_Hero_Abilities.bits >> heroAbility));
    }
    else
    {
        // No hero information
    }
    return value;
}

bool MoMUnit::hasItemPower(eItemPower itemPower) const
{
    bool value = false;
    if ((m_game != 0) && (m_hiredHero != 0))
    {
        for (size_t slotNr = 0; !value && (slotNr < gMAX_ITEMSLOTS); ++slotNr)
        {
            int16_t itemNr = m_hiredHero->m_Items_In_Slot[slotNr];
            Item* item = m_game->getItem(itemNr);
            if (item != 0)
            {
                value = (1 & (item->m_Bitmask_Powers.bits >> itemPower));
            }
        }
    }
    return value;
}

bool MoMUnit::hasMagicWeapon() const
{
    bool value = false;

    if (!isNormal())
    {
        value = true;
    }
    else if ((getWeaponType() == WEAPON_magic) || (getWeaponType() == WEAPON_mithril) || (getWeaponType() == WEAPON_mithril))
    {
        value = true;
    }
    else if (hasUnitEnchantment(UNITENCHANTMENT_Eldritch_Weapon) || hasUnitAbility(UNITABILITY_Eldritch_Weapon_COMBAT)
             || hasUnitEnchantment(UNITENCHANTMENT_Flame_Blade) || hasItemPower(ITEMPOWER_Flaming)
             || hasUnitEnchantment(UNITENCHANTMENT_Holy_Weapon))
    {
        //TODO: GLOBALENCHANTMENT_Holy_Arms
        //      BATTLEENCHANTMENT_Metal_Fires
        value = true;
    }
    else
    {
        value = false;
    }
    return value;
}

bool MoMUnit::hasUnitAbility(eUnitAbility unitAbility) const
{
    bool value = false;
    if (m_unitType != 0)
    {
        unsigned offset = unitAbility / 8;
        unsigned shift = unitAbility % 8;
        uint8_t* ptr = &m_unitType->m_Movement_Flags.bits;
        value = (1 & (ptr[offset] >> shift));
    }
    if (m_battleUnit != 0)
    {
        unsigned offset = unitAbility / 8;
        unsigned shift = unitAbility % 8;
        uint8_t* ptr = &m_battleUnit->m_Movement_Flags.bits;
        value |= (1 & (ptr[offset] >> shift));

        if ((unitAbility >= UNITABILITY_Armor_Piercing) && (unitAbility <= UNITABILITY_Warp_Lightning_COMBAT))
        {
            unsigned shift = ((unsigned)unitAbility - (unsigned)UNITABILITY_Armor_Piercing);
            value |= (1 & (m_battleUnit->m_Cur_Attack_Flags.bits >> shift));
            value |= (1 & (m_battleUnit->m_Item_Attack_Flags.bits >> shift));
        }
    }
    return value;
}

bool MoMUnit::hasUnitEnchantment(eUnitEnchantment unitEnchantment) const
{
    bool value = false;
    if (m_unit != 0)
    {
        value = (1 & (m_unit->m_Unit_Enchantment.bits >> unitEnchantment));
    }
    if (m_battleUnit != 0)
    {
        value |= (1 & (m_battleUnit->m_Flags1_UnitEnchantment.bits >> unitEnchantment));
        value |= (1 & (m_battleUnit->m_Item_UnitEnchantment.bits >> unitEnchantment));
    }
    return value;
}

bool MoMUnit::hasMutation(eUnitMutation unitMutation) const
{
    bool value = false;
    if ((m_unit != 0) && (unitMutation >= UNITMUTATION_Chaos_Channels_Demon_Skin))
    {
        value = (1 & (m_unit->m_Weapon_Mutation.bits >> unitMutation));
    }
    return value;
}

bool MoMUnit::isBuildable(const City &city) const
{
    if ((m_unitType->m_Race_Code != city.m_Race) && (m_unitType->m_Race_Code != RACE_Generic))
    {
        return false;   // Wrong race and not generic
    }
    if (m_unitType->m_Building1Required_or_PortraitLbxIndex == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(m_unitType->m_Building1Required_or_PortraitLbxIndex) < eBuilding_array_MAX)
             && (city.m_Building_Status.a[m_unitType->m_Building1Required_or_PortraitLbxIndex] != BUILDINGSTATUS_Built)
             && (city.m_Building_Status.a[m_unitType->m_Building1Required_or_PortraitLbxIndex] != BUILDINGSTATUS_Replaced))
    {
        return false;   // First required building not present
    }
    else if (static_cast<eBuilding>(m_unitType->m_Building2_or_HeroType) == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(m_unitType->m_Building2_or_HeroType) < eBuilding_array_MAX)
             && (city.m_Building_Status.a[m_unitType->m_Building2_or_HeroType] != BUILDINGSTATUS_Built)
             && (city.m_Building_Status.a[m_unitType->m_Building2_or_HeroType] != BUILDINGSTATUS_Replaced))
    {
        return false;   // Second required building not present
    }

    return true;
}

bool MoMUnit::isCaster() const
{
    return (getCastingSkillBase() > 0);
}

bool MoMUnit::isColor(eRealm_Type color) const
{
    bool value = false;

    // TODO: Can a unit have more than 1 color?

    if (0 != m_unitType)
    {
        switch (m_unitType->m_Race_Code)
        {
        case RACE_Arcane:   value = (color == REALM_Arcane); break;
        case RACE_Nature:   value = (color == REALM_Nature); break;
        case RACE_Sorcery:  value = (color == REALM_Sorcery); break;
        case RACE_Chaos:    value = (color == REALM_Chaos); break;
        case RACE_Life:     value = (color == REALM_Life); break;
        case RACE_Death:    value = (color == REALM_Death); break;
        default:            break;
        }
    }

    if (hasUnitEnchantment(UNITENCHANTMENT_Black_Channels)
        || hasMutation(UNITMUTATION_Undead))
    {
        value = (color == REALM_Death);
    }

    if (hasMutation(UNITMUTATION_Chaos_Channels_Demon_Skin)
            || hasMutation(UNITMUTATION_Chaos_Channels_Demon_Wings)
            || hasMutation(UNITMUTATION_Chaos_Channels_Fiery_Breath))
    {
        value = (color == REALM_Chaos);
    }

    return value;
}

bool MoMUnit::isFlying() const
{
    bool hasFlying = (hasUnitAbility(UNITABILITY_Flying) || hasUnitAbility(UNITABILITY_Flyer)
                      || hasItemPower(ITEMPOWER_Flight) || hasUnitEnchantment(UNITENCHANTMENT_Flight)
                      || hasMutation(UNITMUTATION_Chaos_Channels_Demon_Wings));
    bool isWebbed = hasCombatEnchantment(COMBATENCHANTMENT_Web);
    if (0 != m_battleUnit)
    {
        isWebbed |= (m_battleUnit->m_web_strength != 0);
    }
    return (hasFlying && !isWebbed);
}

bool MoMUnit::isGeneric() const
{
    return (getUnitTypeNr() >= UNITTYPE_Trireme)
            && (getUnitTypeNr() <= UNITTYPE_Warship);
}

bool MoMUnit::isHasted() const
{
    return (hasCombatEnchantment(COMBATENCHANTMENT_Haste)
                 || hasItemPower(ITEMPOWER_Haste));
}

bool MoMUnit::isHero() const
{
	// TODO: A hero only counts as a hero if he occupies a hired hero slot.
    return (toUInt(getUnitTypeNr()) < gMAX_HERO_TYPES);
}

bool MoMUnit::isImmune(eUnitAbility immunity) const
{
    bool value = false;
    if ((immunity >= UNITABILITY_Fire_Immunity) && (immunity <= UNITABILITY_Weapon_Immunity))
    {
        value = hasUnitAbility(immunity);
    }

    switch (immunity)
    {
    case UNITABILITY_Fire_Immunity:
    case UNITABILITY_Stoning_Immunity:
    case UNITABILITY_Cold_Immunity:
    case UNITABILITY_Death_Immunity:
        value |= (       hasUnitAbility(UNITABILITY_Magic_Immunity)
                  || hasUnitEnchantment(UNITENCHANTMENT_Magic_Immunity));
        break;

    case UNITABILITY_Magic_Immunity:
        value |= hasUnitEnchantment(UNITENCHANTMENT_Magic_Immunity);
        break;

    case UNITABILITY_Illusions_Immunity:
        value |= (hasUnitEnchantment(UNITENCHANTMENT_True_Sight)
                     || hasItemPower(ITEMPOWER_True_Sight));
        break;

    case UNITABILITY_Poison_Immunity:
        // TODO: ?
        break;

    case UNITABILITY_Missiles_Immunity:
        value |= (hasUnitEnchantment(UNITENCHANTMENT_Guardian_Wind)
                     || hasItemPower(ITEMPOWER_Guardian_Wind));
        break;

    case UNITABILITY_Weapon_Immunity:
        value |= isInvulnerable();
        value |= (hasUnitEnchantment(UNITENCHANTMENT_Wraith_Form)
                     || hasItemPower(ITEMPOWER_Wraith_Form));
        break;

    default:
        break;
    }

    return value;
}

bool MoMUnit::isInvisible() const
{
    return (hasUnitAbility(UNITABILITY_Invisibility) || hasItemPower(ITEMPOWER_Invisibility) || hasUnitEnchantment(UNITENCHANTMENT_Invisibility));
    // TODO:        BATTLEENCHANTMENT_Mass_Invisibility
}

bool MoMUnit::isInvulnerable() const
{
    return (hasUnitEnchantment(UNITENCHANTMENT_Invulnerability)
               || hasItemPower(ITEMPOWER_Invulnerability));
}

bool MoMUnit::isNormal() const
{
    return ((toUInt(getUnitTypeNr()) >= gMAX_HERO_TYPES)
            && (getUnitTypeNr() < UNITTYPE_Arcane_Magic_Spirit));
}

bool MoMUnit::isSettler() const
{
    return hasUnitAbility(UNITABILITY_Create_Outpost);
}

bool MoMUnit::isShip() const
{
    return hasUnitAbility(UNITABILITY_Sailing);
}

bool MoMUnit::isSummoned() const
{
    return (getUnitTypeNr() >= UNITTYPE_Arcane_Magic_Spirit);
}

void MoMUnit::setGame(MoMGameBase* game)
{
    close();
    m_game = game;
}

void MoMUnit::applyEffects(const Spells_Cast_in_Battle* battleSpells)
{
    applyAbilities();
    applyItems();
    applyLevel();
    applyWeaponType();
    applySpells();
    applyBattleSpells(battleSpells);

//    // Check differences with Battle_Unit if available
//    if (0 != m_battleUnit)
//    {
//        BaseAttributes actualAttr = getActualAttributes();
//		std::cout << "Verify battle unit fields" << std::endl;
//		if (actualAttr.melee != m_battleUnit->m_Melee || this->m_bonuses.melee != m_battleUnit->m_Extra_Melee)
//		{
//            std::cout << "Melee calc=" << actualAttr.melee << " bonus=" << m_bonuses.melee << " battleUnit=" << (int)m_battleUnit->m_Melee << " battle.extraMe=" << (int)m_battleUnit->m_Extra_Melee << std::endl;
//		}
//		if (actualAttr.ranged != m_battleUnit->m_Ranged || this->m_bonuses.ranged != m_battleUnit->m_Extra_Ranged)
//		{
//            std::cout << "Ranged calc=" << actualAttr.ranged << " bonus=" << m_bonuses.ranged << " battleUnit=" << (int)m_battleUnit->m_Ranged << " battle.extraRa=" << (int)m_battleUnit->m_Extra_Ranged << std::endl;
//		}
//		if (actualAttr.defense != m_battleUnit->m_Defense || this->m_bonuses.defense != m_battleUnit->m_Extra_Defense)
//		{
//            std::cout << "Defense calc=" << actualAttr.defense << " bonus=" << m_bonuses.defense << " battleUnit=" << (int)m_battleUnit->m_Defense << " battle.extraDf=" << (int)m_battleUnit->m_Extra_Defense << std::endl;
//		}
//		if (actualAttr.toHitMelee != m_battleUnit->m_To_Hit + m_battleUnit->m_Extra_ToHit_Melee || this->m_bonuses.toHitMelee != m_battleUnit->m_Extra_ToHit_Melee)
//		{
//            std::cout << "ToHit Melee calc=" << actualAttr.toHitMelee << " bonus=" << m_bonuses.toHitMelee << " battleUnit=" << (int)m_battleUnit->m_To_Hit << " battle.extraToMe=" << (int)m_battleUnit->m_Extra_ToHit_Melee << std::endl;
//		}
//		if (actualAttr.toHitRanged != m_battleUnit->m_To_Hit + m_battleUnit->m_Extra_ToHit_Ranged || this->m_bonuses.toHitRanged != m_battleUnit->m_Extra_ToHit_Ranged)
//		{
//            std::cout << "ToHit Ranged calc=" << actualAttr.toHitRanged << " bonus=" << m_bonuses.toHitRanged << " battleUnit=" << (int)m_battleUnit->m_To_Hit << " battle.extraToRa=" << (int)m_battleUnit->m_Extra_ToHit_Ranged << std::endl;
//		}
//		if (actualAttr.toDefend != m_battleUnit->m_Extra_ToDefend)
//		{
//            std::cout << "ToDefend calc=" << actualAttr.toDefend << " bonus=" << m_bonuses.toDefend << " battleUnit=N/A" << " battle.extraToDf=" << (int)m_battleUnit->m_Extra_ToDefend << std::endl;
//		}
//		if (actualAttr.moves != m_battleUnit->m_MoveHalves / 2.0)
//		{
//			std::cout << "Moves calc=" << actualAttr.moves << " get=" << getMoves() << " battleUnit=" << m_battleUnit->m_MoveHalves / 2.0 << std::endl;
//		}
//		if (getGazeModifier() != m_battleUnit->m_Gaze_Modifier)
//		{
//            std::cout << "Gaze calc=" << getGazeModifier() << " battleUnit=" << (int)m_battleUnit->m_Gaze_Modifier << std::endl;
//		}
//    }
}

void MoMUnit::applyAbilities()
{
    m_upAbilities = BaseAttributes();
    BaseAttributes& up = m_upAbilities;
    BaseAttributes up_gold;
    int level = getLevel();
    int bonus = 0;
    int holy_bonus = 0;
    int prayer_bonus = 0;

    // Hero specific
    if (hasHeroAbility(HEROABILITY_Agility))        { up.defense += bonus = level; }
    if (hasHeroAbility(HEROABILITY_Agility_X))      { up.defense += bonus = static_cast<int>(level * 3 / 2); }
    if (hasHeroAbility(HEROABILITY_Arcane_Power))   { up.ranged += bonus = level; }
    if (hasHeroAbility(HEROABILITY_Arcane_Power_X)) { up.ranged += bonus = static_cast<int>(level * 3 / 2); }
    if (hasHeroAbility(HEROABILITY_Armsmaster))     { bonus = 2 * level; }
    if (hasHeroAbility(HEROABILITY_Armsmaster_X))   { bonus = static_cast<int>(2 * level * 3 / 2); }
    if (hasHeroAbility(HEROABILITY_Blademaster))    { up.toHitMelee += bonus = static_cast<int>(level / 2); up.toHitRanged += bonus; }
    if (hasHeroAbility(HEROABILITY_Blademaster_X))  { up.toHitMelee += bonus = static_cast<int>(level * 3 / 4); up.toHitRanged += bonus; }
    if (hasHeroAbility(HEROABILITY_Constitution))   { up.hitpoints += bonus = level; }
    if (hasHeroAbility(HEROABILITY_Constitution_X)) { up.hitpoints += bonus = static_cast<int>(level * 3 / 2); }
	// Treat Leadership bonus as gold bonus, since application depends on highest Leadership in group
    if (hasHeroAbility(HEROABILITY_Leadership))     { up_gold.melee += bonus = static_cast<int>(level / 3); if (hasMissileRangedAttack()) up_gold.ranged += static_cast<int>(bonus / 2); }
    if (hasHeroAbility(HEROABILITY_Leadership_X))   { up_gold.melee += bonus = static_cast<int>(level / 2); if (hasMissileRangedAttack()) up_gold.ranged += static_cast<int>(bonus / 2); }

    if (hasHeroAbility(HEROABILITY_Legendary))      { bonus = 3 * level; }
    if (hasHeroAbility(HEROABILITY_Legendary_X))    { bonus = static_cast<int>(3 * level * 3 / 2); }
    if (hasHeroAbility(HEROABILITY_Lucky)
        || hasUnitAbility(UNITABILITY_Lucky))       { up.toHitMelee += +1; up.toHitRanged += +1; up.toDefend += +1; up.resistance += +1; }
    if (hasHeroAbility(HEROABILITY_Might))          { up.melee += bonus = level; }
    if (hasHeroAbility(HEROABILITY_Might_X))        { up.melee += bonus = static_cast<int>(level * 3 / 2); }
    if (hasHeroAbility(HEROABILITY_Noble))          { ; }
    if (hasHeroAbility(HEROABILITY_Prayermaster))   { bonus = level; prayer_bonus = Max(prayer_bonus, bonus); }
    if (hasHeroAbility(HEROABILITY_Prayermaster_X)) { bonus = static_cast<int>(level * 3 / 2); prayer_bonus = Max(prayer_bonus, bonus); }
    if (hasHeroAbility(HEROABILITY_Sage))           { bonus = 3 * level; }
    if (hasHeroAbility(HEROABILITY_Sage_X))         { bonus = static_cast<int>(3 * level * 3 / 2);}

    // Unit specific
    if (hasUnitAbility(UNITABILITY_Holy_Bonus))     { bonus = getUnitAbility(UNITABILITY_Holy_Bonus); holy_bonus = Max(holy_bonus, bonus); }
    if (hasUnitAbility(UNITABILITY_Resistance_to_All)) { bonus = getUnitAbility(UNITABILITY_Resistance_to_All); prayer_bonus = Max(prayer_bonus, bonus); }

    // Process holy_bonus (highest applies)
    up_gold.melee += holy_bonus;
    up_gold.defense += holy_bonus;
    up_gold.resistance += holy_bonus;
    // Process prayer_bonus (highest applies)
    up_gold.resistance += prayer_bonus;

    // Add hero ability bonus where appropriate
    m_bonuses.addBonus(up_gold);
}

void MoMUnit::applyBattleSpells(const Spells_Cast_in_Battle *battleSpells)
{
    if (0 == battleSpells)
        return;

    // TODO: Figure out who's "us" and who's "them".
    int us = 0;
    int them = 1;

    m_dnSpells = BaseAttributes();
    m_upSpells = BaseAttributes();
    const BaseAttributes baseunit = getBaseAttributes();
    BaseAttributes& dn = m_dnSpells;
    BaseAttributes& up = m_upSpells;

    if (battleSpells->Black_Prayer[them])
    {
        if (baseunit.melee) dn.melee += +1;
        dn.defense += +1; dn.resistance += +2;
    }

    //if (enemy && enemy.spell_active("Blur") && !has("Illusion Imm"))
//    {
//        // TODO: Proper processing: ignore 10% of hits or something like that
//        dn.toHitMelee += +1;
//        dn.toHitRanged += +1;
//    }

    //        BATTLEENCHANTMENT_Call_Lightning,       // 12-13
    //        BATTLEENCHANTMENT_Counter_Magic,        // 14-15
    //        BATTLEENCHANTMENT_Entangle,             // 18-19

    if (battleSpells->High_Prayer[us])
    {
        if (baseunit.melee) up.melee += +2;
        up.defense += +2; up.resistance += +3; up.toHitMelee += +1; up.toHitRanged += +1; up.toDefend += +1;
    }
    else if (battleSpells->Prayer[us])
    {
        // Verified 2010-11-13 with [Tweaker] that Prayer and High Prayer do not stack
        up.toHitMelee += +1; up.toHitRanged += +1; up.toDefend += +1; up.resistance += +1;
    }

    // BATTLEENCHANTMENT_Mana_Leak,            // 1A-1B
    // uint8_t Mass_Invisibility[2];   // 16-17

    if (battleSpells->Metal_Fires[us])
    {
        if (isNormal() && !hasUnitEnchantment(UNITENCHANTMENT_Flame_Blade))
        {
            if (baseunit.melee) up.melee += +1;
            if (baseunit.ranged && ((getRangedType() == MoM::RANGED_Arrow) || (getRangedType() == MoM::RANGED_Bullet))) up.ranged += +1;
//            if (melee) add_special("Magic Weapon");      // Can hit creatures with Weapon Immunity
        }
    }

//    if (spell_active("Terror"))
    //        BATTLEENCHANTMENT_Terror,               // 10-11
//    {
//        // Each turn the unit must resist (Re +1) or cower in fear (cannot attack, but can still counter)
//    }

    if (battleSpells->True_Light[0])
    {
        if (isColor(REALM_Life))
        {
            if (baseunit.melee) up.melee += +1;
            up.defense += +1; up.resistance += +1;
        }
        else if (isColor(REALM_Death))
        {
            if (baseunit.melee) dn.melee += +1;
            dn.defense += +1; dn.resistance += +1;
        }
    }
    if (battleSpells->True_Light[1])
    {
        if (isColor(REALM_Life))
        {
            if (baseunit.melee) dn.melee += +1;
            dn.defense += +1; dn.resistance += +1;
        }
        else if (isColor(REALM_Death))
        {
            if (baseunit.melee) up.melee += +1;
            up.defense += +1; up.resistance += +1;
        }
    }

    if (battleSpells->Warp_Reality[us] || battleSpells->Warp_Reality[them])
    {
        if (!isColor(REALM_Chaos)) { dn.toHitMelee += +2; dn.toHitRanged += +2; }
    }

//    if (spell_active("Wrack"))
        //        BATTLEENCHANTMENT_Wrack,                // 08-09
//    {
//        // each enemy figure resists or loses 1 hp per turn
//    }

    // Apply spell modifiers where appropriate
    m_bonuses.addBonus(up);
    m_penalties.addBonus(dn);
}

void MoMUnit::applyItems()
{
    m_upItems = BaseAttributes();
    BaseAttributes& up = m_upItems;

    for (int itemSlotNr = 0; toUInt(itemSlotNr) < gMAX_ITEMSLOTS; ++itemSlotNr)
    {
        if ((0 == m_hiredHero) || (0 == m_game))
            break;
        MoM::Item* item = m_game->getItem(m_hiredHero->m_Items_In_Slot[itemSlotNr]);
        if (0 == item)
            continue;

        // TODO: Verify how bonus depends on type of artifact
        //       E.g. bow only gives bonus to ranged and toHitRanged
        //       and not to melee or toHitMelee.
        switch (item->m_Item_Type)
        {
        case ITEMTYPE_Sword:
        case ITEMTYPE_Mace:
        case ITEMTYPE_Axe:
            up.melee += item->m_Bonuses.Attack;
            up.toHitMelee += item->m_Bonuses.To_Hit;
            if (getRangedType() == RANGED_Thrown_Weapons)
            {
                up.ranged += item->m_Bonuses.Attack;
                up.toHitRanged += item->m_Bonuses.To_Hit;
            }
            break;
        case ITEMTYPE_Bow:
            // Presumably missile ranged attack
            up.ranged += item->m_Bonuses.Attack;
            up.toHitRanged += item->m_Bonuses.To_Hit;
            break;
        case ITEMTYPE_Staff:
        case ITEMTYPE_Wand:
            // Presumably magical ranged attack
            up.ranged += item->m_Bonuses.Attack;
            break;
        case ITEMTYPE_Various:
            up.melee += item->m_Bonuses.Attack;
            up.ranged += item->m_Bonuses.Attack;
            break;
        case ITEMTYPE_Shield:
        case ITEMTYPE_Chain_Mail:
        case ITEMTYPE_Plate_Mail:
            up.melee += item->m_Bonuses.Attack;
            up.ranged += item->m_Bonuses.Attack;
			// TODO: Add special "Large Shield"
            break;
        default:
            break;
        }

        up.defense += item->m_Bonuses.Defense;
        up.resistance += item->m_Bonuses.Resistance;
        up.hitpoints += 0;
        up.moves += item->m_Bonuses.Movement_in_halves / 2.0;
    }

    m_bonuses.addBonus(up);
}

void MoMUnit::applyLevel()
{
    int level = getLevel();

    m_upLevel = BaseAttributes();
    BaseAttributes& up = m_upLevel;

    // TODO:
    //    UNITENCHANTMENT_Heroism,          // 01

	if (isHero())
	{
		switch (level)
		{
		default:
		case 0:
		case 1: break;
		case 2: up.melee = +1; up.ranged = +1; up.defense = +1; up.resistance = +1; up.hitpoints = +1; break;
		case 3: up.melee = +2; up.ranged = +2; up.defense = +1; up.resistance = +2; up.hitpoints = +2; up.toHitMelee = +1; break;
		case 4: up.melee = +3; up.ranged = +3; up.defense = +2; up.resistance = +3; up.hitpoints = +3; up.toHitMelee = +1; break;
		case 5: up.melee = +4; up.ranged = +4; up.defense = +2; up.resistance = +4; up.hitpoints = +4; up.toHitMelee = +1; break;
		case 6: up.melee = +5; up.ranged = +5; up.defense = +3; up.resistance = +5; up.hitpoints = +5; up.toHitMelee = +2; break;
		case 7: up.melee = +6; up.ranged = +6; up.defense = +3; up.resistance = +6; up.hitpoints = +6; up.toHitMelee = +2; break;
		case 8: up.melee = +7; up.ranged = +7; up.defense = +4; up.resistance = +7; up.hitpoints = +7; up.toHitMelee = +2; break;
		case 9: up.melee = +8; up.ranged = +8; up.defense = +4; up.resistance = +8; up.hitpoints = +8; up.toHitMelee = +3; break;
		}
	}
	else
	{
      switch (level)
      {
         default:
         case 0:
         case 1: break;
         case 2: up.melee = +1; up.ranged = +1;                  up.resistance = +1; break;
         case 3: up.melee = +1; up.ranged = +1; up.defense = +1; up.resistance = +2; break;
         case 4: up.melee = +2; up.ranged = +2; up.defense = +1; up.resistance = +3; up.hitpoints = +1; up.toHitMelee = +1; break;
         case 5: up.melee = +2; up.ranged = +2; up.defense = +2; up.resistance = +4; up.hitpoints = +1; up.toHitMelee = +2; break;
         case 6: up.melee = +3; up.ranged = +3; up.defense = +2; up.resistance = +5; up.hitpoints = +2; up.toHitMelee = +3; break;
      }
	}

    // TODO: Node type
//    enum eNode_Type ENUMSIZE8
//    {
//        NODETYPE_Sorcery = 0,
//        NODETYPE_Nature = 1,
//        NODETYPE_Chaos = 2,

//        eNode_Type_MAX
//    };
//    else // Fantastic
//    {
//       switch (this.level)
//       {
//          default:
//          case 0:
//          case 1: break; // Normal
//          case 2: if (this.baseunit.Me) up.Me = +2; if (this.baseunit.Ra) up.Ra = +2; up.Df = +2; up.Re = +2; break;   // Node
//       }
//       up.Th_Ra = up.Th;

//       // Add level bonus where appropriate
//       this.add_bonus(up);
//       this.bonuses.add_bonus(up);
//    }

    up.toHitRanged = up.toHitMelee;

//    // Add level bonus where appropriate
//    this.add_bonus(up);
//    this.fixedunit.add_bonus(up);
}

void MoMUnit::applySpells(const MoMUnit *enemy)
{
    m_dnSpells = BaseAttributes();
    m_upSpells = BaseAttributes();
    BaseAttributes& dn = m_dnSpells;
    BaseAttributes& up = m_upSpells;

    //
    // Process the regular spells
    //

    // "Berserk" is processed last (after other effects on Me and defense)

    //    if (spell_active("Black Channels"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Black_Channels))
    {
        up.melee += +3; up.ranged += +1; up.hitpoints += +1; up.resistance += +1; up.defense += +1;
        // add_special("Undead");
    }

//    if (spell_active("Black Prayer"))
//    {
//        if (baseunit.Me) dn.Me += +1; dn.defense += +1; dn.Re += +2;
//    }

    // "Black Sleep" is processed last (after other effects on Me/Ra/Df/Re)

    //if (spell_active("Bless"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Bless)
           || hasItemPower(ITEMPOWER_Bless))
    {
        if (enemy && (enemy->isColor(REALM_Death) || enemy->isColor(REALM_Chaos)))
        {
            // [Manual] says Df/Re+2, but [Game] and [Strategy Guide] say Df/Re+3.
            // TODO: Df/Re only against R&G Melee, All Breath, R&G Magic Ranged, R&G Spells
//            up.defense += +3;
//            up.resistance += +3;
        }
    }

    // "Blur" is processed last (after possible True Sight)

    //        BATTLEENCHANTMENT_Call_Lightning,       // 12-13

    //if (spell_active("Chaos Channel"))
    if (hasMutation(UNITMUTATION_Chaos_Channels_Demon_Skin))
    {
        up.defense += +2;
    }
    if (hasMutation(UNITMUTATION_Chaos_Channels_Demon_Wings))
    {
        // TODO: effect on move
//        if (!has("Flying"))
//            add_special("Flying", 2);
//        else if (get_special("Flying") < 2)
//            set_special("Flying", 2);
    }
    if (hasMutation(UNITMUTATION_Chaos_Channels_Fiery_Breath))
    {
        // Strength of fire breath handled in MoMCombat
    }

//    if ((spell_active("Chaos Surge") || enemy && enemy.spell_active("Chaos Surge")) && is_type("Chaos"))
//    GLOBALENCHANTMENT_Chaos_Surge, // 88 Offset EXE:2B440
//    {
//        if (baseunit.Me) up.Me += +2; if (baseunit.ranged) up.ranged += +2;
//        // [Strategy Guide]
//        up.defense += +2;
//    }

//    if (spell_active("Charm of Life"))
//    GLOBALENCHANTMENT_Charm_of_Life, // 9E Offset EXE:2B456
//    {
//        up.Hp += +Math.max(1, Math.floor(Hp / 4));
//    }

    //        BATTLEENCHANTMENT_Counter_Magic,        // 14-15

//    GLOBALENCHANTMENT_Crusade, // 96 Offset EXE:2B44E

//    if (spell_active("Cloak of Fear"))
    //        BATTLEENCHANTMENT_Terror,               // 10-11
    //    UNITENCHANTMENT_Cloak_of_Fear,    // 08
//    UNITABILITY_Cause_Fear_Spell,     // 20
    //     ITEMPOWER_Cloak_Of_Fear,
//    {
//        // Each turn each opposing unit must resist (Re +1) or cower in fear (cannot attack, but can still counter)
//    }

    //        BATTLEENCHANTMENT_Dark_Prayer,          // 06-07

//    if (spell_active("Darkness") || enemy && enemy.spell_active("Darkness"))
    //        BATTLEENCHANTMENT_Darkness,             // 02-03
//    CITYENCH_Cloud_of_Shadow, // 4C Offset EXE:2B404
//    GLOBALENCHANTMENT_Eternal_Night, // 74 Offset EXE:2B42C
//    {
//        if (is_type("Death"))
//        {
//            if (baseunit.Me) up.Me += +1; up.defense += +1; up.resistance += +1;
//        }
//        else if (is_type("Life"))
//        {
//            if (baseunit.Me) dn.Me += +1; dn.defense += +1; dn.resistance += +1;
//        }
//    }

    //     ITEMPOWER_Death,
    //     ITEMPOWER_Destruction,
    //     ITEMPOWER_Doom_equals_Chaos,

    //if (spell_active("Eldritch Weapon"))
    // TODO: UNITABILITY_Eldritch_Weapon_COMBAT;   // 40
    if (enemy && enemy->hasUnitEnchantment(UNITENCHANTMENT_Eldritch_Weapon))
    {
        dn.toDefend += +1;                              // The To-Block (Tb) of the enemy is reduced by 1
    }

    //if (spell_active("Elemental Armor"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Elemental_Armor)
           || hasItemPower(ITEMPOWER_Elemental_Armour))
    {
        // TODO: Df/Re only against All Breath, R&G Magic Ranged, R&G Spells
        // up.defense += +10;
//        up.resistance += +10;
    }
    else if (hasUnitEnchantment(UNITENCHANTMENT_Resist_Elements)
           || hasItemPower(ITEMPOWER_Resist_Elements))
    {
        if (enemy && (enemy->isColor(REALM_Chaos) || enemy->isColor(REALM_Nature)))
        {
            // TODO: Df/Re only against All Breath, R&G Magic Ranged, R&G Spells
            // defense+=3
//            up.resistance += +3;
        }
    }

    if (hasUnitEnchantment(UNITENCHANTMENT_Endurance)
           || hasItemPower(ITEMPOWER_Endurance))
    {
        up.moves += 1.0;
    }

    //        BATTLEENCHANTMENT_Entangle,             // 18-19

    //if (spell_active("Flame Blade"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Flame_Blade)
           || hasItemPower(ITEMPOWER_Flaming))
    {
        up.melee += +2; if (hasMissileRangedAttack()) up.ranged += +2;
    }

//    if (spell_active("Flight") && !has("Flying"))
    //    UNITENCHANTMENT_Flight,           // 02
    //     ITEMPOWER_Flight,
//    {
//        add_special("Flying", 0);                 // ??? amount = landbased move + 1
//    }

    //if (spell_active("Giant Strength"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Giant_Strength)
           || hasItemPower(ITEMPOWER_Giant_Strength))
    {
        up.melee += +1; if (hasThrownRangedAttack()) up.ranged += +1;
    }

    // "Guardian Wind" is handled in isImmune()

    // "Haste" is processed last (after other effects on Me and Ra)

    //    UNITENCHANTMENT_Heroism,          // 01

//    if (spell_active("High Prayer"))
    //        BATTLEENCHANTMENT_High_Prayer,          // 0E-0F
//    {
//        if (baseunit.Me) up.Me += +2; up.defense += +2; up.resistance += +3; up.toHitMelee += +1; up.toHitRanged += +1; up.toDefend += +1;
//    }

    //if (spell_active("Holy Armor"))
    // TODO: Doesn't stack
    if (hasUnitEnchantment(UNITENCHANTMENT_Holy_Armor))
    {
        up.defense += +2;
    }

    //     ITEMPOWER_Holy_Avenger,

    //if (spell_active("Holy Weapon"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Holy_Weapon))
//        GLOBALENCHANTMENT_Holy_Arms, // 9A Offset EXE:2B452
    {
//        if (Me) add_special("Magic Weapon");      // Can hit creatures with Weapon Immunity
        up.toHitMelee += +1;
        if (hasPhysicalRangedAttack()) up.toHitRanged += +1;
    }

    // "Immolation" handled in hasImmolation()

    // "Invisibility" is handled in isInvisible()

    // "Invulnerability" is handled in isImmune()

    //if (spell_active("Iron Skin"))
    // TODO: Doesn't stack
    if (hasUnitEnchantment(UNITENCHANTMENT_Iron_Skin))
    {
        up.defense += +5;
    }
    //if (spell_active("Stone Skin"))
    else if (hasUnitEnchantment(UNITENCHANTMENT_Stone_Skin))
    {
        up.defense += +1;
    }

    //     ITEMPOWER_Lightning,

    //if (spell_active("Lion Heart"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Lionheart)
           || hasItemPower(ITEMPOWER_Lion_Heart))
    {
        // [Manual] says Me/Ra/Hp/Re +2, but [Game] shows Me/Ra/Hp/Re +3.
        up.melee += +3;
        if (hasPhysicalRangedAttack()) up.ranged += +3;
        up.hitpoints += +3;
        up.resistance += +3;
    }

    // "Magic Immunity" is handled in isImmune()

    //        BATTLEENCHANTMENT_Mana_Leak,            // 1A-1B

    //     ITEMPOWER_Merging,

//    if (spell_active("Metal Fires"))
    //        BATTLEENCHANTMENT_Metal_Fires,          // 0A-0B
//    {
//        if (this.is_type("Normal") && !spell_active("Flame Blade"))
//        {
//            if (baseunit.Me) up.Me += +1; if (baseunit.ranged && range_type == "Physical" && !has("Rocks")) up.ranged += +1;
//            if (Me) add_special("Magic Weapon");      // Can hit creatures with Weapon Immunity
//        }
//    }

//    COMBATENCHANTMENT_Mind_Storm,           // 08
//    COMBATENCHANTMENT_Mind_Twist,           // 04
    //    UNITENCHANTMENT_Path_Finding,     // 80
    //    UNITENCHANTMENT_Planar_Travel,    // 10

    //     ITEMPOWER_Pathfinding,

    //     ITEMPOWER_Phantasmal,
    //     ITEMPOWER_Planar_Travel,
    //     ITEMPOWER_Power_Drain,

  //if (spell_active("Prayer"))
      //        BATTLEENCHANTMENT_Prayer,               // 0C-0D
//    {
//        // Verified 2010-11-13 with [Tweaker] that Prayer and High Prayer do not stack
//        if (!spell_active("High Prayer"))
//        {
//            up.toHitMelee += +1; up.toHitRanged += +1; up.toDefend += +1; up.resistance += +1;
//        }
//    }

//    if (spell_active("Regeneration"))
    //    UNITENCHANTMENT_Regeneration,     // 40
//    UNITABILITY_Regeneration,         // 20
      //     ITEMPOWER_Regeneration,
//    {
//        add_special("Regeneration");
//    }

    // "Resist Elements" is handled after "Elemental Armor" since they don't stack

    //if (spell_active("Resist Magic"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Resist_Magic)
           || hasItemPower(ITEMPOWER_Resist_Magic))
    {
        up.resistance += +5;
    }

    //if (spell_active("Righteousness"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Righteousness)
           || hasItemPower(ITEMPOWER_Righteousness))
    {
        // TODO: Df/Re only against All Breath, B&R Magic Ranged, B&R Spells
        // defense = (50)
//        up.resistance += +30;
    }

    // "Shatter" is processed last (after other effects on Me/Ra/Df/Re)

    //    UNITENCHANTMENT_Spell_Lock,       // 40
//    UNITMUTATION_Stasis_initial,                // 40
//    UNITMUTATION_Stasis_lingering,              // 80

    // "Stone Skin" is handled together with "Iron Skin"

  //     ITEMPOWER_Stoning,

//    if (spell_active("Terror"))
//    {
//        // Each turn the unit must resist (Re +1) or cower in fear (cannot attack, but can still counter)
//    }

//    if (spell_active("True Light") || enemy && enemy.spell_active("True Light"))
    //        BATTLEENCHANTMENT_True_Light,           // 00-01    1=True Light (Battle), 2=Heavenly Light (City) / Cloud of Shadow (City), 3=Eternal Darkness (Global)
//    CITYENCH_Heavenly_Light, // 6A Offset EXE:2B422
//    {
//        if (is_type("Life"))
//        {
//            if (baseunit.Me) up.Me += +1; up.defense += +1; up.resistance += +1;
//        }
//        else if (is_type("Death"))
//        {
//            if (baseunit.Me) dn.Me += +1; dn.defense += +1; dn.resistance += +1;
//        }
//    }

    // "True Sight" is handled in isImmune()

  //    UNITMUTATION_Undead,                        // 20

  //     ITEMPOWER_Vampiric = 0,

    //if (spell_active("Vertigo"))
    if (hasCombatEnchantment(COMBATENCHANTMENT_Vertigo))
    {
        dn.toHitMelee += +2;
        dn.toHitRanged += +2;
        dn.defense += +1;
    }

    // "Warp Creature" is processed last (after other effects on Me/Ra/Df/Re)

//    if (spell_active("Warp Reality") || enemy && enemy.spell_active("Warp Reality"))
            //        BATTLEENCHANTMENT_Warp_Reality,         // 04-05
//    {
//        if (!is_type("Chaos")) { dn.toHitMelee += +2; dn.toHitRanged += +2; }
//    }

//    if (spell_active("Warp Wood"))
//    {
//        if (has("Arrows")) set_special("Arrows", 0);
//        if (has("Bullets")) set_special("Bullets", 0);
//    }

            //    UNITENCHANTMENT_Water_Walking,    // 01
            //     ITEMPOWER_Water_Walking,

    //if (spell_active("Weakness"))
    if (hasCombatEnchantment(COMBATENCHANTMENT_Weakness))
    {
        dn.melee += +2; if (hasMissileRangedAttack()) dn.ranged += +2;
    }

    // "Web" is processed last (after Flight and Wraith Form)

//    COMBATENCHANTMENT_Whirlwind,            // 04

//    UNITENCHANTMENT_Wind_Walking,     // 01
//        UNITABILITY_Wind_Walking,         // 10

//    if (spell_active("Wrack"))
        //        BATTLEENCHANTMENT_Wrack,                // 08-09
//    {
//        // each enemy figure resists or loses 1 hp per turn
//    }

        //        UNITABILITY_Non_Corporeal,        // 08
//    if (spell_active("Wraith Form"))
//    UNITENCHANTMENT_Wraith_Form,      // 20
        //     ITEMPOWER_Wraith_Form,
//    {
//        add_special("Weapon Imm");
//        add_special("Non Corporeal");
//    }


    //
    // Process the spells last that must be processed after other spells
    //

    //   if (spell_active("Berserk"))
    if (hasUnitEnchantment(UNITENCHANTMENT_Berserk))
    {
//        up.Me += Me; dn.defense += defense;
    }

    //if (spell_active("Black Sleep"))
    if (hasCombatEnchantment(COMBATENCHANTMENT_Black_Sleep))
    {
        // Chance to resist initially.
        // Me, Ra, Df are effective reduced to zero.
//        dn.Me = Me + up.Me;
//        dn.ranged = Ra + up.ranged;
//        dn.defense = defense + up.defense;
    }

//    if (enemy && enemy.spell_active("Blur") && !has("Illusion Imm"))
//        BATTLEENCHANTMENT_Blur,                 // 1C-1D
//    {
//        // TODO: Proper processing: ignore 10% of hits or something like that
//        dn.toHitMelee += +1;
//        dn.toHitRanged += +1;
//    }

    //if (spell_active("Haste"))
//    COMBATENCHANTMENT_Haste,                // 08
        //     ITEMPOWER_Haste,
//    {
//        if (baseunit.Me) up.Me += Me; if (baseunit.ranged) up.ranged += Ra;
//    }

    //if (spell_active("Shatter"))
    if (hasCombatEnchantment(COMBATENCHANTMENT_Shatter))
    {
        // Resist or Me=Ra=1
//        dn.Me = Max(0, Me + up.Me - 1);
//        dn.ranged = Max(0, Ra + up.ranged - 1);
    }

//    if (spell_active("Warp Creature"))
        //    COMBATENCHANTMENT_Warp_Creature_Attack, // 80
        //    COMBATENCHANTMENT_Warp_Creature_Defense,    // 01
        //    COMBATENCHANTMENT_Warp_Creature_Resistance, // 02
//    {
//        // Resist at -1 or (Me/2 or Df/2 or Re=0)
//        switch (1 * spells[ "Warp Creature" ])
//        {
//        case 1: dn.Me += Math.ceil(Me / 2); break;
//        case 2: dn.defense += Math.ceil(defense / 2); break;
//        case 3: dn.resistance += Re; break;
//        }
//    }

//    if (spell_active("Web"))
//    {
//        // Web does not work on Non Corporeal creatures, and can not Web a unit while a previous Web still present
//        if (!has("Non Corporeal") && !has("Web"))
//        {
//            // The web (Df 0) must be destroyed by full combined melee and magic ranged attacks,
//            // before the unit can move again.
//            // The unit can no longer fly for the remainder of the combat
//            add_special("Web", 12);
//            delete_special("Flying");
//        }
//    }

    // Apply spell modifiers where appropriate
    m_bonuses.addBonus(up);
    m_penalties.addBonus(dn);
}

void MoMUnit::applyWeaponType()
{
	m_upWeaponType = BaseAttributes();
	BaseAttributes& up = m_upWeaponType;

	switch (getWeaponType())
	{
	default:
	case MoM::WEAPON_normal:  
		break;
	case MoM::WEAPON_magic: 
		up.toHitMelee = +1;
		if (hasPhysicalRangedAttack()) up.toHitRanged = +1;
		break;
	case MoM::WEAPON_mithril: 
		up.melee = +1;
		up.defense = +1;
		up.toHitMelee = +1;
		if (hasPhysicalRangedAttack())
		{
			up.toHitRanged = +1;
			up.ranged = +1;
		}
		break;
	case MoM::WEAPON_adamantium:
		up.melee = +2;
		up.defense = +2; 
		up.toHitMelee = +1;
		if (hasPhysicalRangedAttack())
		{
			up.toHitRanged = +1;
			up.ranged = +2;
		}
		break;
	}

	// Add weapon bonus where appropriate
    m_bonuses.addBonus(up);
}

bool MoMUnit::commitData(void *ptr, const void *pNewValue, size_t size)
{
    if ((0 == m_game) || (0 == ptr) || (0 == pNewValue))
        return false;
    return m_game->commitData(ptr, pNewValue, size);
}

void MoMUnit::BaseAttributes::addBonus(const MoMUnit::BaseAttributes& up)
{
    melee += up.melee;
    defense += up.defense;
    ranged += up.ranged;
    resistance += up.resistance;
    hitpoints += up.hitpoints;
    toHitMelee += up.toHitMelee;
    toHitRanged += up.toHitRanged;
    toDefend += up.toDefend;

    moves += up.moves;
}

bool MoMUnit::setUnitTypeData(const Unit_Type_Data &value)
{
    return commitData(m_unitType, &value, sizeof(value));
}

bool MoMUnit::setHeroStats(const Hero_stats &value)
{
    return commitData(m_heroStats, &value, sizeof(value));
}

}
