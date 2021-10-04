#ifndef MOMCITY_H
#define MOMCITY_H

#include "MoMTemplate.h"

namespace MoM {

class MoMGameBase;
struct MoMLocation;

class MoMCity
{
public:
    MoMCity(MoMGameBase* game, const City* city);

    int  calcBasicFood() const;
    int  calcGoldBonus() const;
    int  calcProductionBonusPercentage() const;
    int  calcFoodProduced() const;
    int  calcGoldProduced() const;
    int  calcHammersProduced() const;
    int  calcMaxPopCurrent() const;
    int  calcMaxPopTop() const;
    int  calcNrRebels() const;
    int  calcNrWorkers() const;
    int  calcPacifyingNonReligiousEffects() const;
    int  calcPacifyingReligiousEffects() const;
    int  calcPacifyingUnitEffects() const;
    int  calcRoadBonus() const;
    int  calcTradeBonus() const;
    int  calcUnrestPercentage() const;
    bool canProduce(eBuilding building) const;
    bool canProduce(eProducing produce) const;
    bool canProduce(eUnit_Type unitTypeNr) const;
    int  countConnectedCities() const;
    int  getBuyFactor(eProducing producing = PRODUCING_None) const;
    const City* getCity() const;
    int  getCityNr() const;
    int  getCostToBuy(eProducing producing = PRODUCING_None) const;
    int  getCostToProduce(eProducing producing = PRODUCING_None) const;
    MoMGameBase* getGame() const { return m_game; }
    int  getTimeToComplete(eProducing producing = PRODUCING_None) const;
    int  getUnitReductionPercentage() const;
    int  getWildGameBonus() const;
    bool hasForestRequirement() const;
    bool hasHillRequirement() const;
    bool hasWaterRequirement() const;
    bool isProductionAllowed(eProducing producing) const;
    bool isBuildingAllowed(eBuilding building) const;
    bool isBuildingPresent(eBuilding building) const;
    bool isUnitAllowed(eUnit_Type unitTypeNr) const;

private:
    template<typename Functor>
    bool enumerateTerrain(Functor& functor) const;

    MoMGameBase* m_game;
    const City* m_city;
};

}

#endif // MOMCITY_H
