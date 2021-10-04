// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#include <vector>

#include "MoMGameBase.h"

namespace MoM {

class MoMController
{
public:
    explicit MoMController(MoMGameBase* game) :
        m_game(game)
	{}

	const std::string& errorString() const
	{
		return m_errorString;
	}
    void setErrorString(const std::string& str);

    bool addUnit(ePlayer playerNr, eUnit_Type unitType);

    bool addArtifact(ePlayer playerNr, int prefabNr);

    bool buyProduction(City* city);

    int  calcCastingSkillBase(ePlayer playerNr) const;
    int  calcCastingSkillTotal(ePlayer playerNr) const;
    int  calcFame(ePlayer playerNr) const;
    int  calcFoodProduced(ePlayer playerNr) const;
    int  calcFoodUpkeep(ePlayer playerNr) const;
    int  calcGoldIncome(ePlayer playerNr) const;
    int  calcGoldUpkeep(ePlayer playerNr) const;
    void calcManaSkillResearch(ePlayer playerNr, int& mana, int& skill, int& research) const;
    int  calcManaUpkeep(ePlayer playerNr) const;
    int  calcPower(ePlayer playerNr) const;
    void calcPowerBaseDivision(ePlayer playerNr, int& mana, int& skill, int& research) const;
    int  calcProduction(ePlayer playerNr) const;
    int  calcResearch(ePlayer playerNr) const;
    int  calcResearchBonusPercentage(ePlayer playerNr) const;
    int  countGarrison(const MoMLocation& location);
    int  countUnits(const MoMLocation& location);
    City* findCityAtLocation(const MoMLocation& location);
    eRace findDominantRace(ePlane plane) const;
    Node_Attr* findNodeAttrAtLocation(const MoMLocation& location);

    bool polymorphToHero(ePlayer playerNr, int unitNr, eUnit_Type heroNr);

    bool replaceRace(eRace fromRace, eRace toRace, ePlane plane);

    bool removeCityFromGameQueue(int cityNr);

    bool repopLairs(bool maxOut = false);

    bool updateTaxAndPowerDivision(ePlayer playerNr);

private:
//    bool addCitiesToGameQueue();

    bool createUnit(int& unitNr);

    MoMGameBase* m_game;
	std::string m_errorString;
};

}
