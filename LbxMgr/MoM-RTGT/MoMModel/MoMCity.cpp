#include "MoMCity.h"

#include "MoMGameBase.h"
#include "MoMTerrain.h"

namespace MoM {

MoMCity::MoMCity(MoMGameBase *game, const City *city) :
    m_game(game),
    m_city(city)
{
}

class CountBasicFood
{
public:
    CountBasicFood() : food(0) {}
    bool operator()(const MoMTerrain& terrain)
    {
        food += terrain.getBasicFoodBonus();
        return false;
    }
    int food;
};

int MoMCity::calcBasicFood() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    CountBasicFood count;
    enumerateTerrain(count);

    int basicFood = count.food;
    if (OWNER_None != m_city->m_City_Enchantments.Gaias_Blessing)
    {
        basicFood = basicFood * 3 / 2;
    }

    basicFood /= 4;

    return basicFood;
}

class CountGoldBonus
{
public:
    CountGoldBonus(bool minersGuild, bool dwarven)
        : m_minersGuild(minersGuild), m_dwarven(dwarven), goldBonus(0) {}
    bool operator()(const MoMTerrain& terrain)
    {
        goldBonus += terrain.getGoldBonus(m_minersGuild, m_dwarven);
        return false;
    }
    bool m_minersGuild;
    bool m_dwarven;
    int goldBonus;
};

int MoMCity::calcGoldBonus() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    CountGoldBonus count(isBuildingPresent(BUILDING_Miners_Guild), (m_city->m_Race == RACE_Dwarven));
    enumerateTerrain(count);

    return count.goldBonus;
}

class CountProductionPercentage
{
public:
    CountProductionPercentage(bool gaiasBlessing) : m_gaiasBlessing(gaiasBlessing), productionPercentage(0) {}
    bool operator()(const MoMTerrain& terrain)
    {
        productionPercentage += terrain.getProductionPercentage(m_gaiasBlessing);
        return false;
    }
    bool m_gaiasBlessing;
    int productionPercentage;
};

int MoMCity::calcProductionBonusPercentage() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    CountProductionPercentage count(OWNER_None != m_city->m_City_Enchantments.Gaias_Blessing);
    enumerateTerrain(count);

    return count.productionPercentage;
}

int MoMCity::calcFoodProduced() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (m_city->m_Population <= 0)
        return 0;

    int basicFood = calcBasicFood();
    int nrFarmers = Min(m_city->m_Number_of_farmers_allocated, m_city->m_Population);
    int food = 0;

    if ((m_city->m_Race == RACE_Halfling) || isBuildingPresent(BUILDING_Animists_Guild))
    {
        food = 3 * nrFarmers;
    }
    else
    {
        food = 2 * nrFarmers;
    }

    if (isBuildingPresent(BUILDING_Foresters_Guild))
    {
        food += 2;
    }
    if (OWNER_None != m_city->m_City_Enchantments._Famine_)
    {
        food /= 2;
    }
    if (food > basicFood)
    {
        food = basicFood + (food - basicFood) / 2;
    }
    if (isBuildingPresent(BUILDING_Granary))
    {
        food += 2;
    }
    if (isBuildingPresent(BUILDING_Farmers_Market))
    {
        food += 3;
    }
    food += getWildGameBonus();

    return food;
}

int MoMCity::calcGoldProduced() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (m_city->m_Population <= 0)
        return 0;

    int goldPercentage = 0;

    goldPercentage += calcTradeBonus();
    goldPercentage += calcRoadBonus();

    if (m_city->m_Race == RACE_Nomad)
    {
        goldPercentage += 50;
    }

    if (m_city->m_Population * 3 < goldPercentage)
    {
        goldPercentage = m_city->m_Population * 3;
    }

    if (isBuildingPresent(BUILDING_Merchants_Guild))
    {
        goldPercentage += 100;
    }
    if (isBuildingPresent(BUILDING_Bank))
    {
        goldPercentage += 50;
    }
    if (isBuildingPresent(BUILDING_Marketplace))
    {
        goldPercentage += 50;
    }
    if (OWNER_None != m_city->m_City_Enchantments.Prosperity)
    {
        goldPercentage += 100;
    }

    int productiveCitizens = m_city->m_Population - calcNrRebels();
    int goldIncome = productiveCitizens;
    if (m_city->m_Owner != PLAYER_NEUTRAL)
    {
        Wizard* wizard = m_game->getWizard(m_city->m_Owner);
        goldIncome = productiveCitizens * (int)wizard->m_Tax_Rate / 2;
    }
    if (m_city->m_Race == RACE_Dwarven)
    {
        goldIncome *= 2;
    }

    goldIncome += calcGoldBonus();

    goldIncome = goldIncome * (100 + goldPercentage) / 100;

    if (m_city->m_Producing == PRODUCING_Trade_Goods)
    {
        goldIncome += m_city->m_Hammers / 2;
    }

    goldIncome = Min(255, goldIncome);

    return goldIncome;
}

int MoMCity::calcHammersProduced() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (m_city->m_Population <= 0)
        return 0;

    int nrFarmers = Min(m_city->m_Number_of_farmers_allocated, m_city->m_Population);
    int nrWorkers = calcNrWorkers();
    int doubledHammersPerWorker = 4;
    if ((m_city->m_Race == RACE_Dwarven) || (m_city->m_Race == RACE_Klackon))
    {
        doubledHammersPerWorker = 6;
    }
    int hammers = (nrWorkers * doubledHammersPerWorker + nrFarmers + 1) / 2;

    int prodPercentage = 100;
    prodPercentage += calcProductionBonusPercentage();
    if (isBuildingPresent(BUILDING_Foresters_Guild))
    {
        prodPercentage += 25;
    }
    if (isBuildingPresent(BUILDING_Sawmill))
    {
        prodPercentage += 25;
    }
    if (isBuildingPresent(BUILDING_Miners_Guild))
    {
        prodPercentage += 50;
    }
    if (isBuildingPresent(BUILDING_Mechanicians_Guild))
    {
        prodPercentage += 50;
    }
    if (OWNER_None != m_city->m_City_Enchantments.Inspirations)
    {
        prodPercentage += 100;
    }

    hammers = hammers * prodPercentage / 100;

    if (OWNER_None != m_city->m_City_Enchantments._Cursed_Lands_)
    {
        hammers /= 2;
    }

    return hammers;
}

int MoMCity::calcMaxPopCurrent() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int maxPop = calcBasicFood();

    if (isBuildingPresent(BUILDING_Granary))
    {
        maxPop += 2;
    }
    if (isBuildingPresent(BUILDING_Farmers_Market))
    {
        maxPop += 3;
    }
    maxPop += getWildGameBonus();

    return maxPop;
}

int MoMCity::calcMaxPopTop() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int maxPop = calcBasicFood();
    maxPop += 2;    // Granary
    maxPop += 3;    // Farmers Market
    maxPop += getWildGameBonus();

    return maxPop;
}

int MoMCity::calcNrRebels() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int nrRebels = 0;
    if (OWNER_None == m_city->m_City_Enchantments.Stream_of_Life)
    {
        int unrestPercentage = calcUnrestPercentage();
        nrRebels = m_city->m_Population * unrestPercentage / 100;
        int pacifyingEffects = calcPacifyingReligiousEffects();
        pacifyingEffects += calcPacifyingNonReligiousEffects();
        pacifyingEffects += calcPacifyingUnitEffects();
        nrRebels = Range(nrRebels - pacifyingEffects, 0, (int)m_city->m_Population);
    }
    return nrRebels;
}

int MoMCity::calcNrWorkers() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (m_city->m_Population <= 0)
        return 0;

    int nrFarmers = Min(m_city->m_Number_of_farmers_allocated, m_city->m_Population);
    int nrRebels  = calcNrRebels();
    int nrWorkers = m_city->m_Population - nrFarmers - nrRebels;

    return nrWorkers;
}

int MoMCity::calcPacifyingNonReligiousEffects() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    Wizard* wizard = m_game->getWizard(m_city->m_Owner);
    if (0 == wizard)
        return -1;

    int pacifyingEffects = 0;

    if (isBuildingPresent(BUILDING_Oracle))
    {
        pacifyingEffects += 2;
    }
    if (isBuildingPresent(BUILDING_Animists_Guild))
    {
        pacifyingEffects += 1;
    }

    for (int playerNr = 0; playerNr < m_game->getNrWizards(); ++playerNr)
    {
        if (playerNr == m_city->m_Owner)
            continue;
        Wizard* opponent = m_game->getWizard(playerNr);
        if (opponent->m_Global_Enchantments.Great_Wasting)
        {
            pacifyingEffects -= 1;
        }
        if (opponent->m_Global_Enchantments.Armageddon)
        {
            pacifyingEffects -= 2;
        }
    }

    if (OWNER_None != m_city->m_City_Enchantments.Dark_Rituals)
    {
        pacifyingEffects -= 1;
    }
    if (OWNER_None != m_city->m_City_Enchantments._Pestilence_)
    {
        pacifyingEffects -= 2;
    }
    if (OWNER_None != m_city->m_City_Enchantments._Cursed_Lands_)
    {
        pacifyingEffects -= 1;
    }
    if (wizard->m_Global_Enchantments.Just_Cause)
    {
        pacifyingEffects += 1;
    }
    if (OWNER_None != m_city->m_City_Enchantments.Gaias_Blessing)
    {
        pacifyingEffects += 2;
    }

    return pacifyingEffects;
}

int MoMCity::calcPacifyingReligiousEffects() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    Wizard* wizard = m_game->getWizard(m_city->m_Owner);
    if (0 == wizard)
        return -1;

    int religiousEffects = 0;
    if ((OWNER_None == m_city->m_City_Enchantments._Evil_Presence_)
            || (wizard->m_Number_of_Spellbooks_Death > 0))
    {
        if (isBuildingPresent(BUILDING_Shrine))
        {
            religiousEffects++;
        }
        if (isBuildingPresent(BUILDING_Temple))
        {
            religiousEffects++;
        }
        if (isBuildingPresent(BUILDING_Parthenon))
        {
            religiousEffects++;
        }
        if (isBuildingPresent(BUILDING_Cathedral))
        {
            religiousEffects++;
        }
        if (wizard->m_Skills.s.eSkill_Infernal_Power || wizard->m_Skills.s.eSkill_Divine_Power)
        {
            religiousEffects += religiousEffects / 2;
        }
    }
    return religiousEffects;
}

int MoMCity::calcPacifyingUnitEffects() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int unitEffects = 0;
    for (int unitNr = 0; unitNr < m_game->getNrUnits(); ++unitNr)
    {
        const Unit* unit = m_game->getUnit(unitNr);
        if (0 == unit)
            break;
        if (unit->m_Owner != m_city->m_Owner)
            continue;
        if (MoMLocation(*unit) != MoMLocation(*m_city))
            continue;
        if (unit->m_Unit_Type < UNITTYPE_Arcane_Magic_Spirit)
        {
            unitEffects += 1;
        }
    }
    unitEffects /= 2;
    return unitEffects;
}

int MoMCity::calcRoadBonus() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int roadBonus = 0;
    for (int cityNr = 0; (cityNr < m_game->getNrCities()) && (toUInt(cityNr) < gMAX_CITIES); ++cityNr)
    {
        if (cityNr == getCityNr())
            continue;
        unsigned bitmask = (1U << (cityNr % 8));
        if (0 == (m_city->m_bitsetConnectedCities[cityNr / 8] & bitmask))
            continue;
        const City* connectedCity = m_game->getCity(cityNr);
        if (connectedCity->m_Race == m_city->m_Race)
        {
            roadBonus += connectedCity->m_Population / 2;
        }
        else
        {
            roadBonus += connectedCity->m_Population;
        }
    }

    roadBonus = Min(m_city->m_Population * 3, roadBonus);

    return roadBonus;
}

int MoMCity::calcTradeBonus() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    bool onRiver = false;
    bool atSea = false;
    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            MoMLocation location(
                        (m_city->m_XPos + dx + gMAX_MAP_COLS) % gMAX_MAP_COLS,
                        (m_city->m_YPos + dy + gMAX_MAP_ROWS) % gMAX_MAP_ROWS,
                        m_city->m_Plane,
                        MoMLocation::MAP_overland);
            MoMTerrain terrain(m_game, location);
            if ((dx == 0) && (dy == 0))
            {
                onRiver = terrain.isRiver();
            }
            else
            {
                atSea |= terrain.isSea();
            }
        }
    }

    int tradeBonus = 0;
    if (onRiver)
    {
        if (atSea)
        {
            // City on river mouth
            tradeBonus = 30;
        }
        else
        {
            // City on river
            tradeBonus = 20;
        }
    }
    else
    {
        if (atSea)
        {
            // City at sea
            tradeBonus = 10;
        }
    }
    return tradeBonus;
}

int MoMCity::calcUnrestPercentage() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    Wizard* wizard = m_game->getWizard(m_city->m_Owner);
    if (0 == wizard)
        return -1;

    const uint16_t* taxUnrestTable = m_game->getTaxUnrestTable();
    if (0 == taxUnrestTable)
        return -1;
    int unrestPercentage = taxUnrestTable[wizard->m_Tax_Rate];

    eRace homeRace = wizard->m_Home_Race;
    const int8_t* racialUnrestTable = m_game->getUnrest_Table(homeRace);
    if (0 == racialUnrestTable)
        return -1;
    unrestPercentage += 10 * racialUnrestTable[m_city->m_Race];

    if (OWNER_None != m_city->m_City_Enchantments._Famine_)
    {
        unrestPercentage += 25;
    }

    return unrestPercentage;
}

bool MoMCity::canProduce(eBuilding building) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    // Cannot build what is already there
    if (isBuildingPresent(building))
        return false;

    bool allowed = true;

    // Check prohibited buildings
    allowed = isBuildingAllowed(building);

    // Check prerequisites
    Building_Data* buildingData = m_game->getBuildingData(building);
    if (0 == buildingData)
        return false;
    if (!isBuildingPresent(buildingData->m_Prerequisite1))
    {
        allowed = false;
    }
    if (!isBuildingPresent(buildingData->m_Prerequisite2))
    {
        allowed = false;
    }

    return allowed;
}

bool MoMCity::canProduce(eProducing produce) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    bool allowed = canProduce(static_cast<eBuilding>(produce));
    if (!allowed)
    {
        allowed = canProduce(static_cast<eUnit_Type>(toUInt(produce) - 100));
    }
    return allowed;
}

bool MoMCity::canProduce(eUnit_Type unitTypeNr) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    Unit_Type_Data* unitType = m_game->getUnitTypeData(unitTypeNr);
    if (0 == unitType)
        return false;

    // Check race
    if ((unitType->m_Race_Code != m_city->m_Race) && (unitType->m_Race_Code != RACE_Generic))
    {
        return false;   // Wrong race and not generic
    }

    // Check building requirements
    if (unitType->m_Building1Required_or_PortraitLbxIndex == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(unitType->m_Building1Required_or_PortraitLbxIndex) < eBuilding_array_MAX)
             && (m_city->m_Building_Status.a[unitType->m_Building1Required_or_PortraitLbxIndex] != BUILDINGSTATUS_Built)
             && (m_city->m_Building_Status.a[unitType->m_Building1Required_or_PortraitLbxIndex] != BUILDINGSTATUS_Replaced))
    {
        return false;   // First required building not present
    }
    else if (static_cast<eBuilding>(unitType->m_Building2_or_HeroType) == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(unitType->m_Building2_or_HeroType) < eBuilding_array_MAX)
             && (m_city->m_Building_Status.a[unitType->m_Building2_or_HeroType] != BUILDINGSTATUS_Built)
             && (m_city->m_Building_Status.a[unitType->m_Building2_or_HeroType] != BUILDINGSTATUS_Replaced))
    {
        return false;   // Second required building not present
    }

    // Check if there is space
    std::vector<int> units;
    m_game->findUnitsAtLocation(MoMLocation(*m_city), units);
    if (units.size() >= gMAX_UNITS_IN_LOCATION)
        return false;

    return true;
}

int MoMCity::countConnectedCities() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    int connectedCities = 0;
    for (int cityNr = 0; (cityNr < m_game->getNrCities()) && (toUInt(cityNr) < gMAX_CITIES); ++cityNr)
    {
        unsigned bitmask = (1U << (cityNr % 8));
        if (0 == (m_city->m_bitsetConnectedCities[cityNr / 8] & bitmask))
            continue;
        connectedCities++;
    }
    return connectedCities;
}

int MoMCity::getBuyFactor(eProducing producing) const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (PRODUCING_None == producing)
    {
        producing = m_city->m_Producing;
    }

    int buildingCost = getCostToProduce(producing);
    int producedNextTurn = m_city->m_HammersAccumulated + m_city->m_Hammers;
    int buyFactor;
    if (producedNextTurn >= buildingCost)
    {
        buyFactor = 0;    // Cannot buy - completed next turn
    }
    else if (m_city->m_HammersAccumulated <= 0)
    {
        buyFactor = 4;
    }
    else if (m_city->m_HammersAccumulated < buildingCost / 3)
    {
        buyFactor = 3;
    }
    else
    {
        buyFactor = 2;
    }
    return buyFactor;
}

const City *MoMCity::getCity() const
{
    return m_city;
}

int MoMCity::getCityNr() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    return (int)(m_city - m_game->getCity(0));
}

template<typename Functor>
bool MoMCity::enumerateTerrain(Functor& functor) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    for (int dy = -2; dy <= 2; ++dy)
    {
        for (int dx = -2; dx <= 2; ++dx)
        {
            if (Abs(dx) + Abs(dy) >= 4)
                continue;
            MoMLocation location(
                        (m_city->m_XPos + dx + gMAX_MAP_COLS) % gMAX_MAP_COLS,
                        (m_city->m_YPos + dy + gMAX_MAP_ROWS) % gMAX_MAP_ROWS,
                        m_city->m_Plane,
                        MoMLocation::MAP_overland);
            MoMTerrain terrain(m_game, location);
            if (functor(terrain))
                return true;
        }
    }
    return false;
}

int MoMCity::getCostToBuy(eProducing producing) const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (PRODUCING_None == producing)
    {
        producing = m_city->m_Producing;
    }

    int buildingCost = getCostToProduce(producing);
    int productionRemaining = buildingCost - m_city->m_HammersAccumulated;
    int buyFactor = getBuyFactor();
    int buyCost = buyFactor * productionRemaining;
    return buyCost;
}

int MoMCity::getCostToProduce(eProducing producing) const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (PRODUCING_None == producing)
    {
        producing = m_city->m_Producing;
    }

    int buildingCost = -1;
    if (producing < MoM::eProducing_Building_MAX)
    {
        MoM::Building_Data* buildingData = m_game->getBuildingData((eBuilding)producing);
        if (0 != buildingData)
        {
            buildingCost = buildingData->m_Building_cost;
        }
    }
    else
    {
        MoM::eUnit_Type unitTypeNr = (MoM::eUnit_Type)((int)producing - (int)MoM::PRODUCING_Trireme + (int)MoM::UNITTYPE_Trireme);
        MoM::Unit_Type_Data* unitData = m_game->getUnitTypeData(unitTypeNr);
        if (0 != unitData)
        {
            buildingCost = unitData->m_Cost;
        }

        int reductionPercentage = getUnitReductionPercentage();
        buildingCost = buildingCost * (100 - reductionPercentage) / 100;
        if (buildingCost < 0)
        {
            buildingCost = 0;
        }
    }
    return buildingCost;
}

int MoMCity::getTimeToComplete(eProducing producing) const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    if (PRODUCING_None == producing)
    {
        producing = m_city->m_Producing;
    }

    int timeCompletion = 999;
    if (m_city->m_Hammers > 0)
    {
        int buildingCost = getCostToProduce(producing);
        timeCompletion = (buildingCost - m_city->m_HammersAccumulated + m_city->m_Hammers - 1) / m_city->m_Hammers;
    }
    return timeCompletion;
}

// TODO: Refactor to MoMTerrain::getUnitReduction()
class CountUnitReduction
{
public:
    CountUnitReduction() : reduction(0) {}
    bool operator()(const MoMTerrain& terrain)
    {
        if (terrain.getChanges().corruption)
            return false;

        if (terrain.getBonus() == DEPOSIT_Iron_Ore)
        {
            reduction += 5;
        }
        else if (terrain.getBonus() == DEPOSIT_Coal)
        {
            reduction += 10;
        }

        if (terrain.isSharedBetweenCities())
        {
            reduction /= 2;
        }
        return false;
    }
    int reduction;
};

int MoMCity::getUnitReductionPercentage() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    CountUnitReduction count;
    enumerateTerrain(count);
    return count.reduction;
}

class CountWildGame
{
public:
    CountWildGame() : food(0) {}
    bool operator()(const MoMTerrain& terrain)
    {
        if (terrain.getChanges().corruption)
            return false;
        if (terrain.getBonus() == DEPOSIT_Wild_Game)
        {
            if (terrain.isSharedBetweenCities())
            {
                food++;
            }
            else
            {
                food += 2;
            }
        }
        return false;
    }
    int food;
};

int MoMCity::getWildGameBonus() const
{
    if ((0 == m_game) || (0 == m_city))
        return -1;

    CountWildGame count;
    enumerateTerrain(count);
    return count.food;
}

class CheckCleanForest
{
public:
    bool operator()(const MoMTerrain& terrain)
    {
        if (terrain.getChanges().corruption)
            return false;
        return ((terrain.getCategory() == TERRAINCATEGORY_Forest)
                || (terrain.getCategory() == TERRAINCATEGORY_NatureNode));
    }
};

bool MoMCity::hasForestRequirement() const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    CheckCleanForest check;
    return enumerateTerrain(check);
}

class CheckCleanHill
{
public:
    bool operator()(const MoMTerrain& terrain)
    {
        if (terrain.getChanges().corruption)
            return false;
        return ((terrain.getCategory() == TERRAINCATEGORY_Hills)
                || (terrain.getCategory() == TERRAINCATEGORY_Mountain)
                || (terrain.getCategory() == TERRAINCATEGORY_Volcano)
                || (terrain.getCategory() == TERRAINCATEGORY_ChaosNode));
    }
};

bool MoMCity::hasHillRequirement() const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    CheckCleanHill check;
    return enumerateTerrain(check);
}

class CheckCleanWater
{
public:
    bool operator()(const MoMTerrain& terrain)
    {
        if (terrain.getChanges().corruption)
            return false;
        return ((terrain.getCategory() == TERRAINCATEGORY_Ocean)
                || (terrain.getCategory() == TERRAINCATEGORY_Shore));
    }
};

bool MoMCity::hasWaterRequirement() const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    CheckCleanWater check;
    return enumerateTerrain(check);
}

bool MoMCity::isProductionAllowed(eProducing producing) const
{
    return ((inRange(producing, PRODUCING_Trade_Goods, eProducing_Building_MAX) && isBuildingAllowed(static_cast<eBuilding>(producing)))
            || (inRange(producing, PRODUCING_Trireme, eProducing_MAX) && isUnitAllowed(static_cast<eUnit_Type>(toUInt(producing) - 100))));
}

bool MoMCity::isBuildingAllowed(eBuilding building) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    if (toUInt(building) < BUILDING_Barracks)
        return true;
    if (building >= eBuilding_array_MAX)
        return false;

    // Check prohibited buildings
    Race_Data* raceData = m_game->getRaceData(m_city->m_Race);
    if (0 == raceData)
        return false;
    bool allowed = true;
    for (unsigned i = 0; allowed
        && (i < raceData->m_Number_of_prohibited_buildings)
        && (i < ARRAYSIZE(raceData->m_Prohibited_buildings));
        ++i)
    {
        if (building == raceData->m_Prohibited_buildings[i])
        {
            allowed = false;
        }
    }

    // Check if prerequisite buildings are allowed
    Building_Data* buildingData = m_game->getBuildingData(building);
    if (0 == buildingData)
        return false;
    if (!isBuildingPresent(buildingData->m_Prerequisite1) && !isBuildingAllowed(buildingData->m_Prerequisite1))
    {
        allowed = false;
    }
    else if (!isBuildingPresent(buildingData->m_Prerequisite2) && !isBuildingAllowed(buildingData->m_Prerequisite2))
    {
        allowed = false;
    }

    return allowed;
}

bool MoMCity::isBuildingPresent(eBuilding building) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    if (building == BUILDING_Forest)
    {
        return hasForestRequirement();
    }
    else if (building == BUILDING_Hill)
    {
        return hasHillRequirement();
    }
    else if (building == BUILDING_Water)
    {
        return hasWaterRequirement();
    }
    else if (toUInt(building) < eBuilding_array_MAX)
    {
        return (BUILDINGSTATUS_Built == m_city->m_Building_Status.a[building])
                || (BUILDINGSTATUS_Replaced == m_city->m_Building_Status.a[building]);
    }
    else
    {
        return false;
    }
}

bool MoMCity::isUnitAllowed(eUnit_Type unitTypeNr) const
{
    if ((0 == m_game) || (0 == m_city))
        return false;

    if (unitTypeNr < UNITTYPE_FIRST)
        return false;   // Cannot build heroes

    Unit_Type_Data* unitType = m_game->getUnitTypeData(unitTypeNr);
    if (0 == unitType)
        return false;

    // Check race
    if ((unitType->m_Race_Code != m_city->m_Race) && (unitType->m_Race_Code != RACE_Generic))
    {
        return false;   // Wrong race and not generic
    }

    // Check building requirements
    if (unitType->m_Building1Required_or_PortraitLbxIndex == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(unitType->m_Building1Required_or_PortraitLbxIndex) < eBuilding_array_MAX)
             && !isBuildingAllowed(static_cast<eBuilding>(unitType->m_Building1Required_or_PortraitLbxIndex)))
    {
        return false;   // First required building not allowed
    }
    else if (static_cast<eBuilding>(unitType->m_Building2_or_HeroType) == BUILDING_None)
    {
        // Building requirements satisfied
    }
    else if ((toUInt(unitType->m_Building2_or_HeroType) < eBuilding_array_MAX)
             && !isBuildingAllowed(static_cast<eBuilding>(unitType->m_Building2_or_HeroType)))
    {
        return false;   // Second required building not present
    }

    return true;
}

}
