#include "MoMManageCities.h"

#include "MoMController.h"
#include "MoMGameBase.h"
#include "MoMGenerated.h"
#include "MoMUnit.h"
#include "MoMUtility.h"

namespace MoM {

/////////////////////////////////////////

std::ostream& operator<<(std::ostream& os, eAdditionalCityTargets target)
{
    std::string str = "<UNDEF eAdditionalCityTargets>";
    switch (target)
    {
    case CITYTARGET_Player:     str = "Player"; break;
    case CITYTARGET_Growth:     str = "Growth"; break;
    case CITYTARGET_Economy:    str = "Economy"; break;
    case CITYTARGET_Power:      str = "Power"; break;
    case CITYTARGET_Research:   str = "Research"; break;
    default:                    ;
    }
    os << str;
    return os;
}

static const char* sStrCityState[MoMManageCities::MoMCityState::eCityState_MAX + 1] =
{
    "Unknown",
    "Disabled",
    "Outpost",
    "Conquered",
    "ExpectedBuilding",
    "ExpectedUnit",
    "BuildingCompleted",
    "UnitCompleted",
    "UnexpectedBuilding",
    "UnexpectedUnit",
    "EverythingBuilt",
    "eCityState_MAX"
};

std::ostream& operator<<(std::ostream& os, MoMManageCities::MoMCityState::eCityState cityState)
{
    std::string str = "<UNDEF eCityState>";
    if (toUInt(cityState) < ARRAYSIZE(sStrCityState))
    {
        str = sStrCityState[cityState];
    }
    os << str;
    return os;
}

/////////////////////////////////////////

MoMManageCities::MoMCityState::MoMCityState(const MoMCity& momCity) :
    m_producingTarget(PRODUCING_None),
    m_momCity(momCity),
    m_cityState(CITYSTATE_Unknown)
{
    updateState();
}

bool MoMManageCities::MoMCityState::apply()
{
    updateState();

    bool changed = false;
    eProducing produce = calcProduction();
    // Don't change production if it would lose us production
    if (produce != m_momCity.getCity()->m_Producing)
    {
        if (m_momCity.getCity()->m_HammersAccumulated >= m_momCity.getCostToProduce(produce))
        {
            produce = PRODUCING_None;
        }
    }

    changed = commitProduction(produce);
    return changed;
}

void MoMManageCities::MoMCityState::setTarget(eTarget target)
{
    m_producingTarget = target;

    eProducing produce = calcProduction();

    // Don't change production if it would lose us production
    if (produce != m_momCity.getCity()->m_Producing)
    {
        if (m_momCity.getCity()->m_HammersAccumulated >= m_momCity.getCostToProduce(produce))
        {
            produce = PRODUCING_None;
        }
    }

    (void)commitProduction(produce);
}

eProducing MoMManageCities::MoMCityState::calcProductionGrowth() const
{
    const City* city = m_momCity.getCity();

    eProducing producingAfter = city->m_Producing;
    eProducing produce = PRODUCING_None;

    MoMLocation location(*city, MoMLocation::MAP_overland);
    int garrisonSize = MoMController(m_momCity.getGame()).countGarrison(location);

    // Only change if we producing 'Housing' or 'Trade Goods'
    // This is a good trigger because when something is built
    // the game switches to Housing automatically (unless it was a unit).
    // And when you conquer a city the game sets it tot 'Trade Goods'.
    // The exception is the Garrison - if there is no Garrison, built one first!

    // OPTIMAL GROWTH W/O GOLD AND W/O SIGNIFICANT UNREST
    //[Garrison]
    //[Builders Hall]
    //[Granary]
    //[Smithy]
    //[Marketplace]
    //[Farmers Market]

    // FURTHER RULES
    //[Housing]             till Pop 4
    //[Shrine]
    //[Sawmill]
    //[Foresters Guild]
    //[Library]
    //[Sages Guild]
    //[Temple]
    //[Alchemists Guild]
    //[University]
    //[Bank]
    //[Miners Guild]
    //[Stable]              if Animists Guild allowed
    //[Animists Guild]
    //[Parthenon]
    //[Shipwright Guild]    if Merchants Guild allowed
    //[Ship Yard Guild]     if Merchants Guild allowed
    //[Merchants Guild]
    //[Cathedral]
    //[Wizards Guild]

    if (CITYSIZE_Outpost == city->m_Size)
    {
        // Do nothing - we're an outpost
//        std::cout << "City '" << city->m_City_Name << "' [" << cityNr << "] "
//            << city->m_Producing << " is still an outpost" << std::endl;
    }
    else if ((garrisonSize < 2) && findCheapestUnitToProduce(produce) && m_momCity.canProduce(produce))
    {
        producingAfter = produce;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Builders_Hall))
    {
        producingAfter = PRODUCING_Builders_Hall;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Granary))
    {
        producingAfter = PRODUCING_Granary;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Smithy))
    {
        producingAfter = PRODUCING_Smithy;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Marketplace))
    {
        producingAfter = PRODUCING_Marketplace;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Farmers_Market))
    {
        producingAfter = PRODUCING_Farmers_Market;
    }
    else if (city->m_Population < 4)
    {
        producingAfter = PRODUCING_Housing;
    }

    else if (!m_momCity.isBuildingPresent(BUILDING_Shrine))
    {
        producingAfter = PRODUCING_Shrine;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Sawmill) && m_momCity.canProduce(BUILDING_Sawmill))
    {
        producingAfter = PRODUCING_Sawmill;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Foresters_Guild) && m_momCity.canProduce(BUILDING_Foresters_Guild))
    {
        producingAfter = PRODUCING_Foresters_Guild;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Library) && m_momCity.canProduce(BUILDING_Library))
    {
        producingAfter = PRODUCING_Library;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Sages_Guild) && m_momCity.canProduce(BUILDING_Sages_Guild))
    {
        producingAfter = PRODUCING_Sages_Guild;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Temple) && m_momCity.canProduce(BUILDING_Temple))
    {
        producingAfter = PRODUCING_Temple;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Alchemist_Guild) && m_momCity.canProduce(BUILDING_Alchemist_Guild))
    {
        producingAfter = PRODUCING_Alchemist_Guild;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_University) && m_momCity.canProduce(BUILDING_University))
    {
        producingAfter = PRODUCING_University;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Bank) && m_momCity.canProduce(BUILDING_Bank))
    {
        producingAfter = PRODUCING_Bank;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Miners_Guild) && m_momCity.canProduce(BUILDING_Miners_Guild))
    {
        producingAfter = PRODUCING_Miners_Guild;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Animists_Guild) && m_momCity.canProduce(BUILDING_Animists_Guild))
    {
        producingAfter = PRODUCING_Animists_Guild;
    }
    else if (m_momCity.isBuildingAllowed(BUILDING_Animists_Guild) && !m_momCity.isBuildingPresent(BUILDING_Stable) && m_momCity.canProduce(BUILDING_Stable))
    {
        producingAfter = PRODUCING_Stable;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Parthenon) && m_momCity.canProduce(BUILDING_Parthenon))
    {
        producingAfter = PRODUCING_Parthenon;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Merchants_Guild) && m_momCity.canProduce(BUILDING_Merchants_Guild))
    {
        producingAfter = PRODUCING_Merchants_Guild;
    }
    else if (m_momCity.isBuildingAllowed(BUILDING_Merchants_Guild) && !m_momCity.isBuildingPresent(BUILDING_Shipwright_Guild)
             && m_momCity.canProduce(BUILDING_Shipwright_Guild))
    {
        producingAfter = PRODUCING_Shipwright_Guild;
    }
    else if (m_momCity.isBuildingAllowed(BUILDING_Merchants_Guild) && !m_momCity.isBuildingPresent(BUILDING_Ship_Yard)
             && m_momCity.canProduce(BUILDING_Ship_Yard))
    {
        producingAfter = PRODUCING_Ship_Yard;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Cathedral) && m_momCity.canProduce(BUILDING_Cathedral))
    {
        producingAfter = PRODUCING_Cathedral;
    }
    else if (!m_momCity.isBuildingPresent(BUILDING_Wizards_Guild) && m_momCity.canProduce(BUILDING_Wizards_Guild))
    {
        producingAfter = PRODUCING_Wizards_Guild;
    }
    else
    {
        // Do nothing - all specified buildings have been built
        //std::cout << "City " << city->m_City_Name
        //    << " (" << city->m_Producing << ") has built all specified buildings" << std::endl;
        producingAfter = PRODUCING_Trade_Goods;
    }

    return producingAfter;
}

eProducing MoMManageCities::MoMCityState::calcProduction() const
{
    eProducing produce = PRODUCING_None;

    // If strategy is disabled
    if (m_cityState == CITYSTATE_Disabled)
    {
        // Nothing to select
    }
    // If building or unit selected
    else if (m_producingTarget < eProducing_MAX)
    {
        std::vector<eProducing> reqList;
        if (findRequiredBuildings(m_producingTarget, reqList))
        {
            // Select cheapest
            int lowestCost = 99999;
            for (size_t i = 0; i < reqList.size(); ++i)
            {
                int cost = m_momCity.getCostToProduce(reqList[i]);
                if (cost < lowestCost)
                {
                    produce = reqList[i];
                    lowestCost = cost;
                }
            }
        }
    }
    // If other strategy
    else
    {
        // Select next production item on the list
        produce = calcProductionGrowth();
    }

    return produce;
}

bool MoMManageCities::MoMCityState::commitProduction(eProducing produce)
{
    if (produce <= PRODUCING_None)
        return false;   // Invalid
    if (produce >= eProducing_MAX)
        return false;   // Invalid

    City* city = const_cast<City*>(m_momCity.getCity());

    if (city->m_Producing == produce)
    {
        return false;   // Nothing to change
    }

    eProducing producingBefore = city->m_Producing;
    if (!m_momCity.getGame()->commitData(&city->m_Producing, &produce, sizeof(city->m_Producing)))
    {
        return false;   // Failed to commit
    }

    // Try to remove city from game queue to pop up the city screen, if we managed the production
    MoMController(m_momCity.getGame()).removeCityFromGameQueue(m_momCity.getCityNr());

    std::cout << "Changed production of '" << city->m_City_Name << "' [" << m_momCity.getCityNr() << "] "
              << producingBefore << " to " << produce << std::endl;

    return true;
}

bool MoMManageCities::MoMCityState::findRequiredBuildings(eProducing produce, std::vector<eProducing> &reqList) const
{
    if (toUInt(produce) < eProducing_Building_MAX)
    {
        return findRequiredBuildings((eBuilding)produce, reqList);
    }
    else
    {
        return findRequiredBuildings((eUnit_Type)(toUInt(produce) - 100), reqList);
    }
}

bool MoMManageCities::MoMCityState::findRequiredBuildings(eBuilding building, std::vector<eProducing>& reqList) const
{
    if (toUInt(building) < BUILDING_Barracks)
        return true;
    if (building >= eBuilding_array_MAX)
        return false;

    if (m_momCity.isBuildingPresent(building))
    {
        return true;
    }
    if (m_momCity.canProduce(building))
    {
        reqList.push_back((eProducing)building);
        return true;
    }

    // Check prerequisite buildings
    const Building_Data* buildingData = m_momCity.getGame()->getBuildingData(building);
    if (0 == buildingData)
        return false;
    bool ok = findRequiredBuildings(buildingData->m_Prerequisite1, reqList);
    if (ok)
    {
        ok = findRequiredBuildings(buildingData->m_Prerequisite2, reqList);
    }
    return ok;
}

bool MoMManageCities::MoMCityState::findRequiredBuildings(eUnit_Type unitTypeNr, std::vector<eProducing>& reqList) const
{
    if (unitTypeNr < UNITTYPE_FIRST)
        return false;   // Cannot build heroes
    if (unitTypeNr >= UNITTYPE_Arcane_Magic_Spirit)
        return false;   // Cannot build summoned creatures

    if (m_momCity.canProduce(unitTypeNr))
    {
        reqList.push_back((eProducing)(toUInt(unitTypeNr) + 100));
        return true;
    }

    Unit_Type_Data* unitType = m_momCity.getGame()->getUnitTypeData(unitTypeNr);
    if (0 == unitType)
        return false;

    // Check building requirements
    bool ok = findRequiredBuildings(static_cast<eBuilding>(unitType->m_Building1Required_or_PortraitLbxIndex), reqList);
    if (ok)
    {
        ok = findRequiredBuildings(static_cast<eBuilding>(unitType->m_Building2_or_HeroType), reqList);
    }
    return ok;
}

bool MoMManageCities::MoMCityState::producingBuilding() const
{
    return ((m_momCity.getCity()->m_Producing >= PRODUCING_Barracks)
            && (m_momCity.getCity()->m_Producing < eProducing_Building_MAX));
}

bool MoMManageCities::MoMCityState::producingGarrison() const
{
    return (m_momCity.getCity()->m_Producing >= eProducing_Building_MAX);
}

void MoMManageCities::MoMCityState::updateState()
{
    const City* city = m_momCity.getCity();
    eProducing producing = city->m_Producing;

    if (m_producingTarget == PRODUCING_None)
    {
        m_producingTarget = (eTarget)CITYTARGET_Growth;
    }

    eCityState prev = m_cityState;
    if (city->m_Size == CITYSIZE_Outpost)
    {
        m_cityState = CITYSTATE_Outpost;
    }
    else if (producing == PRODUCING_Housing)
    {
        if ((m_cityState == CITYSTATE_Unknown)
                || (m_cityState == CITYSTATE_Outpost)
                || (m_cityState == CITYSTATE_ExpectedBuilding)
                || (m_cityState == CITYSTATE_UnexpectedBuilding)
                || (m_cityState == CITYSTATE_BuildingCompleted))
        {
            m_cityState = CITYSTATE_BuildingCompleted;
        }
        else
        {
            m_cityState = CITYSTATE_Disabled;
        }
    }
    else if (producing == PRODUCING_Trade_Goods)
    {
        eProducing expected = calcProduction();
        if (expected == PRODUCING_Trade_Goods)
        {
            m_cityState = CITYSTATE_EverythingBuilt;
        }
        else if ((m_cityState == CITYSTATE_Unknown)
            || (m_cityState == CITYSTATE_Conquered))
        {
            m_cityState = CITYSTATE_Conquered;
        }
        else
        {
            m_cityState = CITYSTATE_Disabled;
        }
    }
    else if (producingGarrison())
    {
        eProducing expected = calcProduction();
        if (city->m_HammersAccumulated == 0)
        {
            if ((m_cityState == CITYSTATE_Unknown)
                    || (m_cityState == CITYSTATE_ExpectedUnit)
                    || (m_cityState == CITYSTATE_UnitCompleted))
            {
                m_cityState = CITYSTATE_UnitCompleted;
            }
            else if (m_cityState == CITYSTATE_UnexpectedUnit)
            {
                m_cityState = CITYSTATE_UnexpectedUnit;
            }
            else
            {
                m_cityState = CITYSTATE_Disabled;
            }
        }
        else if (expected == producing)
        {
            m_cityState = CITYSTATE_ExpectedUnit;
        }
        else
        {
            m_cityState = CITYSTATE_UnexpectedUnit;
        }
    }
    else if (producingBuilding())
    {
        eProducing expected = calcProduction();
        if (expected == producing)
        {
            m_cityState = CITYSTATE_ExpectedBuilding;
        }
        else
        {
            m_cityState = CITYSTATE_UnexpectedBuilding;
        }
    }
    else
    {
        m_cityState = CITYSTATE_Disabled;
    }

    if (m_cityState == CITYSTATE_Disabled)
    {
        m_producingTarget = (eTarget)CITYTARGET_Player;
    }

    if (prev != m_cityState)
    {
        std::cout << "Change state of " << m_momCity.getCity()->m_City_Name << " from " << prev << " to " << m_cityState << std::endl;
    }
}

bool MoMManageCities::MoMCityState::findCheapestUnitToProduce(eProducing &produce) const
{
    bool found = false;
    for (eUnit_Type unitTypeNr = UNITTYPE_FIRST; !found && (toUInt(unitTypeNr) < eUnit_Type_MAX); inc(unitTypeNr))
    {
        MoMUnit momUnit(m_momCity.getGame());
        momUnit.changeUnit(unitTypeNr);
        if (momUnit.isShip())
            continue;   // Disregard ships
        if (momUnit.isSettler())
            continue;   // Disregard settlers
        if (momUnit.isBuildable(*m_momCity.getCity()))
        {
            produce = static_cast<eProducing>(toUInt(unitTypeNr) - toUInt(UNITTYPE_Trireme) + toUInt(PRODUCING_Trireme));
            found = true;
        }
    }
    return found;
}

/////////////////////////////////////////

MoMManageCities::MoMManageCities(MoMGameBase* game) :
    m_game(game),
    m_cityStates(gMAX_CITIES)
{
}

MoMManageCities::~MoMManageCities()
{
    for (size_t i = m_cityStates.size(); i-- > 0; )
    {
        delete m_cityStates[i];
    }
}

bool MoMManageCities::apply()
{
    // Do not apply build queues if we're in battle

    if (m_game->isBattleInProgress())
    {
        std::cout << "Battle in progress. Skipping ManageCities" << std::endl;
        return false;
    }

    return applyBuildingQueue(PLAYER_YOU);
}

bool MoMManageCities::applyBuildingQueue(ePlayer playerNr)
{
    if (0 == m_game)
        return false;

    bool changed = false;
    for (int cityNr = 0; cityNr < m_game->getNrCities(); ++cityNr)
    {
        const City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;

        if (city->m_Owner == playerNr)
        {
            changed |= getCityState(cityNr)->apply();
        }
    }
    return changed;
}

MoMManageCities::MoMCityState* MoMManageCities::getCityState(int cityNr)
{
    if (toUInt(cityNr) >= m_cityStates.size())
        return 0;
    if (0 == m_cityStates[cityNr])
    {
        m_cityStates[cityNr] = new MoMCityState(MoMCity(m_game, m_game->getCity(cityNr)));
    }
    return m_cityStates[cityNr];
}

}
