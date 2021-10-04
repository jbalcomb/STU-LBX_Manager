#ifndef MOMMANAGECITIES_H
#define MOMMANAGECITIES_H

#include <iostream>
#include <vector>

#include "MoMCity.h"
#include "MoMTemplate.h"

namespace MoM {

class MoMGameBase;

enum eAdditionalCityTargets
{
    CITYTARGET_Player = 300,
    CITYTARGET_FIRST_ADDITIONAL_TARGET = 300,
    CITYTARGET_Growth = 301,
    CITYTARGET_Economy = 302,
    CITYTARGET_Power = 303,
    CITYTARGET_Research = 304,

    eAdditionalCityTargets_MAX
};

std::ostream& operator<<(std::ostream& os, eAdditionalCityTargets target);

class MoMManageCities
{
public:
    //! Keeps track of the city management state of a city.
    //! That is, which target it is working towards producing.
    //! And whether, the city is managed by the player versus the management system.
    class MoMCityState
    {
    public:
        typedef eProducing eTarget;
        enum eCityState
        {
            CITYSTATE_Unknown,
            CITYSTATE_Disabled,
            CITYSTATE_Outpost,
            CITYSTATE_Conquered,
            CITYSTATE_ExpectedBuilding,
            CITYSTATE_ExpectedUnit,
            CITYSTATE_BuildingCompleted,
            CITYSTATE_UnitCompleted,
            CITYSTATE_UnexpectedBuilding,
            CITYSTATE_UnexpectedUnit,
            CITYSTATE_EverythingBuilt,
            eCityState_MAX
        };

        explicit MoMCityState(const MoMCity& momCity);

        //! \return Whether production was changed
        bool apply();
        const MoMCity& getMoMCity() const { return m_momCity; }
        eCityState getState() const { return m_cityState; }
        eTarget getTarget() const { return m_producingTarget; }
        void setTarget(eProducing target);
    private:
        eProducing calcProductionGrowth() const;
        eProducing calcProduction() const;
        //! \return Whether production was changed
        bool commitProduction(eProducing produce);
        //! Creates the list of prerequisite buildings that can be built now.
        //! \pre reqList is initially empty (to allow recursion).
        //! \param building The building that we are targeting.
        //! \return Whether we can get to the building.
        bool findRequiredBuildings(eProducing produce, std::vector<eProducing>& reqList) const;
        bool findRequiredBuildings(eBuilding building, std::vector<eProducing>& reqList) const;
        bool findRequiredBuildings(eUnit_Type unitTypeNr, std::vector<eProducing>& reqList) const;
        bool producingBuilding() const;
        bool producingGarrison() const;
        void updateState();
        bool findCheapestUnitToProduce(eProducing& produce) const;

        eTarget m_producingTarget;
        MoMCity m_momCity;
        eCityState m_cityState;
    };

    MoMManageCities(MoMGameBase* game);
    virtual ~MoMManageCities();

    bool apply();

    MoMCityState* getCityState(int cityNr);

private:
    bool applyBuildingQueue(ePlayer playerNr);

    MoMGameBase* m_game;
    std::vector<MoMCityState*> m_cityStates;
};

std::ostream& operator<<(std::ostream& os, MoMManageCities::MoMCityState::eCityState cityState);

}

#endif // MOMMANAGECITIES_H
