// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <iostream>
#include <string.h>
#include <string>

#include "MoMCity.h"
#include "MoMGenerated.h"
#include "MoMUnit.h"
#include "MoMUtility.h"

#include "MoMController.h"

namespace MoM {

// TODO: Get XP levels from game
int gXPforLevel[] =
{
    0,      // 1	Hero		
    20,     // 2	Myrmidon		
    60,     // 3	Captain		
    120,    // 4	Commander		
    200,    // 5	Champion		
    300,    // 6	Lord		
    450,    // 7	Grand Lord		
    600,    // 8	Super Hero		
    1000    // 9	Demi-God		
};

void MoMController::setErrorString(const std::string &str)
{
    m_errorString = str;
    std::cout << m_errorString << std::endl;
}

//bool MoMController::addCityToGameQueue(int cityNr)
//{
//    MoMDataSegment* dataSegment = m_game->getDataSegment();
//    if (0 == dataSegment)
//        return false;
//    if (dataSegment->m_nr_city_queue <= 0)
//        return false;   // Queue is not active now. It has no point to fiddle with it now.

//    bool found = false;
//    for (int queueNr = 0; (queueNr < dataSegment->m_nr_city_queue) && (queueNr < ARRAYSIZE(dataSegment->m_arr20_city_queue)); ++queueNr)
//    {
//        CityQueueElement* queueElt = dataSegment->m_arr20_city_queue[queueNr];
//        if (cityNr == queueElt->m_CityNr)
//        {
//            found = true;
//            break;      // Already in queue
//        }
//        if (producingGarrison(*city) && 2 == unitsInCity.size())
//        if (cityNr == queueElt->m_CityNr)
//        {
//            int8_t newCityNr = -1;
//            removed = m_game->commitData(&queueElt->m_CityNr, &newCityNr, sizeof(queueElt->m_CityNr));
//            break;
//        }
//    }

//    return removed;
//    if (producingGarrison(*city) && unitsInCity.size()
//    {
//        // Do nothing - we're producing a Garrison
//        //std::cout << "City '" << city->m_City_Name << "' [" << cityNr << "] "
//        //    << city->m_Producing << " keeps producing a garrison" << std::endl;
//    }
//    else if (0 == unitsInCity.size() && !producingGarrison(*city) && findCheapestUnitToProduce(*city, produce))

//}

bool MoMController::addUnit(ePlayer playerNr, eUnit_Type unitType)
{
	m_errorString.clear();

    if (0 == m_game)
        return false;

    if ((unsigned)unitType >= eUnit_Type_MAX)
    {
        setErrorString("cannot add unit of type '" + toStr(unitType) + "' because it is out-of-range");
        return false;
    }

    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
	{
        setErrorString("wizard data of player '" + toStr(playerNr) + "' is not accessible");
        return false;
	}

    // TODO: Check playerNr range + wizard not destroyed

	// Create a new unit that is not in the game yet
    int unitNr = -1;
    bool ok = createUnit(unitNr);

    Unit* unit = m_game->getUnit(unitNr);
    if (0 == unit)
	{
        setErrorString("Failed to create a unit for unitNr '" + toStr(unitNr) + "'");
        ok = false;
	}

	// Initialize the unit to be on the board
	if (ok) 
	{
        assert(unitNr == m_game->getNrUnits());

		unit->m_Unit_Type = unitType;
		// TODO: Check if there is space
		unit->m_XPos = (uint8_t)wizard->m_X_Coordinate_of_Summoning_Circle;
		unit->m_YPos = (uint8_t)wizard->m_Y_Coordinate_of_Summoning_Circle;
		unit->m_Plane = wizard->m_Plane_of_Summoning_Circle;
		unit->m_Owner = playerNr;
		unit->m_Hero_Slot_Number = -1;
		unit->m_Moves_Total = 2;
		unit->m_Moves_Left = 2;
        unit->m_Scouting = 1;
		unit->m_Road_Building_left_to_complete = -1;
	}

	// Polymorph the unit to a hero if applicable
    if (ok && ((unsigned)unitType < gMAX_HERO_TYPES))
	{
        ok = polymorphToHero(playerNr, unitNr, unitType);
	}

	// Put the unit in the game
    if (ok)
    {
        ok = m_game->setNrUnits(unitNr + 1);
    }

    return ok;
}

bool MoMController::addArtifact(ePlayer playerNr, int prefabNr)
{
    m_errorString.clear();
    if (0 == m_game)
        return false;

    // Retrieve item data (or fail)
    ItemDataLbx* prefabItemData = m_game->getItemDataLbx(prefabNr);
    if (0 == prefabItemData)
    {
        setErrorString("no such item available");
        return false;
    }

    // Check if already in game
    uint8_t* artifactsInGame = m_game->getArtifacts_in_game();
    if (0 == artifactsInGame)
        return false;
    if (artifactsInGame[prefabNr])
    {
        setErrorString("item has already been seen in the game");
        return false;
    }

    // Find free item slot for its attributes (or fail)
    int16_t freeItemNr = -1;
    Item* freeItem = 0;
    for (int itemNr = 0; (0 == freeItem) && (toUInt(itemNr) < gMAX_ITEMS_IN_GAME); ++itemNr)
    {
        Item* item = m_game->getItem(itemNr);
        if ((0 == item->m_Cost) || (-1 == item->m_Cost))
        {
            freeItemNr = itemNr;
            freeItem = item;
        }
    }
    if (0 == freeItem)
    {
        setErrorString("maximum items in the games reached");
        return false;
    }

    // Find free item slot in fortress (or fail)
    Wizard* wizard = m_game->getWizard(playerNr);
    int fortressSlotNr = -1;
    for (int slotNr = 0; (-1 == fortressSlotNr) && (toUInt(slotNr) < ARRAYSIZE(wizard->m_Items_in_Slots)); ++slotNr)
    {
        if (-1 == wizard->m_Items_in_Slots[slotNr])
        {
            fortressSlotNr = slotNr;
        }
    }
    if (-1 == fortressSlotNr)
    {
        setErrorString("no free space in your fortress");
        return false;
    }

    // NiceToHave - check if book requirements satisfied

    // Mark the item as ingame (or fail if already in game)
    // Copy the item's attributes
    // Link item to item slot in fortress
    uint8_t newInGame = true;
    if (!m_game->commitData(&artifactsInGame[prefabNr], &newInGame, sizeof(artifactsInGame[prefabNr]))
            || !m_game->commitData(freeItem, &prefabItemData->m_Item, sizeof(*freeItem))
            || !m_game->commitData(&wizard->m_Items_in_Slots[fortressSlotNr], &freeItemNr, sizeof(wizard->m_Items_in_Slots[fortressSlotNr])))
    {
        setErrorString("failed to commit the changes to the game");
        return false;
    }

    return true;
}

bool MoMController::buyProduction(City *city)
{
    m_errorString.clear();
    if (0 == m_game)
        return false;
    MoMCity momCity(m_game, city);
    int costToBuy = momCity.getCostToBuy(city->m_Producing);
    Wizard* wizard = m_game->getWizard(PLAYER_YOU);
    if (0 == wizard)
        return false;
    if (costToBuy <= 0)
    {
        setErrorString("Cannot buy. Production will already be complete.");
        return false;
    }
    if (wizard->m_Gold_Coins < costToBuy)
    {
        setErrorString("Not enough gold to buy production");
        return false;
    }

    int16_t newGold = wizard->m_Gold_Coins - costToBuy;
    int16_t newHammersAccumulated = momCity.getCostToProduce(city->m_Producing);
    if (!m_game->commitData(&wizard->m_Gold_Coins, &newGold, sizeof(wizard->m_Gold_Coins)))
    {
        setErrorString("Failed to commit gold coin changes");
        return false;
    }
    if (!m_game->commitData(&city->m_HammersAccumulated, &newHammersAccumulated, sizeof(city->m_HammersAccumulated)))
    {
        setErrorString("Failed to commit accumulated hammer changes");
        return false;
    }

    std::cout << "Bought " << city->m_Producing << " for " << costToBuy << " gold" << std::endl;
    return true;
}

int MoMController::calcCastingSkillBase(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;
    int castingSkill = wizard->m_Nominal_Casting_Skill_available_this_turn;
    return castingSkill;
}

int MoMController::calcCastingSkillTotal(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;

    int castingSkill = calcCastingSkillBase(playerNr);

    int castingSkillBonus = 0;
    for (int heroSlotNr = 0; toUInt(heroSlotNr) < gMAX_HIRED_HEROES; ++heroSlotNr)
    {
        MoMUnit hiredHero(m_game, &wizard->m_Heroes_hired_by_wizard[heroSlotNr]);
        Unit unit = hiredHero.getUnitInGame();
        Fortress* fortresses = m_game->getFortresses();
        if (MoMLocation(unit) == MoMLocation(fortresses[playerNr]))
        {
            castingSkillBonus += hiredHero.getCastingSkillTotal() / 2;
        }
    }
    castingSkill += castingSkillBonus;

    return castingSkill;
}

int MoMController::calcFame(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;

    int fame = 0;
    for (int heroSlotNr = 0; toUInt(heroSlotNr) < gMAX_HIRED_HEROES; ++heroSlotNr)
    {
        Hired_Hero* hiredHero = m_game->getHiredHero(playerNr, heroSlotNr);
        MoMUnit momUnit(m_game, hiredHero);
        if (momUnit.hasHeroAbility(HEROABILITY_Legendary))
        {
            fame += momUnit.getHeroAbility(HEROABILITY_Legendary);
        }
        if (momUnit.hasHeroAbility(HEROABILITY_Legendary_X))
        {
            fame += momUnit.getHeroAbility(HEROABILITY_Legendary_X);
        }
    }

    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 != wizard)
    {
        if (wizard->m_Global_Enchantments.Just_Cause)
        {
            fame += 10;
        }

        fame += wizard->m_Fame;
    }

    return fame;
}

int MoMController::calcFoodProduced(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    int foodProduced = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        if (city->m_Population < city->m_Food_Produced)
        {
            foodProduced += city->m_Food_Produced - city->m_Population;
        }
    }
    return foodProduced;
}

int MoMController::calcFoodUpkeep(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    int foodUpkeep = 0;
    for (int unitNr = 0; unitNr < m_game->getNrUnits(); ++unitNr)
    {
        Unit* unit = m_game->getUnit(unitNr);
        if (0 == unit)
            break;
        if (playerNr != unit->m_Owner)
            continue;
        MoMUnit momUnit(m_game, unit);
        if (momUnit.isNormal() && !unit->m_Weapon_Mutation.s.Undead)
        {
            foodUpkeep++;
        }
    }
    return foodUpkeep;
}

int MoMController::calcGoldIncome(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    int goldProduced = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        goldProduced += city->m_Coins - city->m_Maintenance;
    }

    for (int heroSlotNr = 0; toUInt(heroSlotNr) < gMAX_HIRED_HEROES; ++heroSlotNr)
    {
        Hired_Hero* hiredHero = m_game->getHiredHero(playerNr, heroSlotNr);
        MoMUnit momUnit(m_game, hiredHero);
        if (momUnit.hasHeroAbility(HEROABILITY_Noble))
        {
            goldProduced += momUnit.getHeroAbility(HEROABILITY_Noble);
        }
    }

    int foodSurplus = calcFoodProduced(playerNr) - calcFoodUpkeep(playerNr);
    goldProduced += foodSurplus / 2;

    return goldProduced;
}

int MoMController::calcGoldUpkeep(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    int goldUpkeep = 0;
    for (int unitNr = 0; unitNr < m_game->getNrUnits(); ++unitNr)
    {
        Unit* unit = m_game->getUnit(unitNr);
        if (0 == unit)
            break;
        if (playerNr != unit->m_Owner)
            continue;
        MoMUnit momUnit(m_game, unit);
        goldUpkeep += momUnit.calcGoldUpkeep();
    }

    goldUpkeep -= calcFame(playerNr);

    if (goldUpkeep < 0)
    {
        goldUpkeep = 0;
    }

    // TODO: Difficulty multiplier

    return goldUpkeep;
}

void MoMController::calcManaSkillResearch(ePlayer playerNr, int &mana, int &skill, int &research) const
{
    mana     = 0;
    skill    = 0;
    research = 0;

    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return;

    if (wizard->m_Spell_being_cast == SPELL16_Spell_Of_Return)
        return;

    calcPowerBaseDivision(playerNr, mana, skill, research);

    int researchBonusPercentage = calcResearchBonusPercentage(playerNr);
    research += research * researchBonusPercentage / 100;
}

int MoMController::calcManaUpkeep(ePlayer playerNr) const
{
    if (0 == m_game)
        return 0;
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;
    MoM::Upkeep_Enchantments* upkeepEnchantments = 0;
    if (0 != m_game->getDataSegment())
    {
        upkeepEnchantments = &m_game->getDataSegment()->m_Upkeep_Enchantments;
    }
    if (0 == upkeepEnchantments)
        return 0;

    int manaUpkeep = 0;
    for (int unitNr = 0; unitNr < m_game->getNrUnits(); ++unitNr)
    {
        Unit* unit = m_game->getUnit(unitNr);
        if (0 == unit)
            break;
        if (playerNr != unit->m_Owner)
            continue;
        MoMUnit momUnit(m_game, unit);
        manaUpkeep += momUnit.calcManaUpkeep(); // Includes unit enchantments
    }


    // Global enchantments
    MOM_FOREACH(eGlobalEnchantment, enchantment, eGlobalEnchantment_MAX)
    {
        uint8_t* activeEnchantments = &wizard->m_Global_Enchantments.Eternal_Night;
        if (activeEnchantments[enchantment])
        {
            manaUpkeep += (&upkeepEnchantments->Eternal_Night)[enchantment];
        }
    }

    // City enchantments
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        MOM_FOREACH(eCityEnchantments, enchantment, eCityEnchantments_MAX)
        {
            eOwner* activeEnchantments = &city->m_City_Enchantments.Wall_of_Fire;
            if (OWNER_None != activeEnchantments[enchantment])
            {
                manaUpkeep += (&upkeepEnchantments->Wall_of_Fire)[enchantment];
            }
        }
    }

    // TODO: Difficulty multiplier

    return manaUpkeep;
}

int MoMController::calcPower(ePlayer playerNr) const
{
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;

    // Buildings, citizens, global spells, events
    int power = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        power += city->m_Mana_cr;
    }

    // Fortress
    power += wizard->m_Number_of_Spellbooks_Chaos + wizard->m_Number_of_Spellbooks_Death
            + wizard->m_Number_of_Spellbooks_Life + wizard->m_Number_of_Spellbooks_Nature
            + wizard->m_Number_of_Spellbooks_Sorcery;

    // Myrran
    if (wizard->m_Skills.s.eSkill_Myrran)
    {
        power += 5;
    }

    // TODO: Nodes

    return power;
}

void MoMController::calcPowerBaseDivision(ePlayer playerNr, int &mana, int &skill, int &research) const
{
    mana     = 0;
    skill    = 0;
    research = 0;
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return;
    int power   = wizard->m_Power_Base;
    mana        = (power * wizard->m_Mana_Percentage + 50) / 100;
    skill       = (power * wizard->m_Skill_Percentage + 50) / 100;
    research    = power - mana - skill;
    if (wizard->m_Research_Percentage <= 0)
    {
        if (wizard->m_Skill_Percentage == 0)
        {
            mana += research;
        }
        else
        {
            skill += research;
        }
        research = 0;
    }

    if (wizard->m_Skills.s.eSkill_Mana_Focusing)
    {
        mana += mana / 4;
    }

    // NOTE: v1.31 does not show the added 50% skill for archmage, although Insecticide does
    if (wizard->m_Skills.s.eSkill_Archmage)
    {
        skill += skill / 2;
    }

    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        research += city->m_Research;
    }

    for (int heroSlotNr = 0; toUInt(heroSlotNr) < gMAX_HIRED_HEROES; ++heroSlotNr)
    {
        Hired_Hero* hiredHero = m_game->getHiredHero(playerNr, heroSlotNr);
        MoMUnit momUnit(m_game, hiredHero);
        if (momUnit.hasHeroAbility(HEROABILITY_Sage))
        {
            research += momUnit.getHeroAbility(HEROABILITY_Sage);
        }
        if (momUnit.hasHeroAbility(HEROABILITY_Sage_X))
        {
            research += momUnit.getHeroAbility(HEROABILITY_Sage_X);
        }
    }
}

int MoMController::calcProduction(ePlayer playerNr) const
{
    int production = 0;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (playerNr != city->m_Owner)
            continue;
        production += city->m_Hammers;
    }
    return production;
}

int MoMController::calcResearch(ePlayer playerNr) const
{
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;

    if (wizard->m_Spell_being_cast == SPELL16_Spell_Of_Return)
        return 0;

    int mana, skill, research;
    calcPowerBaseDivision(playerNr, mana, skill, research);

    int researchBonusPercentage = calcResearchBonusPercentage(playerNr);
    research += research * researchBonusPercentage / 100;

    return research;
}

int MoMController::calcResearchBonusPercentage(ePlayer playerNr) const
{
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return 0;

    Spell_Data* spellData = m_game->getSpellData(wizard->m_Researching_Spell);
    eRealm_Type realmResearching = spellData->m_Magic_Realm;
    int researchBonusPercentage = 0;
    if (wizard->m_Skills.s.eSkill_Sage_Master)
    {
        researchBonusPercentage += 25;
    }
    if (wizard->m_Skills.s.eSkill_Conjurer && (spellData->m_Spell_Category == SPELLCATEGORY_Normal_summon))
    {
        researchBonusPercentage += 25;
    }

    switch (realmResearching)
    {
    case REALM_Nature:
        if (wizard->m_Skills.s.eSkill_Nature_Master)
        {
            researchBonusPercentage += 15;
        }
        if (wizard->m_Number_of_Spellbooks_Nature > 7)
        {
            researchBonusPercentage += (wizard->m_Number_of_Spellbooks_Nature - 7) * 10;
        }
        break;
    case REALM_Sorcery:
        if (wizard->m_Skills.s.eSkill_Sorcery_Master)
        {
            researchBonusPercentage += 15;
        }
        if (wizard->m_Number_of_Spellbooks_Sorcery > 7)
        {
            researchBonusPercentage += (wizard->m_Number_of_Spellbooks_Sorcery - 7) * 10;
        }
        break;
    case REALM_Chaos:
        if (wizard->m_Skills.s.eSkill_Chaos_Master)
        {
            researchBonusPercentage += 15;
        }
        if (wizard->m_Number_of_Spellbooks_Chaos > 7)
        {
            researchBonusPercentage += (wizard->m_Number_of_Spellbooks_Chaos - 7) * 10;
        }
        break;
    case REALM_Life:
        if (wizard->m_Number_of_Spellbooks_Life > 7)
        {
            researchBonusPercentage += (wizard->m_Number_of_Spellbooks_Life - 7) * 10;
        }
        break;
    case REALM_Death:
        if (wizard->m_Number_of_Spellbooks_Death > 7)
        {
            researchBonusPercentage += (wizard->m_Number_of_Spellbooks_Death - 7) * 10;
        }
        break;
    case REALM_Arcane:
        if (wizard->m_Skills.s.eSkill_Runemaster)
        {
            researchBonusPercentage += 25;
        }
        break;
    default:
        ;
    }

    return researchBonusPercentage;
}

int MoMController::countGarrison(const MoMLocation &location)
{
    std::vector<int> units;
    m_game->findUnitsAtLocation(location, units);

    // Count regular units, including catapults, excluding ships, settlers, heroes
    int count = 0;
    for (size_t i = 0; i < units.size(); ++i)
    {
        Unit* unit = m_game->getUnit(units[i]);
        MoMUnit momUnit(m_game);
        momUnit.changeUnit(unit);
        if (!momUnit.isNormal())
            continue;
        if (momUnit.isShip())
            continue;
        if (momUnit.isSettler())
            continue;
        count++;
    }

    return count;
}

int MoMController::countUnits(const MoMLocation &location)
{
    std::vector<int> units;
    m_game->findUnitsAtLocation(location, units);
    return units.size();
}

bool MoMController::createUnit(int& unitNr)
{
	m_errorString.clear();

    if (0 == m_game)
        return false;

    if (m_game->getNrUnits() < 0
        || m_game->getNrUnits() >= (int)gMAX_UNITS)
    {
        setErrorString("Cannot create a unit because NrUnits '" + toStr(m_game->getNrUnits()) + "' is out-of-range");
        return false;
    }

    unitNr = m_game->getNrUnits();

    Unit* unit = m_game->getUnit(unitNr);
    if (0 == unit)
	{
        setErrorString("Cannot retrieve data for unit '" + toStr(unitNr) + "'");
        return false;
	}

    memset(unit, '\0', sizeof(Unit));

    // Make sure the unit contains "reasonable" data
    unit->m_Hero_Slot_Number = -1;
    unit->m_Moves_Total = 2;
    unit->m_Moves_Left = 2;
    unit->m_Road_Building_left_to_complete = -1;

    return true;
}

City* MoMController::findCityAtLocation(const MoMLocation &location)
{
    City* value = 0;

    if (0 == m_game)
        return value;

    for (int i = 0; i < m_game->getNrCities(); ++i)
    {
        City* city = m_game->getCity(i);
        if (0 == city)
            break;
        if (location == MoMLocation(*city, MoMLocation::MAP_overland))
        {
            value = city;
            break;
        }
    }
    return value;
}

eRace MoMController::findDominantRace(ePlane plane) const
{
    if (0 == m_game)
        return eRace_MAX;

    eRace dominantRace = eRace_MAX;
    std::vector<unsigned> histo(eRace_MAX);
    unsigned dominantCount = 0;

    // Count races in cities and find dominant race
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        const City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (city->m_Plane != plane)
            continue;
        if (toUInt(city->m_Race) < eRace_MAX)
        {
            histo[city->m_Race]++;
            if (histo[city->m_Race] > dominantCount)
            {
                dominantRace = city->m_Race;
                dominantCount = histo[city->m_Race];
            }
        }
    }

    return dominantRace;
}

Node_Attr* MoMController::findNodeAttrAtLocation(const MoMLocation& location)
{
    Node_Attr* value = 0;

    if (0 == m_game)
        return value;

    for (size_t i = 0; i < gMAX_NODES; ++i)
    {
        Node_Attr* nodeAttr = m_game->getNodeAttr(i);
        if (0 == nodeAttr)
            break;
        if (location == MoMLocation(*nodeAttr, MoMLocation::MAP_overland))
        {
            value = nodeAttr;
            break;
        }
    }
    return value;
}

bool MoMController::polymorphToHero(ePlayer playerNr, int unitNr, eUnit_Type heroNr)
{
	m_errorString.clear();

    if (0 == m_game)
        return false;

    // EXTRA INPUT:
	// TODO: Retrieve heroName from game
	// TODO: Retrieve heroSlotTypes from game
    std::string heroName("Tweaker");
    int heroSlotNr = 0;
    eSlot_Type16 heroSlotTypes[3] = {SLOT16_Sword_Staff_Wand, SLOT16_Armor_Shield, SLOT16_Amulet};

    // CHECK INPUT:
    assert((unsigned)unitNr < gMAX_UNITS);
    assert((unsigned)playerNr < gMAX_VALID_WIZARDS);
    assert((unsigned)heroNr < gMAX_HERO_TYPES);
    assert((unsigned)heroSlotNr < gMAX_HIRED_HEROES);

    const Unit_Type_Data* unitTypeData = m_game->getUnitTypeData(heroNr);
    if (0 == unitTypeData)
        return false;
    Hero_stats* heroStats = m_game->getHeroStats(playerNr, heroNr);
    if (0 == heroStats)
        return false;
    Unit* unit = m_game->getUnit(unitNr);
    if (0 == unit)
        return false;
    Wizard* wizard = m_game->getWizard(playerNr);
    if (0 == wizard)
        return false;

    // CHECKS

	// TODO: Check if the unit is already a hero in which case we need a different scenario
	//       1. Reuse the current heroSlotNr
	//		 2. Bury the old hero

	// Retrieve the slot types
    m_game->getHeroSlotTypes(unitTypeData->m_Building2_or_HeroType, heroSlotTypes);

    // Check if hero slot is free or search for first free slot
    if (-1 != wizard->m_Heroes_hired_by_wizard[heroSlotNr].m_Unit_Nr)
    {
        for (unsigned i = 0; i < gMAX_HIRED_HEROES; ++i)
        {
            if (-1 == wizard->m_Heroes_hired_by_wizard[i].m_Unit_Nr)
            {
                heroSlotNr = i;
                break;
            }
        }
        if (-1 != wizard->m_Heroes_hired_by_wizard[heroSlotNr].m_Unit_Nr)
        {
            setErrorString("Player " + toStr(playerNr) + " cannot add hero " + toStr(heroNr) + " because he has no free slot");
            return false;
        }
    }

    Hired_Hero& hiredHero = wizard->m_Heroes_hired_by_wizard[heroSlotNr];

    // Check if hero is already in your army
    if (HEROLEVELSTATUS_Active_in_Wizards_army == heroStats->m_Level_Status)
    {
        setErrorString("Player " + toStr(playerNr) + " cannot add hero " + toStr(heroNr) + " because that hero is already active in his army");
        return false;
    }


    // PROCEDURE

    // Determine Level & XP
    int level = (int8_t)heroStats->m_Level_Status;
    if (level < 0)
    {
        // Resurrect
        level = -1 - level;
    }
    else
    {
        // Summon
        // Level is already correctly set
    }
    if ((unsigned)level >= ARRAYSIZE(gXPforLevel))
    {
        // In case of doubt, make it Level_1
        level = 0;
    }
    int xp = gXPforLevel[level];

    // Bring the unit to your Summoning Circle if it was not yours
    // or if its not on the board
    if ((playerNr != unit->m_Owner)
        || (unit->m_XPos <= 0 || unit->m_YPos <= 0)   // TODO: Include 0? Or exclude?
        || (unit->m_XPos >= 60 || unit->m_YPos >= 40)
        || ((unsigned)unit->m_Plane >= ePlane_MAX))
    {
        // TODO: Check if there is space for our unit
        unit->m_XPos = (uint8_t)wizard->m_X_Coordinate_of_Summoning_Circle;
        unit->m_YPos = (uint8_t)wizard->m_Y_Coordinate_of_Summoning_Circle;
        unit->m_Plane = wizard->m_Plane_of_Summoning_Circle;
    }

    // Change unit to your hero
    unit->m_Owner = playerNr;
    unit->m_Hero_Slot_Number = heroSlotNr;
    unit->m_Unit_Type = heroNr;
    unit->m_Level = (eLevel)level;
    unit->m_Experience = xp;

    // Set hero data
    hiredHero.m_Unit_Nr = unitNr;
    memset(hiredHero.m_Hero_name, '\0', sizeof(hiredHero.m_Hero_name));
    strncpy(hiredHero.m_Hero_name, heroName.c_str(), sizeof(hiredHero.m_Hero_name) - 1);
    hiredHero.m_Items_In_Slot[0] = -1;
    hiredHero.m_Items_In_Slot[1] = -1;
    hiredHero.m_Items_In_Slot[2] = -1;
    hiredHero.m_Slot_Types[0] = heroSlotTypes[0];
    hiredHero.m_Slot_Types[1] = heroSlotTypes[1];
    hiredHero.m_Slot_Types[2] = heroSlotTypes[2];

    // Make hero active in your army
    heroStats->m_Level_Status = HEROLEVELSTATUS_Active_in_Wizards_army;

    // Commit
    if (!m_game->commitData(unit, unit, sizeof(*unit))
        || !m_game->commitData(&hiredHero, &hiredHero, sizeof(hiredHero))
        || !m_game->commitData(&heroStats->m_Level_Status, &heroStats->m_Level_Status, sizeof(heroStats->m_Level_Status)))
    {
        setErrorString("Failed to commit to game");
        return false;
    }

    return true;
}

bool MoMController::replaceRace(eRace fromRace, eRace toRace, ePlane plane)
{
    m_errorString.clear();
    if (0 == m_game)
        return false;
    if ((toUInt(fromRace) >= gMAX_RACES) || (toUInt(toRace) >= gMAX_RACES) || (toUInt(plane) >= ePlane_MAX))
        return false;

    eUnit_Type firstUnitOfRace[(int)gMAX_RACES + 1] =
    {
        UNITTYPE_Barbarian_Spearmen,    //RACE_Barbarian = 0,
        UNITTYPE_Beastmen_Spearmen,     //RACE_Beastmen = 1,
        UNITTYPE_Dark_Elf_Spearmen,     //RACE_Dark_Elf = 2,
        UNITTYPE_Draconian_Spearmen,    //RACE_Draconian = 3,
        UNITTYPE_Dwarven_Swordsmen,     //RACE_Dwarven = 4,
        UNITTYPE_Gnoll_Spearmen,        //RACE_Gnoll = 5,
        UNITTYPE_Halfling_Spearmen,     //RACE_Halfling = 6,
        UNITTYPE_High_Elf_Spearmen,     //RACE_High_Elf = 7,
        UNITTYPE_High_Men_Spearmen,     //RACE_High_Men = 8,
        UNITTYPE_Klackon_Spearmen,      //RACE_Klackon = 9,
        UNITTYPE_Lizardman_Spearmen,    //RACE_Lizardman = 10,
        UNITTYPE_Nomad_Spearmen,        //RACE_Nomad = 11,
        UNITTYPE_Orc_Spearmen,          //RACE_Orc = 12,
        UNITTYPE_Troll_Spearmen,        //RACE_Troll = 13,
        UNITTYPE_Arcane_Magic_Spirit    //14
    };

    // Change race
    bool ok = true;
    for (int cityNr = 0; ok && (cityNr < m_game->getNrCities()); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (city->m_Plane != plane)
            continue;
        if (city->m_Race != fromRace)
            continue;

        // Change race
        if (!m_game->commitData(&city->m_Race, &toRace, sizeof(city->m_Race)))
        {
            setErrorString("Failed to commit race. Aborting");
            ok = false;
        }

        // Change garrison
        std::vector<int> units;
        m_game->findUnitsAtLocation(MoMLocation(*city), units);
        for (unsigned stackNr = 0; ok && (stackNr < units.size()); ++stackNr)
        {
            Unit* unit = m_game->getUnit(units[stackNr]);
            if (0 == unit)
            {
                setErrorString("Failed to retrieve unit. Aborting");
                ok = false;
                break;
            }
            MoMUnit momUnit(m_game, unit);
            if (momUnit.getRace() == fromRace)
            {
                // Replace to equivalent unit of the toRace
                eUnit_Type firstFrom = firstUnitOfRace[fromRace];
                eUnit_Type firstAfterFrom = firstUnitOfRace[Succ(fromRace)];
                eUnit_Type firstTo = firstUnitOfRace[toRace];
                eUnit_Type firstAfterTo = firstUnitOfRace[Succ(toRace)];

                eUnit_Type toType = firstTo;
                if ((unit->m_Unit_Type >= firstFrom) && (unit->m_Unit_Type < firstAfterFrom))
                {
                   eUnit_Type toType  = (eUnit_Type)((int)firstTo + (int)momUnit.getUnitTypeNr() - (int)firstFrom);
                   if (toType >= firstAfterTo)
                   {
                        toType = (eUnit_Type)((int)firstAfterTo - 1);
                   }
                }
                if (!m_game->commitData(&unit->m_Unit_Type, &toType, sizeof(unit->m_Unit_Type)))
                {
                    setErrorString("Failed to commit unit type. Aborting");
                    ok = false;
                }
            }
        }
    }

    return ok;
}

bool MoMController::removeCityFromGameQueue(int cityNr)
{
    MoMDataSegment* dataSegment = m_game->getDataSegment();
    if (0 == dataSegment)
        return false;
    bool removed = false;
    for (int queueNr = 0; (queueNr < dataSegment->m_nr_city_queue) && (toUInt(queueNr) < ARRAYSIZE(dataSegment->m_arr20_city_queue)); ++queueNr)
    {
        CityQueueElement* queueElt = &dataSegment->m_arr20_city_queue[queueNr];
        if (cityNr == queueElt->m_CityNr)
        {
            int8_t newCityNr = -1;
            removed = m_game->commitData(&queueElt->m_CityNr, &newCityNr, sizeof(queueElt->m_CityNr));
            break;
        }
    }
    return removed;
}

bool MoMController::repopLairs(bool maxOut)
{
	m_errorString.clear();

    if (0 == m_game)
        return false;

    for (unsigned lairNr = 0; lairNr < gMAX_NODES_LAIRS_TOWERS; ++lairNr)
    {
        Tower_Node_Lair* lair = m_game->getLair(lairNr);
        if (0 == lair)
            break;

        // Skip lairs that are occupied
        Node_Attr* nodeAttr = findNodeAttrAtLocation(MoMLocation(*lair));
        if (lair->m_Status == LAIRSTATUS_intact
            || lair->m_Inhabitant1.m_Remaining_Nr_of_Inhabitant != 0
            || lair->m_Inhabitant2.m_Remaining_Nr_of_Inhabitant != 0
            || countUnits(MoMLocation(*lair)) > 0
            || (nodeAttr != NULL && nodeAttr->m_Owner != PLAYER_Dismissed_Deceased))
        {
            continue;
        }

        if (maxOut)
        {
            if (0 == lair->m_Inhabitant1.m_Initial_Nr_of_Inhabitant || 0 == lair->m_Inhabitant1.m_Inhabitant)
            {
                // No inhabitants
            }
            else if (0 == lair->m_Inhabitant2.m_Initial_Nr_of_Inhabitant || 0 == lair->m_Inhabitant2.m_Inhabitant)
            {
                // Only 1 type of inhabitant
                lair->m_Inhabitant1.m_Initial_Nr_of_Inhabitant = 9;
            }
            else
            {
                lair->m_Inhabitant1.m_Initial_Nr_of_Inhabitant = 3;
                lair->m_Inhabitant2.m_Initial_Nr_of_Inhabitant = 6;
            }
            if (lair->m_Reward_Specials + 1 < eReward_Specials_MAX)
            {
                inc(lair->m_Reward_Specials);
            }
            lair->m_Item_Rewards++;
            if (lair->m_Item_Rewards > 3)
            {
                lair->m_Item_Rewards = 3;
            }
        }
        lair->m_Inhabitant1.m_Remaining_Nr_of_Inhabitant = lair->m_Inhabitant1.m_Initial_Nr_of_Inhabitant;
        lair->m_Inhabitant2.m_Remaining_Nr_of_Inhabitant = lair->m_Inhabitant2.m_Initial_Nr_of_Inhabitant;
        lair->m_Status = LAIRSTATUS_intact;
    }

    bool ok = m_game->commitData(m_game->getLair(0), m_game->getLair(0), gMAX_NODES_LAIRS_TOWERS * sizeof(Tower_Node_Lair));

    return ok;
}

bool MoMController::updateTaxAndPowerDivision(ePlayer playerNr)
{
    m_errorString.clear();
    if (0 == m_game)
        return false;

    bool ok = true;
    for (int cityNr = 0; ok && (cityNr < m_game->getNrCities()); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (city->m_Owner != playerNr)
            continue;
        MoMCity momCity(m_game, city);

        int8_t goldProduced = momCity.calcGoldProduced();
        int8_t hammersProduced = momCity.calcHammersProduced();
        if (!m_game->commitData(&city->m_Coins, &goldProduced, sizeof(city->m_Coins))
                || !m_game->commitData(&city->m_Hammers, &hammersProduced, sizeof(city->m_Hammers)))
        {
            setErrorString("Failed to commit tax changes to city");
            ok = false;
        }
    }

    // TODO: PowerDivision

    return ok;
}

}
