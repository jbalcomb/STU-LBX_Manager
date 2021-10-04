#ifndef MOMTERRAIN_H
#define MOMTERRAIN_H

#include "MoMLocation.h"
#include "MoMTemplate.h"

namespace MoM
{

class MoMGameBase;

class MoMTerrain
{
public:
    explicit MoMTerrain(MoMGameBase* game, const MoMLocation& loc);

    const MoMLocation& getLocation() const
    {
        return m_location;
    }
    void                    setLocation(const MoMLocation& loc);

    /// \brief Returns the unit that is alive if there is one.
    ///        Otherwise the first inactive unit is returned.
    Battle_Unit*            getBattleUnit() const;
    eTerrainBonusDeposit    getBonus() const;
    eTerrainCategory        getCategory() const;
    Terrain_Changes         getChanges() const;
    City*                   getCity() const;
    uint8_t                 getExplored() const;

    /// Returns 4 times the documented food bonus.
    /// Excludes the effect of Wild Game.
    /// Accounts for sharing terrain with another city.
    int                     getBasicFoodBonus() const;

    int                     getGoldBonus(bool minersGuild, bool dwarven) const;

    /// Returns the hammer bonus percentage.
    /// Accounts for sharing terrain with another city.
    /// \param gaiasBlessing Doubles hammer bonus of forests and nature nodes
    int                     getProductionPercentage(bool gaiasBlessing) const;

    Tower_Node_Lair*        getLair() const;
    eTerrainType            getType() const;
    std::vector<int>        getUnits() const;
    bool                    isRiver() const;
    bool                    isSea() const;
    bool                    isSharedBetweenCities() const;

    static eTerrainCategory getTerrainCategory(eTerrainType terrainType);

private:
    static int sDoubleFoodBonus[eTerrainCategory_MAX];
    static int sDoubleHammerBonus[eTerrainCategory_MAX];

    MoMGameBase* m_game;
    MoMLocation m_location;
};

}

#endif // MOMTERRAIN_H
