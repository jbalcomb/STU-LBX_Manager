// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2011-09-30
// ---------------------------------------------------------------------------

#include "QMoMResources.h"

#include "MoMGameBase.h"
#include "MoMGenerated.h"
#include "MoMLbxBase.h"
#include "MoMResources.h"
#include "MoMUtility.h"
#include "MoMExeWizards.h"
#include "QMoMLbx.h"
#include "QMoMTreeItemModel.h"
#include "QMoMUtility.h"

#include <QFontDatabase>

#include <string>

namespace MoM
{

namespace {

template<typename Enum>
struct Special
{
    Enum            ability;
    eHelpIndex      helpIndex;
    std::string     lbxFile;
    int             lbxIndex;
};

static Special<eHeroAbility> gTableHeroSpecials[eHeroAbility_MAX] =
{
    { HEROABILITY_Leadership, HELP_hero_SPECIAL_LEADERSHIP, "SPECIAL.LBX", 43 },
    { HEROABILITY_Leadership_X, HELP_hero_SPECIAL_LEADERSHIP, "SPECIAL.LBX", 43 },
    { HEROABILITY_2, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Legendary, HELP_hero_SPECIAL_LEGENDARY, "SPECIAL.LBX", 45 },
    { HEROABILITY_Legendary_X, HELP_hero_SPECIAL_LEGENDARY, "SPECIAL.LBX", 45 },
    { HEROABILITY_5, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Blademaster, HELP_hero_SPECIAL_BLADEMASTER, "SPECIAL.LBX", 46 },
    { HEROABILITY_Blademaster_X, HELP_hero_SPECIAL_BLADEMASTER, "SPECIAL.LBX", 46 },
    { HEROABILITY_8, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Armsmaster, HELP_hero_SPECIAL_ARMSMASTER, "SPECIAL.LBX", 47 },
    { HEROABILITY_Armsmaster_X, HELP_hero_SPECIAL_ARMSMASTER, "SPECIAL.LBX", 47 },
    { HEROABILITY_11, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Constitution, HELP_hero_SPECIAL_CONSTITUTION, "SPECIAL.LBX", 50 },
    { HEROABILITY_Constitution_X, HELP_hero_SPECIAL_CONSTITUTION, "SPECIAL.LBX", 50 },
    { HEROABILITY_14, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Might, HELP_hero_SPECIAL_MIGHT, "SPECIAL.LBX", 52 },
    { HEROABILITY_Might_X, HELP_hero_SPECIAL_MIGHT, "SPECIAL.LBX", 52 },
    { HEROABILITY_17, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Arcane_Power, HELP_hero_SPECIAL_ARCANE_POWER, "SPECIAL.LBX", 54 },
    { HEROABILITY_Arcane_Power_X, HELP_hero_SPECIAL_ARCANE_POWER, "SPECIAL.LBX", 56 },  // TODO: ??
    { HEROABILITY_20, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Sage, HELP_hero_SPECIAL_SAGE, "SPECIAL.LBX", 61 },
    { HEROABILITY_Sage_X, HELP_hero_SPECIAL_SAGE, "SPECIAL.LBX", 61 },
    { HEROABILITY_23, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Prayermaster, HELP_hero_SPECIAL_PRAYERMASTER, "SPECIAL.LBX", 57 },
    { HEROABILITY_Prayermaster_X, HELP_hero_SPECIAL_PRAYERMASTER, "SPECIAL.LBX", 57 },
    { HEROABILITY_Agility_X, HELP_hero_SPECIAL2_AGILE, "SPECIAL2.LBX", 32 },
    { HEROABILITY_Lucky, HELP_hero_SPECIAL_LUCKY, "SPECIAL.LBX", 58 },
    { HEROABILITY_Charmed, HELP_hero_SPECIAL_CHARMED, "SPECIAL.LBX", 86 },
    { HEROABILITY_Noble, HELP_hero_SPECIAL_NOBLE, "SPECIAL.LBX", 87 },
    { HEROABILITY_30, eHelpIndex_NONE, "", -1 },
    { HEROABILITY_Agility, HELP_hero_SPECIAL2_AGILE, "SPECIAL2.LBX", 32 },
};

static Special<eItemPower> gTableItemSpecials[eItemPower_MAX] =
{
    { ITEMPOWER_Vampiric, eHelpIndex_NONE, "SPECIAL.LBX", 31 },
    { ITEMPOWER_Guardian_Wind, HELP_ITEMISC_GUARDIAN_WIND, "ITEMISC.LBX", 35 },
    { ITEMPOWER_Lightning, eHelpIndex_NONE, "SPECIAL.LBX", 28 },
    { ITEMPOWER_Cloak_Of_Fear, HELP_ITEMISC_CLOAK_OF_FEAR, "ITEMISC.LBX", 15 },
    { ITEMPOWER_Destruction, HELP_SPECIAL2_DESTRUCTION, "SPECIAL2.LBX", 5 },
    { ITEMPOWER_Wraith_Form, HELP_SPECIAL_WRAITH_FORM, "SPECIAL.LBX", 68 },
    { ITEMPOWER_Regeneration, HELP_ITEMISC_REGENERATE, "ITEMISC.LBX", 1 },
    { ITEMPOWER_Pathfinding, HELP_ITEMISC_PATH_FINDING, "ITEMISC.LBX", 4 },

    { ITEMPOWER_Water_Walking, HELP_ITEMISC_WATER_WALKING, "ITEMISC.LBX", 5 },
    { ITEMPOWER_Resist_Elements, HELP_ITEMISC_RESIST_ELEMENTS, "ITEMISC.LBX", 13 },
    { ITEMPOWER_Elemental_Armour, HELP_ITEMISC_ELEMENTAL_ARMOR, "ITEMISC.LBX", 14 },
    { ITEMPOWER_Chaos, HELP_SPECIAL2_CHAOS, "SPECIAL2.LBX", 29 },
    { ITEMPOWER_Stoning, HELP_SPECIAL_STONING_TOUCH, "SPECIAL.LBX", 27 },
    { ITEMPOWER_Endurance, HELP_ITEMISC_ENDURANCE, "ITEMISC.LBX", 10 },
    { ITEMPOWER_Haste, HELP_ITEMISC_HASTE, "ITEMISC.LBX", 2 },
    { ITEMPOWER_Invisibility, HELP_ITEMISC_INVISIBILITY, "ITEMISC.LBX", 8 },

    { ITEMPOWER_Death, eHelpIndex_NONE, "SPECIAL2.LBX", 30 },
    { ITEMPOWER_Flight, HELP_ITEMISC_FLIGHT, "ITEMISC.LBX", 16 },
    { ITEMPOWER_Resist_Magic, HELP_ITEMISC_RESIST_MAGIC, "ITEMISC.LBX", 17 },
    { ITEMPOWER_Magic_Immunity, HELP_ITEMISC_MAGIC_IMMUNITY, "ITEMISC.LBX", 0 },
    { ITEMPOWER_Flaming, eHelpIndex_NONE, "SPECIAL.LBX", 83 },
    { ITEMPOWER_Holy_Avenger, eHelpIndex_NONE, "ITEMISC.LBX", 33 },
    { ITEMPOWER_True_Sight, HELP_ITEMISC_TRUE_SIGHT, "ITEMISC.LBX", 3 },
    { ITEMPOWER_Phantasmal, eHelpIndex_NONE, "SPECIAL.LBX", 35 },

    { ITEMPOWER_Power_Drain, eHelpIndex_NONE, "SPECIAL2.LBX", 31 },
    { ITEMPOWER_Bless, HELP_ITEMISC_BLESS, "ITEMISC.LBX", 6 },
    { ITEMPOWER_Lion_Heart, HELP_ITEMISC_LION_HEART, "ITEMISC.LBX", 7 },
    { ITEMPOWER_Giant_Strength, HELP_ITEMISC_GIANT_STRENGTH, "ITEMISC.LBX", 34 },
    { ITEMPOWER_Planar_Travel, HELP_ITEMISC_PLANAR_TRAVEL, "ITEMISC.LBX", 9 },
    { ITEMPOWER_Merging, HELP_ITEMISC_MERGING_2, "ITEMISC.LBX", 18 },
    { ITEMPOWER_Righteousness, HELP_ITEMISC_RIGHTEOUSNESS, "ITEMISC.LBX", 11 },
    { ITEMPOWER_Invulnerability, HELP_ITEMISC_INVULNERABILITY, "ITEMISC.LBX", 12 },
};

static Special<eSpell> gTableAdditionalSpells[] =
{
    { SPELL_True_Light, (eHelpIndex)600, "COMPIX.LBX", 5 },
    { SPELL_Darkness, (eHelpIndex)601, "COMPIX.LBX", 6 },
    { SPELL_Warp_Reality, (eHelpIndex)602, "COMPIX.LBX", 7 },
    { SPELL_Black_Prayer, (eHelpIndex)603, "COMPIX.LBX", 8 },
    { SPELL_Wrack, (eHelpIndex)604, "COMPIX.LBX", 9 },
    { SPELL_Metal_Fires, (eHelpIndex)605, "COMPIX.LBX", 10 },
    { SPELL_Prayer, (eHelpIndex)606, "COMPIX.LBX", 11 },
    { SPELL_High_Prayer, (eHelpIndex)607, "COMPIX.LBX", 12 },
    { SPELL_Terror, (eHelpIndex)608, "COMPIX.LBX", 13 },
    { SPELL_Call_Lightning, (eHelpIndex)609, "COMPIX.LBX", 14 },
    { SPELL_Counter_Magic, (eHelpIndex)610, "COMPIX.LBX", 15 },
    { SPELL_Mass_Invisibility, (eHelpIndex)611, "COMPIX.LBX", 41 },
//    { SPELL_DISPELS NON-SORCERY, (eHelpIndex)612, "COMPIX.LBX", 54 },
//    { SPELL_DISPELS NON-CHAOS, 613, "COMPIX.LBX", 47 },
//    { SPELL_DISPELS NON-NATURE, 614, "COMPIX.LBX", 52 },
//    { SPELL_SORCERY NODE AURA, 615, "COMPIX.LBX", 55 },
//    { SPELL_CHAOS NODE AURA, 616, "COMPIX.LBX", 48 },
//    { SPELL_NATURE NODE AURA, 617, "COMPIX.LBX", 53 },
    { SPELL_Cloud_Of_Shadow, (eHelpIndex)618, "COMPIX.LBX", 50 },
//    { SPELL_HOLY LIGHT, 619, "COMPIX.LBX", 44 },
    { SPELL_Chaos_Surge, (eHelpIndex)620, "COMPIX.LBX", 46 },
    { SPELL_Eternal_Night, (eHelpIndex)621, "COMPIX.LBX", 49 },
    { SPELL_Crusade, (eHelpIndex)622, "COMPIX.LBX", 42 },
    { SPELL_Holy_Arms, (eHelpIndex)623, "COMPIX.LBX", 43 },
    { SPELL_Charm_of_Life, (eHelpIndex)624, "COMPIX.LBX", 45 },
    { SPELL_Zombie_Mastery, (eHelpIndex)625, "COMPIX.LBX",51 },

    { SPELL_Entangle, (eHelpIndex)632, "COMPIX.LBX", 60 },

    { SPELL_Immolation, (eHelpIndex)449, "SPECIAL2.LBX", 6 },
    { SPELL_Guardian_Wind, (eHelpIndex)450, "SPECIAL2.LBX", 7 },
    { SPELL_Berserk, (eHelpIndex)451, "SPECIAL2.LBX	", 7 },
    { SPELL_Cloak_of_Fear, (eHelpIndex)452, "SPECIAL.LBX", 97 },
    { SPELL_Black_Channels, (eHelpIndex)453, "SPECIAL.LBX", 67 },
    { SPELL_Wraith_Form, (eHelpIndex)454, "SPECIAL.LBX", 68 },
    { SPELL_Regeneration, (eHelpIndex)455, "SPECIAL.LBX", 69 },
    { SPELL_Path_Finding, (eHelpIndex)456, "SPECIAL.LBX", 70 },
    { SPELL_Water_Walking, (eHelpIndex)457, "SPECIAL.LBX", 71 },
    { SPELL_Elemental_Armor, (eHelpIndex)458, "SPECIAL.LBX", 73 },
    { SPELL_Resist_Elements, (eHelpIndex)459, "SPECIAL.LBX", 72 },
    { SPELL_Stone_Skin, (eHelpIndex)460, "SPECIAL.LBX", 74 },
    { SPELL_Iron_Skin, (eHelpIndex)461, "SPECIAL.LBX", 75 },
    { SPELL_Endurance, (eHelpIndex)462, "SPECIAL.LBX", 76 },
    { SPELL_Spell_Lock, (eHelpIndex)463, "L2.LBX", 8 },
    { SPELL_Invisibility, (eHelpIndex)464, "SPECIAL.LBX", 78 },
    { SPELL_Wind_Walking, (eHelpIndex)465, "SPECIAL.LBX", 79 },
    { SPELL_Flight, (eHelpIndex)466, "SPECIAL.LBX", 80 },
    { SPELL_Resist_Magic, (eHelpIndex)467, "SPECIAL.LBX", 81 },
    { SPELL_Magic_Immunity, (eHelpIndex)468, "SPECIAL.LBX", 82 },
    { SPELL_Flame_Blade, (eHelpIndex)469, "SPECIAL.LBX", 83 },
    { SPELL_Eldritch_Weapon, (eHelpIndex)470, "SPECIAL.LBX", 84 },
    { SPELL_True_Sight, (eHelpIndex)471, "SPECIAL.LBX", 85 },
    { SPELL_Holy_Weapon, (eHelpIndex)472, "SPECIAL.LBX", 86 },
    { SPELL_Heroism, (eHelpIndex)473, "SPECIAL.LBX", 87 },
    { SPELL_Bless, (eHelpIndex)474, "SPECIAL.LBX", 88 },
    { SPELL_Lionheart, (eHelpIndex)475, "SPECIAL.LBX", 89 },
    { SPELL_Giant_Strength, (eHelpIndex)476, "SPECIAL.LBX", 65 },
    { SPELL_Planar_Travel, (eHelpIndex)477, "SPECIAL.LBX", 91 },
    { SPELL_Holy_Armor, (eHelpIndex)478, "SPECIAL.LBX", 92 },
    { SPELL_Righteousness, (eHelpIndex)479, "SPECIAL.LBX", 93 },
    { SPELL_Invulnerability, (eHelpIndex)480, "SPECIAL.LBX", 94 },
    { SPELL_Vertigo, (eHelpIndex)481, "SPECIAL.LBX", 101 },
    { SPELL_Confusion, (eHelpIndex)482, "SPECIAL2.LBX", 0 },
//    { SPELL_WHIRLWIND, 483, "SPECIAL2.LBX", 1 },
    { SPELL_Mind_Storm, (eHelpIndex)484, "SPECIAL2.LBX", 2 },
    { SPELL_Shatter, (eHelpIndex)485, "SPECIAL.LBX", 95 },
    { SPELL_Weakness, (eHelpIndex)486, "SPECIAL.LBX", 96 },
    { SPELL_Black_Sleep, (eHelpIndex)487, "SPECIAL2.LBX", 3 },
//    { SPELL_Warp_Creature, 488, "SPECIAL2.LBX", 11 },
//    { SPELL_Warp_Creature, 489, "SPECIAL2.LBX", 12 },
//    { SPELL_Warp_Creature, 490, "SPECIAL2.LBX", 13 },
    { SPELL_Mana_Leak, (eHelpIndex)491, "SPECIAL2.LBX", 10 },
    { SPELL_Haste, (eHelpIndex)492, "SPECIAL.LBX", 77 },
    { SPELL_Web, (eHelpIndex)493, "SPECIAL.LBX", 99 },
    { SPELL_Creature_Binding, (eHelpIndex)494, "SPECIAL.LBX", 100 },
    { SPELL_Possession, (eHelpIndex)495, "SPECIAL.LBX", 98 },
};

// lbxIndex is not the actual index, but must still be multiplied by 8
// This is because there are 8 animations for each special.
// One for each direction.
static Special<eRanged_Type> gTableRangedTypes[eRanged_Type_MAX] =
{
    // Zeroes
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },

    // 10-19 Rocks
    { RANGED_Rock, HELP_UNITVIEW_Rock, "CMBMAGIC.LBX", 4 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },

    // 20-29 Missiles
    { RANGED_Arrow, HELP_UNITVIEW_Bow, "CMBMAGIC.LBX", 1 },
    { RANGED_Bullet, eHelpIndex_NONE, "CMBMAGIC.LBX", 5 }, // TODO
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },
    { RANGED_None, eHelpIndex_NONE, "", -1 },

    // 30-39 Magic ranged attack
    { RANGED_Chaos_Lightning, eHelpIndex_NONE, "CMBMAGIC.LBX", 0 },
    { RANGED_Chaos_Fireball, eHelpIndex_NONE, "CMBMAGIC.LBX", 2 },
    { RANGED_Sorcery_Illusion, eHelpIndex_NONE, "CMBMAGIC.LBX", 3 },
    { RANGED_Chaos_Deathbolt, eHelpIndex_NONE, "CMBMAGIC.LBX", 6 },
    { RANGED_Nature_Icebolt, eHelpIndex_NONE, "CMBMAGIC.LBX", 7 },
    { RANGED_Nature_Priest, eHelpIndex_NONE, "CMBMAGIC.LBX", 9 },
    { RANGED_Chaos_Drow, eHelpIndex_NONE, "CMBMAGIC.LBX", 10 },
    { RANGED_Nature_Shimmer, eHelpIndex_NONE, "CMBMAGIC.LBX", 11 },
    { RANGED_Nature_Green, eHelpIndex_NONE, "CMBMAGIC.LBX", 11 },
    { RANGED_None, eHelpIndex_NONE,  "", -1 },

    // TODO
    // 100-109 Thrown/Breath/Gaze
//    RANGED_Thrown_Weapons = 100,
//    RANGED_Fire_Breath = 101,               // (Chaos)
//    RANGED_Lightning_Breath = 102,          // (Chaos)  Sky_Drake
//    RANGED_Stoning_Gaze = 103,              // (Nature) Basilisk, Gorgons (resistance modifier in byte 17)
//    RANGED_Multiple_Gaze = 104,             // (Chaos)  Chaos Spawn
//                                            //          This is actually 3 gazes, including stoning and death
//    RANGED_Death_Gaze = 105,                // (Death)  Night stalker (resistance modifier in byte 17)
};

static Special<eUnitAbility> gTableUnitSpecials[eUnitAbility_MAX] =
{
    // Movement_Flags
    // TODO: Image numbers?? In MAIN.LBX but also in SPECIAL.LBX:0,1,2??
    // TODO: left is: Pathfinding, Wind Walking, Planar Travel
    { UNITABILITY_Cavalry, eHelpIndex_NONE, "MAIN.LBX", 38 },
    { UNITABILITY_Sailing, HELP_MAIN_SAILING, "MAIN.LBX", 18 },
    { UNITABILITY_Swimming, HELP_MAIN_SWIMMING, "MAIN.LBX", 19 },
    { UNITABILITY_Flying, HELP_MAIN_FLYING, "MAIN.LBX", 22 },
    { UNITABILITY_Teleporting, HELP_SPECIAL_TELEPORTING, "SPECIAL.LBX", 37 },
    { UNITABILITY_Forester, HELP_SPECIAL_FORESTER, "SPECIAL.LBX", 1 },
    { UNITABILITY_Mountaineer, HELP_SPECIAL_MOUNTAINEER, "SPECIAL.LBX", 2 },
    { UNITABILITY_Merging, HELP_ITEMISC_MERGING_1, "ITEMISC.LBX", 36 },

    // Zeroes
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },

    // Immunity_Flags
    { UNITABILITY_Fire_Immunity, HELP_SPECIAL_FIRE_IMMUNITY, "SPECIAL.LBX", 6 },
    { UNITABILITY_Stoning_Immunity, HELP_SPECIAL_STONING_IMMUNITY, "SPECIAL.LBX", 7 },
    { UNITABILITY_Missiles_Immunity, HELP_SPECIAL_MISSILES_IMMUNITY, "SPECIAL.LBX", 9 },
    { UNITABILITY_Illusions_Immunity, HELP_SPECIAL_ILLUSIONS_IMMUNITY, "SPECIAL.LBX", 10 },
    { UNITABILITY_Cold_Immunity, HELP_SPECIAL_COLD_IMMUNITY, "SPECIAL.LBX", 11 },
    { UNITABILITY_Magic_Immunity, HELP_SPECIAL_MAGIC_IMMUNITY_1, "SPECIAL.LBX", 12 },
    { UNITABILITY_Death_Immunity, HELP_SPECIAL_DEATH_IMMUNITY, "SPECIAL.LBX", 49 },
    { UNITABILITY_Poison_Immunity, HELP_SPECIAL_POISON_IMMUNITY, "SPECIAL.LBX", 5 },

    // Attribute_Flags
    { UNITABILITY_Weapon_Immunity, HELP_SPECIAL_WEAPON_IMMUNITY, "SPECIAL.LBX", 8 },
    { UNITABILITY_Flyer, HELP_SPECIAL_FLIGHT, "SPECIAL.LBX", 80 },
    { UNITABILITY_Lucky, HELP_SPECIAL2_LUCKY, "SPECIAL2.LBX", 16 },
    { UNITABILITY_Summon_Demons_1, HELP_SPECIAL2_SUMMON_DEMONS, "SPECIAL2.LBX", 28 },
    { UNITABILITY_Summon_Demons_2, HELP_SPECIAL2_SUMMON_DEMONS, "SPECIAL2.LBX", 28 },
    { UNITABILITY_Caster_20_MP, HELP_SPECIAL_CASTER, "SPECIAL.LBX", 55 },
    { UNITABILITY_Caster_40_MP, HELP_SPECIAL_CASTER, "SPECIAL.LBX", 55 },
    { UNITABILITY_Standard, eHelpIndex_NONE, "SPECIAL.LBX", -1 },

    { UNITABILITY_Healing_Spell, HELP_SPECIAL_HEALING_SPELL, "SPECIAL.LBX", 38 },
    { UNITABILITY_Fire_Ball_Spell, HELP_SPECIAL_FIRE_BALL_SPELL, "SPECIAL.LBX", 39 },
    { UNITABILITY_Doombolt_Spell, HELP_SPECIAL_DOOMBOLT_SPELL, "SPECIAL.LBX", 41 },
    { UNITABILITY_Immolation, HELP_SPECIAL_IMMOLATION, "SPECIAL.LBX", 32 },
    { UNITABILITY_Web_Spell, HELP_SPECIAL2_WEB_SPELL, "SPECIAL2.LBX", 20 },
    { UNITABILITY_Cause_Fear_Spell, HELP_SPECIAL2_CAUSE_FEAR_SPELL, "SPECIAL2.LBX", 21},
    { UNITABILITY_Resistance_to_All, HELP_SPECIAL_RESISTANCE_TO_ALL, "SPECIAL.LBX", 33 },
    { UNITABILITY_Holy_Bonus, HELP_SPECIAL_HOLY_BONUS, "SPECIAL.LBX", 34 },

    // Zeroes
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },
    { eUnitAbility_NONE, eHelpIndex_NONE, "", -1 },

    // Ability_Flags
    { UNITABILITY_Summoned_Unit, eHelpIndex_NONE, "", -1 },
    { UNITABILITY_Large_Shield, HELP_SPECIAL_LARGE_SHIELD, "SPECIAL.LBX", 14 },
    { UNITABILITY_Plane_Shift, HELP_SPECIAL_PLANE_SHIFT, "SPECIAL.LBX", 4 },
    { UNITABILITY_Wall_Crusher, HELP_SPECIAL_WALL_CRUSHER, "SPECIAL.LBX", 15 },
    { UNITABILITY_Healer, HELP_SPECIAL_HEALER, "SPECIAL.LBX", 16 },
    { UNITABILITY_Create_Outpost, HELP_SPECIAL_CREATE_OUTPOST, "SPECIAL.LBX", 17 },
    { UNITABILITY_Invisibility, HELP_SPECIAL_INVISIBILITY_1, "SPECIAL.LBX", 18 },
    { UNITABILITY_Create_Undead, HELP_SPECIAL_CREATE_UNDEAD, "SPECIAL.LBX", 19 },

    { UNITABILITY_Long_Range, HELP_SPECIAL2_LONG_RANGE, "SPECIAL2.LBX", 18 },
    { UNITABILITY_Land_Corruption, HELP_SPECIAL_LAND_CORRUPTION, "SPECIAL.LBX", 21 },
    { UNITABILITY_Meld_With_Node, HELP_SPECIAL_MELD_WITH_NODE, "SPECIAL.LBX", 40 },
    { UNITABILITY_Non_Corporeal, HELP_SPECIAL_NON_CORPOREAL, "SPECIAL.LBX", 22 },
    { UNITABILITY_Wind_Walking, HELP_SPECIAL_WIND_WALKING_1, "SPECIAL.LBX", 23 },
    { UNITABILITY_Regeneration, HELP_SPECIAL_REGENERATION, "SPECIAL.LBX", 24 },
    { UNITABILITY_Purify, HELP_SPECIAL_PURIFY, "SPECIAL.LBX", 25 },
    { UNITABILITY_Negate_First_Strike, HELP_SPECIAL_NEGATE_FIRST_STRIKE, "SPECIAL.LBX", 48 },

    // Attack_Flags
    { UNITABILITY_Armor_Piercing, HELP_SPECIAL_ARMOR_PIERCING, "SPECIAL.LBX", 28 },
    { UNITABILITY_First_Strike, HELP_SPECIAL_FIRST_STRIKE, "SPECIAL.LBX", 29 },
    { UNITABILITY_Poison_attack, HELP_SPECIAL_POISON, "SPECIAL.LBX", 30 },
    { UNITABILITY_Life_Stealing, HELP_SPECIAL_LIFE_STEAL, "SPECIAL.LBX", 31 },
    { UNITABILITY_Automatic_Damage, HELP_SPECIAL2_CHAOS, "SPECIAL2.LBX", 29 },
    { UNITABILITY_Destruction, HELP_SPECIAL2_DESTRUCTION, "SPECIAL2.LBX", 5 },
    { UNITABILITY_Illusionary_attack, HELP_SPECIAL_ILLUSION, "SPECIAL.LBX", 35 },
    { UNITABILITY_Stoning_Touch, HELP_SPECIAL_STONING_TOUCH, "SPECIAL.LBX", 27 },

    { UNITABILITY_No_effect05, eHelpIndex_NONE, "", -1 },
    { UNITABILITY_Death_Touch, HELP_SPECIAL2_CAUSE_DEATH, "SPECIAL2.LBX", 30 },
    { UNITABILITY_Power_Drain, HELP_SPECIAL2_POWER_DRAIN, "SPECIAL2.LBX", 31 },
    { UNITABILITY_Dispel_Evil, HELP_SPECIAL2_DISPEL_EVIL, "SPECIAL.LBX", 22 },
    { UNITABILITY_Ball_COMBAT, eHelpIndex_NONE, "", -1 },
    { UNITABILITY_Simultaneous_Damage_COMBAT, eHelpIndex_NONE, "", -1 },
    { UNITABILITY_Eldritch_Weapon_COMBAT, HELP_SPECIAL_ELDRITCH_WEAPON, "SPECIAL.LBX", 84 },
    { UNITABILITY_Warp_Lightning_COMBAT, eHelpIndex_NONE, "", -1 },
};

static Special<eUnitEnchantment> gTableUnitEnchantments[eUnitEnchantment_MAX] =
{
    { UNITENCHANTMENT_Immolation, HELP_SPECIAL2_IMMOLATION, "SPECIAL2.LBX", 6 },
    { UNITENCHANTMENT_Guardian_Wind, HELP_SPECIAL2_GUARDIAN_WIND, "SPECIAL2.LBX", 7 },
    { UNITENCHANTMENT_Berserk, HELP_SPECIAL2_BERSERK, "SPECIAL2.LBX", 17 },
    { UNITENCHANTMENT_Cloak_of_Fear, HELP_SPECIAL_CLOAK_OF_FEAR, "SPECIAL.LBX", 97 },
    { UNITENCHANTMENT_Black_Channels, HELP_SPECIAL_BLACK_CHANNELS, "SPECIAL.LBX", 67 },
    { UNITENCHANTMENT_Wraith_Form, HELP_SPECIAL_WRAITH_FORM, "SPECIAL.LBX", 68 },
    { UNITENCHANTMENT_Regeneration, HELP_SPECIAL_REGENERATE, "SPECIAL.LBX", 69 },
    { UNITENCHANTMENT_Path_Finding, HELP_SPECIAL_PATHFINDING_1, "SPECIAL.LBX", 70 },

    { UNITENCHANTMENT_Water_Walking, HELP_SPECIAL_WATER_WALKING, "SPECIAL.LBX", 71 },
    { UNITENCHANTMENT_Resist_Elements, HELP_SPECIAL_RESIST_ELEMENTS, "SPECIAL.LBX", 72 },
    { UNITENCHANTMENT_Elemental_Armor, HELP_SPECIAL_ELEMENTAL_ARMOR, "SPECIAL.LBX", 73 },
    { UNITENCHANTMENT_Stone_Skin, HELP_SPECIAL_STONE_SKIN, "SPECIAL.LBX", 74 },
    { UNITENCHANTMENT_Iron_Skin, HELP_SPECIAL_IRON_SKIN, "SPECIAL.LBX", 75 },
    { UNITENCHANTMENT_Endurance, HELP_SPECIAL_ENDURANCE, "SPECIAL.LBX", 76 },
    { UNITENCHANTMENT_Spell_Lock, HELP_SPECIAL2_SPELL_LOCK, "SPECIAL2.LBX", 8 },
    { UNITENCHANTMENT_Invisibility, HELP_SPECIAL_INVISIBILITY_2, "SPECIAL.LBX", 78 },

    { UNITENCHANTMENT_Wind_Walking, HELP_SPECIAL_WIND_WALKING_2, "SPECIAL.LBX", 79 },
    { UNITENCHANTMENT_Flight, HELP_SPECIAL_FLIGHT, "SPECIAL.LBX", 80 },
    { UNITENCHANTMENT_Resist_Magic, HELP_SPECIAL_RESIST_MAGIC, "SPECIAL.LBX", 81 },
    { UNITENCHANTMENT_Magic_Immunity, HELP_SPECIAL_MAGIC_IMMUNITY_2, "SPECIAL.LBX", 82 },
    { UNITENCHANTMENT_Flame_Blade, HELP_SPECIAL_FLAME_BLADE, "SPECIAL.LBX", 83 },
    { UNITENCHANTMENT_Eldritch_Weapon, HELP_SPECIAL_ELDRITCH_WEAPON, "SPECIAL.LBX", 84 },
    { UNITENCHANTMENT_True_Sight, HELP_SPECIAL_TRUE_SIGHT, "SPECIAL.LBX", 85 },
    { UNITENCHANTMENT_Holy_Weapon, HELP_SPECIAL_HOLY_WEAPON, "SPECIAL.LBX", 86 },

    { UNITENCHANTMENT_Heroism, HELP_SPECIAL_HEROISM, "SPECIAL.LBX", 87 },
    { UNITENCHANTMENT_Bless, HELP_SPECIAL_BLESS, "SPECIAL.LBX", 88 },
    { UNITENCHANTMENT_Lionheart, HELP_SPECIAL_LION_HEART, "SPECIAL.LBX", 89 },
    { UNITENCHANTMENT_Giant_Strength, HELP_SPECIAL_GIANT_STRENGTH, "SPECIAL.LBX", 65 },
    { UNITENCHANTMENT_Planar_Travel, HELP_SPECIAL_PLANAR_TRAVEL, "SPECIAL.LBX", 91 },
    { UNITENCHANTMENT_Holy_Armor, HELP_SPECIAL_HOLY_ARMOR, "SPECIAL.LBX", 90 },
    { UNITENCHANTMENT_Righteousness, HELP_SPECIAL_RIGHTEOUSNESS, "SPECIAL.LBX", 93 },
    { UNITENCHANTMENT_Invulnerability, HELP_SPECIAL_INVULNERABILITY, "SPECIAL.LBX", 94 },
};

static Special<eUnitMutation> gTableUnitMutations[eUnitMutation_MAX] =
{
    { UNITMUTATION_Chaos_Channels_Demon_Skin, HELP_SPECIAL_CHAOS_CHANNEL_1, "SPECIAL.LBX", 62 },
    { UNITMUTATION_Chaos_Channels_Demon_Wings, HELP_SPECIAL_CHAOS_CHANNEL_2, "SPECIAL.LBX", 63 },
    { UNITMUTATION_Chaos_Channels_Fiery_Breath, HELP_SPECIAL_CHAOS_CHANNEL_3, "SPECIAL.LBX", 64 },
    { UNITMUTATION_Undead, HELP_SPECIAL2_UNDEAD, "SPECIAL2.LBX", 9 },
    { UNITMUTATION_Stasis_initial, HELP_SPECIAL2_STASIS, "SPECIAL2.LBX", 4 },
    { UNITMUTATION_Stasis_lingering, HELP_SPECIAL2_STASIS, "SPECIAL2.LBX", 4 },
};

MoM::MoMLbxBase loadLbx(const std::string& gameDirectory, const std::string& lbxFileTitle)
{
    if (gameDirectory.empty())
        return {};
    std::string lbxFile = gameDirectory + "/" + lbxFileTitle;
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return {};
    return lbx;
}

}

const QFont QMoMResources::g_FontSmall("Sans Serif", 8, -1, false);
#ifdef _WIN32
QFont QMoMResources::g_Font("Monotype Corsiva", 10, -1, false);
#else
// Note: attribute italic=true is required for URW Chancery L
QFont QMoMResources::g_Font("URW Chancery L", 12, -1, true);
#endif
const QFont QMoMResources::g_FontFixed("Courier New", 10, -1, false);

QMoMResources* QMoMResources::m_instance = 0;

QMoMResources::QMoMResources(const QMoMGamePtr& game) : m_game()
{
//    int idMoMFont4 = QFontDatabase::addApplicationFont(":/fonts/MoMFont4.ttf");
//    qDebug() << "Loading/setting MoMFont4:" << idMoMFont4;
//    int idMoMFont7 = QFontDatabase::addApplicationFont(":/fonts/MoMFont7.ttf");
//    g_Font = QFont("MoMFont4");

    setGame(game);
}

QMoMResources::~QMoMResources()
{
}

void QMoMResources::createInstance(const QMoMGamePtr &game)
{
    delete m_instance;
    m_instance = new QMoMResources(game);
}

QMoMResources &QMoMResources::instance()
{
    if (0 == m_instance)
    {
        m_instance = new QMoMResources(QMoMGamePtr());
    }
    return *m_instance;
}

void QMoMResources::setGame(const QMoMGamePtr& game)
{
    if (game.data() != m_game.data())
    {
//        qDebug() << getDateTimeStr() << ">QMoMResources::setGame() begin";
        m_game = game;
        m_wizardsExe.reset();

        (void)createColorTable();

        createLbxImages("BACKGRND", m_backgrndImages);
        createBuildingAnimations();
        createCitySizeImages();
        createLbxImages("CMBTCITY", m_cmbtcityImages);
        createLbxAnimations("CITYWALL", m_citywallAnimations);
        createFigureAnimations();
        createLbxImages("ITEMISC", m_itemiscImages);
        createLbxImages("ITEMS", m_itemsImages);
        createLbxImages("MAPBACK", m_mapBackImages);
        // Lair images use MAPBACK images
        createLairImages();

        createLbxImages("SPECIAL", m_specialImages);
        QVector<QMoMImagePtr> special2Images;
        createLbxImages("SPECIAL2", special2Images);
        m_specialImages.resize(120 + special2Images.size());
        for(int i = 0; i < special2Images.size(); ++i)
        {
            m_specialImages[120 + i] = special2Images[i];
        }

        // UnitImages are created before SpellImages, because SpellImages uses them
        createUnitImages();
        // m_specialImages and UnitImages should already have been created, because SpellImages uses them
        createSpellImages();
        createLbxImages("CMBGRASS", m_terrainBattleImages);
        createTerrainImages();

        if (!m_game->getGameDirectory().empty())
        {
            auto wizardsExe = std::make_unique<MoMExeWizards>();
            if (wizardsExe->load(m_game->getGameDirectory() + "/" + "WIZARDS.EXE"))
            {
                m_wizardsExe.swap(wizardsExe);
            }
        }

//        qDebug() << getDateTimeStr() << "<QMoMResources::setGame() end";
    }
}

const QMoMAnimation QMoMResources::getAnimation(const LBXRecordID &lbxRecordID) const
{
    QMoMAnimation animation;
    if (m_game.isNull())
        return animation;
    std::string lbxFile = m_game->getGameDirectory() + "/" + lbxRecordID.lbxTitle + ".LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return animation;
    MoM::convertLbxToImages(lbx.getRecord(lbxRecordID.lbxIndex), lbx.getRecordSize(lbxRecordID.lbxIndex), m_colorTable, animation, lbxRecordID.lbxTitle + toStr(lbxRecordID.lbxIndex));
    return animation;
}

const QMoMAnimation QMoMResources::getAnimation(MoM::eBuilding building) const
{
    QMoMAnimation animation;
    if (inVectorRange(m_buildingAnimations, building))
    {
        animation = m_buildingAnimations[building];
    }
    return animation;
}

const QMoMAnimation QMoMResources::getAnimation(MoM::eRanged_Type rangedType, int heading) const
{
    QMoMAnimation animation;
    if (MoM::toUInt(rangedType) < ARRAYSIZE(gTableRangedTypes))
    {
        int lbxIndex = gTableRangedTypes[rangedType].lbxIndex;
        lbxIndex = lbxIndex * 8 + heading;
        animation = getAnimation(LBXRecordID(gTableRangedTypes[rangedType].lbxFile.c_str(), lbxIndex));
    }
    return animation;
}

const QMoMAnimation QMoMResources::getAnimation(eTerrainType terrain) const
{
    QMoMAnimation animation;
    if (inVectorRange(m_terrainAnimations, terrain))
    {
        animation = m_terrainAnimations[terrain];
    }
    return animation;
}

const QMoMAnimation QMoMResources::getAnimation(MoM::eUnit_Type unitType, int heading, MoM::eBannerColor bannerColor) const
{
    QMoMAnimation animation;
    if (heading >= 0 && heading < 8)
    {
        int figureIndex = (int)unitType * 8 + heading;
        if (inVectorRange(m_figureAnimations, figureIndex) && m_figureAnimations[figureIndex].size() >= 4)
        {
            animation = m_figureAnimations[figureIndex];
        }

        changeBannerColor(bannerColor, animation);
    }
    else
    {
        if (inVectorRange(m_unitImages, unitType))
        {
            animation.append(m_unitImages[unitType]);
        }
    }

    return animation;
}

const HelpLBXentry* QMoMResources::getHelpEntry(eHelpIndex helpTextNr)
{
    if (0 == m_helpLbx)
    {
        m_helpLbx = QMoMLbxPtr(new MoMLbxBase);
        // Only try to load once and ignore result.
        // We'll check if it's there when we use it.
        if (m_helpLbx->load(m_game->getGameDirectory() + "/" + "HELP.LBX"))
        {
            // TODO: Remove
            //const HelpLBXentry* helpLbxEntries = reinterpret_cast<const HelpLBXentry*>(m_HelpLbx->getRecord(2) + 4);
            //std::cout << "HelpLBXentries" << std::endl;
            //for (size_t i = 0; i < MoM::eHelpIndex_MAX; ++i)
            //{
            //    std::cout << i << "\t" << helpLbxEntries[i].title << "\t" << helpLbxEntries[i].lbxFile << "\t" << helpLbxEntries[i].lbxIndex << std::endl;
            //}
        }
    }

    const HelpLBXentry* helpEntry = (const HelpLBXentry*)m_helpLbx->getSubRecord(2, helpTextNr);

    return helpEntry;
}

std::string QMoMResources::getHelpText(eHelpIndex helpTextNr)
{
    const size_t maxwidth = 60;

    const HelpLBXentry* helpEntry = getHelpEntry(helpTextNr);

    std::string text;
    if (0 != helpEntry)
    {
        const char* title = helpEntry->title;
        const char* description = helpEntry->description;

        text = std::string(title) + ": " + description;
    }

    // Replace code for newline by actual newline
    text = replaceStrInStr(text, "\x14", "\n");

    // Wrap text within maxwidth
    size_t posStart = 0;
    while (posStart + maxwidth < text.length())
    {
        size_t pos;
        for (pos = posStart + maxwidth; (text[pos] != ' ') && (text[pos] != '\n') && (text[pos] != '\x14') && (pos > posStart); --pos)
        {
        }
        if (pos <= posStart)
        {
            break;
        }
        text[pos] = '\n';
        posStart = pos + 1;
    }

    return text;
}

std::string QMoMResources::getHelpText(eBuilding building)
{
    return getHelpText((MoM::eHelpIndex)((int)MoM::HELP_cityscap_TRADE_GOODS - (int)MoM::BUILDING_Trade_Goods + (int)building));
}

std::string QMoMResources::getHelpText(eHeroAbility heroAbility)
{
    std::string value;
    if (toUInt(heroAbility) < ARRAYSIZE(gTableHeroSpecials))
    {
        value = getHelpText(gTableHeroSpecials[heroAbility].helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(eItemPower itemPower)
{
    std::string value;
    if (toUInt(itemPower) < ARRAYSIZE(gTableItemSpecials))
    {
        value = getHelpText(gTableItemSpecials[itemPower].helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(ePortrait wizardPortrait)
{
    return getHelpText((MoM::eHelpIndex)((int)MoM::HELP_MERLIN - (int)PORTRAIT_Merlin + (int)wizardPortrait));
}

std::string QMoMResources::getHelpText(eRace race)
{
    return getHelpText((MoM::eHelpIndex)(MoM::HELP_BARBARIAN_TOWNSFOLK + (int)race)) + "\n"
        + getHelpText((MoM::eHelpIndex)(MoM::HELP_BACKGRND_Barbarian_Farmers + (int)race)) + "\n"
        + getHelpText((MoM::eHelpIndex)(MoM::HELP_BACKGRND_Barbarian_Workers + (int)race)) + "\n"
        + getHelpText((MoM::eHelpIndex)(MoM::HELP_BACKGRND_Barbarian_Rebels + (int)race));

}

std::string QMoMResources::getHelpText(eRanged_Type rangedType)
{
    eHelpIndex helpIndex = eHelpIndex_NONE;

    switch (rangedType)
    {
    case RANGED_None:                   break;

    case RANGED_Rock:                   helpIndex = HELP_UNITVIEW_Rock; break;

    case RANGED_Arrow:
    case RANGED_Bullet:                 helpIndex = HELP_UNITVIEW_Bow; break;

    case RANGED_Chaos_Lightning:
    case RANGED_Chaos_Fireball:
    case RANGED_Sorcery_Illusion:
    case RANGED_Chaos_Deathbolt:
    case RANGED_Nature_Icebolt:
    case RANGED_Nature_Priest:
    case RANGED_Chaos_Drow:
    case RANGED_Nature_Shimmer:
    case RANGED_Nature_Green:           helpIndex = HELP_UNITVIEW_Magical_Attack; break;

    case RANGED_Thrown_Weapons:         helpIndex = HELP_SPECIAL2_THROWN; break;
    case RANGED_Fire_Breath:            helpIndex = HELP_SPECIAL2_FIRE_BREATH; break;
    case RANGED_Lightning_Breath:       helpIndex = HELP_SPECIAL2_LIGHTNING_BREATH; break;
    case RANGED_Stoning_Gaze:           helpIndex = HELP_SPECIAL_STONING_GAZE; break;
    case RANGED_Multiple_Gaze:          helpIndex = HELP_SPECIAL2_DOOM_GAZE; break;
    case RANGED_Death_Gaze:             helpIndex = HELP_SPECIAL2_DEATH_GAZE; break;

    default:                            break;
    }

    std::string value = getHelpText(helpIndex);

    return value;
}

const HelpLBXentry* QMoMResources::getHelpEntry(eSpell spell)
{
    const HelpLBXentry* value = 0;
    if ((spell >= 1) && (spell < eSpell_MAX))
    {
        eHelpIndex helpIndex = (MoM::eHelpIndex)(0 + spell);
        value = getHelpEntry(helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(eSpell spell)
{
    std::string value;
    if ((spell >= 1) && (spell < eSpell_MAX))
    {
        eHelpIndex helpIndex = (MoM::eHelpIndex)(0 + spell);
        value = getHelpText(helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(eUnitAbility unitAbility)
{
    std::string value;
    if (toUInt(unitAbility) < ARRAYSIZE(gTableUnitSpecials))
    {
        value = getHelpText(gTableUnitSpecials[unitAbility].helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(eUnitEnchantment unitEnchantment)
{
    std::string value;
    if (toUInt(unitEnchantment) < ARRAYSIZE(gTableUnitEnchantments))
    {
        value = getHelpText(gTableUnitEnchantments[unitEnchantment].helpIndex);
    }
    return value;
}

std::string QMoMResources::getHelpText(eUnitMutation unitMutation)
{
    std::string value;
    if (toUInt(unitMutation) < ARRAYSIZE(gTableUnitMutations))
    {
        value = getHelpText(gTableUnitMutations[unitMutation].helpIndex);
    }
    return value;
}

const QMoMImagePtr QMoMResources::getImage(const LBXRecordID &lbxRecordID) const
{
    QMoMImagePtr image;
    if (m_game.isNull())
        return image;
    std::string lbxFile = m_game->getGameDirectory() + "/" + lbxRecordID.lbxTitle + ".LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return image;
    image = MoM::convertLbxToImage(lbx.getRecord(lbxRecordID.lbxIndex), lbx.getRecordSize(lbxRecordID.lbxIndex), m_colorTable, lbxRecordID.lbxTitle + toStr(lbxRecordID.lbxIndex));
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eBannerColor bannerColor) const
{
    QMoMImagePtr image;
    unsigned index = 14 + toUInt(bannerColor);
    if (inVectorRange(m_mapBackImages, index))
    {
        image = m_mapBackImages[index];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eBuilding building) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_buildingAnimations, building) && !m_buildingAnimations[building].empty())
    {
        image = m_buildingAnimations[building].at(0);
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eCentralStructure structure) const
{
    static const int lookup[MoM::eCentralStructure_MAX] =
    {
        -1, // CENTRALSTRUCTURE_none,
        18, // CENTRALSTRUCTURE_outpost,
        0,  // CENTRALSTRUCTURE_city_grid,
        17, // CENTRALSTRUCTURE_wizards_fortress,
        21, // CENTRALSTRUCTURE_small_tower,
        20, // CENTRALSTRUCTURE_tower_between_planes,
        19, // CENTRALSTRUCTURE_cave,
        23, // CENTRALSTRUCTURE_temple,
        22, // CENTRALSTRUCTURE_medium_tower,
        66, // CENTRALSTRUCTURE_sorcery_node,   ANIMATION
        120, // CENTRALSTRUCTURE_chaos_node,     ANIMATION - located in another LBX file
        65, // CENTRALSTRUCTURE_nature_node,    ANIMATION
        121,// CENTRALSTRUCTURE_ruins,
    };

    int index = -1;
    if (MoM::toUInt(structure) < ARRAYSIZE(lookup))
    {
        index = lookup[structure];
    }
    QMoMImagePtr image;
    if (CENTRALSTRUCTURE_chaos_node == structure)
    {
        image = getImage(LBXRecordID("CHRIVER", 24));
    }
    else if (inVectorRange(m_cmbtcityImages, index))
    {
        image = m_cmbtcityImages[index];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eCity_Size citySize, MoM::eBannerColor bannerColor) const
{
    QMoMImagePtr image;
    int index = (int)citySize - 1;
    if (index < 0)
    {
        index = 0;  // Treat outpost as hamlet
    }
    if (inVectorRange(m_citySizeImages, index))
    {
        image = m_citySizeImages[index];
    }

    changeBannerColor(bannerColor, image);

    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eCityWall wall, bool broken) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_citywallAnimations, wall) && (m_citywallAnimations[wall].size() >= 2))
    {
        image = m_citywallAnimations[wall].at((int)broken);
    }
    return image;
}

const QMoMImagePtr  QMoMResources::getImage(MoM::eItem_Icon itemIcon) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_itemsImages, itemIcon))
    {
        image = m_itemsImages[itemIcon];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eItemPower itemPower) const
{
    QMoMImagePtr image;
    int lbxIndex = -1;
    std::string lbxFile;
    if (toUInt(itemPower) < ARRAYSIZE(gTableItemSpecials))
    {
        lbxIndex = gTableItemSpecials[itemPower].lbxIndex;
        lbxFile = gTableItemSpecials[itemPower].lbxFile;
    }
    if ((lbxFile == std::string("ITEMISC.LBX")) && inVectorRange(m_itemiscImages, lbxIndex))
    {
        image = m_itemiscImages[lbxIndex];
    }
    else if ((lbxFile == std::string("SPECIAL.LBX")) && inVectorRange(m_specialImages, lbxIndex))
    {
        image = m_specialImages[lbxIndex];
    }
    else if ((lbxFile == std::string("SPECIAL2.LBX")) && inVectorRange(m_specialImages, 120 + lbxIndex))
    {
        image = m_specialImages[120 + lbxIndex];
    }
    return image;
}

const QMoMImagePtr  QMoMResources::getImage(MoM::eTower_Node_Lair_Type lair, bool overland) const
{
    QMoMImagePtr image;
    if (overland)
    {
        if (inVectorRange(m_lairImagesOverland, lair))
        {
            image = m_lairImagesOverland[lair];
        }
    }
    else
    {
        if (inVectorRange(m_lairImagesBig, lair))
        {
            image = m_lairImagesBig[lair];
        }
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eProducing producing) const
{
    QMoMImagePtr image;
    if (toUInt(producing) < eProducing_Building_MAX)
    {
        eBuilding building = (eBuilding)producing;
        image = getImage(building);
    }
    else
    {
        eUnit_Type unitType = (eUnit_Type)(toUInt(producing) - 100);
        image = getImage(unitType);
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(eRace race) const
{
    QMoMImagePtr image(new QImage(QString(":/race/%0.gif").arg(prettyQStr(race))));
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eRandomPickType randomPickType) const
{
    QMoMImagePtr image(new QImage(QString(":/abilities/%0.gif").arg(prettyQStr(randomPickType))));
    return image;
}

const QMoMImagePtr QMoMResources::getImage(eResource resource) const
{
    int lbxIndex = -1;
    switch (resource)
    {
    case RESOURCE_Food:         lbxIndex = 40; break;
    case RESOURCE_Production:   lbxIndex = 41; break;
    case RESOURCE_Gold:         lbxIndex = 42; break;
    case RESOURCE_Power:        lbxIndex = 43; break;
    case RESOURCE_Mana:         lbxIndex = 43; break;
    case RESOURCE_Research:     lbxIndex = 44; break;
    case RESOURCE_Gray_Food:    lbxIndex = 94; break;
    case RESOURCE_Gray_Gold:    lbxIndex = 73; break;
    case RESOURCE_10_Food:      lbxIndex = 88; break;
    case RESOURCE_10_Production:lbxIndex = 89; break;
    case RESOURCE_10_Gold:      lbxIndex = 90; break;
    case RESOURCE_10_Power:     lbxIndex = 91; break;
    case RESOURCE_10_Mana:      lbxIndex = 91; break;
    case RESOURCE_10_Research:  lbxIndex = 92; break;
    case RESOURCE_10_Gray_Food: lbxIndex = 95; break;
    case RESOURCE_10_Gray_Gold: lbxIndex = 93; break;
    default:                    ;
    }
    QMoMImagePtr image;
    if (inVectorRange(m_backgrndImages, lbxIndex))
    {
        image = m_backgrndImages[lbxIndex];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eRanged_Type rangedType, int heading) const
{
    QMoMAnimation animation = getAnimation(rangedType, heading);
    QMoMImagePtr image;
    if (animation.count() > 2)
    {
        image = animation[2];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(eSlot_Type8 slotType) const
{
    return getImage((MoM::eSlot_Type16)slotType);
}

const QMoMImagePtr QMoMResources::getImage(MoM::eSlot_Type16 slotType) const
{
    static const int lookupItemisc[MoM::eSlot_Type16_MAX] =
    {
        -1,
        27, // SLOT16_Sword = 1,
        28, // SLOT16_Bow = 2,
        29, // SLOT16_Sword_Staff_Wand = 3,
        30, // SLOT16_Staff_Wand = 4,
        32, // SLOT16_Armor_Shield = 5,
        31, // SLOT16_Amulet = 6,
    };

    int index = -1;
    if (MoM::toUInt(slotType) < ARRAYSIZE(lookupItemisc))
    {
        index = lookupItemisc[slotType];
    }
    QMoMImagePtr image;
    if (inVectorRange(m_itemiscImages, index))
    {
        image = m_itemiscImages[index];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eSpell spell) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_spellImages, spell))
    {
        image = m_spellImages[spell];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eTerrainBattle terrain) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_terrainBattleImages, terrain))
    {
        image = m_terrainBattleImages[terrain];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eTerrainBonusDeposit bonusDeposit) const
{
    QMoMImagePtr image;
    unsigned index = -1;
    switch (bonusDeposit)
    {
    case DEPOSIT_Iron_Ore:      index = 78; break;
    case DEPOSIT_Coal:          index = 79; break;
    case DEPOSIT_Silver_Ore:    index = 80; break;
    case DEPOSIT_Gold_Ore:      index = 81; break;
    case DEPOSIT_Gems:          index = 82; break;
    case DEPOSIT_Mithril_Ore:   index = 83; break;
    case DEPOSIT_Adamantium_Ore:    index = 84; break;
    case DEPOSIT_Quork:         index = 85; break;
    case DEPOSIT_Crysx:         index = 86; break;
    case DEPOSIT_Wild_Game:     index = 92; break;
    case DEPOSIT_Nightshade:    index = 91; break;
    case DEPOSIT_no_deposit:
    default:                    break;
    }
    if (inVectorRange(m_mapBackImages, index))
    {
        image = m_mapBackImages[index];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eTerrainChange terrainChange, int roadDirection) const
{
    QMoMImagePtr image;
    unsigned index = -1;
    switch (terrainChange)
    {
//    case TERRAINCHANGE_Volcano_owner:   index = 77; break;
    case TERRAINCHANGE_Road:            index = 45 + roadDirection; break;  // 45-53
    case TERRAINCHANGE_Enchanted_Road:  index = 54 + roadDirection; break;  // 54-62
    case TERRAINCHANGE_Corruption:      index = 77; break;
    default:                    break;
    }
    if (inVectorRange(m_mapBackImages, index))
    {
        image = m_mapBackImages[index];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eTerrainType terrain) const
{
    QMoMImagePtr image;
    if (inVectorRange(m_terrainAnimations, terrain)
            && (m_terrainAnimations[terrain].size() > 0)
            && (0 != m_terrainAnimations[terrain][0]))
    {
            image = m_terrainAnimations[terrain][0];
    }
    return image;
}

const QMoMImagePtr QMoMResources::getImage(MoM::eUnit_Type unitType, int heading, MoM::eBannerColor bannerColor) const
{
    QMoMAnimation animation = getAnimation(unitType, heading, bannerColor);
    QMoMImagePtr image;
    if (animation.count() > 1)
    {
        image = animation[1];
    }
    else if (animation.count() > 0)
    {
        image = animation[0];
    }

    return image;
}

const QString QMoMResources::getNameSkill(MoM::eSkill skill) const
{
    if (!inRange(skill, eSkill_MAX))
        return {};

    // First try the game
    const auto* wizardsDS = m_game->getWizardsExeDataSegment();
    // Then try the game directory
    if ((wizardsDS == nullptr) && m_wizardsExe)
    {
        wizardsDS = reinterpret_cast<const MoMDataSegment*>(m_wizardsExe->getDataSegment());
    }

    if (wizardsDS != nullptr)
    {
        // TODO: Validate range and ensure terminating zero
        const char* ptrName = reinterpret_cast<const char*>(wizardsDS) + wizardsDS->m_Offsets_SkillNames[skill];
        return QString(ptrName);
    }

    // Fall back to enum constant as string
    return skill._to_string();
}

const QString QMoMResources::getName(MoM::eSpell spell) const
{
    // First try the game
    const auto* gameSpellData = m_game->getSpellData(spell);
    if (gameSpellData != nullptr)
    {
        return gameSpellData->m_SpellName;
    }
    // Then try the game directory
    MoM::MoMLbxBase lbx = loadLbx(m_game->getGameDirectory(), "SPELLDAT.LBX");
    const auto* lbxSpellData = getLbxSpellData(lbx);
    if (lbxSpellData && inRange(spell, eSpell_MAX))
    {
        return lbxSpellData[spell].m_SpellName;
    }
    // If all fails, fall back to the enum constant as string
    return prettyQStr(spell);
}

void QMoMResources::changeBannerColor(MoM::eBannerColor bannerColor, QMoMAnimation& animation) const
{
    for (int i = 0; i < animation.count(); ++i)
    {
        changeBannerColor(bannerColor, animation[i]);
    }
}

void QMoMResources::changeBannerColor(MoM::eBannerColor bannerColor, QMoMImagePtr& image) const
{
    const int gCOLORS_IN_BANNERS[MoM::eBannerColor_MAX][gCOUNT_BANNER_COLOR] =
    {
        // From dseg:56E8
        {97, 98, 99, 100},
        {66, 67, 68, 69},
        {33, 34, 35, 36},
        {201, 202, 203, 166},
        {160, 161, 162, 163},
        {28, 27, 26, 25},
    };

    if ((BANNER_Green != bannerColor) && (0 != image) && (image->colorCount() >= gFIRST_BANNER_COLOR + gCOUNT_BANNER_COLOR))
    {
        image = QMoMImagePtr(new QImage(*image));
        for (int i = 0; i < gCOUNT_BANNER_COLOR; ++i)
        {
            image->setColor(gFIRST_BANNER_COLOR + i, image->color(gCOLORS_IN_BANNERS[bannerColor][i]));
        }
    }
}

bool QMoMResources::createColorTable()
{
    if (m_game.isNull())
        return false;
    m_colorTable.resize(256);
    std::string fontsLbxFile = m_game->getGameDirectory() + "/" + "FONTS.LBX";
    MoM::MoMLbxBase fontsLbx;
    if (!fontsLbx.load(fontsLbxFile))
        return false;

    uint8_t* dataPalette = fontsLbx.getRecord(2);
    MoM::convertLbxToPalette(dataPalette, m_colorTable);

    return true;
}

void QMoMResources::createBuildingAnimations()
{
    if (m_game.isNull())
        return;
    std::string lbxFile = m_game->getGameDirectory() + "/" + "CITYSCAP.LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return;
    m_buildingAnimations.resize(lbx.getNrRecords());
    for (MoM::eBuilding building = (MoM::eBuilding)1; building < MoM::eBuilding_array_MAX; MoM::inc(building))
    {
        size_t recordNr = 45 + building - MoM::BUILDING_Barracks;
        if (MoM::BUILDING_Trade_Goods == building)
        {
            recordNr = 41;
        }
        else if (MoM::BUILDING_Housing == building)
        {
            recordNr = 42;
        }
        else if (MoM::BUILDING_Foresters_Guild == building)
        {
            recordNr = 78;
        }
        else if (MoM::BUILDING_City_Walls == building)
        {
            recordNr = 114;
        }
        else if (building > MoM::BUILDING_Foresters_Guild)
        {
            recordNr--;
        }
        else
        {
        }

        MoM::convertLbxToImages(lbx.getRecord(recordNr), lbx.getRecordSize(recordNr), m_colorTable, m_buildingAnimations[building], toStr(building));
    }
}

void QMoMResources::createCitySizeImages()
{
    if (m_game.isNull())
        return;
    std::string lbxFile = m_game->getGameDirectory() + "/" + "MAPBACK.LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return;
    QMoMAnimation citySizeAnimation;
    MoM::convertLbxToImages(lbx.getRecord(20), lbx.getRecordSize(20), m_colorTable, citySizeAnimation, "city sizes");
    m_citySizeImages.resize(citySizeAnimation.count());
    for (int i = 0; i < citySizeAnimation.count(); ++i)
    {
        m_citySizeImages[i] = citySizeAnimation[i];
    }
}

void QMoMResources::createFigureAnimations()
{
    int firstUnused = 0;
    // Reserve fir 16 files with max 120 images per file
    int sizeReserved = 16 * 120;
    m_figureAnimations.resize(sizeReserved);
    for (int fileNr = 1; fileNr <= 16; ++fileNr)
    {
        std::string lbxTitle;
        if (fileNr < 10)
        {
            lbxTitle = "FIGURES" + toStr(fileNr);
        }
        else
        {
            lbxTitle = "FIGURE" + toStr(fileNr);
        }
        QVector<QMoMAnimation> tmpAnimations;
        (void)createLbxAnimations(lbxTitle, tmpAnimations);
        for(int i = 0; (i < tmpAnimations.size()) && (firstUnused + i < sizeReserved); ++i)
        {
            m_figureAnimations[firstUnused + i] = tmpAnimations[i];
        }
        firstUnused += tmpAnimations.size();
    }
    m_figureAnimations.resize(firstUnused);
}

void QMoMResources::createLairImages()
{
    int mapBackIndices[eTower_Node_Lair_Type_MAX] =
    {
        //TODO: 70 for owned tower
        69, // LAIRTYPE_Tower = 0,             // random color if a book is awarded
        -1, // LAIRTYPE_Chaos_node = 1,
        -1, // LAIRTYPE_Nature_node = 2,
        -1, // LAIRTYPE_Sorcery_node = 3,
        71, // LAIRTYPE_Mysterious_cave = 4,   // random color if a book is awarded
        74, // LAIRTYPE_Dungeon = 5,           // death book if a book is awarded
        72, // LAIRTYPE_Ancient_temple = 6,    // life book if a book is awarded
        73, // LAIRTYPE_Abandoned_keep = 7,    // death book if a book is awarded
        71, // LAIRTYPE_Monster_lair = 8,      // random color if a book is awarded
        74, // LAIRTYPE_Ruins = 9,             // death book if a book is awarded
        75, // LAIRTYPE_Fallen_temple = 10,    // life book if a book is awarded
    };

    if (m_game.isNull())
        return;
    std::string lairsLbxFile = m_game->getGameDirectory() + "/" + "RELOAD.LBX";
    MoM::MoMLbxBase lairsLbx;
    if (!lairsLbx.load(lairsLbxFile))
        return;
    m_lairImagesBig.resize(MoM::eTower_Node_Lair_Type_MAX);
    m_lairImagesOverland.resize(MoM::eTower_Node_Lair_Type_MAX);
    for (size_t i = 0; i < MoM::eTower_Node_Lair_Type_MAX; ++i)
    {
        m_lairImagesBig[i] = MoM::convertLbxToImage(lairsLbx.getRecord(9 + i), lairsLbx.getRecordSize(9 + i), m_colorTable, toStr((MoM::eTower_Node_Lair_Type)i));
        int index = mapBackIndices[i];
        if (inVectorRange(m_mapBackImages, index))
        {
            m_lairImagesOverland[i] = m_mapBackImages[index];
        }
    }
}

void QMoMResources::createLbxAnimations(const std::string& lbxTitle, QVector<QMoMAnimation>& vecAnimations)
{
    if (m_game.isNull())
        return;
    std::string lbxFile = m_game->getGameDirectory() + "/" + lbxTitle + ".LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return;
    vecAnimations.resize(lbx.getNrRecords());
    for (size_t i = 0; i < lbx.getNrRecords(); ++i)
    {
        (void)MoM::convertLbxToImages(lbx.getRecord(i), lbx.getRecordSize(i), m_colorTable, vecAnimations[i], lbxTitle + toStr(i));
    }
}

void QMoMResources::createLbxImages(const std::string& lbxTitle, QVector<QMoMImagePtr>& vecImages)
{
    if (m_game.isNull())
        return;
    std::string lbxFile = m_game->getGameDirectory() + "/" + lbxTitle + ".LBX";
    MoM::MoMLbxBase lbx;
    if (!lbx.load(lbxFile))
        return;
    vecImages.resize(lbx.getNrRecords());
    for (size_t i = 0; i < lbx.getNrRecords(); ++i)
    {
        vecImages[i] = MoM::convertLbxToImage(lbx.getRecord(i), lbx.getRecordSize(i), m_colorTable, lbxTitle + toStr(i));
    }
}

void QMoMResources::createSpellImages()
{
    // PRE: UnitImages should already have been created, because SpellImages uses them

    if (m_game.isNull())
        return;
    std::string cityscapLbxFile = m_game->getGameDirectory() + "/" + "CITYSCAP.LBX";
    std::string compixLbxFile = m_game->getGameDirectory() + "/" + "COMPIX.LBX";
    std::string monsterLbxFile = m_game->getGameDirectory() + "/" + "MONSTER.LBX";
    std::string specfxLbxFile = m_game->getGameDirectory() + "/" + "SPECFX.LBX";
    MoM::MoMLbxBase cityscapLbx;
    MoM::MoMLbxBase compixLbx;
    MoM::MoMLbxBase monsterLbx;
    MoM::MoMLbxBase specfxLbx;
    if (!cityscapLbx.load(cityscapLbxFile))
        return;
    if (!compixLbx.load(compixLbxFile))
        return;
    if (!monsterLbx.load(monsterLbxFile))
        return;
    if (!specfxLbx.load(specfxLbxFile))
        return;

    m_spellImages.resize(MoM::eSpell_MAX);

    // MONSTER.LBX, UNITS1.LBX, UNITS2.LBX
    for (MoM::eSpell spell = (MoM::eSpell)0; spell < MoM::eSpell_MAX; MoM::inc(spell))
    {
        const HelpLBXentry* helpEntry = getHelpEntry(spell);
        if (0 != helpEntry)
        {
            if (std::string(helpEntry->lbxFile) == std::string("SPECIAL.LBX"))
            {
                if (inVectorRange(m_specialImages, helpEntry->lbxIndex))
                {
                    m_spellImages[spell] = m_specialImages[helpEntry->lbxIndex];
                }
            }
            else if (std::string(helpEntry->lbxFile) == std::string("SPECIAL2.LBX"))
            {
                if (inVectorRange(m_specialImages, 120 + helpEntry->lbxIndex))
                {
                    m_spellImages[spell] = m_specialImages[120 + helpEntry->lbxIndex];
                }
            }
        }

        MoM::Spell_Data* spellData = m_game->getSpellData(spell);
        if ((0 != spellData) && (MoM::SPELLCATEGORY_Normal_summon == spellData->m_Spell_Category))
        {
            if (inVectorRange(m_unitImages, spellData->m_Parameters[0]))
            {
                m_spellImages[spell] = m_unitImages[ spellData->m_Parameters[0] ];
            }
        }
    }

    MoM::eSpell spell;

    // CITYSCAP.LBX
    spell = MoM::SPELL_Summoning_Circle;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(6), cityscapLbx.getRecordSize(6), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Altar_of_Battle;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(12), cityscapLbx.getRecordSize(12), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Dark_Rituals;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(81), cityscapLbx.getRecordSize(81), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Earth_Gate;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(83), cityscapLbx.getRecordSize(83), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Stream_of_Life;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(84), cityscapLbx.getRecordSize(84), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Astral_Gate;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(85), cityscapLbx.getRecordSize(85), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Spell_Ward;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(96), cityscapLbx.getRecordSize(96), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Inspirations;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(100), cityscapLbx.getRecordSize(100), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Prosperity;
    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(101), cityscapLbx.getRecordSize(101), m_colorTable, toStr(spell));
//    spell = MoM::SPELL_; // Mana cross
//    m_spellImages[spell] = MoM::convertLbxToImage(cityscapLbx.getRecord(102), cityscapLbx.getRecordSize(102), m_colorTable, toStr(spell));

    // MONSTER.LBX
    spell = MoM::SPELL_Summon_Hero;
    m_spellImages[spell] = MoM::convertLbxToImage(monsterLbx.getRecord(45), monsterLbx.getRecordSize(45), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Summon_Champion;
    m_spellImages[spell] = MoM::convertLbxToImage(monsterLbx.getRecord(44), monsterLbx.getRecordSize(44), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Enchant_Item;
    m_spellImages[spell] = MoM::convertLbxToImage(monsterLbx.getRecord(46), monsterLbx.getRecordSize(46), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Create_Artifact;
    m_spellImages[spell] = MoM::convertLbxToImage(monsterLbx.getRecord(46), monsterLbx.getRecordSize(46), m_colorTable, toStr(spell));

    // SPECFX.LBX
    spell = MoM::SPELL_Eternal_Night;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(15), specfxLbx.getRecordSize(15), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Evil_Omens;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(16), specfxLbx.getRecordSize(16), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Zombie_Mastery;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(17), specfxLbx.getRecordSize(17), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Aura_of_Majesty;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(18), specfxLbx.getRecordSize(18), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Wind_Mastery;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(19), specfxLbx.getRecordSize(19), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Suppress_Magic;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(20), specfxLbx.getRecordSize(20), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Time_Stop;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(21), specfxLbx.getRecordSize(21), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Nature_Awareness;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(22), specfxLbx.getRecordSize(22), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Natures_Wrath;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(23), specfxLbx.getRecordSize(23), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Herb_Mastery;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(24), specfxLbx.getRecordSize(24), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Chaos_Surge;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(25), specfxLbx.getRecordSize(25), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Doom_Mastery;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(26), specfxLbx.getRecordSize(26), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Great_Wasting;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(27), specfxLbx.getRecordSize(27), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Meteor_Storm;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(28), specfxLbx.getRecordSize(28), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Armageddon;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(29), specfxLbx.getRecordSize(29), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Tranquility;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(30), specfxLbx.getRecordSize(30), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Life_Force;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(31), specfxLbx.getRecordSize(31), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Crusade;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(32), specfxLbx.getRecordSize(32), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Just_Cause;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(33), specfxLbx.getRecordSize(33), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Holy_Arms;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(34), specfxLbx.getRecordSize(34), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Planar_Seal;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(35), specfxLbx.getRecordSize(35), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Charm_of_Life;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(36), specfxLbx.getRecordSize(36), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Detect_Magic;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(37), specfxLbx.getRecordSize(37), m_colorTable, toStr(spell));

    spell = MoM::SPELL_Death_Wish;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(38), specfxLbx.getRecordSize(38), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Great_Unsummoning;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(39), specfxLbx.getRecordSize(39), m_colorTable, toStr(spell));
    spell = MoM::SPELL_Awareness;
    m_spellImages[spell] = MoM::convertLbxToImage(specfxLbx.getRecord(56), specfxLbx.getRecordSize(56), m_colorTable, toStr(spell));

    for (int entryNr = 0; toUInt(entryNr) < ARRAYSIZE(gTableAdditionalSpells); ++entryNr)
    {
        eSpell spell = gTableAdditionalSpells[entryNr].ability;
        if (0 != m_spellImages[spell])
        {
//            qDebug() << "Spell image for " << prettyQStr(spell) << " already present. Skipping";
            continue;
        }
        std::string lbxFile = gTableAdditionalSpells[entryNr].lbxFile;
        int lbxIndex = gTableAdditionalSpells[entryNr].lbxIndex;
        if (lbxFile == std::string("COMPIX.LBX"))
        {
            m_spellImages[spell] = convertLbxToImage(compixLbx.getRecord(lbxIndex),
                                                     compixLbx.getRecordSize(lbxIndex),
                                                     m_colorTable, toStr(spell));
        }
        else if (lbxFile == std::string("SPECIAL.LBX"))
        {
            if (inVectorRange(m_specialImages, lbxIndex))
            {
                m_spellImages[spell] = m_specialImages[lbxIndex];
            }
        }
        else if (lbxFile == std::string("SPECIAL2.LBX"))
        {
            if (inVectorRange(m_specialImages, 120 + lbxIndex))
            {
                m_spellImages[spell] = m_specialImages[120 + lbxIndex];
            }
        }
    }
}

void QMoMResources::createTerrainImages()
{
    if (m_game.isNull())
        return;
    std::string terrainLbxFile = m_game->getGameDirectory() + "/" + "TERRAIN.LBX";
    MoM::MoMLbxBase terrainLbx;
    if (!terrainLbx.load(terrainLbxFile))
        return;
    // TODO: Why does the bitmap data in TERRAIN.LBX start 192 bytes later
    uint8_t* data = terrainLbx.getRecord(0) + 192;
    uint8_t* ptrLbx1 = terrainLbx.getRecord(1);
    m_terrainAnimations.resize(2 * MoM::eTerrainType_MAX);
//    qDebug() << "terrainNr" << "i" << "loByte" << "hiByte" << "terrainPicIndex" << "offset" << "realOffset";
    for (size_t terrainNr = 0; terrainNr < (int)MoM::eTerrainType_MAX; ++terrainNr)
    {
        // TODO: How do I know that the bitmap data in TERRAIN.LBX has actually
        //       fixed-size subrecords of 0x0180 bytes each?
        // TODO: How do I know that these records contain uncompressed data?

        // Arcanum
        uint16_t loByte = ptrLbx1[2 * terrainNr];
        uint16_t hiByte = ptrLbx1[2 * terrainNr + 1];
        size_t sizeImage = 0x180;
        int nrImages = 1;
        if (0 != (loByte & 0x80))
        {
            nrImages = 4;  // Animation of 4 frames
        }
        loByte &= 0x7F;
        unsigned terrainPicIndex = hiByte;
        unsigned offset = 0x4000 * loByte + sizeImage * hiByte - 2 * sizeImage;

        m_terrainAnimations[terrainNr].resize(nrImages);
        for (int imageNr = 0; imageNr < nrImages; ++imageNr)
        {
            m_terrainAnimations[terrainNr][imageNr] = MoM::convertLbxToImage(data + offset + imageNr * sizeImage, terrainLbx.getRecordSize(0), m_colorTable, toStr(terrainNr) + "-" + toStr(terrainPicIndex));
        }

        // Myrror
        loByte = ptrLbx1[2 * (762 + terrainNr)];
        hiByte = ptrLbx1[2 * (762 + terrainNr) + 1];
        sizeImage = 0x180;
        nrImages = 1;
        if (0 != (loByte & 0x80))
        {
            nrImages = 4;  // Animation of 4 frames
        }
        loByte &= 0x7F;
        terrainPicIndex = hiByte;
        offset = 0x4000 * loByte + sizeImage * hiByte - 2 * sizeImage;

        m_terrainAnimations[MoM::eTerrainType_MAX + terrainNr].resize(nrImages);
        for (int imageNr = 0; imageNr < nrImages; ++imageNr)
        {
            m_terrainAnimations[MoM::eTerrainType_MAX + terrainNr][imageNr] = MoM::convertLbxToImage(data + offset + imageNr * sizeImage, terrainLbx.getRecordSize(0), m_colorTable, toStr(terrainNr) + "-" + toStr(terrainPicIndex));
        }
    }
}

void QMoMResources::createUnitImages()
{
    if (m_game.isNull())
        return;
    std::string units1LbxFile = m_game->getGameDirectory() + "/" + "UNITS1.LBX";
    std::string units2LbxFile = m_game->getGameDirectory() + "/" + "UNITS2.LBX";
    std::string monsterLbxFile = m_game->getGameDirectory() + "/" + "MONSTER.LBX";
    MoM::MoMLbxBase units1Lbx;
    MoM::MoMLbxBase units2Lbx;
    MoM::MoMLbxBase monsterLbx;
    if (!units1Lbx.load(units1LbxFile))
        return;
    if (!units2Lbx.load(units2LbxFile))
        return;
    if (!monsterLbx.load(monsterLbxFile))
        return;

    m_unitImages.resize(MoM::eUnit_Type_MAX);

    // Put specific hero pictures in, since the unit-type pictures are only small horses
    m_unitImages[UNITTYPE_Dwarf] = QMoMImagePtr(new QImage(":/units/Dwarf.gif"));
    m_unitImages[UNITTYPE_Barbarian] = QMoMImagePtr(new QImage(":/units/Barbarian.gif"));
    m_unitImages[UNITTYPE_Sage] = QMoMImagePtr(new QImage(":/units/Sage.gif"));
    m_unitImages[UNITTYPE_Dervish] = QMoMImagePtr(new QImage(":/units/Dervish.gif"));
    m_unitImages[UNITTYPE_Beastmaster] = QMoMImagePtr(new QImage(":/units/Beastmaster.gif"));
    m_unitImages[UNITTYPE_Bard] = QMoMImagePtr(new QImage(":/units/Bard.gif"));
    m_unitImages[UNITTYPE_Orc_Warrior] = QMoMImagePtr(new QImage(":/units/Orc Warrior.gif"));
    m_unitImages[UNITTYPE_Healer] = QMoMImagePtr(new QImage(":/units/Healer.gif"));
    m_unitImages[UNITTYPE_Huntress] = QMoMImagePtr(new QImage(":/units/Huntress.gif"));
    m_unitImages[UNITTYPE_Thief] = QMoMImagePtr(new QImage(":/units/Thief.gif"));
    m_unitImages[UNITTYPE_Druid] = QMoMImagePtr(new QImage(":/units/Druid.gif"));
    m_unitImages[UNITTYPE_War_Monk] = QMoMImagePtr(new QImage(":/units/War Monk.gif"));
    m_unitImages[UNITTYPE_Warrior_Mage] = QMoMImagePtr(new QImage(":/units/Warrior Mage.gif"));
    m_unitImages[UNITTYPE_Magician] = QMoMImagePtr(new QImage(":/units/Magician.gif"));
    m_unitImages[UNITTYPE_Assassin] = QMoMImagePtr(new QImage(":/units/Assassin.gif"));
    m_unitImages[UNITTYPE_Wind_Mage] = QMoMImagePtr(new QImage(":/units/Wind Mage.gif"));
    m_unitImages[UNITTYPE_Ranger] = QMoMImagePtr(new QImage(":/units/Ranger.gif"));
    m_unitImages[UNITTYPE_Draconian] = QMoMImagePtr(new QImage(":/units/Draconian.gif"));
    m_unitImages[UNITTYPE_Witch] = QMoMImagePtr(new QImage(":/units/Witch.gif"));
    m_unitImages[UNITTYPE_Golden_One] = QMoMImagePtr(new QImage(":/units/Golden One.gif"));
    m_unitImages[UNITTYPE_Ninja] = QMoMImagePtr(new QImage(":/units/Ninja.gif"));
    m_unitImages[UNITTYPE_Rogue] = QMoMImagePtr(new QImage(":/units/Rogue.gif"));
    m_unitImages[UNITTYPE_Amazon] = QMoMImagePtr(new QImage(":/units/Amazon.gif"));
    m_unitImages[UNITTYPE_Warlock] = QMoMImagePtr(new QImage(":/units/Warlock.gif"));
    m_unitImages[UNITTYPE_Unknown] = QMoMImagePtr(new QImage(":/units/Unknown.gif"));
    m_unitImages[UNITTYPE_Illusionist] = QMoMImagePtr(new QImage(":/units/Illusionist.gif"));
    m_unitImages[UNITTYPE_Swordsman] = QMoMImagePtr(new QImage(":/units/Swordsman.gif"));
    m_unitImages[UNITTYPE_Priestess] = QMoMImagePtr(new QImage(":/units/Priestess.gif"));
    m_unitImages[UNITTYPE_Paladin] = QMoMImagePtr(new QImage(":/units/Paladin.gif"));
    m_unitImages[UNITTYPE_Black_Knight] = QMoMImagePtr(new QImage(":/units/Black Knight.gif"));
    m_unitImages[UNITTYPE_Elven_Archer] = QMoMImagePtr(new QImage(":/units/Elven Archer.gif"));
    m_unitImages[UNITTYPE_Knight] = QMoMImagePtr(new QImage(":/units/Knight.gif"));
    m_unitImages[UNITTYPE_Necromancer] = QMoMImagePtr(new QImage(":/units/Necromancer.gif"));
    m_unitImages[UNITTYPE_Chaos_Warrior] = QMoMImagePtr(new QImage(":/units/Chaos Warrior.gif"));
    m_unitImages[UNITTYPE_Chosen] = QMoMImagePtr(new QImage(":/units/Chosen.gif"));

    for (MoM::eUnit_Type unitType = (MoM::eUnit_Type)MoM::gMAX_HERO_TYPES; unitType < MoM::eUnit_Type_MAX; MoM::inc(unitType))
    {
        if (unitType < 120)
        {
            m_unitImages[unitType] = MoM::convertLbxToImage(units1Lbx.getRecord(unitType), units1Lbx.getRecordSize(unitType), m_colorTable, toStr(unitType));
        }
        else
        {
            m_unitImages[unitType] = MoM::convertLbxToImage(units2Lbx.getRecord(unitType - 120), units2Lbx.getRecordSize(unitType - 120), m_colorTable, toStr(unitType));
        }
    }
    for (size_t i = 0; i < monsterLbx.getNrRecords(); ++i)
    {
        MoM::eUnit_Type unitType = (MoM::eUnit_Type)(MoM::UNITTYPE_Arcane_Magic_Spirit + i);
        if (unitType >= MoM::eUnit_Type_MAX)
            break;
        if ((MoM::UNITTYPE_Red_Fire_Elemental != unitType) && (MoM::UNITTYPE_Blue_Floating_Island != unitType)
                && (MoM::UNITTYPE_Blue_Phantom_Warriors != unitType) && (MoM::UNITTYPE_Blue_Phantom_Beast != unitType)
                && (MoM::UNITTYPE_Blue_Air_Elemental != unitType))
        {
            m_unitImages[unitType] = MoM::convertLbxToImage(monsterLbx.getRecord(i), monsterLbx.getRecordSize(i), m_colorTable, toStr(unitType));
        }
    }
}

}
