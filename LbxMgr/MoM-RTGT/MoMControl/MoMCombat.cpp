/// This module has been ported from javascript.

#include "MoMCombat.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>

#include "MoMGameBase.h"
#include "MoMUnit.h"
#include "MoMUtility.h"

namespace MoM {

const void* null = 0;
const double g_epsilon = 0.00001;

void CombatUnit::applyEffects(const Spells_Cast_in_Battle* battleSpells)
{
    MoMUnit::applyEffects(battleSpells);

    m_simulatedDamage = MoMUnit::getDamage();
    m_baseAttributes = MoMUnit::getActualAttributes();
    m_suppressionCounter = MoMUnit::getBattleUnit().m_Suppression;
    // TODO: Find out what the current ranged shots are in a battle unit
    m_rangedShots = MoMUnit::getMaxRangedShots();
    m_currentMana = MoMUnit::getCastingSkillTotal();
}

int CombatUnit::getCurRangedShots() const
{
    return m_rangedShots;
}

void CombatUnit::decCurRangedShots()
{
    m_rangedShots--;
}

int CombatUnit::getSuppressionCounter() const
{
    return m_suppressionCounter;
}

void CombatUnit::incSuppressionCounter()
{
    m_suppressionCounter++;
}

double CombatUnit::getMaxTotalHp() const
{
    return (getMaxFigures() * m_baseAttributes.hitpoints);
}

double CombatUnit::getCurTotalHp() const
{
    return (getMaxFigures() * m_baseAttributes.hitpoints - m_simulatedDamage);
}

double CombatUnit::getCurFiguresFrac() const
{
    // Basically the nr of figures is the total_hp divided by the figure_hp rounded upward
    // However, if a figures has between 0 and 1 hitpoints left, the chance that he is still
    // standing equals the fraction of a hitpoint he has left.

    int Hp = m_baseAttributes.hitpoints;
    if (Hp <= 0)
        return 0;

    double nr_full_figures = std::floor(getCurTotalHp() / Hp + g_epsilon);
    double hp_last_figure = getCurTotalHp() - nr_full_figures * Hp;

    if (hp_last_figure >= 1) return nr_full_figures + 1;
    else return nr_full_figures + hp_last_figure;
}

std::string CombatUnit::toString() const
{
    BaseAttributes attr = getActualAttributes();
    char buf[4096] = "";
    sprintf(buf, "%.1fx %d-%d-%d-%d-%d (%.1f) %+d/%+d",
            getCurFiguresFrac(), attr.melee, attr.ranged, attr.defense, attr.resistance, attr.hitpoints,
            getCurTotalHp(), attr.toHitMelee, attr.toDefend);
    return getDisplayName() + " " + buf;
}

MoMCombat::MoMCombat(MoMGameBase* game) :
    m_game(game),
    m_verbose(false),
    m_method(0),
    m_nr_simulations(100)
{
}

//!
//! The function combat_round(attacker, defender) resolves 1 melee combat round. That is:\n
//!   1. Pre-melee: Attacker executes Breath, Thrown, Gaze attacks, and a 2nd Gaze attack if hasted.\n
//!   2. Pre-melee: Resolve casualties of defender.\n
//!   3. Pre-melee: Defender executes Gaze attack (2 if hasted) if not sleeping.\n
//!   4. Pre-melee: Wall of fire.\n
//!   5. Pre-melee: Resolve casualties of attacker.\n
//!   7. Melee: Attacker (non-fearful) executes Immolation, Poison, Touch, and Melee attacks.\n
//!   8. First Strike: Resolve casualties if attacker has (unnegated) First Strike.\n
//!   9. Melee: If hasted, attacker (non-fearful) executes Immolation, Poison, Touch, and Melee attacks.\n
//!   10. Melee: Defender (non-fearful) executes Immolation, Poison, Touch, and Melee attacks (2 times if hasted) if not sleeping.\n
//!   11. Melee: Resolve casualties.\n
//!
//! The function shoot_round(attacker, defender, distance) resolves a ranged attack (TODO: 2 if hasted).
//!

//
// Functions
//

//
// LIBRARY FUNCTIONS (sorted alphabetically)
//


//! Calculates the mean of an array and optionally the standard deviation.
//! \param a      The array with values.
//! \param n      The number of elements in the array.
//! \param stdev  An Object that receives the standard deviation, if it is not null.
//! \return       The mean of the array.
double MoMCombat::calc_mean_stdev(const std::vector<double>& a, double* stdev)
{
    size_t n = a.size();
    double mean = 0;
    if (n <= 0)
        mean = std::numeric_limits<double>::quiet_NaN();
    else
    {
        double sum = 0;
        for (size_t i = 0; i < n; ++i)
        {
            sum += a[i];
        }
        mean = sum / n;
    }
    if (stdev != null)
    {
        if (n <= 1)
            *stdev = std::numeric_limits<double>::infinity();
        else
        {
            double sum = 0;
            for (size_t i = 0; i < n; ++i)
            {
                double dif = a[i] - mean;
                sum += dif * dif;
            }
            *stdev = std::sqrt(sum / (n - 1));
        }
    }
    return mean;
}


//! Returns a dice roll between 1 and n.
int MoMCombat::dice(int n)
{
    return std::rand() % n;
}


//! Returns a string in which the argument is formatted as a modifier (e.g. +1,-2,+0)
std::string MoMCombat::format_modifier(int modifier)
{
    if (modifier >= 0) return ("+" + toStr(modifier));
    return toStr(modifier);
}


//! Returns the argument v rounded to within 1 digit after the decimal point.
std::string MoMCombat::prec1(double v)
{
    // Format the number v to have 1 digit max after the decimal point.
    return toStr(std::floor(v * 10 + 0.5) / 10);
}

//! Returns the number val rounded to have a limited number of digits after the decimal point.
//! \param val       The number to be rounded.
//! \param decimals  The number of digits after the decimal point.
//!                  This may also be a negative number, in which case the number is rounded so that is ends in zeroes.
std::string MoMCombat::round(double val, int decimals)
{
    // Format the number v to have a maximum of 'decimals' digits max after the decimal point.
    double divider = 1;
    for (; decimals < 0; decimals++, divider /= 10.) {};
    for (; decimals > 0; decimals--, divider *= 10) {};
    return toStr(std::floor(val * divider + 0.5) / divider);
}


//
// PROJECT SPECIFIC FUNCTIONS
//


//! Make a verbose log of a string into a separate window, which is created if it did not exist.
void MoMCombat::verbose_log(const std::string& str)
{
    std::cout << str << std::endl;
}


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
double MoMCombat::expected_dam(int att_N, int A, int Th, int def_max_N, int Df, int Hp, int Tb, double old_damage, int ignore_damage, std::vector<double>* distr)
{
    if (m_verbose) verbose_log("expected_dam: (att_N, A, Th, def_max_N, Df, Hp, Tb, old_damage, ignore_damage) = "
                               + toStr(att_N) + ", " + toStr(A) + ", " + toStr(Th) + ", "
                               + toStr(def_max_N) + ", " + toStr(Df) + ", " + toStr(Hp) + ", " + toStr(Tb) + ", "
                               + round(old_damage, 3) + ", " + toStr(ignore_damage) + "<br>");
    switch (1 * m_method)
    {
    default:
    case 0:
    case 1:
        if (distr == null)
        {
            return att_N * expected_dam_per_figure(A, Th, Df, Tb, ignore_damage);
        }
        else
        {
            return expected_dam_per_unit(att_N, A, Th, Df, Tb, ignore_damage, distr);
        }

    case 2:
        return simulate_dam(att_N, A, Th, def_max_N, Df, Hp, Tb, old_damage, ignore_damage, m_verbose);

    case 3:
        return monte_carlo_dam(m_nr_simulations, 10, att_N, A, Th, def_max_N, Df, Hp, Tb, old_damage, ignore_damage);
    }
}


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
double MoMCombat::expected_dam_per_figure(int A, int Th, int D, int Tb, int ignore_damage, std::vector<double>* distr)
{
    //! ph       chance to hit (base 30% plus 10% for each to-hit bonus point)
    //! pb       chance to block (base 30% plus 10% for each to-block bonus point)
    //! ph10     chance to hit times 10
    //! pb10     chance to block times 10
    //! EDD      Expected Damage Done
    //! P_hits   chance to hit 'hits' times = (ph10/10.)^hits
    //! P_blocks change to block 'blocks' times = (pb10/10.)^blocks

    //! \verbatim
    //!         A       D              A      hits           A-hits           D         blocks           D-blocks
    //! EDD =  SUM     SUM      binom(    ) ph     * (1 - ph)       * binom(      ) * pb       * (1 - ph)         * max(hits - blocks, 0)
    //!       hits=0 blocks=0         hits                                  blocks
    //! \endverbatim
    //!

    // Min 0% and max 100%
    Th = Max(-3, Min(7, Th));
    Tb = Max(-3, Min(7, Tb));

    if (m_verbose) verbose_log("Figure: (A, Th, D, Tb) = " + toStr(A) + ", " + toStr(Th) + ", " + toStr(D) + ", " + toStr(Tb) + "<br>");
    int ph10 = 3 + Th, pb10 = 3 + Tb;
    double EDD = 0;
    int hits;
    double P_hits;
    int blocks;
    double P_blocks;

    if (m_verbose) verbose_log("<table border=1 cellspacing=0><tr><th>hits<th>P_hits<th>blocks<th>P_blocks<th>damage");
    for (hits = 0, P_hits = std::pow((10 - ph10) / 10., A)
         ; hits <= A
         ;)
    {
        double expectation_hits = 0;
        int max_blocks = Min(D, hits + ignore_damage - 1);
        for (blocks = 0, P_blocks = std::pow((10 - pb10) / 10., D)
             ; blocks <= max_blocks
             ;)
        {
            int damage = Max(0, hits - blocks - ignore_damage);
            if (damage > 0)
            {
                expectation_hits += P_blocks * damage;
                if (distr != null)
                {
                    if (distr->size() <= static_cast<size_t>(damage))
                    {
                        distr->resize(damage + 1);
                    }
                    distr->at(damage) += P_hits * P_blocks;
                }
                if (m_verbose) verbose_log("<tr><td>" + toStr(hits) + "<td>" + round(P_hits,3) + "<td>"
                                           + toStr(blocks) + "<td>" + round(P_blocks,3) + "<td>" + toStr(hits - blocks));
            }

            if (pb10 >= 10)
            {
                ++blocks;
                P_blocks = (blocks < max_blocks ? 0 : 1);
            }
            else
            {
                P_blocks = P_blocks * (D - blocks) / (blocks + 1) * pb10 / (10 - pb10);
                ++blocks;
            }
        }
        EDD += P_hits * expectation_hits;
        if (m_verbose) verbose_log("<tr><td>" + toStr(hits) + "<td>" + round(P_hits,3) + "<td><td><td>" + round(expectation_hits, 3));

        if (ph10 >= 10)
        {
            ++hits;
            P_hits = (hits < A ? 0 : 1);
        }
        else
        {
            P_hits = P_hits * (A - hits) / (hits + 1) * ph10 / (10 - ph10);
            ++hits;
        }
    }

    if (m_verbose) verbose_log("</table>");
    if (m_verbose) verbose_log("Figure: Expected damage = " + round(EDD, 3) + "<br>");

    if (distr != null)
    {
        double pdam0 = 1.0;
        for (size_t damage = 1; damage < distr->size(); ++damage)
        {
            pdam0 -= distr->at(damage);
        }
        if (distr->size() <= 0)
        {
            distr->resize(1);
        }
        distr->at(0) = pdam0;

        if (m_verbose)
        {
            verbose_log("Figure damage distribution:");
            verbose_log("<table border=1 cellspacing=0><tr><th>dam<th>probability");
            for (size_t damage = 0; damage < distr->size(); ++damage)
            {
                verbose_log("<tr><td>" + toStr(damage) + "<td>" + round(distr->at(damage), 3));
            }
            verbose_log("</table>");
        }
    }

    return EDD;
}


//! Calculates the expected damage if a unit attacks 1 figure of another unit.
//! \param N        Number of attacking figures
//! \param A        Attack (melee or ranged)
//! \param Th       To-hit bonus
//! \param D        Defense
//! \param Tb       To-block bonus
//! \param ignore_damage The amount of damage each figure of the unit will ignore (for the spell Invulnerability)
//! \param[out] distr Array of possible damages specifying the chance
//! \return         Expected amount of damage
double MoMCombat::expected_dam_per_unit(int N, int A, int Th, int D, int Tb, int ignore_damage, std::vector<double>* distr)
{
    if (distr == null)
        return 0.0;

    std::vector<double> cum_distr(1);
    cum_distr[0] = 1.0;

    //! EDD      Expected Damage Done
    std::vector<double> fig_distr;
    double EDD = expected_dam_per_figure(A, Th, D, Tb, ignore_damage, &fig_distr);

    for (int n = 0; n < N; ++n)
    {
        std::vector<double> new_cum_distr;
        for (size_t cum_dam = 0; cum_dam < cum_distr.size(); ++cum_dam)
        {
            for (size_t fig_dam = 0; fig_dam < fig_distr.size(); ++fig_dam)
            {
                if (new_cum_distr.size() <= cum_dam + fig_dam)
                {
                    new_cum_distr.resize(cum_dam + fig_dam + 1);
                }
                new_cum_distr[cum_dam + fig_dam] += cum_distr[cum_dam] * fig_distr[fig_dam];
            }
        }
        cum_distr = new_cum_distr;
    }

    *distr = cum_distr;

    if (m_verbose)
    {
        verbose_log("Unit cumulative damage distribution:");
        verbose_log("<table border=1 cellspacing=0><tr><th>dam<th>probability");
        for (size_t damage = 0; damage < distr->size(); ++damage)
        {
            verbose_log("<tr><td>" + toStr(damage) + "<td>" + round(distr->at(damage), 3));
        }
        verbose_log("</table>");
    }

    return EDD;
}


//! Simulate an attack of one unit against another.
//! Recipe taken from [Strategy Guide].
double MoMCombat::simulate_dam(int att_N, int A, int Th, int def_max_N, int Df, int Hp, int Tb, double old_damage, int ignore_damage, bool verbose)
{
    int def_total_Hp = def_max_N * Hp - old_damage;
    int def_N = def_total_Hp / Hp;
    int Hp1 = def_total_Hp - (def_N - 1) * Hp;

    // Min 0% and max 100%
    Th = Max(-3, Min(7, Th));
    Tb = Max(-3, Min(7, Tb));

    if (verbose) verbose_log("Sim: (att_N, A, Th, def_N, Df, Hp1, Hp, Tb) = "
                             + toStr(att_N) + "," + toStr(A) + "," + toStr(Th) + ","
                             + toStr(def_N) + "," + toStr(Df) + "," + toStr(Hp1) + "," + toStr(Hp) + "," + toStr(Tb) + "<br>");

    int damage = 0;

    if (verbose) verbose_log("<table border=1 cellspacing=0><tr><th>fig<th>raw<th>blocks<th>damage<th>Hp1");

    // For each attacking figure
    int fig_attack = 0;
next_attacker:
    while (fig_attack < att_N)
    {
        ++fig_attack;

        // Roll raw hits for the attacking figure
        int raw_hits = 0;
        for (int roll = 0; roll < A; ++roll)
            if (dice(10) <= 3 + Th)
                ++raw_hits;
        if (verbose) verbose_log("<tr><td>" + toStr(fig_attack) + "<td>" + toStr(raw_hits));

        // While there are still defenders standing
        while (def_N > 0)
        {
            // Roll blocks for the first defending figure
            int blocks = 0;
            for (int roll = 0; roll < Df; ++roll)
                if (dice(10) <= 3 + Tb)
                    ++blocks;
            blocks += ignore_damage;
            if (verbose) verbose_log("<td>" + toStr(blocks));

            // If all raw hits were blocked
            if (raw_hits <= blocks)
            {
                if (verbose) verbose_log("<td>0<td>" + prec1(Hp1));
                // Continue with the next attacking figure
                goto next_attacker;
            }

            // If the penetrating hits did not kill the defending figure
            int penetrating_hits = raw_hits - blocks;
            if (verbose) verbose_log("<td>" + toStr(penetrating_hits) + "<td>" + prec1(Hp1));
            if (penetrating_hits < Hp1)
            {
                // Process the damage
                damage += penetrating_hits;
                Hp1 -= penetrating_hits;
                // Continue with the next attacking figure
                goto next_attacker;
            }

            // First figure was killed - process the damage
            damage += Hp1;
            --def_N;
            raw_hits -= (blocks + Hp1);

            // Continue the attack on the next defending figure
            Hp1 = Hp;
            if (verbose) verbose_log("<tr><td><td>" + toStr(raw_hits));
        }

        // All defending figures are dead
        // Process remaining damage
        damage += raw_hits;
        if (verbose) verbose_log("<td><td><td>");
        // Continue with the next attacking figure
    }
    if (verbose) verbose_log("</table>");
    if (verbose) verbose_log("Sim: Simulated damage = " + toStr(damage) + "<br>");

    return damage;
}


//! Monte Carlo simulation of the attack of one unit against another.
double MoMCombat::monte_carlo_dam(int nr_simulations, int nr_sub_simulations, int att_N, int A, int Th, int def_max_N, int Df, int Hp, int Tb, double old_damage, int ignore_damage)
{
    if (m_verbose) verbose_log("MonteCarlo: N=" + toStr(nr_simulations) + " subN=" + toStr(nr_sub_simulations)
                               + " (att_N x A / Th, def_max_N x Df / Hp / Tb ( old_damage)) = "
                               + toStr(att_N) + "x" + toStr(A) + "/" + format_modifier(Th) + ","
                               + toStr(def_max_N) + "x" + toStr(Df) + "/" + toStr(Hp) + "/" + format_modifier(Tb)
                               + "(" + toStr(old_damage) + ")<br>");

    std::vector<double> aDamage(nr_simulations);
    for (int simulation_nr = 0; simulation_nr < nr_simulations; ++simulation_nr)
    {
        aDamage[simulation_nr] = simulate_dam(att_N, A, Th, def_max_N, Df, Hp, Tb, old_damage, ignore_damage, false);
    }

    if (m_verbose)
    {
        int sub_N = nr_simulations / nr_sub_simulations;
        std::vector<double> aMean(sub_N);
        for (int sub_nr = 0; sub_nr < nr_sub_simulations; ++sub_nr)
        {
            std::vector<double> aDamageSub(sub_N);
            for (int i = 0; i < sub_N; ++i)
                aDamageSub[i] = aDamage[sub_nr * sub_N + i];
            aMean[sub_nr] = calc_mean_stdev(aDamageSub);
        }
        double stdev_mean = 0;
        double stdev = 0;
        calc_mean_stdev(aMean, &stdev_mean);
        aDamage.resize(nr_sub_simulations * sub_N);
        double mean = calc_mean_stdev(aDamage, &stdev);
        if (m_verbose)
        {
            verbose_log("Monte Carlo: Mean damage = " + round(mean, 3)
                        + ", stdev_mean = " + round(stdev, 3)
                        + ", stdev = " + round(stdev, 3) + "<br>");
        }
        return mean;
    }
    else
    {
        return calc_mean_stdev(aDamage);
    }
}


double MoMCombat::expected_resistable_kills(int NrDef, int Re)
{
    if (Re >= 10) return 0;
    if (Re < 0) Re = 0;
    // Each figure must resist separately or be killed
    return NrDef * (10 - Re) / 10.;
}


double MoMCombat::expected_lifesteal_dam(int NrAtt, int Re)
{
    if (Re >= 10) return 0;
    if (Re < 0) Re = 0;
    // Defending unit loses a heart for each point the die roll is above the resistance
    // Each attacking figures has a life steal attack (TODO: check)
    return NrAtt * (11 - Re) * (10 - Re) / 20.;
}


double MoMCombat::expected_poison_dam(int Nr, int Poison, int Re)
{
    if (Re >= 10) return 0;
    if (Re < 0) Re = 0;
    // Defending unit loses a hitpoint for each Poison attack that is not resisted
    // Each attacking figure has a poison attack (TODO: check)
    return Nr * Poison * (10 - Re) / 10.;
}


double MoMCombat::expected_hp_from_Life_Steal(double EDD)
{
    // Life Steal heals for half of the damage rounded down [Manual]
    // Life Steal heals for all damage done [Strategy Guide]
    return EDD;
}


bool MoMCombat::life_steal_applicable(const CombatUnit& attacker, const CombatUnit& defender)
{
    return (attacker.hasUnitAbility(UNITABILITY_Life_Stealing) && !defender.isImmune(UNITABILITY_Death_Immunity));
}


double MoMCombat::special_attack(const CombatUnit& attacker, const CombatUnit& defender,
                                 const std::string& special, std::string& o)
{
    // Handled here: Breaths, Gazes, Immolation, Life Steal, Poison, Thrown, Touch, Melee
    // Not handled here: First Strike, Ranged
    double EDD = 0;
    double EDD_tmp;

    MoMUnit::BaseAttributes att = attacker.getCombatAttributes();
    MoMUnit::BaseAttributes def = defender.getCombatAttributes();

    // TODO: Color attack (ranged type, melee of summoned, color of special)

    int ignore_damage = 0;
    if (defender.isInvulnerable())
    {
        ignore_damage = 2;
        o += "Invulnerability ignores the first " + toStr(ignore_damage) + " points of damage\n";
    }

    if (special == "Breath")
    {
        if (attacker.hasFireBreath() && !defender.hasUnitAbility(UNITABILITY_Fire_Immunity))
        {
            int Df = def.defense;
            if (defender.hasUnitAbility(UNITABILITY_Large_Shield))
            {
                Df += 2;
                o += "Large Shield raises defense of " + defender.getDisplayName() + " to " + toStr(Df) + " against Fire Breath\n";
            }
            if (defender.isImmune(UNITABILITY_Magic_Immunity) && Df < 50)
            {
                Df = 50;
                o += "Magic Immunity raises defense of " + defender.getDisplayName() + " to 50 against Fire Breath\n";
            }
            int strength = att.ranged;  // attacker.get_special("Fire Breath")
            if ((strength < 2) && attacker.hasMutation(UNITMUTATION_Chaos_Channels_Fiery_Breath))
            {
                strength = 2;
            }
            EDD_tmp = expected_dam(attacker.getCurFiguresFrac(), strength, att.toHitRanged,
                                   defender.getMaxFigures(), Df, def.hitpoints, def.toDefend,
                                   defender.m_simulatedDamage, ignore_damage);
            EDD += EDD_tmp;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Fire Breath " + toStr(strength) + " for " + prec1(EDD_tmp) + " damage\n";
        }
        if (attacker.hasLightningBreath()) // Note: Lightning Imm does not exist
        {
            int Df = def.defense;
            if (defender.hasUnitAbility(UNITABILITY_Large_Shield))
            {
                Df += 2;
                o += "Large Shield raises defense of " + defender.getDisplayName() + " to " + toStr(Df) + " against Fire Breath\n";
            }
            if (defender.isImmune(UNITABILITY_Magic_Immunity) && Df < 50)
            {
                Df = 50;
                o += "Magic Immunity raises defense of " + defender.getDisplayName() + " to 50 against Fire Breath\n";
            }
            else
            {
                Df = Df / 2;
                o += "Lightning reduces defense of " + defender.getDisplayName() + " to " + toStr(Df) + "\n";
            }
            int strength = att.ranged;  // attacker.get_special("Lightning")
            EDD +=  EDD_tmp = expected_dam(attacker.getCurFiguresFrac(), strength, att.toHitRanged,
                                   defender.getMaxFigures(), Df, def.hitpoints, def.toDefend,
                                   defender.m_simulatedDamage, ignore_damage);
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Lightning " + toStr(strength) + " for " + prec1(EDD_tmp) + " damage\n";
        }
    }
    else if (special == "Gaze")
    {
        // TODO: Resolve sequentially
        if ((attacker.getRangedType() == RANGED_Multiple_Gaze)) // "Doom Gaze"
        {
            int strength = att.ranged;  // attacker.get_special("Doom Gaze")
            EDD += EDD_tmp = strength;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Doom Gaze " + toStr(strength) + " for " + prec1(EDD_tmp) + " damage\n";
        }
        if (((attacker.getRangedType() == RANGED_Death_Gaze) || (attacker.getRangedType() == RANGED_Multiple_Gaze))
                && !defender.isImmune(UNITABILITY_Death_Immunity))
        {
            int modifier = -Abs(attacker.getGazeModifier());  // attacker.get_special("Death Gaze")
            double nr_kills = expected_resistable_kills(defender.getCurFiguresFrac(), def.resistance + modifier);
            EDD += EDD_tmp = nr_kills * def.hitpoints;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Death Gaze " + format_modifier(modifier) + " for " + prec1(nr_kills) + " kills\n";
        }
        if (((attacker.getRangedType() == RANGED_Stoning_Gaze) || (attacker.getRangedType() == RANGED_Multiple_Gaze))
                && !defender.isImmune(UNITABILITY_Stoning_Immunity))
        {
            int modifier = -Abs(attacker.getGazeModifier());  // attacker.get_special("Stoning Gaze")
            double nr_kills = expected_resistable_kills(defender.getCurFiguresFrac(), def.resistance + modifier);
            EDD += EDD_tmp = nr_kills * def.hitpoints;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Stoning Gaze " + format_modifier(modifier) + " for " + prec1(nr_kills) + " kills\n";
        }
    }
    else if (special == "Immolation" && attacker.hasImmolation() && !defender.isImmune(UNITABILITY_Fire_Immunity))
    {
        int strength = attacker.getUnitAbility(UNITABILITY_Immolation);
        EDD = expected_dam(attacker.getCurFiguresFrac(), strength,  att.toHitRanged,
                           defender.getMaxFigures(), def.defense, def.hitpoints, def.toDefend,
                           defender.m_simulatedDamage, ignore_damage);
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Immolation " + toStr(strength) + " for " + prec1(EDD) + " damage\n";
    }
    else if (special == "Life Steal" && life_steal_applicable(attacker, defender))
    {
        int modifier = attacker.getUnitAbility(UNITABILITY_Life_Stealing);
        EDD = expected_lifesteal_dam(attacker.getCurFiguresFrac(), def.resistance + modifier);
        double heal_hp = expected_hp_from_Life_Steal(EDD);
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses " + special + " " + format_modifier(modifier) + " for " + prec1(EDD) + " damage and heals " + prec1(heal_hp) + " hitpoints\n";
    }
    else if (special == "Melee")
    {
        EDD = expected_dam(attacker.getCurFiguresFrac(), att.melee, att.toHitMelee,
            defender.getMaxFigures(), def.defense, def.hitpoints, def.toDefend,
            defender.m_simulatedDamage, ignore_damage);
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " attacks for " + prec1(EDD) + " damage\n";
    }
    else if (special == "Poison" && attacker.hasUnitAbility(UNITABILITY_Poison_attack) && !defender.isImmune(UNITABILITY_Poison_Immunity))
    {
        int strength = attacker.getUnitAbility(UNITABILITY_Poison_attack);
        EDD = expected_poison_dam(attacker.getCurFiguresFrac(), strength, def.resistance);
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Poison " + toStr(strength) + " for " + prec1(EDD) + " damage\n";
    }
    else if (special == "Thrown" && attacker.hasThrownRangedAttack())
    {
        EDD = expected_dam(attacker.getCurFiguresFrac(), att.ranged, att.toHitRanged,
                           defender.getMaxFigures(), def.defense, def.hitpoints, def.toDefend, defender.m_simulatedDamage, ignore_damage);
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses " + special + " " + toStr(att.ranged) + " for " + prec1(EDD) + " damage\n";
    }
    else if (special == "Touch")
    {
        if (attacker.hasUnitAbility(UNITABILITY_Death_Touch) && !defender.isImmune(UNITABILITY_Death_Immunity))
        {
            int modifier = attacker.getUnitAbility(UNITABILITY_Death_Touch);
            double nr_kills = expected_resistable_kills(defender.getCurFiguresFrac(), def.resistance + modifier);
            EDD += nr_kills * def.hitpoints;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Death Touch " + format_modifier(modifier) + " for " + prec1(nr_kills) + " kills\n";
        }
        if (attacker.hasUnitAbility(UNITABILITY_Stoning_Touch) && !defender.isImmune(UNITABILITY_Stoning_Immunity))
        {
            int modifier = attacker.getUnitAbility(UNITABILITY_Stoning_Touch);
            double nr_kills = expected_resistable_kills(defender.getCurFiguresFrac(), def.resistance + modifier);
            EDD += nr_kills * def.hitpoints;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Stoning Touch " + format_modifier(modifier) + " for " + prec1(nr_kills) + " kills\n";
        }
    }

    return EDD;
}


void MoMCombat::resolve_casualties(CombatUnit& attacker, CombatUnit& defender, double EDD_att, double EDD_def, std::string& o, std::vector<double>* distr_att)
{
    // TODO: Blur ignores each point of damage with a 10% chance

    if (EDD_def != 0)
    {
        attacker.m_simulatedDamage += EDD_def;
        if (attacker.m_simulatedDamage < 0)
            attacker.m_simulatedDamage = 0;
        o += "Unit of " + attacker.getDisplayName() + " has " + prec1(attacker.getCurFiguresFrac()) + " units left with a total of " + prec1(attacker.getCurTotalHp()) + " hp\n";
    }
    if (EDD_att != 0)
    {
        defender.m_simulatedDamage += EDD_att;
        if (defender.m_simulatedDamage < 0)
            defender.m_simulatedDamage = 0;
        o += "Unit of " + defender.getDisplayName() + " has " + prec1(defender.getCurFiguresFrac()) + " units left with a total of " + prec1(defender.getCurTotalHp()) + " hp\n";

//        if (distr_att != null)
//        {
//            double kill_damage = defender.getCurTotalHp();
//            double kill_probability = 0;
//            for (int damage = std::ceil(kill_damage); damage < distr_att->size(); ++damage)
//            {
//                kill_probability += distr_att->at(damage);
//            }
//            if (kill_probability >= 0.0005)
//            {
//                o += "Chance to be killed was " + prec1(100 * kill_probability) + "% (experimental feature)\n";
//            }
//        }
    }
}


//! \param attacker The attacker
//! \param defender The defender
std::string MoMCombat::combat_attack(CombatUnit& attacker, CombatUnit& defender)
{
    std::string o;   // output string

    //! Check if attacker is alive
    if (attacker.getCurTotalHp() <= 0)
    {
        o += "Cannot attack since attacker is not alive. Attack aborted.\n";
        o += "\n";
        return o;
    }

    //! Check if the attacker is sleeping
    if (attacker.hasCombatEnchantment(COMBATENCHANTMENT_Black_Sleep))
    {
        o += "Unit of " + attacker.getDisplayName() + " can not attack because it is asleep. Attack aborted.\n";
        o += "\n";
        return o;
    }

    //! Check if the attacker is allowed to attack a flying defender
    if (defender.isFlying() && !attacker.isFlying()
            && !attacker.hasMagicalBreathAttack() && !attacker.hasMagicalGazeAttack() && !attacker.hasThrownRangedAttack())
    {
        o += "Unit of " + attacker.getDisplayName() + " can not attack a flying unit. Attack aborted.\n";
        o += "\n";
        return o;
    }

    MoMUnit::BaseAttributes& att = attacker.getCombatAttributes();
    MoMUnit::BaseAttributes& def = defender.getCombatAttributes();

    do // Single-iteration-loop to allow for a break followed by a centralized clean-up
    {
        double EDD, EDD_att, EDD_def;

        // TODO: Move Eldritch here
        // TODO: City wall defense

        if (attacker.getSuppressionCounter() >= 2)
        {
            att.toHitMelee -= attacker.getSuppressionCounter() / 2;
            att.toHitRanged -= attacker.getSuppressionCounter() / 2;
            o += "Suppression reduces to hit melee/ranged of " + attacker.getDisplayName()
                    + " to " + format_modifier(att.toHitMelee) + "/" + format_modifier(att.toHitRanged) + "\n";
        }

        //! Weapon Immunity increase Df to 10 if opponent is normal and does not have a magic weapon
        if (attacker.isImmune(UNITABILITY_Weapon_Immunity) && att.defense < 10 && defender.isNormal() && !defender.hasMagicWeapon())
        {
            att.defense = 10;
            o += "Weapon Immunity increases defense of " + attacker.getDisplayName() + " to " + toStr(att.defense) + "\n";
        }
        if (defender.isImmune(UNITABILITY_Weapon_Immunity) && def.defense < 10 && attacker.isNormal() && !attacker.hasMagicWeapon())
        {
            def.defense = 10;
            o += "Weapon Immunity increases defense of " + defender.getDisplayName() + " to " + toStr(def.defense) + "\n";
        }

        //! Reduce Df of an opponent of Armor Piercing
        if (attacker.hasUnitAbility(UNITABILITY_Armor_Piercing))
        {
            def.defense = def.defense / 2;
            o += "Armor Piercing reduces defense of " + toStr(def.defense) + " to " + toStr(def.defense) + "\n";
        }
        if (defender.hasUnitAbility(UNITABILITY_Armor_Piercing))
        {
            att.defense = att.defense / 2;
            o += "Armor Piercing reduces defense of " + attacker.getDisplayName() + " to " + toStr(att.defense) + "\n";
        }

        //! Reduce Df to 0 for an opponent of an Illusion
        if (attacker.hasIllusionaryAttack() && !defender.isImmune(UNITABILITY_Illusions_Immunity))
        {
            def.defense = 0;
            o += "Illusion reduces defense of " + defender.getDisplayName() + " to " + toStr(def.defense) + "\n";
        }
        if (defender.hasIllusionaryAttack() && !attacker.isImmune(UNITABILITY_Illusions_Immunity))
        {
            att.defense = 0;
            o += "Illusion reduces defense of " + attacker.getDisplayName() + " to " + toStr(def.defense) + "\n";
        }

        if (defender.isInvisible() && !attacker.isImmune(UNITABILITY_Illusions_Immunity))
        {
            att.toHitMelee -= 1;
            o += "Invisibility reduces to hit of " + attacker.getDisplayName() + " to " + format_modifier(att.toHitMelee) + "\n";
        }
        if (attacker.isInvisible() && !defender.isImmune(UNITABILITY_Illusions_Immunity))
        {
            def.toHitMelee -= 1;
            o += "Invisibility reduces to hit of " + defender.getDisplayName() + " to " + format_modifier(def.toHitMelee) + "\n";
        }

        //! Pre-melee: attacker's Breaths, First Strike, Gazes, Thrown, defender's Gazes
        EDD_att = EDD_def = 0;

        // Pre-melee: Attacker
        EDD_att += special_attack(attacker, defender, "Thrown", o);
        EDD_att += special_attack(attacker, defender, "Breath", o);
        EDD_att += special_attack(attacker, defender, "Gaze", o);
        if (attacker.isHasted())
        {
            EDD_att += special_attack(attacker, defender, "Gaze", o);
        }
        resolve_casualties(attacker, defender, EDD_att, EDD_def, o);
        if (attacker.getCurTotalHp() <= 0 || defender.getCurTotalHp() <= 0)
            break;  // End combat if anyone defeated
        EDD_att = EDD_def = 0;

        // Pre-melee: Defender
        if (!defender.hasCombatEnchantment(COMBATENCHANTMENT_Black_Sleep))
        {
            EDD_def += special_attack(defender, attacker, "Gaze", o);
            if (defender.isHasted())
            {
                EDD_att += special_attack(attacker, defender, "Gaze", o);
            }
            resolve_casualties(attacker, defender, EDD_att, EDD_def, o);
            if (attacker.getCurTotalHp() <= 0 || defender.getCurTotalHp() <= 0)
                break;  // End combat if anyone defeated
            EDD_att = EDD_def = 0;
        }

        //! TODO: Wall of Fire + resolve
        //! TODO: Fear on attackers and defenders

        //! Attacker attacks with Immolation, Life Steal, Poison, Touch, and Melee
        // TODO: centralize sets of attacks
        // TODO: (repeatedly) Dispel Evil
        // TODO: (repeatedly) Destruction
        // TODO: (repeatedly) Heal immediately although after this set of attacks
        EDD_att += special_attack(attacker, defender, "Immolation", o);
        EDD_att += EDD = special_attack(attacker, defender, "Life Steal", o);
        EDD_def -= expected_hp_from_Life_Steal(EDD);
        EDD_att += special_attack(attacker, defender, "Poison", o);
        EDD_att += special_attack(attacker, defender, "Touch", o);
        EDD_att += special_attack(attacker, defender, "Melee", o);

        //! First Strike: resolve casualties
        if (attacker.hasUnitAbility(UNITABILITY_First_Strike) && !defender.hasUnitAbility(UNITABILITY_Negate_First_Strike))
        {
            resolve_casualties(attacker, defender, EDD_att, EDD_def, o);
            EDD_att = EDD_def = 0;

            // End combat if anyone defeated
            if (attacker.getCurTotalHp() <= 0 || defender.getCurTotalHp() <= 0)
                break;
        }

        if (attacker.isHasted())
        {
            EDD_att += special_attack(attacker, defender, "Immolation", o);
            EDD_att += EDD = special_attack(attacker, defender, "Life Steal", o);
            EDD_def -= expected_hp_from_Life_Steal(EDD);
            EDD_att += special_attack(attacker, defender, "Poison", o);
            EDD_att += special_attack(attacker, defender, "Touch", o);
            EDD_att += special_attack(attacker, defender, "Melee", o);
        }

        //! Defender attacks with Immolation, Life Steal, Poison, Touch, and regular Melee

        EDD_att += special_attack(defender, attacker, "Immolation", o);
        EDD_def += EDD = special_attack(defender, attacker, "Life Steal", o);
        EDD_att -= expected_hp_from_Life_Steal(EDD);
        EDD_def += special_attack(defender, attacker, "Poison", o);
        EDD_def += special_attack(defender, attacker, "Touch", o);
        EDD_def += special_attack(defender, attacker, "Melee", o);

        if (defender.isHasted())
        {
            EDD_att += special_attack(defender, attacker, "Immolation", o);
            EDD_def += EDD = special_attack(defender, attacker, "Life Steal", o);
            EDD_att -= expected_hp_from_Life_Steal(EDD);
            EDD_def += special_attack(defender, attacker, "Poison", o);
            EDD_def += special_attack(defender, attacker, "Touch", o);
            EDD_def += special_attack(defender, attacker, "Melee", o);
        }

        resolve_casualties(attacker, defender, EDD_att, EDD_def, o);
        EDD_att = EDD_def = 0;

    } while (0);   // Single-iteration-loop to allow for a break followed by a centralized clean-up

    // Restore actual attributes of attacker and defender
    att = attacker.getActualAttributes();
    def = defender.getActualAttributes();

    //! Update suppressionCounter
    // TODO: Update suppressionCounter
    //   defender.other_effects["suppressionCounter"]++;

    o += "\n";

    return o;
}


std::string MoMCombat::shoot_attack(CombatUnit& attacker, CombatUnit& defender, int distance)
{
    std::string o;        // output string
    std::vector<double> distr;     // track damage distribution
    double EDD = 0;
    double life_stolen = 0;
    bool has_attacked = false;
    double EDD_tmp;

    //! Check if attacker is alive
    if (attacker.getCurTotalHp() <= 0)
    {
        o += "Cannot shoot since attacker is not alive. Attack aborted.\n";
        o += "\n";
        return o;
    }

    //! Check if the attacker is sleeping
    if (attacker.hasCombatEnchantment(COMBATENCHANTMENT_Black_Sleep))
    {
        o += "Unit of " + attacker.getDisplayName() + " can not shoot because it is asleep. Attack aborted.\n";
        o += "\n";
        return o;
    }

    if (defender.isInvisible() && !attacker.isImmune(UNITABILITY_Illusions_Immunity) && (distance > 1))
    {
        o += "Unit of " + attacker.getDisplayName() + " can not shoot an invisible unit at a distance. Attack aborted.\n";
        o += "\n";
        return o;
    }

    int distance_modifier = - distance / 3;
    if (attacker.hasUnitAbility(UNITABILITY_Long_Range))
    {
        distance_modifier = Max(-1, distance_modifier);
    }

    MoMUnit::BaseAttributes& att = attacker.getCombatAttributes();
    MoMUnit::BaseAttributes& def = defender.getCombatAttributes();

    // Chance to hit is always at least 10%, which maps to -2
    int Th_physical = Max(-2, att.toHitRanged + distance_modifier);
    int Th_magical = att.toHitRanged;

    int ranged_Df_modifier;
    if (defender.hasUnitAbility(UNITABILITY_Large_Shield)) ranged_Df_modifier = +2;
    else ranged_Df_modifier = 0;

    int ignore_damage = 0;
    if (defender.isInvulnerable())
    {
        ignore_damage = 2;
        o += "Invulnerability ignores the first " + toStr(ignore_damage) + " points of damage\n";
    }

    if (defender.isInvisible() && !attacker.isImmune(UNITABILITY_Illusions_Immunity))
    {
        Th_physical = Max(-2, Th_physical - 1);
        Th_magical = Max(-2, Th_magical - 1);
        o += "Invisibility reduces to hit ranged physical/magical of " + attacker.getDisplayName()
                + " to " + format_modifier(Th_physical) + "/" + format_modifier(Th_magical) + "\n";
    }

    if (attacker.getSuppressionCounter() >= 2)
    {
        Th_physical = Max(-2, Th_physical - attacker.getSuppressionCounter() / 2);
        Th_magical = Max(-2, Th_magical - attacker.getSuppressionCounter() / 2);
        o += "Suppression reduces to hit ranged physical/magical of " + attacker.getDisplayName()
                + " to " + format_modifier(Th_physical) + "/" + format_modifier(Th_magical) + "\n";
    }

    if ((attacker.getRangedType() == RANGED_Arrow) || (attacker.getRangedType() == RANGED_Bullet))
    {
        if (attacker.getCurRangedShots() <= 0)
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " is out of ammo.\n";
        else if (defender.isImmune(UNITABILITY_Missiles_Immunity))
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " can not shoot a unit with Missile Immunity\n";
        else
        {
            EDD = expected_dam(attacker.getCurFiguresFrac(), att.ranged, Th_physical, defender.getMaxFigures()
                               , def.defense + ranged_Df_modifier, def.hitpoints, def.toDefend, defender.m_simulatedDamage, ignore_damage, &distr);
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " shoots " + defender.getDisplayName()
                    + " at " + toStr(distance) + " squares with " + format_modifier(Th_physical) + " to hit for " + prec1(EDD) + " damage\n";
            attacker.decCurRangedShots();
            has_attacked = true;
        }
    }
    else if (attacker.getRangedType() == RANGED_Rock)
    {
        if (attacker.getCurRangedShots() <= 0)
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " is out of ammo.\n";
        else
        {
            EDD = expected_dam(attacker.getCurFiguresFrac(), att.ranged, Th_physical, defender.getMaxFigures()
                               , def.defense + ranged_Df_modifier, def.hitpoints, def.toDefend, defender.m_simulatedDamage, ignore_damage, &distr);
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " throws a rock at " + defender.getDisplayName()
                    + " at " + toStr(distance) + " squares with " + format_modifier(Th_physical) + " to hit for " + prec1(EDD) + " damage\n";
            attacker.decCurRangedShots();
            has_attacked = true;
        }
    }
    else if (attacker.hasMagicalRangedAttack() && attacker.getMaxRangedShots() > 0)
    {
        // TODO: Check the different colors of the ranged attacks??
        if (attacker.getCurRangedShots() <= 0)
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " is out of ammo.\n";
        else if (defender.isImmune(UNITABILITY_Magic_Immunity))
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " can not magically attack a unit with Magic Immunity\n";
        else
        {
            if (life_steal_applicable(attacker, defender)) // Demon Lord
            {
                int modifier = attacker.getUnitAbility(UNITABILITY_Life_Stealing);
                EDD = expected_lifesteal_dam(attacker.getCurFiguresFrac(), def.resistance + modifier);
                life_stolen = expected_hp_from_Life_Steal(EDD);
                o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Ranged Life Steal for " + prec1(EDD) + " damage and heals " + prec1(life_stolen) + " hitpoints\n";
            }

            EDD_tmp = expected_dam(attacker.getCurFiguresFrac(), att.ranged, Th_magical, defender.getMaxFigures()
                                   , def.defense + ranged_Df_modifier, def.hitpoints, def.toDefend, defender.m_simulatedDamage, ignore_damage, &distr);
            EDD += EDD_tmp;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " magically attacks " + defender.getDisplayName()
                    + " with " + format_modifier(Th_magical) + " to hit for " + prec1(EDD_tmp) + " damage\n";
            attacker.decCurRangedShots();
            has_attacked = true;
        }
    }
    else if (attacker.isCaster() && (att.ranged > 0))
    {
        // TODO: Check the different colors of the ranged attacks??
        if (attacker.getCurrentMana() < 3)
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " is out of mana.\n";
        else if (defender.isImmune(UNITABILITY_Magic_Immunity))
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " can not magically attack a unit with Magic Immunity\n";
        else
        {
            if (life_steal_applicable(attacker, defender)) // Necromancer
            {
                int modifier = attacker.getUnitAbility(UNITABILITY_Life_Stealing);
                EDD = expected_lifesteal_dam(attacker.getCurFiguresFrac(), def.resistance + modifier);
                life_stolen = expected_hp_from_Life_Steal(EDD);
                o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " uses Ranged Life Steal for " + prec1(EDD) + " damage and heals " + prec1(life_stolen) + " hitpoints\n";
            }

            EDD_tmp = expected_dam(attacker.getCurFiguresFrac(), att.ranged, Th_magical, defender.getMaxFigures()
                                   , def.defense + ranged_Df_modifier, def.hitpoints, def.toDefend, defender.m_simulatedDamage, ignore_damage, &distr);
            EDD += EDD_tmp;
            o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " magically attacks " + defender.getDisplayName()
                    + " with " + format_modifier(Th_magical) + " to hit for " + prec1(EDD_tmp) + " damage\n";
            attacker.setCurrentMana(attacker.getCurrentMana() - 3);
            has_attacked = true;
        }
    }
    else
    {
        o += "Unit of " + prec1(attacker.getCurFiguresFrac()) + " " + attacker.getDisplayName() + " can not shoot.\n";
    }

    if (has_attacked)
    {
        // TODO: Suppression
        //      defender.other_effects["suppressionCounter"]++;
    }

    resolve_casualties(attacker, defender, EDD, -life_stolen, o, &distr);

    o += "\n";

    // Restore actual attributes of attacker and defender
    att = attacker.getActualAttributes();
    def = defender.getActualAttributes();

    return o;
}


//! \param attacker The attacker
//! \param defender The defender
std::string MoMCombat::combat_round(CombatUnit& attacker, CombatUnit& defender)
{
    std::string ostr = "Unit " + attacker.toString() + "\nattacks unit " + defender.toString() + "\n";

    /*
   if (g_defender == attacker && g_defender.other_effects["suppressionCounter"] > 0)
   {
      g_defender.other_effects["suppressionCounter"] = g_attacker.other_effects["suppressionCounter"] = 0;
   }
   else if (g_defender == attacker && g_defender.other_effects["suppressionCounter"] <= 0 && g_attacker.other_effects["suppressionCounter"] >=2)
   {
      g_defender.other_effects["suppressionCounter"] = g_attacker.other_effects["suppressionCounter"] = 0;
   }
   else if (g_defender == defender && g_defender.other_effects["suppressionCounter"] >= 2)
   {
      g_defender.other_effects["suppressionCounter"] = g_attacker.other_effects["suppressionCounter"] = 0;
   }
   */

    ostr += combat_attack(attacker, defender);

    return ostr;
}


std::string MoMCombat::shoot_round(CombatUnit& attacker, CombatUnit& defender, int distance)
{
    std::string ostr = "Unit " + attacker.toString() + "\nshoots unit " + defender.toString() + "\n";

    // g_defender.other_effects["suppressionCounter"] = g_attacker.other_effects["suppressionCounter"] = 0;

    ostr += shoot_attack(attacker, defender, distance);

    return ostr;
}


//! Calculated the result of a full combat between 2 units, exchanging blows starting with the attacker
//!
//! UNDER DEVELOPMENT!!!
//!
//! \param attacker The attacker
//! \param defender The defender
std::string MoMCombat::full_combat(StackUnits& attackers, StackUnits& defenders, int& result)
{
    result = 0;      // Draw
    std::string o;   // output string

    StackUnits origAttackers = attackers;
    StackUnits origDefenders = defenders;

    // For each round
    for (int round_nr = 1; (round_nr <= 50) && (result == 0); ++round_nr)
    {
        // For each live attacker
        for (size_t attacker_nr = 0; attacker_nr < attackers.size(); ++attacker_nr)
        {
            CombatUnit& attacker = attackers[attacker_nr];
            if (attacker.getCurTotalHp() <= 0)
                continue;

            // Find first live defender
            size_t defender_nr = findFirstLivingUnit(defenders);
            if (defender_nr >= defenders.size())
            {
                result = +1;
            }
            else
            {
                CombatUnit& defender = defenders[defender_nr];
                // Attack
                o += combat_round(attacker, defender);
            }
        }

        // For each live defender
        for (size_t defender_nr = 0; defender_nr < defenders.size(); ++defender_nr)
        {
            CombatUnit& defender = defenders[defender_nr];
            if (defender.getCurTotalHp() <= 0)
                continue;

            // Find first live attacker
            size_t attacker_nr = findFirstLivingUnit(attackers);
            if (attacker_nr >= attackers.size())
            {
                result = -1;
            }
            else
            {
                CombatUnit& attacker = attackers[attacker_nr];
                // Counter attack
                o += combat_round(defender, attacker);
            }
        }
    }

    // Discard intermediary log
//    if (!m_verbose)
//    {
//        o.clear();
//    }

    o += "\nResult is " + toStr(result) + "\n";

    char buf[4096] = "";
    double origTotalHitpoints = 0;
    double origCurrentHitpoints = 0;
    double totalHitpoints = 0;
    double currentHitpoints = 0;

    countHitpointsUnits(origAttackers, origTotalHitpoints, origCurrentHitpoints);
    countHitpointsUnits(attackers, totalHitpoints, currentHitpoints);
    sprintf(buf, "%d (%.1f/%.1f hp) attackers started, %d (%.1f hp) died, %d (%.1f/%.1f hp) attackers left\n",
            countLivingUnits(origAttackers), origCurrentHitpoints, origTotalHitpoints,
            countLivingUnits(origAttackers) - countLivingUnits(attackers), origCurrentHitpoints - currentHitpoints,
            countLivingUnits(attackers), currentHitpoints, totalHitpoints);
    o += buf;

    countHitpointsUnits(origDefenders, origTotalHitpoints, origCurrentHitpoints);
    countHitpointsUnits(defenders, totalHitpoints, currentHitpoints);
    sprintf(buf, "%d (%.1f/%.1f hp) defenders started, %d (%.1f hp) died, %d (%.1f/%.1f hp) defenders left\n",
            countLivingUnits(origDefenders), origCurrentHitpoints, origTotalHitpoints,
            countLivingUnits(origDefenders) - countLivingUnits(defenders), origCurrentHitpoints - currentHitpoints,
            countLivingUnits(defenders), currentHitpoints, totalHitpoints);
    o += buf;

    o += "\n";

    return o;
}

void MoMCombat::countHitpointsUnits(const StackUnits& units, double& totalHitpoints, double& currentHitpoints)
{
    totalHitpoints = currentHitpoints = 0;
    for (size_t i = 0; i < units.size(); ++i)
    {
        if (units[i].getCurTotalHp() > 0)
        {
            totalHitpoints += units[i].getMaxTotalHp();
            currentHitpoints += units[i].getCurTotalHp();
        }
    }
}

int MoMCombat::countLivingUnits(const StackUnits& units)
{
    int count = 0;
    for (size_t i = 0; i < units.size(); ++i)
    {
        if (units[i].getCurTotalHp() > 0)
        {
            count++;
        }
    }
    return count;
}

size_t MoMCombat::findFirstLivingUnit(const StackUnits& units)
{
    int index = units.size();
    for (size_t i = 0; i < units.size(); ++i)
    {
        if (units[i].getCurTotalHp() > 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

}
