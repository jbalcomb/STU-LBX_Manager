/// This module has been ported from javascript.
#ifndef MOMCOMBAT_H
#define MOMCOMBAT_H

#include <string>
#include <vector>

#include "MoMUnit.h"

namespace MoM {

// TODO: - Split CombatUnit off in a separate file
//       - Create a common MoMUnitBase class to allow proper overrides
class CombatUnit : public MoMUnit
{
public:
    explicit CombatUnit(class MoMGameBase* game = 0) :
        MoMUnit(game),
        m_simulatedDamage(),
        m_baseAttributes(),
        m_suppressionCounter(),
        m_rangedShots(),
        m_currentMana()
    {
    }

    /// Modifies MoMUnit::applyEffects() to make sure the extra fields are initialized
    virtual void applyEffects(const Spells_Cast_in_Battle* battleSpells = 0);

    BaseAttributes& getCombatAttributes()
    {
        return m_baseAttributes;
    }
    const BaseAttributes& getCombatAttributes() const
    {
        return m_baseAttributes;
    }

    int getCurrentMana() const
    {
        return m_currentMana;
    }
    void setCurrentMana(int mana)
    {
        m_currentMana = mana;
    }

    int getCurRangedShots() const;
    void decCurRangedShots();

    int getSuppressionCounter() const;
    void incSuppressionCounter();

    double getMaxTotalHp() const;
    double getCurTotalHp() const;
    double getCurFiguresFrac() const;

    std::string toString() const;

public:
    double m_simulatedDamage;
private:
    BaseAttributes m_baseAttributes;
    int m_suppressionCounter;
    int m_rangedShots;
    int m_currentMana;
};

class MoMCombat
{
public:
    typedef std::vector<CombatUnit> StackUnits;

    MoMCombat(class MoMGameBase* game = 0);

    //
    // LIBRARY FUNCTIONS (sorted alphabetically)
    //

    //! Calculates the mean of an array and optionally the standard deviation.
    //! \param a      The array with values.
    //! \param n      The number of elements in the array.
    //! \param stdev  An Object that receives the standard deviation, if it is not null.
    //! \return       The mean of the array.
    static double calc_mean_stdev(const std::vector<double>& a, double* stdev = 0);

    //! Returns a dice roll between 1 and n.
    static int dice(int n);

    //! Returns a string in which the argument is formatted as a modifier (e.g. +1,-2,+0)
    static std::string format_modifier(int modifier);

    //! Returns the argument v rounded to within 1 digit after the decimal point.
    static std::string prec1(double v);

    //! Returns the number val rounded to have a limited number of digits after the decimal point.
    //! \param val       The number to be rounded.
    //! \param decimals  The number of digits after the decimal point.
    //!                  This may also be a negative number, in which case the number is rounded so that is ends in zeroes.
    static std::string round(double val, int decimals);

    //
    // PROJECT SPECIFIC FUNCTIONS
    //


    //! Make a verbose log of a string into a separate window, which is created if it did not exist.
    void verbose_log(const std::string& str);

    //! Calculates the expected damage if a unit attacks another unit.
    //! \param att_N  number of figures that attack
    //! \param A      attack strength (melee or ranged)
    //! \param Th     to-hit bonus
    //! \param def_max_N maximum number of figures in the defending unit
    //! \param Df     defense (shields)
    //! \param Hp     hitpoints of 1 defending figure
    //! \param Tb     to-block bonus
    //! \param old_damage   the amount of damage the unit has already sustained
    //! \param ignore_damage   the amount of damage each figure of the unit will ignore (for the spell Invulnerability)
    //! \param[out] distr  An Object that receives the probabilities for each damage amount, if it is not null.
    //! \return         Expected amount of damage
    double expected_dam(int att_N, int A, int Th,
                        int def_max_N, int Df, int Hp, int Tb,
                        double old_damage, int ignore_damage,
                        std::vector<double>* distr = 0);

    //! Calculates the expected damage if 1 figure of a unit attacks 1 figure of another unit.
    //!
    //! \param A        Attack (melee or ranged)
    //! \param Th       To-hit bonus
    //! \param D        Defense
    //! \param Tb       To-block bonus
    //! \param ignore_damage The amount of damage each figure of the unit will ignore (for the spell Invulnerability)
    //! \param[out] distr Array of possible damages specifying the chance (if param not null)
    //! \return         Expected amount of damage
    //!
    //! To find the expected amount of damage, we need to combine each number of possible hits with each number of possible blocks.
    //! In each case the damage is the number of hits minus the number of blocks, but at least zero.
    //! We get the expected amount of damage by multiplying the chance on each case by the amount of damage, and summing it all.
    //! The following formula sums it up:
    //! \image html Expected_damage.jpg
    //!
    //! The chance on a certain number of hits is given by:
    //! \image html P_hits.jpg
    //!
    //! and the chance on a certain number of blocks is given by:
    //! \image html P_blocks.jpg
    double expected_dam_per_figure(int A, int Th, int D, int Tb, int ignore_damage, std::vector<double>* distr = 0);

    //! Calculates the expected damage if a unit attacks 1 figure of another unit.
    //! \param N        Number of attacking figures
    //! \param A        Attack (melee or ranged)
    //! \param Th       To-hit bonus
    //! \param D        Defense
    //! \param Tb       To-block bonus
    //! \param ignore_damage The amount of damage each figure of the unit will ignore (for the spell Invulnerability)
    //! \param[out] distr Array of possible damages specifying the chance
    //! \return         Expected amount of damage
    double expected_dam_per_unit(int N, int A, int Th, int D, int Tb, int ignore_damage, std::vector<double>* distr = 0);

    //! Simulate an attack of one unit against another.
    //! Recipe taken from [Strategy Guide].
    double simulate_dam(int att_N, int A, int Th, int def_max_N, int Df, int Hp, int Tb, double old_damage, int ignore_damage, bool verbose);

    //! Monte Carlo simulation of the attack of one unit against another.
    double monte_carlo_dam(int nr_simulations, int nr_sub_simulations, int att_N, int A, int Th, int def_max_N, int Df, int Hp, int Tb, double old_damage, int ignore_damage);

    double expected_resistable_kills(int NrDef, int Re);
    double expected_lifesteal_dam(int NrAtt, int Re);
    double expected_poison_dam(int Nr, int Poison, int Re);
    double expected_hp_from_Life_Steal(double EDD);

    bool life_steal_applicable(const CombatUnit& attacker, const CombatUnit& defender);

    double special_attack(const CombatUnit& attacker, const CombatUnit& defender, const std::string& special, std::string& o);

    void resolve_casualties(CombatUnit& attacker, CombatUnit& defender, double EDD_att, double EDD_def, std::string& o, std::vector<double>* distr_att = 0);

    //! \param attacker The attacker
    //! \param defender The defender
    std::string combat_attack(CombatUnit& attacker, CombatUnit& defender);

    std::string shoot_attack(CombatUnit& attacker, CombatUnit& defender, int distance);

    //! \param attacker The attacker
    //! \param defender The defender
    std::string combat_round(CombatUnit& attacker, CombatUnit& defender);

    std::string shoot_round(CombatUnit& attacker, CombatUnit& defender, int distance);

    //! Calculated the result of a full combat between 2 units, exchanging blows starting with the attacker
    //!
    //! UNDER DEVELOPMENT!!!
    //!
    //! \param attackers The attackers
    //! \param defenders The defenders
    std::string full_combat(StackUnits& attackers, StackUnits& defenders, int& result);

private:
    static void countHitpointsUnits(const StackUnits& units, double& totalHitpoints, double& currentHitpoints);
    static int countLivingUnits(const StackUnits& units);
    static size_t findFirstLivingUnit(const StackUnits& units);

private:
    class MoMGameBase* m_game;
    bool m_verbose;
    int m_method;
    int m_nr_simulations;
};

}

#endif // MOMCOMBAT_H
