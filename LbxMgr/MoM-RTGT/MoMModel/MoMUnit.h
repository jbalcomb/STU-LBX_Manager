// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MOMUNIT_H_
#define MOMUNIT_H_

#include <map>
#include <string>
#include <vector>

#include "MoMTemplate.h"

namespace MoM
{

class MoMGameBase;

/// \brief Defines a class for accessing MoM units
class MoMUnit
{
public:
    typedef std::vector<eBuilding> ListBuildings;
    typedef std::vector<eSpell> ListSpells;

    struct BaseAttributes
    {
        BaseAttributes() :
            melee(),
            defense(),
            ranged(),
            resistance(),
            hitpoints(),
            toHitMelee(),
            toHitRanged(),
            toDefend(),
            moves()
        {}
        void addBonus(const BaseAttributes& up);

        int melee;
        int defense;
        int ranged;
        int resistance;
        int hitpoints;
        int toHitMelee;
        int toHitRanged;
        int toDefend;
        double moves;
    };

    MoMUnit();
    explicit MoMUnit(MoMGameBase* game);
    MoMUnit(MoMGameBase* game, Battle_Unit* battleUnit);
    MoMUnit(MoMGameBase* game, eUnit_Type unitTypeNr);
    MoMUnit(MoMGameBase* game, Hero_stats* heroStats);
    MoMUnit(MoMGameBase* game, Hero_Stats_Initializer* heroStatsInitializer);
    MoMUnit(MoMGameBase* game, Hired_Hero* hiredHero);
    MoMUnit(MoMGameBase* game, Unit_Type_Data* unitType);
    MoMUnit(MoMGameBase* game, Unit* unit);
    void zeroFields();

    virtual ~MoMUnit();
    MoMUnit(const MoMUnit& rhs);
    MoMUnit& operator=(const MoMUnit& rhs);

    void close();

    void changeUnit(Battle_Unit* battleUnit);
    void changeUnit(eUnit_Type unitTypeNr);
    void changeUnit(Hero_stats* heroStats);
    void changeUnit(Hero_Stats_Initializer* heroStatsInitializer);
    void changeUnit(Hired_Hero* hiredHero);
    void changeUnit(Unit_Type_Data* unitType);
    void changeUnit(Unit* unit);
    void setGame(MoMGameBase* game);

    int             calcGoldUpkeep() const;
    int             calcManaUpkeep() const;

    Battle_Unit     getBattleUnit() const;
    Hero_stats      getHeroStats() const;
    Hero_Stats_Initializer getHeroStatsInitializer() const;
    Hired_Hero      getHiredHero() const;
    Unit            getUnitInGame() const;
    Unit_Type_Data  getUnitTypeData() const;

    BaseAttributes  getActualAttributes() const;
    BaseAttributes  getBaseAttributes() const;
    BaseAttributes  getBonusAttributes() const;
    int             getCastingSkillBase() const;
    int             getCastingSkillTotal() const;
    int             getConstructionSkill() const;
    int             getCost() const;
    int             getCurFigures() const;
    int             getDamage() const;
    std::string     getDisplayName() const;
    int             getGazeModifier() const;
    int             getHeroAbility(eHeroAbility heroAbility) const;
    std::string     getHeroName() const;
    ListSpells      getHeroSpells() const;
    eHero_TypeCode  getHeroTypeCode() const;
//    MapSpecials getItemEffects() const;
    Item*           getSlotItem(int itemSlotNr) const;
    eSlot_Type16    getSlotType(int itemSlotNr) const;
    int             getLevel() const;
    std::string     getLevelName() const;
    int             getMaxFigures() const;
    double          getMoves() const;
    ePlayer         getOwner() const;
    BaseAttributes  getPenaltyAttributes() const;
    eRace           getRace() const;
    std::string     getRaceName() const;
    int             getMaxRangedShots() const;
    eRanged_Type    getRangedType() const;
    ListBuildings   getRequiredBuildings() const;
    int             getScouting() const;
    int             getTransportCapacity() const;
    int             getUnitAbility(eUnitAbility unitAbility) const;
    std::string     getUnitName() const;
    eUnit_Type      getUnitTypeNr() const;
    int             getUpkeep() const;
    eWeaponType     getWeaponType() const;
    int             getXP() const;

    // Ranged and other special attacks
    bool hasFireBreath() const;
    bool hasIllusionaryAttack() const;
    bool hasImmolation() const;
    bool hasLightningBreath() const;
    bool hasMagicalBreathAttack() const;
    bool hasMagicalGazeAttack() const;
    bool hasMagicalRangedAttack() const;
    bool hasMissileRangedAttack() const;
    bool hasPhysicalRangedAttack() const;
    bool hasThrownRangedAttack() const;

    // Bitmask flags
    bool hasCombatEnchantment(eCombatEnchantment combatEnchantment) const;
    bool hasHeroAbility(eHeroAbility heroAbility) const;
    bool hasItemPower(eItemPower itemPower) const;
    bool hasMagicWeapon() const;
    bool hasUnitAbility(eUnitAbility unitAbility) const;
    bool hasUnitEnchantment(eUnitEnchantment unitEnchantment) const;
    bool hasMutation(eUnitMutation unitMutation) const;

    // Tests
    bool isBuildable(const City& city) const;
    bool isCaster() const;
    bool isColor(eRealm_Type color) const;
    bool isFlying() const;
    bool isGeneric() const;
    bool isHasted() const;
    bool isHero() const;
    bool isImmune(eUnitAbility immunity) const;
    bool isInvisible() const;
    bool isInvulnerable() const;
    bool isNormal() const;
    bool isSettler() const;
    bool isShip() const;
    bool isSummoned() const;

    // Commit data
    bool setBattleUnit(const Battle_Unit& value);
    bool setHeroStats(const Hero_stats& value);
    bool setHeroStatsInitializer(const Hero_Stats_Initializer& value);
    bool setHiredHero(const Hired_Hero& value);
    bool setUnitInGame(const Unit& value);
    bool setUnitTypeData(const Unit_Type_Data& value);

protected:

    /// \brief (Re)applies all effects
    virtual void applyEffects(const Spells_Cast_in_Battle* battleSpells = 0);

private:
    /// \brief (Re)applies effects of (hero) abilities
    ///        However, spell like effects are applied in applySpells()
    void applyAbilities();

    /// \brief (Re)applies effects of battle spells
    void applyBattleSpells(const Spells_Cast_in_Battle* battleSpells);

    /// \brief (Re)applies effects of items
    ///        However, spell like effects are applied in applySpells()
    void applyItems();

    /// \brief (Re)applies effects of level
    void applyLevel();

    /// \brief (Re)applies effects of spells
    ///        This includes spell like abilities and item powers
    void applySpells(const MoMUnit* enemy = 0);

    /// \brief (Re)applies effects of weapon type
	void applyWeaponType();

    /// \brief Write changed data to the game
    bool commitData(void* ptr, const void* pNewValue, size_t size);
 
    void copyMemberData(const MoMUnit& rhs);

private:
    // NOT IMPLEMENTED

    // CONFIG
    class MoMGameBase* m_game;

    // STATUS
    Battle_Unit* m_battleUnit;
    Hero_stats* m_heroStats;
    Hero_Stats_Initializer* m_heroStatsInitializer;
    Hired_Hero* m_hiredHero;
    Unit* m_unit;
    Unit_Type_Data* m_unitType;

    BaseAttributes m_bonuses;
    BaseAttributes m_dnSpells;
    BaseAttributes m_penalties;
    BaseAttributes m_upAbilities;
    BaseAttributes m_upItems;
    BaseAttributes m_upLevel;
    BaseAttributes m_upSpells;
    BaseAttributes m_upWeaponType;
};

}

#endif
