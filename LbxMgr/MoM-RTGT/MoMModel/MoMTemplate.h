// ---------------------------------------------------------------------------
// Copyright:   2011-2020 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file
/// \brief      Defines enums and structs for mapping memory of Master of Magic

#ifndef MOMTEMPLATE_H
#define MOMTEMPLATE_H

#include "MoMCommon.h"

// Better enums
#include <enum.h>

static_assert(__cplusplus >= 201103L, "c++11 required");

// Specify to pack structures
#if defined(_MSC_VER)       // Compiler MS Visual Studio
  #pragma pack(push, 1)
  #define PACKED_STRUCT
#elif defined(__linux__)    // Compiler Linux g++
  #define PACKED_STRUCT __attribute__((packed))
#elif defined(__GNUC__)     // Compiler Linux g++ (use g++ -fshort-enums)
  #pragma pack(push, 1)
  #define PACKED_STRUCT __attribute__((packed))
#elif defined(SWIG)         // Swig wrapper generator
  #define PACKED_STRUCT
#endif

// Specify to use short enums
#if __cplusplus >= 201103L  // C++11 or later
  #define ENUMSIZE8       : uint8_t
  #define ENUMSIZE16      : uint16_t
  #define PACKED_ENUM
#elif defined(_MSC_VER)     // @deprecated Compiler MS Visual Studio
  #pragma warning( once : 4480 )  // nonstandard extension used: specifying underlying type for enum
  #define ENUMSIZE8       : uint8_t
  #define ENUMSIZE16      : uint16_t
  #define PACKED_ENUM
#elif defined(__GNUC__)     // @deprecated Compiler Linux g++ (use g++ -fshort-enums)
  #define ENUMSIZE8
  #define ENUMSIZE16
  #define PACKED_ENUM __attribute__((packed))
#elif defined(SWIG)         // Swig wrapper generator
  #define ENUMSIZE8
  #define ENUMSIZE16
  #define PACKED_ENUM
#endif

namespace MoM {

//
// ENUMS
//

enum eBannerColor ENUMSIZE16
{
    BANNER_Blue = 0,
    BANNER_Green = 1,
    BANNER_Purple = 2,
    BANNER_Red = 3,
    BANNER_Yellow = 4,
    BANNER_Brown = 5,

    eBannerColor_MAX,
    eBannerColor__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eBattleEnchantment
{
    BATTLEENCHANTMENT_True_Light,           // 00-01    1=True Light (Battle), 2=Heavenly Light (City) / Cloud of Shadow (City), 3=Eternal Darkness (Global)
    BATTLEENCHANTMENT_Darkness,             // 02-03
    BATTLEENCHANTMENT_Warp_Reality,         // 04-05
    BATTLEENCHANTMENT_Black_Prayer,         // 06-07
    BATTLEENCHANTMENT_Wrack,                // 08-09
    BATTLEENCHANTMENT_Metal_Fires,          // 0A-0B
    BATTLEENCHANTMENT_Prayer,               // 0C-0D
    BATTLEENCHANTMENT_High_Prayer,          // 0E-0F
    BATTLEENCHANTMENT_Terror,               // 10-11
    BATTLEENCHANTMENT_Call_Lightning,       // 12-13
    BATTLEENCHANTMENT_Counter_Magic,        // 14-15
    BATTLEENCHANTMENT_Mass_Invisibility,    // 16-17
    BATTLEENCHANTMENT_Entangle,             // 18-19
    BATTLEENCHANTMENT_Mana_Leak,            // 1A-1B
    BATTLEENCHANTMENT_Blur,                 // 1C-1D

    eBattleEnchantment_MAX
};

enum eBattleUnitActive ENUMSIZE8
{
    BATTLEUNITACTIVE_alive = 0,
    BATTLEUNITACTIVE_recalled = 1,
    BATTLEUNITACTIVE_fleeing = 2,
    BATTLEUNITACTIVE_not_involved = 3,      // floating island, non-flying ship
    BATTLEUNITACTIVE_dead = 4,
    BATTLEUNITACTIVE_undeaded = 5,
    BATTLEUNITACTIVE_removed = 6,           // stoned, destroyed, dispelled, cracks call, combat summoning

    eBattleUnitActive_MAX,
    eBattleUnitActive_SIZE__ = 0xFF
} PACKED_ENUM;

enum eBattleUnitTactic ENUMSIZE16
{
    TACTIC_ready = 0,

    TACTIC_done = 4,
    TACTIC_wait = 5,

    TACTIC_melee = 100,
    TACTIC_shoot = 101,
    TACTIC_unclear_102 = 102,
    TACTIC_unclear_103 = 103,
    TACTIC_doom_bolt = 104,
    TACTIC_fireball = 105,
    TACTIC_healing = 106,
    TACTIC_cast_item_spell = 107,
    TACTIC_cast_spell = 108,
    TACTIC_summon_demon = 109,
    TACTIC_web = 110,

    TACTIC_flee_150 = 150,
    TACTIC_unset_333 = 333,

    eBattleUnitTactic_MAX,
    eBattleUnitTactic_SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eBattleCondition ENUMSIZE16
{
    BATTLECONDITION_other = 0,
    BATTLECONDITION_strategic_city_combat = 1,
    BATTLECONDITION_sorcery_node = 2,
    BATTLECONDITION_nature_node = 3,
    BATTLECONDITION_chaos_node = 4,
    BATTLECONDITION_sea = 6,

    eBattleCondition_MAX,
    eBattleCondition__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eBuilding ENUMSIZE16
{
    BUILDING_Not_applicable = 0xFFFF,

    BUILDING_None = 0,                      // 0

    BUILDING_Trade_Goods,                   // 1
    BUILDING_Housing,                       // 2

    BUILDING_Barracks,                      // 3
    BUILDING_Armory,
    BUILDING_Fighters_Guild,
    BUILDING_Armorers_Guild,
    BUILDING_War_College,
    BUILDING_Smithy,
    BUILDING_Stable,
    BUILDING_Animists_Guild,                // 10
    BUILDING_Fantastic_Stable,
    BUILDING_Shipwright_Guild,
    BUILDING_Ship_Yard,
    BUILDING_Maritime_Guild,
    BUILDING_Sawmill,
    BUILDING_Library,
    BUILDING_Sages_Guild,
    BUILDING_Oracle,
    BUILDING_Alchemist_Guild,
    BUILDING_University,                    // 20
    BUILDING_Wizards_Guild,
    BUILDING_Shrine,
    BUILDING_Temple,
    BUILDING_Parthenon,
    BUILDING_Cathedral,
    BUILDING_Marketplace,
    BUILDING_Bank,
    BUILDING_Merchants_Guild,
    BUILDING_Granary,
    BUILDING_Farmers_Market,                // 30
    BUILDING_Foresters_Guild,
    BUILDING_Builders_Hall,
    BUILDING_Mechanicians_Guild,
    BUILDING_Miners_Guild,
    BUILDING_City_Walls,                    // 35

    eBuilding_array_MAX,                    // 36

    BUILDING_Forest = 101,                  // 101 prerequisite for sawmill
    BUILDING_Grass = 104,                   // 104 prerequisite UNUSED
    BUILDING_Water = 110,                   // 110 prerequisite for ship's wright
    BUILDING_Hill = 200,                    // 200 prerequisite for miner's guild

    eBuilding_extra_MAX,                    // 201
    eBuilding__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eBuilding8 ENUMSIZE8
{
    BUILDING8_Not_applicable = 0xFF,

    BUILDING8_None = 0,                     // 0

    BUILDING8_Trade_Goods,                  // 1
    BUILDING8_Housing,                      // 2

    BUILDING8_Barracks,                     // 3
    BUILDING8_Armory,
    BUILDING8_Fighters_Guild,
    BUILDING8_Armorers_Guild,
    BUILDING8_War_College,
    BUILDING8_Smithy,
    BUILDING8_Stable,
    BUILDING8_Animists_Guild,               // 10
    BUILDING8_Fantastic_Stable,
    BUILDING8_Shipwright_Guild,
    BUILDING8_Ship_Yard,
    BUILDING8_Maritime_Guild,
    BUILDING8_Sawmill,
    BUILDING8_Library,
    BUILDING8_Sages_Guild,
    BUILDING8_Oracle,
    BUILDING8_Alchemist_Guild,
    BUILDING8_University,                   // 20
    BUILDING8_Wizards_Guild,
    BUILDING8_Shrine,
    BUILDING8_Temple,
    BUILDING8_Parthenon,
    BUILDING8_Cathedral,
    BUILDING8_Marketplace,
    BUILDING8_Bank,
    BUILDING8_Merchants_Guild,
    BUILDING8_Granary,
    BUILDING8_Farmers_Market,               // 30
    BUILDING8_Foresters_Guild,
    BUILDING8_Builders_Hall,
    BUILDING8_Mechanicians_Guild,
    BUILDING8_Miners_Guild,
    BUILDING8_City_Walls,                   // 35

    eBuilding8_array_MAX,                   // 36,

    BUILDING8_Forest = 101,                 // 101 prerequisite for sawmill
    BUILDING8_Grass = 104,                  // 104 prerequisite UNUSED
    BUILDING8_Water = 110,                  // 110 prerequisite for ship's wright
    BUILDING8_Hill = 200,                   // 200 prerequisite for miner's guild

    eBuilding8_extra_MAX                    // 201
} PACKED_ENUM;

enum eBuildingCategory ENUMSIZE16
{
    BUILDINGCATEGORY_trade_housing,
    BUILDINGCATEGORY_monetary,
    BUILDINGCATEGORY_religious,
    BUILDINGCATEGORY_research,
    BUILDINGCATEGORY_military,
    BUILDINGCATEGORY_productive,
    BUILDINGCATEGORY_food,
    BUILDINGCATEGORY_ships,

    eBuildingCategory_MAX,
    eBuildingCategory__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eBuildingStatus ENUMSIZE8
{
    BUILDINGSTATUS_Not_Built = 0xFF,
    BUILDINGSTATUS_Replaced = 0,
    BUILDINGSTATUS_Built = 1,
    BUILDINGSTATUS_CoM_Destroyed = 0xFE,
    BUILDINGSTATUS_Destroyed = 2,

    eBuildingStatus_MAX,
    eBuildingStatus__SIZE__ = 0xFF
} PACKED_ENUM;

enum eCentralStructure ENUMSIZE16
{
    CENTRALSTRUCTURE_none = 0,
    CENTRALSTRUCTURE_outpost = 1,
    CENTRALSTRUCTURE_city_grid = 2,
    CENTRALSTRUCTURE_wizards_fortress = 3,
    CENTRALSTRUCTURE_small_tower = 4,
    CENTRALSTRUCTURE_tower_between_planes = 5,
    CENTRALSTRUCTURE_cave = 6,
    CENTRALSTRUCTURE_temple = 7,
    CENTRALSTRUCTURE_medium_tower = 8,
    CENTRALSTRUCTURE_sorcery_node = 9,
    CENTRALSTRUCTURE_chaos_node = 10,
    CENTRALSTRUCTURE_nature_node = 11,
    CENTRALSTRUCTURE_ruins = 12,

    eCentralStructure_MAX,
    eCentralStructure__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eCityEnchantments
{
    CITYENCH_Wall_of_Fire, // 40 Offset EXE:2B3F8
    CITYENCH_Chaos_Rift, // 42 Offset EXE:2B3FA
    CITYENCH_Dark_Rituals, // 44 Offset EXE:2B3FC
    CITYENCH_Evil_Presence, // 46 Offset EXE:2B3FE
    CITYENCH_Cursed_Lands, // 48 Offset EXE:2B400
    CITYENCH_Pestilence, // 4A Offset EXE:2B402
    CITYENCH_Cloud_of_Shadow, // 4C Offset EXE:2B404
    CITYENCH_Famine, // 4E Offset EXE:2B406
    CITYENCH_Flying_Fortress, // 50 Offset EXE:2B408
    CITYENCH_Nature_Ward, // 52 Offset EXE:2B40A
    CITYENCH_Sorcery_Ward, // 54 Offset EXE:2B40C
    CITYENCH_Chaos_Ward, // 56 Offset EXE:2B40E
    CITYENCH_Life_Ward, // 58 Offset EXE:2B410
    CITYENCH_Death_Ward, // 5A Offset EXE:2B412
    CITYENCH_Natures_Eye, // 5C Offset EXE:2B414
    CITYENCH_Earth_Gate, // 5E Offset EXE:2B416
    CITYENCH_Stream_of_Life, // 60 Offset EXE:2B418
    CITYENCH_Gaias_Blessing, // 62 Offset EXE:2B41A
    CITYENCH_Inspirations, // 64 Offset EXE:2B41C
    CITYENCH_Prosperity, // 66 Offset EXE:2B41E
    CITYENCH_Astral_Gate, // 68 Offset EXE:2B420
    CITYENCH_Heavenly_Light, // 6A Offset EXE:2B422
    CITYENCH_Consecration, // 6C Offset EXE:2B424
    CITYENCH_Wall_of_Darkness, // 6E Offset EXE:2B426
    CITYENCH_Altar_of_Battle, // 70 Offset EXE:2B428
    CITYENCH_Nightshade, // 72 Offset EXE:2B42A

    eCityEnchantments_MAX
};

enum eCity_Size ENUMSIZE8
{
    CITYSIZE_Outpost = 0,
    CITYSIZE_Hamlet = 1,
    CITYSIZE_Village = 2,
    CITYSIZE_Town = 3,
    CITYSIZE_City = 4,
    CITYSIZE_Capital = 5,

    eCity_Size_MAX
} PACKED_ENUM;

enum eCityWall
{
    CITYWALL_type1_first = 0,
    CITYWALL_type2_first = 12,
    CITYWALL_type3_first = 24,
    CITYWALL_walloffire_first = 36,
    CITYWALL_wallofdarkness_first = 50,

    eCityWall_MAX = 64
};

enum eCombatEnchantment
{
    COMBATENCHANTMENT_Vertigo = 0,          // 01
    COMBATENCHANTMENT_Confusion,            // 02
    COMBATENCHANTMENT_Whirlwind,            // 04
    COMBATENCHANTMENT_Mind_Storm,           // 08
    COMBATENCHANTMENT_Shatter,              // 10
    COMBATENCHANTMENT_Weakness,             // 20
    COMBATENCHANTMENT_Black_Sleep,          // 40
    COMBATENCHANTMENT_Warp_Creature_Attack, // 80

    COMBATENCHANTMENT_Warp_Creature_Defense,// 01
    COMBATENCHANTMENT_Warp_Creature_Resistance,// 02
    COMBATENCHANTMENT_Mind_Twist,           // 04
    COMBATENCHANTMENT_Haste,                // 08
    COMBATENCHANTMENT_Web,                  // 10
    COMBATENCHANTMENT_Creature_Binding,     // 20
    COMBATENCHANTMENT_Possession,           // 40
    COMBATENCHANTMENT_No_Effect80,          // 80

    eCombatEnchantment_MAX
};

enum eDifficulty ENUMSIZE16
{
    DIFFICULTY_Tutorial = 0,
    DIFFICULTY_Easy = 1,
    DIFFICULTY_Normal = 2,
    DIFFICULTY_Hard = 3,
    DIFFICULTY_Impossible = 4,

    eDifficulty_MAX,
    eDifficulty__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eDifficulty140m ENUMSIZE16
{
    DIFFICULTY140M_Easy = 0,
    DIFFICULTY140M_Normal = 1,
    DIFFICULTY140M_Hard = 2,
    DIFFICULTY140M_Extreme = 3,
    DIFFICULTY140M_Impossible = 4,

    eDifficulty140m_MAX,
    eDifficulty140m__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eDifficulty150 ENUMSIZE16
{
    DIFFICULTY150_Easy = 0,
    DIFFICULTY150_Normal = 1,
    DIFFICULTY150_Advanced = 2,
    DIFFICULTY150_Expert = 3,
    DIFFICULTY150_Master = 4,
    DIFFICULTY150_Lunatic = 5,

    eDifficulty150_MAX,
    eDifficulty150__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eDifficulty60 ENUMSIZE16
{
    DIFFICULTY60_Easy = 0,
    DIFFICULTY60_Normal = 1,
    DIFFICULTY60_Fair = 2,
    DIFFICULTY60_Advanced = 3,
    DIFFICULTY60_Expert = 4,
    DIFFICULTY60_Master = 5,
    DIFFICULTY60_Lunatic = 6,

    eDIFFICULTY60_MAX,
    eDifficulty60__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eEvent
{
    EVENT_None                = 0,

    EVENT_Meteor              = 1,
    EVENT_Gift_from_the_Gods  = 2,
    EVENT_Disjunction         = 3,
    EVENT_Diplomatic_Marriage = 4,
    EVENT_Earthquake          = 5,
    EVENT_Pirates             = 6,
    EVENT_Plague              = 7,
    EVENT_Rebellion           = 8,
    EVENT_Donation            = 9,
    EVENT_Depletion           = 10,
    EVENT_Mine_found          = 11,
    EVENT_Population_Boom     = 12,
    EVENT_Good_Moon           = 13,
    EVENT_Bad_Moon            = 14,
    EVENT_Conjunction_Chaos   = 15,
    EVENT_Conjunction_Nature  = 16,
    EVENT_Conjunction_Sorcery = 17,
    EVENT_Mana_Short          = 18,

    eEvent_MAX
};

enum eGameState ENUMSIZE16
{
    STATE_City_display = 0,
    STATE_Game = 1,
    STATE_Armies = 2,
    STATE_Cities = 3,
    STATE_Exit = 4,
    STATE_Main = 5,
    STATE_Magic = 6,
    STATE_Road_Building_You = 7,
    STATE_Change = 8,
    STATE_Item_You = 9, 
    STATE_Next_turn_seq = 10,
    STATE_11_default = 11,
    STATE_Spells = 12,
    STATE_13_default = 13,
    STATE_Info = 14,
    STATE_Diplomacy = 15,
    STATE_Computers_turn_GUESS = 0xFFFF
} PACKED_ENUM;

enum eGlobalEnchantment
{
    GLOBALENCHANTMENT_Eternal_Night, // 74 Offset EXE:2B42C
    GLOBALENCHANTMENT_Evil_Omens, // 76 Offset EXE:2B42E
    GLOBALENCHANTMENT_Zombie_Mastery, // 78 Offset EXE:2B430
    GLOBALENCHANTMENT_Aura_of_Majesty, // 7A Offset EXE:2B432
    GLOBALENCHANTMENT_Wind_Mastery, // 7C Offset EXE:2B434
    GLOBALENCHANTMENT_Suppress_Magic, // 7E Offset EXE:2B436
    GLOBALENCHANTMENT_Time_Stop, // 80 Offset EXE:2B438
    GLOBALENCHANTMENT_Nature_Awareness, // 82 Offset EXE:2B43A
    GLOBALENCHANTMENT_Natures_Wrath, // 84 Offset EXE:2B43C
    GLOBALENCHANTMENT_Herb_Mastery, // 86 Offset EXE:2B43E
    GLOBALENCHANTMENT_Chaos_Surge, // 88 Offset EXE:2B440
    GLOBALENCHANTMENT_Doom_Mastery, // 8A Offset EXE:2B442
    GLOBALENCHANTMENT_Great_Wasting, // 8C Offset EXE:2B444
    GLOBALENCHANTMENT_Meteor_Storm, // 8E Offset EXE:2B446
    GLOBALENCHANTMENT_Armageddon, // 90 Offset EXE:2B448
    GLOBALENCHANTMENT_Tranquility, // 92 Offset EXE:2B44A
    GLOBALENCHANTMENT_Life_Force, // 94 Offset EXE:2B44C
    GLOBALENCHANTMENT_Crusade, // 96 Offset EXE:2B44E
    GLOBALENCHANTMENT_Just_Cause, // 98 Offset EXE:2B450
    GLOBALENCHANTMENT_Holy_Arms, // 9A Offset EXE:2B452
    GLOBALENCHANTMENT_Planar_Seal, // 9C Offset EXE:2B454
    GLOBALENCHANTMENT_Charm_of_Life, // 9E Offset EXE:2B456
    GLOBALENCHANTMENT_Detect_Magic, // A0 Offset EXE:2B458
    GLOBALENCHANTMENT_Awareness, // A2 Offset EXE:2B45A

    eGlobalEnchantment_MAX
};

enum eGrand_Vizier ENUMSIZE16
{
    GRANDVIZIER_Off = 0,
    GRANDVIZIER_On = 1,
    eGrand_Vizier__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eHelpIndex
{
    eHelpIndex_NONE = -1,

    HELP_EXIT_SPELL_BOOK = 0,
    HELP_EARTH_TO_MUD = 1,
    HELP_RESIST_ELEMENTS = 2,
    HELP_WALL_OF_STONE = 3,
    HELP_GIANT_STRENGTH = 4,
    HELP_WEB = 5,
    HELP_figure12_WAR_BEARS = 6,
    HELP_STONE_SKIN = 7,
    HELP_WATER_WALKING = 8,
    HELP_figure13_SPRITES = 9,
    HELP_EARTH_LORE = 10,
    HELP_CRACKS_CALL = 11,
    HELP_NATURES_EYE = 12,
    HELP_ICE_BOLT = 13,
    HELP_figure13_GIANT_SPIDERS = 14,
    HELP_CHANGE_TERRAIN = 15,
    HELP_PATH_FINDING = 16,
    HELP_figure13_COCKATRICES = 17,
    HELP_TRANSMUTE = 18,
    HELP_NATURES_CURES = 19,
    HELP_figure13_BASILISKS = 20,
    HELP_ELEMENTAL_ARMOR = 21,
    HELP_PETRIFY = 22,
    HELP_figure13_STONE_GIANT = 23,
    HELP_IRON_SKIN = 24,
    HELP_ICE_STORM = 25,
    HELP_EARTHQUAKE = 26,
    HELP_figure13_GORGONS = 27,
    HELP_MOVE_FORTRESS = 28,
    HELP_GAIAS_BLESSING = 29,
    HELP_figure13_EARTH_ELEMENTAL = 30,
    HELP_REGENERATION = 31,
    HELP_figure13_BEHEMOTH = 32,
    HELP_ENTANGLE = 33,
    HELP_NATURES_AWARENESS = 34,
    HELP_CALL_LIGHTNING = 35,
    HELP_figure13_COLOSSUS = 36,
    HELP_EARTH_GATE = 37,
    HELP_HERB_MASTERY = 38,
    HELP_figure13_GREAT_WYRM = 39,
    HELP_NATURES_WRATH = 40,
    HELP_RESIST_MAGIC = 41,
    HELP_DISPEL_MAGIC_TRUE = 42,
    HELP_FLOATING_ISLANDS = 43,
    HELP_GUARDIAN_WIND = 44,
    HELP_Figure13_PHANTOM_WARRIORS = 45,
    HELP_CONFUSION = 46,
    HELP_WORD_OF_RECALL = 47,
    HELP_COUNTER_MAGIC = 48,
    HELP_figure14_NAGAS = 49,
    HELP_PSIONIC_BLAST = 50,
    HELP_BLUR = 51,
    HELP_DISENCHANT_TRUE = 52,
    HELP_VERTIGO = 53,
    HELP_SPELL_LOCK = 54,
    HELP_ENCHANT_ROAD = 55,
    HELP_FLIGHT = 56,
    HELP_WIND_MASTERY = 57,
    HELP_SPELL_BLAST = 58,
    HELP_AURA_OF_MAJESTY = 59,
    HELP_Figure13_PHANTOM_BEAST = 60,
    HELP_DISJUNCTION_TRUE = 61,
    HELP_INVISIBILITY = 62,
    HELP_WIND_WALKING = 63,
    HELP_BANISH = 64,
    HELP_Figure13_STORM_GIANT = 65,
    HELP_Figure13_AIR_ELEMENTAL = 66,
    HELP_MIND_STORM = 67,
    HELP_STASIS = 68,
    HELP_MAGIC_IMMUNITY = 69,
    HELP_HASTE = 70,
    HELP_figure14_DJINN = 71,
    HELP_SPELL_WARD = 72,
    HELP_CREATURE_BINDING = 73,
    HELP_MASS_INVISIBILITY = 74,
    HELP_GREAT_UNSUMMONING = 75,
    HELP_SPELL_BINDING = 76,
    HELP_FLYING_FORTRESS = 77,
    HELP_Figure14_SKY_DRAKE = 78,
    HELP_SUPPRESS_MAGIC = 79,
    HELP_TIME_STOP = 80,
    HELP_WARP_WOOD = 81,
    HELP_DISRUPT = 82,
    HELP_FIRE_BOLT = 83,
    HELP_figure11_HELL_HOUNDS = 84,
    HELP_CORRUPTION = 85,
    HELP_spell_SPECIAL_ELDRITCH_WEAPON = 86,
    HELP_WALL_OF_FIRE = 87,
    HELP_spell_SPECIAL_SHATTER = 88,
    HELP_spell_SPECIAL2_WARP_CREATURE = 89,
    HELP_figure11_FIRE_ELEMENTAL = 90,
    HELP_LIGHTNING_BOLT = 91,
    HELP_Figure11_FIRE_GIANT = 92,
    HELP_spell_SPECIAL_CHAOS_CHANNELS = 93,
    HELP_spell_SPECIAL_FLAME_BLADE = 94,
    HELP_figure11_GARGOYLES = 95,
    HELP_FIREBALL = 96,
    HELP_figure11_DOOM_BAT = 97,
    HELP_RAISE_VOLCANO = 98,
    HELP_spell_SPECIAL2_IMMOLATION = 99,
    HELP_Figure11_CHIMERA = 100,
    HELP_WARP_LIGHTNING = 101,
    HELP_spell_COMPIX_METAL_FIRES = 102,
    HELP_figure11_CHAOS_SPAWN = 103,
    HELP_DOOM_BOLT = 104,
    HELP_MAGIC_VORTEX = 105,
    HELP_Figure11_EFREET = 106,
    HELP_FIRE_STORM = 107,
    HELP_spell_COMPIX_WARP_REALITY = 108,
    HELP_FLAME_STRIKE = 109,
    HELP_CHAOS_RIFT = 110,
    HELP_Figure11_HYDRA = 111,
    HELP_DISINTEGRATE = 112,
    HELP_METEOR_STORM = 113,
    HELP_GREAT_WASTING = 114,
    HELP_CALL_CHAOS = 115,
    HELP_CHAOS_SURGE = 116,
    HELP_DOOM_MASTERY = 117,
    HELP_figure11_GREAT_DRAKE = 118,
    HELP_CALL_THE_VOID = 119,
    HELP_ARMAGEDON = 120,
    HELP_BLESS = 121,
    HELP_STAR_FIRES = 122,
    HELP_ENDURANCE = 123,
    HELP_HOLY_WEAPON = 124,
    HELP_HEALING = 125,
    HELP_HOLY_ARMOR = 126,
    HELP_JUST_CAUSE = 127,
    HELP_TRUE_LIGHT = 128,
    HELP_figure12_GUARDIAN_SPIRIT = 129,
    HELP_HEROISM = 130,
    HELP_TRUE_SIGHT = 131,
    HELP_PLANE_SHIFT = 132,
    HELP_RESURRECTION = 133,
    HELP_DISPEL_EVIL = 134,
    HELP_PLANAR_SEAL = 135,
    HELP_Figure12_UNICORNS = 136,
    HELP_RAISE_DEAD = 137,
    HELP_PLANAR_TRAVEL = 138,
    HELP_HEAVENLY_LIGHT = 139,
    HELP_PRAYER = 140,
    HELP_LION_HEART = 141,
    HELP_figures3_INCARNATION = 142,
    HELP_INVULNERABILITY = 143,
    HELP_RIGHTEOUSNESS = 144,
    HELP_PROSPERITY = 145,
    HELP_ALTAR_OF_BATTLE = 146,
    HELP_figure12_ANGEL = 147,
    HELP_STREAM_OF_LIFE = 148,
    HELP_MASS_HEALING = 149,
    HELP_HOLY_WORD = 150,
    HELP_HIGH_PRAYER = 151,
    HELP_INSPIRATIONS = 152,
    HELP_ASTRAL_GATE = 153,
    HELP_HOLY_ARMS = 154,
    HELP_CONSECRATION = 155,
    HELP_LIFE_FORCE = 156,
    HELP_TRANQUILITY = 157,
    HELP_CRUSADE = 158,
    HELP_figure12_ARCHANGEL = 159,
    HELP_CHARM_OF_LIFE = 160,
    HELP_Figure12_SKELETONS = 161,
    HELP_spell_SPECIAL_WEAKNESS = 162,
    HELP_DARK_RITUALS = 163,
    HELP_spell_SPECIAL_CLOAK_OF_FEAR = 164,
    HELP_spell_SPECIAL2_BLACK_SLEEP = 165,
    HELP_Figure12_GHOULS = 166,
    HELP_LIFE_DRAIN = 167,
    HELP_TERROR = 168,
    HELP_spell_COMPIX_DARKNESS = 169,
    HELP_MANA_LEAK = 170,
    HELP_DRAIN_POWER = 171,
    HELP_spell_SPECIAL_POSSESSION = 172,
    HELP_Figure12_LYCANTHROPY = 173,
    HELP_spell_COMPIX_BLACK_PRAYER = 174,
    HELP_spell_SPECIAL_BLACK_CHANNELS = 175,
    HELP_Figure12_NIGHT_STALKER = 176,
    HELP_SUBVERSION = 177,
    HELP_WALL_OF_DARKNESS = 178,
    HELP_spell_SPECIAL2_BERSERK = 179,
    HELP_Figure12_SHADOW_DEMONS = 180,
    HELP_spell_SPECIAL_WRAITH_FORM = 181,
    HELP_spell_COMPIX_WRACK = 182,
    HELP_EVIL_PRESENCE = 183,
    HELP_Figure12_WRAITHS = 184,
    HELP_CLOUD_OF_SHADOW = 185,
    HELP_WARP_NODE = 186,
    HELP_BLACK_WIND = 187,
    HELP_Figure12_ZOMBIE_MASTERY = 188,
    HELP_FAMINE = 189,
    HELP_CURSED_LANDS = 190,
    HELP_CRUEL_UNMINDING = 191,
    HELP_WORD_OF_DEATH = 192,
    HELP_Figure12_DEATH_KNIGHTS = 193,
    HELP_DEATH_SPELL = 194,
    HELP_ANIMATE_DEAD = 195,
    HELP_PESTILENCE = 196,
    HELP_ETERNAL_NIGHT = 197,
    HELP_EVIL_OMENS = 198,
    HELP_DEATH_WISH = 199,
    HELP_Figure12_DEMON_LORD = 200,
    HELP_figure11_MAGIC_SPIRIT = 201,
    HELP_DISPEL_MAGIC = 202,
    HELP_spell_cityscap_SUMMONING_CIRCLE = 203,
    HELP_DISENCHANT_AREA = 204,
    HELP_RECALL_HERO = 205,
    HELP_DETECT_MAGIC = 206,
    HELP_ENCHANT_ITEM = 207,
    HELP_SUMMON_HERO = 208,
    HELP_AWARENESS = 209,
    HELP_DISJUNCTION = 210,
    HELP_CREATE_ARTIFACT = 211,
    HELP_SUMMON_CHAMPION = 212,
    HELP_spell_SPELL_OF_MASTERY = 213,
    HELP_SPECIAL2_PAGE_BACK = 214,
    HELP_SPECIAL2_PAGE_FORWARD = 215,
    HELP_MAIN_GAME = 216,
    HELP_MAIN_SPELLS = 217,
    HELP_MAIN_ARMIES = 218,
    HELP_MAIN_CITIES = 219,
    HELP_MAIN_MAGIC = 220,
    HELP_MAIN_INFORMATION = 221,
    HELP_MAIN_PLANE = 222,
    HELP_MAIN_DONE = 223,
    HELP_MAIN_PATROL = 224,
    HELP_MAIN_WAIT = 225,
    HELP_MAIN_BUILD = 226,
    HELP_MAIN_PURIFY = 227,
    HELP_MAIN_MELD = 228,
    HELP_NEXT_TURN = 229,
    HELP_REDUCED_MAP = 230,
    HELP_TOTAL_GOLD = 231,
    HELP_TOTAL_MANA = 232,
    HELP_GOLD_INCOME = 233,
    HELP_FOOD_RESERVE = 234,
    HELP_MANA_INCOME = 235,
    HELP_MOVES = 236,
    HELP_1_REDUCED_MAP = 237,
    HELP_UNIT_NAME = 238,
    HELP_UNIT_ENCHANTMENTS = 239,
    HELP_ARMYLIST_ITEMS = 240,
    HELP_1_ARMYLIST_OK = 241,
    HELP_1_UPKEEP = 242,
    HELP_EMPTY_HERO_SLOT = 243,
    HELP_ARMYLIST_UP_ARROW = 244,
    HELP_ARMYLIST_DOWN_ARROW = 245,
    HELP_ANVIL = 246,
    HELP_ARMYLIST_ALCHEMY = 247,
    HELP_2_ARMYLIST_OK = 248,
    HELP_2_REDUCED_MAP = 249,
    HELP_2_CITY_NAME = 250,
    HELP_CITY_ENCHANTMENTS = 251,
    HELP_RELOAD_OK = 252,
    HELP_MAIN_WALKING = 253,
    HELP_MAIN_FORESTER = 254,
    HELP_MAIN_MOUNTAINEER = 255,
    HELP_MAIN_FLYING = 256,
    HELP_MAIN_PATHFINDING = 257,
    HELP_MAIN_SWIMMING = 258,
    HELP_MAIN_SAILING = 259,
    HELP_MAIN_WIND_WALKING = 260,
    HELP_MAIN_PLANAR_TRAVEL = 261,
    HELP_MAGIC_GRAY_GEM = 262,
    HELP_MAGIC_SHATTERED_GEM = 263,
    HELP_POWER_DISTRIBUTION = 264,
    HELP_CASTING_SKILL_RATIO = 265,
    HELP_MANA_POINTS_RATIO = 266,
    HELP_RESEARCH_RATIO = 267,
    HELP_MAGIC_ALCHEMY = 268,
    HELP_3_MAGIC_OK = 269,
    HELP_3_ENCHANTMENTS = 270,
    HELP_RELATIONS = 271,
    HELP_WIZARD_STATISTICS_delete = 272,
    HELP_SPELL_CASTING_SKILL = 273,
    HELP_MAGIC_RESERVE = 274,
    HELP_POWER_BASE = 275,
    HELP_CASTING_1 = 276,
    HELP_RESEARCHING = 277,
    HELP_SUMMON_TO = 278,
    HELP_BARBARIAN_TOWNSFOLK = 279,
    HELP_BEASTMEN_TOWNSFOLK = 280,
    HELP_DARK_ELF_TOWNSFOLK = 281,
    HELP_DRACONIAN_TOWNSFOLK = 282,
    HELP_DWARF_TOWNSFOLK = 283,
    HELP_GNOLL_TOWNSFOLK = 284,
    HELP_HALFLING_TOWNSFOLK = 285,
    HELP_HIGH_ELF_TOWNSFOLK = 286,
    HELP_HIGH_MEN_TOWNSFOLK = 287,
    HELP_KLACKON_TOWNSFOLK = 288,
    HELP_LIZARDMEN_TOWNSFOLK = 289,
    HELP_NOMAD_TOWNSFOLK = 290,
    HELP_ORC_TOWNSFOLK = 291,
    HELP_TROLL_TOWNSFOLK = 292,
    HELP_3_CITY_NAME = 293,
    HELP_CITY_RESOURCES_delete = 294,
    HELP_BACKGRND_FOOD = 295,
    HELP_BACKGRND_PRODUCTION = 296,
    HELP_BACKGRND_GOLD = 297,
    HELP_BACKGRND_GOLD_MAINTENANCE = 298,
    HELP_BACKGRND_MAGICAL_POWER = 299,
    HELP_BACKGRND_SPELL_RESEARCH = 300,
    HELP_4_ENCHANTMENTS = 301,
    HELP_CITY_VIEW = 302,
    HELP_4_CITY_SCAPE = 303,
    HELP_PRODUCING = 304,
    HELP_BACKGRND_BUY = 305,
    HELP_BACKGRND_CHANGE = 306,
    HELP_BACKGRND_OK = 307,
    HELP_BACKGRND_Barbarian_Farmers = 308,
    HELP_BACKGRND_Barbarian_Workers = 309,
    HELP_BACKGRND_Barbarian_Rebels = 310,
    HELP_BACKGRND_Beastmen_Farmers = 311,
    HELP_BACKGRND_Beastmen_Workers = 312,
    HELP_BACKGRND_Beastmen_Rebels = 313,
    HELP_BACKGRND_Dark_Elf_Farmers = 314,
    HELP_BACKGRND_Dark_Elf_Workers = 315,
    HELP_BACKGRND_Dark_Elf_Rebels = 316,
    HELP_BACKGRND_Draconian_Farmers = 317,
    HELP_BACKGRND_Draconian_Workers = 318,
    HELP_BACKGRND_Draconian_Rebels = 319,
    HELP_BACKGRND_Dwarf_Farmers = 320,
    HELP_BACKGRND_Dwarf_Workers = 321,
    HELP_BACKGRND_Dwarf_Rebels = 322,
    HELP_BACKGRND_Gnoll_Farmers = 323,
    HELP_BACKGRND_Gnoll_Workers = 324,
    HELP_BACKGRND_Gnoll_Rebels = 325,
    HELP_BACKGRND_Halfling_Farmers = 326,
    HELP_BACKGRND_Halfling_Workers = 327,
    HELP_BACKGRND_Halfling_Rebels = 328,
    HELP_BACKGRND_High_Elf_Farmers = 329,
    HELP_BACKGRND_High_Elf_Workers = 330,
    HELP_BACKGRND_High_Elf_Rebels = 331,
    HELP_BACKGRND_High_Men_Farmers = 332,
    HELP_BACKGRND_High_Men_Workers = 333,
    HELP_BACKGRND_High_Men_Rebels = 334,
    HELP_BACKGRND_Klackon_Farmers = 335,
    HELP_BACKGRND_Klackon_Workers = 336,
    HELP_BACKGRND_Klackon_Rebels = 337,
    HELP_BACKGRND_Lizardmen_Farmers = 338,
    HELP_BACKGRND_Lizardmen_Workers = 339,
    HELP_BACKGRND_Lizardmen_Rebels = 340,
    HELP_BACKGRND_Nomad_Farmers = 341,
    HELP_BACKGRND_Nomad_Workers = 342,
    HELP_BACKGRND_Nomad_Rebels = 343,
    HELP_BACKGRND_Orc_Farmers = 344,
    HELP_BACKGRND_Orc_Workers = 345,
    HELP_BACKGRND_Orc_Rebels = 346,
    HELP_BACKGRND_Troll_Farmers = 347,
    HELP_BACKGRND_Troll_Workers = 348,
    HELP_BACKGRND_Troll_Rebels = 349,
    HELP_PORTRAIT = 350,
    HELP_NAME = 351,
    HELP_STATISTICS = 352,
    HELP_GOLD = 353,
    HELP_MANA_1 = 354,
    HELP_SPELL_RANKS = 355,
    HELP_SPECIAL_ABILITIES = 356,
    HELP_HEROES = 357,
    HELP_PLANE = 358,
    HELP_MAP = 359,
    HELP_CLOSE = 360,
    HELP_CHANGE_PLANES = 361,
    HELP_MAP_KEY = 362,
    HELP_POWER_GRAPH = 363,
    HELP_STATUS = 364,
    HELP_MAGIC_POWER = 365,
    HELP_SPELL_RESEARCH = 366,
    HELP_ARMY_STRENGTH = 367,
    HELP_ALCHEMY_GOLD = 368,
    HELP_MAGIC_ALCHEMY_RATIO = 369,
    HELP_ALCHEMY_POWER = 370,
    HELP_MAGIC_CANCEL = 371,
    HELP_MAGIC_ALCHEMY_TOGGLE = 372,
    HELP_4_MAGIC_OK = 373,
    HELP_BUILDING_OPTIONS = 374,
    HELP_UNIT_OPTIONS = 375,
    HELP_1_CANCEL = 376,
    HELP_1_OK = 377,
    HELP_BUILDING_DESCRIPTION = 378,
    HELP_ALLOWS = 379,
    HELP_MAINTENANCE = 380,
    HELP_COST = 381,
    HELP_UNIT_MOVES = 382,
    HELP_MELEE = 383,
    HELP_UNITVIEW_Sword = 384,
    HELP_UNITVIEW_Magic_Weapon = 385,
    HELP_UNITVIEW_Mithril_Weapon = 386,
    HELP_RANGE_1 = 387,
    HELP_UNITVIEW_THROWN_delete = 388,
    HELP_UNITVIEW_Bow = 389,
    HELP_UNITVIEW_Rock = 390,
    HELP_UNITVIEW_Magical_Attack = 391,
    HELP_UNITVIEW_MAGICAL_BREATH_delete = 392,
    HELP_UNITVIEW_ARMOR = 393,
    HELP_UNITVIEW_RESISTANCE = 394,
    HELP_UNITVIEW_HITS = 395,
    HELP_SPECIAL_LARGE_SHIELD = 396,        // Ability_Flags (2nd)
    HELP_SPECIAL_PLANE_SHIFT = 397,
    HELP_SPECIAL_WALL_CRUSHER = 398,
    HELP_SPECIAL_HEALER = 399,
    HELP_SPECIAL_CREATE_OUTPOST = 400,
    HELP_SPECIAL_INVISIBILITY_1 = 401,
    HELP_SPECIAL_CREATE_UNDEAD = 402,
    HELP_SPECIAL2_LONG_RANGE = 403,
    HELP_SPECIAL_LAND_CORRUPTION = 404,
    HELP_SPECIAL_MELD_WITH_NODE = 405,
    HELP_SPECIAL_NON_CORPOREAL = 406,
    HELP_SPECIAL_WIND_WALKING_1 = 407,
    HELP_SPECIAL_REGENERATION = 408,
    HELP_SPECIAL_PURIFY = 409,
    HELP_SPECIAL_NEGATE_FIRST_STRIKE = 410, // Ability_Flags (last-ordered)
    HELP_SPECIAL2_LUCKY = 411,              // Attribute_Flags (3rd)     
    HELP_SPECIAL_POISON_IMMUNITY = 412,     // Immunity_Flags (last)
    HELP_SPECIAL_FIRE_IMMUNITY = 413,       // Immunity_Flags (1st)
    HELP_SPECIAL_STONING_IMMUNITY = 414,    // Immunity_Flags (2nd)
    HELP_SPECIAL_WEAPON_IMMUNITY = 415,
    HELP_SPECIAL_MISSILES_IMMUNITY = 416,
    HELP_SPECIAL_ILLUSIONS_IMMUNITY = 417,
    HELP_SPECIAL_COLD_IMMUNITY = 418,
    HELP_SPECIAL_MAGIC_IMMUNITY_1 = 419,
    HELP_SPECIAL_DEATH_IMMUNITY = 420,      // Immunity_Flags (last-unordered)
    HELP_SPECIAL_HEALING_SPELL = 421,       // Attribute_Flags
    HELP_SPECIAL_FIRE_BALL_SPELL = 422,
    HELP_SPECIAL_DOOMBOLT_SPELL = 423,
    HELP_SPECIAL_IMMOLATION = 424,
    HELP_SPECIAL2_CAUSE_FEAR_SPELL = 425,
    HELP_SPECIAL2_WEB_SPELL = 426,
    HELP_SPECIAL_RESISTANCE_TO_ALL = 427,
    HELP_SPECIAL_HOLY_BONUS = 428,          // Attribute_Flags (last-almost ordered)
    HELP_SPECIAL_ARMOR_PIERCING = 429,      // Attack_Flags (1st)
    HELP_SPECIAL_FIRST_STRIKE = 430,
    HELP_SPECIAL_POISON = 431,
    HELP_SPECIAL_LIFE_STEAL = 432,
    HELP_SPECIAL2_CHAOS = 433,
    HELP_SPECIAL2_DESTRUCTION = 434,
    HELP_SPECIAL_ILLUSION = 435,
    HELP_SPECIAL_STONING_TOUCH = 436,
    HELP_SPECIAL2_CAUSE_DEATH = 437,
    HELP_SPECIAL2_POWER_DRAIN = 438,
    HELP_SPECIAL2_DISPEL_EVIL = 439,        // Attack_Flags (last-unordered)
    HELP_SPECIAL_CHAOS_CHANNEL_1 = 440,     // Unit_Weapon_Mutation
    HELP_SPECIAL_CHAOS_CHANNEL_2 = 441,     // Unit_Weapon_Mutation
    HELP_SPECIAL_CHAOS_CHANNEL_3 = 442,     // Unit_Weapon_Mutation
    HELP_SPECIAL2_THROWN = 443,             // Ranged type (100)
    HELP_SPECIAL2_FIRE_BREATH = 444,
    HELP_SPECIAL2_LIGHTNING_BREATH = 445,
    HELP_SPECIAL_STONING_GAZE = 446,
    HELP_SPECIAL2_DOOM_GAZE = 447,
    HELP_SPECIAL2_DEATH_GAZE = 448,         // Ranged type (105 - ordered)
    HELP_SPECIAL2_IMMOLATION = 449,         // Unit_Enchantment (1st)
    HELP_SPECIAL2_GUARDIAN_WIND = 450,
    HELP_SPECIAL2_BERSERK = 451,
    HELP_SPECIAL_CLOAK_OF_FEAR = 452,
    HELP_SPECIAL_BLACK_CHANNELS = 453,
    HELP_SPECIAL_WRAITH_FORM = 454,
    HELP_SPECIAL_REGENERATE = 455,
    HELP_SPECIAL_PATHFINDING_1 = 456,
    HELP_SPECIAL_WATER_WALKING = 457,
    HELP_SPECIAL_ELEMENTAL_ARMOR = 458,
    HELP_SPECIAL_RESIST_ELEMENTS = 459,
    HELP_SPECIAL_STONE_SKIN = 460,
    HELP_SPECIAL_IRON_SKIN = 461,
    HELP_SPECIAL_ENDURANCE = 462,
    HELP_SPECIAL2_SPELL_LOCK = 463,
    HELP_SPECIAL_INVISIBILITY_2 = 464,
    HELP_SPECIAL_WIND_WALKING_2 = 465,
    HELP_SPECIAL_FLIGHT = 466,
    HELP_SPECIAL_RESIST_MAGIC = 467,
    HELP_SPECIAL_MAGIC_IMMUNITY_2 = 468,
    HELP_SPECIAL_FLAME_BLADE = 469,
    HELP_SPECIAL_ELDRITCH_WEAPON = 470,
    HELP_SPECIAL_TRUE_SIGHT = 471,
    HELP_SPECIAL_HOLY_WEAPON = 472,
    HELP_SPECIAL_HEROISM = 473,
    HELP_SPECIAL_BLESS = 474,
    HELP_SPECIAL_LION_HEART = 475,
    HELP_SPECIAL_GIANT_STRENGTH = 476,
    HELP_SPECIAL_PLANAR_TRAVEL = 477,
    HELP_SPECIAL_HOLY_ARMOR = 478,
    HELP_SPECIAL_RIGHTEOUSNESS = 479,
    HELP_SPECIAL_INVULNERABILITY = 480,     // Unit_Enchantment (last-almost ordered)
    HELP_SPECIAL_VERTIGO = 481,             // Combat_Enchantment (1st)
    HELP_SPECIAL2_CONFUSION = 482,
    HELP_SPECIAL2_WHIRLWIND = 483,
    HELP_SPECIAL2_MIND_STORM = 484,
    HELP_SPECIAL_SHATTER = 485,
    HELP_SPECIAL_WEAKNESS = 486,
    HELP_SPECIAL2_BLACK_SLEEP = 487,
    HELP_SPECIAL2_WARP_CREATURE_1 = 488,
    HELP_SPECIAL2_WARP_CREATURE_2 = 489,
    HELP_SPECIAL2_WARP_CREATURE_3 = 490,
    HELP_SPECIAL2_MANA_LEAK = 491,
    HELP_SPECIAL_HASTE = 492,
    HELP_SPECIAL_WEB = 493,
    HELP_SPECIAL_CREATURE_BINDING = 494,
    HELP_SPECIAL_POSSESSION = 495,          // Combat_Enchantment (last-almost ordered)
    HELP_2_UPKEEP = 496,
    HELP_UNITVIEW_PAGE_UP = 497,
    HELP_UNITVIEW_PAGE_DOWN = 498,
    HELP_DISMISS = 499,
    HELP_2_OK = 500,
    HELP_2_CITY_SCAPE = 501,
    HELP_CITY_GARRISONS = 502,
    HELP_UNIT_LIST = 503,
    HELP_BACKGRND_OUTPOST_SIZE_1 = 504,
    HELP_BACKGRND_OUTPOST_SIZE_2 = 505,
    HELP_BACKGRND_OUTPOST_SIZE_3 = 506,
    HELP_OUTPOST_SCAPE = 507,
    HELP_MAIN_CANCEL_1 = 508,
    HELP_CASTING_2 = 509,
    HELP_ROAD_BUILDING = 510,
    HELP_MAIN_OK = 511,
    HELP_MAIN_CANCEL_2 = 512,
    HELP_SURVEY = 513,
    HELP_MAIN_CANCEL_3 = 514,
    HELP_hero_SPECIAL_LEADERSHIP = 515,
    HELP_hero_SPECIAL_LEGENDARY = 516,
    HELP_hero_SPECIAL_BLADEMASTER = 517,
    HELP_hero_SPECIAL_ARMSMASTER = 518,
    HELP_hero_SPECIAL_CONSTITUTION = 519,
    HELP_hero_SPECIAL_MIGHT = 520,
    HELP_hero_SPECIAL_ARCANE_POWER = 521,
    HELP_hero_SPECIAL_SAGE = 522,
    HELP_hero_SPECIAL_PRAYERMASTER = 523,
    HELP_hero_SPECIAL_LUCKY = 524,
    HELP_hero_SPECIAL_CHARMED = 525,
    HELP_hero_SPECIAL_NOBLE = 526,
    HELP_unit_SPECIAL_RECRUIT = 527,
    HELP_unit_SPECIAL_REGULAR = 528,
    HELP_unit_SPECIAL_VETERAN = 529,
    HELP_unit_SPECIAL_ELITE = 530,
    HELP_unit_SPECIAL_ULTRA_ELITE = 531,
    HELP_unit_SPECIAL_CHAMPION = 532,
    HELP_hero_SPECIAL_HERO = 533,
    HELP_hero_SPECIAL_MYRMIDON = 534,
    HELP_hero_SPECIAL_CAPTAIN = 535,
    HELP_hero_SPECIAL_COMMANDER = 536,
    HELP_hero_SPECIAL_CHAMPION = 537,
    HELP_hero_SPECIAL_LORD = 538,
    HELP_hero_SPECIAL_GRAND_LORD = 539,
    HELP_hero_SPECIAL_SUPER_HERO = 540,
    HELP_hero_SPECIAL_DEMI_GOD = 541,
    HELP_SPECIAL2_QUIVER = 542,
    HELP_SPECIAL2_PLUS_TO_HIT = 543,
    HELP_SPECIAL2_MIN_TO_HIT = 544,
    HELP_SPECIAL_CONSTRUCTION = 545,
    HELP_SPECIAL_SCOUTING = 546,
    HELP_ITEMISC_MERGING_1 = 547,
    HELP_SPECIAL_TELEPORTING = 548,
    HELP_SPECIAL_PATHFINDING_2 = 549,
    HELP_SPECIAL_MOUNTAINEER = 550,
    HELP_SPECIAL_FORESTER = 551,
    HELP_SPECIAL2_SUMMON_DEMONS = 552,
    HELP_SPECIAL2_UNDEAD = 553,
    HELP_SPECIAL2_STASIS = 554,
    HELP_SPECIAL_CASTER = 555,
    HELP_ITEMISC_MAGIC_IMMUNITY = 556,
    HELP_ITEMISC_REGENERATE = 557,
    HELP_ITEMISC_HASTE = 558,
    HELP_ITEMISC_TRUE_SIGHT = 559,
    HELP_ITEMISC_PATH_FINDING = 560,
    HELP_ITEMISC_WATER_WALKING = 561,
    HELP_ITEMISC_BLESS = 562,
    HELP_ITEMISC_LION_HEART = 563,
    HELP_ITEMISC_INVISIBILITY = 564,
    HELP_ITEMISC_PLANAR_TRAVEL = 565,
    HELP_ITEMISC_ENDURANCE = 566,
    HELP_ITEMISC_RIGHTEOUSNESS = 567,
    HELP_ITEMISC_INVULNERABILITY = 568,
    HELP_ITEMISC_RESIST_ELEMENTS = 569,
    HELP_ITEMISC_ELEMENTAL_ARMOR = 570,
    HELP_ITEMISC_CLOAK_OF_FEAR = 571,
    HELP_ITEMISC_FLIGHT = 572,
    HELP_ITEMISC_RESIST_MAGIC = 573,
    HELP_ITEMISC_GUARDIAN_WIND = 574,
    HELP_ITEMISC_GIANT_STRENGTH = 575,
    HELP_ITEMISC_MERGING_2 = 576,
    HELP_ITEMISC_SWORD_SLOT_1 = 577,
    HELP_ITEMISC_BOW_SLOT_1 = 578,
    HELP_ITEMISC_STAVE_SWORD_SLOT_1 = 579,
    HELP_ITEMISC_STAVE_SLOT_1 = 580,
    HELP_ITEMISC_ARMOR_SLOT_1 = 581,
    HELP_ITEMISC_MISCELLANEOUS_SLOT_1 = 582,
    HELP_ITEMISC_SWORD_SLOT_2 = 583,
    HELP_ITEMISC_BOW_SLOT_2 = 584,
    HELP_ITEMISC_STAVE_SWORD_SLOT_2 = 585,
    HELP_ITEMISC_STAVE_SLOT_2 = 586,
    HELP_ITEMISC_ARMOR_SLOT_2 = 587,
    HELP_ITEMISC_MISCELLANEOUS_SLOT_2 = 588,
    HELP_COMPIX_SPELL_BUTTON = 589,
    HELP_COMPIX_WAIT_BUTTON = 590,
    HELP_COMPIX_INFO_BUTTON = 591,
    HELP_COMPIX_AUTO_BUTTON = 592,
    HELP_COMPIX_FLEE_BUTTON = 593,
    HELP_COMPIX_DONE_BUTTON = 594,
    HELP_SKILL = 595,
    HELP_MANA_2 = 596,
    HELP_RANGE_2 = 597,
    HELP_ATTACKER = 598,
    HELP_DEFENDER = 599,
    HELP_combat_COMPIX_LIGHT = 600,
    HELP_combat_COMPIX_DARKNESS = 601,
    HELP_combat_COMPIX_WARP_REALITY = 602,
    HELP_combat_COMPIX_BLACK_PRAYER = 603,
    HELP_combat_COMPIX_WRACK = 604,
    HELP_combat_COMPIX_METAL_FIRES = 605,
    HELP_combat_COMPIX_PRAYER = 606,
    HELP_combat_COMPIX_HIGH_PRAYER = 607,
    HELP_combat_COMPIX_TERROR = 608,
    HELP_combat_COMPIX_CALL_LIGHTNING = 609,
    HELP_combat_COMPIX_COUNTER_MAGIC = 610,
    HELP_combat_COMPIX_MASS_INVISIBILITY = 611,
    HELP_combat_COMPIX_DISPELS_NON_SORCERY = 612,
    HELP_combat_COMPIX_DISPELS_NON_CHAOS = 613,
    HELP_combat_COMPIX_DISPELS_NON_NATURE = 614,
    HELP_combat_COMPIX_SORCERY_NODE_AURA = 615,
    HELP_combat_COMPIX_CHAOS_NODE_AURA = 616,
    HELP_combat_COMPIX_NATURE_NODE_AURA = 617,
    HELP_combat_COMPIX_CLOUD_OF_DARKNESS = 618,
    HELP_combat_COMPIX_HOLY_LIGHT = 619,
    HELP_combat_COMPIX_CHAOS_SURGE = 620,
    HELP_combat_COMPIX_ETERNAL_NIGHT = 621,
    HELP_combat_COMPIX_CRUSADE = 622,
    HELP_combat_COMPIX_HOLY_ARMS = 623,
    HELP_combat_COMPIX_CHARM_OF_LIFE = 624,
    HELP_combat_COMPIX_ZOMBIE_MASTERY = 625,
    HELP_DAMAGE = 626,
    HELP_hero_SPECIAL2_AGILE = 627,
    HELP_CONTINUE = 628,
    HELP_LOAD = 629,
    HELP_NEW_GAME = 630,
    HELP_QUIT_TO_DOS = 631,
    HELP_COMPIX_ENTANGLE = 632,
    HELP_LOAD_LOAD_BUTTON = 633,
    HELP_LOAD_QUIT_BUTTON = 634,
    HELP_LOAD_SETTINGS_BUTTON = 635,
    HELP_LOAD_SAVE_BUTTON = 636,
    HELP_LOAD_OK_BUTTON = 637,
    HELP_SAVED_GAMES_1 = 638,
    HELP_SAVED_GAMES_2 = 639,
    HELP_setup_NEWGAME_OK_BUTTON = 640,
    HELP_NEWGAME_CANCEL_BUTTON = 641,
    HELP_DIFFICULTY_LEVEL = 642,
    HELP_difficulty_INTRO = 643,
    HELP_difficulty_EASY = 644,
    HELP_difficulty_NORMAL = 645,
    HELP_difficulty_HARD = 646,
    HELP_difficulty_IMPOSSIBLE = 647,
    HELP_OPPONENTS = 648,
    HELP_LAND_SIZE = 649,
    HELP_land_SMALL = 650,
    HELP_land_MEDIUM = 651,
    HELP_land_LARGE = 652,
    HELP_MAGIC_INTENSITY = 653,
    HELP_nodes_WEAK = 654,
    HELP_nodes_NORMAL = 655,
    HELP_nodes_POWERFUL = 656,
    HELP_SPELL_OF_RETURN = 657,
    HELP_SELECT_A_BANNER = 658,
    HELP_MERLIN = 659,
    HELP_RAVEN = 660,
    HELP_SHAREE = 661,
    HELP_LO_PAN = 662,
    HELP_JAFAR = 663,
    HELP_OBERIC = 664,
    HELP_TAURON = 665,
    HELP_SSSRA = 666,
    HELP_RJAK = 667,
    HELP_FREYA = 668,
    HELP_HORUS = 669,
    HELP_ARIEL = 670,
    HELP_TLALOC = 671,
    HELP_KALI = 672,
    HELP_CUSTOM = 673,
    HELP_SELECT_WIZARD_PICTURE = 674,
    HELP_SOUND_EFFECTS = 675,
    HELP_BACKGROUND_MUSIC = 676,
    HELP_EVENT_MUSIC = 677,
    HELP_CITY_SPELL_EVENTS = 678,
    HELP_OVERLAND_SPELL_EVENTS = 679,
    HELP_SUMMONING_EVENTS = 680,
    HELP_END_OF_TURN_SUMMARY = 681,
    HELP_AUTOMATIC_ADVICE = 682,
    HELP_RANDOM_EVENTS = 683,
    HELP_AUTOMATIC_COMBAT = 684,
    HELP_REVOLTING_RAIDERS = 685,
    HELP_MONSTERS_GONE_WILD = 686,
    HELP_ENEMY_MOVES = 687,
    HELP_ENEMY_SPELLS = 688,
    HELP_SPELL_BOOK_ORDERING = 689,
    HELP_SPELL_ANIMATIONS = 690,
    HELP_SHOW_NODE_OWNERS = 691,
    HELP_EXPANDING_HELP = 692,
    HELP_ALCHEMY = 693,
    HELP_WARLORD = 694,
    HELP_CHANNELER = 695,
    HELP_ARCHMAGE = 696,
    HELP_ARTIFICER = 697,
    HELP_CONJURER = 698,
    HELP_SAGE_MASTER = 699,
    HELP_MYRRAN = 700,
    HELP_DIVINE_POWER = 701,
    HELP_FAMOUS = 702,
    HELP_RUNEMASTER = 703,
    HELP_CHARISMATIC = 704,
    HELP_CHAOS_MASTERY = 705,
    HELP_NATURE_MASTERY = 706,
    HELP_SORCERY_MASTERY = 707,
    HELP_INFERNAL_POWER = 708,
    HELP_MANA_FOCUSING = 709,
    HELP_NODE_MASTERY = 710,
    HELP_MAGIC_PICKS = 711,
    HELP_play_NEWGAME_OK_BUTTON = 712,
    HELP_SPECIAL2_GUISE = 713,
    HELP_cityscap_TRADE_GOODS = 714,
    HELP_cityscap_HOUSING = 715,
    HELP_cityscap_BARRACKS = 716,
    HELP_cityscap_ARMORY = 717,
    HELP_cityscap_FIGHTERS_GUILD = 718,
    HELP_cityscap_ARMORERS_GUILD = 719,
    HELP_cityscap_WAR_COLLEGE = 720,
    HELP_cityscap_SMITHY = 721,
    HELP_cityscap_STABLES = 722,
    HELP_cityscap_ANIMISTS_GUILD = 723,
    HELP_cityscap_FANTASTIC_STABLE = 724,
    HELP_cityscap_SHIP_WRIGHTS_GUILD = 725,
    HELP_cityscap_SHIPYARD = 726,
    HELP_cityscap_MARITIME_GUILD = 727,
    HELP_cityscap_SAWMILL = 728,
    HELP_cityscap_LIBRARY = 729,
    HELP_cityscap_SAGES_GUILD = 730,
    HELP_cityscap_ORACLE = 731,
    HELP_cityscap_ALCHEMISTS_GUILD = 732,
    HELP_cityscap_UNIVERSITY = 733,
    HELP_cityscap_WIZARDS_GUILD = 734,
    HELP_cityscap_SHRINE = 735,
    HELP_cityscap_TEMPLE = 736,
    HELP_cityscap_PARTHENON = 737,
    HELP_cityscap_CATHEDRAL = 738,
    HELP_cityscap_MARKETPLACE = 739,
    HELP_cityscap_BANK = 740,
    HELP_cityscap_MERCHANTS_GUILD = 741,
    HELP_cityscap_GRANARY = 742,
    HELP_cityscap_FARMERS_MARKET = 743,
    HELP_cityscap_FORESTERS_GUILD = 744,
    HELP_cityscap_BUILDERS_HALL = 745,
    HELP_cityscap_MECHANICIANS_GUILD = 746,
    HELP_cityscap_MINERS_GUILD = 747,
    HELP_CITYSCAP_CITY_WALLS = 748,
    HELP_cityscap_SUMMONING_CIRCLE = 749,
    HELP_cityscap_EARTH_GATE = 750,
    HELP_cityscap_STREAM_OF_LIFE = 751,
    HELP_cityscap_ASTRAL_GATE = 752,
    HELP_cityscap_FORTRESS = 753,
    HELP_cityscap_DARK_RITUALS = 754,
    HELP_cityscap_ALTAR_OF_BATTLE = 755,
    HELP_TREATIES = 756,
    HELP_SCROLL_PAGE_BACK = 757,
    HELP_SCROLL_PAGE_FORWARD = 758,
    HELP_spellscr_CANCEL = 759,
    HELP_EXTRA_MANA_COST = 760,
    HELP_NONE = 761,
    HELP_UNITVIEW_Adamantium_Weapon = 762,
    HELP_TAX_PER_POPULATION = 763,
    HELP_ADVISORS = 764,
    HELP_mirror_FAMOUS = 765,
    HELP_NIGHTSHADE = 766,
    HELP_COMPIX_MANA_LEAK = 767,
    HELP_PLAGUE = 768,
    HELP_POPULATION_BOOM = 769,
    HELP_BLUE_CONJUNCTION = 770,
    HELP_RED_CONJUNCTION = 771,
    HELP_GREEN_CONJUNCTION = 772,
    HELP_BAD_MOON = 773,
    HELP_GOOD_MOON = 774,
    HELP_MANA_SHORT = 775,
    HELP_POPULATION = 776,
    HELP_DEATH_SPELLS = 777,
    HELP_CHAOS_SPELLS = 778,
    HELP_LIFE_SPELLS = 779,
    HELP_NATURE_SPELLS = 780,
    HELP_SORCERY_SPELLS = 781,
    HELP_MANIACAL = 782,
    HELP_RUTHLESS = 783,
    HELP_AGGRESSIVE = 784,
    HELP_CHAOTIC = 785,
    HELP_LAWFUL = 786,
    HELP_PEACEFUL = 787,
    HELP_PRAGMATIST = 788,
    HELP_MILITARIST = 789,
    HELP_THEURGIST = 790,
    HELP_PERFECTIONIST = 791,
    HELP_EXPANSIONIST = 792,
    HELP_WIZARD_PACT = 793,
    HELP_ALLIANCE = 794,
    HELP_NO_REPORT = 795,
    HELP_MOVEMENT_ANIMATIONS = 796,
    HELP_COMPIX_BLUR = 797,
    HELP_HALL_OF_FAME = 798,
    HELP_SPELLS = 799,
    HELP_TOWNSFOLK = 800,
    HELP_BANISHED_WIZARDS = 801,
    HELP_FAME = 802,
    HELP_YEAR = 803,
    HELP_score_SPELL_OF_MASTERY = 804,
    HELP_FINAL_SCORE = 805,
    HELP_RAZE_CITY = 806,

    eHelpIndex_MAX = 1614
};

enum eHeroAbility
{
    HEROABILITY_Leadership = 0,       // 01
    HEROABILITY_Leadership_X = 1,     // 02
    HEROABILITY_2 = 2,                // 04
    HEROABILITY_Legendary = 3,        // 08
    HEROABILITY_Legendary_X = 4,      // 10
    HEROABILITY_5 = 5,                // 20
    HEROABILITY_Blademaster = 6,      // 40
    HEROABILITY_Blademaster_X = 7,    // 80

    HEROABILITY_8 = 8,                 // 01
    HEROABILITY_Armsmaster = 9,        // 02
    HEROABILITY_Armsmaster_X = 10,     // 04
    HEROABILITY_11 = 11,               // 08
    HEROABILITY_Constitution = 12,     // 10
    HEROABILITY_Constitution_X = 13,   // 20
    HEROABILITY_14 = 14,               // 40
    HEROABILITY_Might = 15,            // 80

    HEROABILITY_Might_X = 16,          // 01
    HEROABILITY_17 = 17,               // 02
    HEROABILITY_Arcane_Power = 18,     // 04
    HEROABILITY_Arcane_Power_X = 19,   // 08
    HEROABILITY_20 = 20,               // 10
    HEROABILITY_Sage = 21,             // 20
    HEROABILITY_Sage_X = 22,           // 40
    HEROABILITY_23 = 23,               // 80

    HEROABILITY_Prayermaster = 24,     // 01
    HEROABILITY_Prayermaster_X = 25,   // 02
    HEROABILITY_Agility_X = 26,        // 04
    HEROABILITY_Lucky = 27,            // 08
    HEROABILITY_Charmed = 28,          // 10
    HEROABILITY_Noble = 29,            // 20
    HEROABILITY_30 = 30,               // 40
    HEROABILITY_Agility = 31,          // 80

    eHeroAbility_MAX
};

enum eHero_Level_Status ENUMSIZE16
{
    HEROLEVELSTATUS_Level_1 = 0,
    HEROLEVELSTATUS_Level_2 = 1,
    HEROLEVELSTATUS_Level_3 = 2,
    HEROLEVELSTATUS_Level_4 = 3,
    HEROLEVELSTATUS_Level_5 = 4,
    HEROLEVELSTATUS_Level_6 = 5,
    HEROLEVELSTATUS_Level_7 = 6,
    HEROLEVELSTATUS_Level_8 = 7,
    HEROLEVELSTATUS_Level_9 = 8,
    HEROLEVELSTATUS_Dead_Level_1 = 0xFFFF,
    HEROLEVELSTATUS_Dead_Level_2 = 0xFFFE,
    HEROLEVELSTATUS_Dead_Level_3 = 0xFFFD,
    HEROLEVELSTATUS_Dead_Level_4 = 0xFFFC,
    HEROLEVELSTATUS_Dead_Level_5 = 0xFFFB,
    HEROLEVELSTATUS_Dead_Level_6 = 0xFFFA,
    HEROLEVELSTATUS_Dead_Level_7 = 0xFFF9,
    HEROLEVELSTATUS_Dead_Level_8 = 0xFFF8,
    HEROLEVELSTATUS_Dead_Level_9 = 0xFFF7,
    HEROLEVELSTATUS_Active_in_Wizards_army = 0xFFEC,

    eHero_Level_Status__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eHero_TypeCode ENUMSIZE8
{
    HEROTYPE_Fighter = 0,
    HEROTYPE_Bow = 1,
    HEROTYPE_Fighter_Wizard = 2,
    HEROTYPE_Wizard = 3,

    eHero_TypeCode_MAX
} PACKED_ENUM;

enum eHousing ENUMSIZE16
{
    HOUSING_wooden_house = 0,
    HOUSING_tree_house = 1,
    HOUSING_mud_hut = 2,

    eHousing_MAX,
    eHousing__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eItemPower
{
    ITEMPOWER_Vampiric = 0,
    ITEMPOWER_Guardian_Wind,
    ITEMPOWER_Lightning,
    ITEMPOWER_Cloak_Of_Fear,
    ITEMPOWER_Destruction,
    ITEMPOWER_Wraith_Form,
    ITEMPOWER_Regeneration,
    ITEMPOWER_Pathfinding,

    ITEMPOWER_Water_Walking,
    ITEMPOWER_Resist_Elements,
    ITEMPOWER_Elemental_Armour,
    ITEMPOWER_Chaos,
    ITEMPOWER_Stoning,
    ITEMPOWER_Endurance,
    ITEMPOWER_Haste,
    ITEMPOWER_Invisibility,

    ITEMPOWER_Death,
    ITEMPOWER_Flight,
    ITEMPOWER_Resist_Magic,
    ITEMPOWER_Magic_Immunity,
    ITEMPOWER_Flaming,
    ITEMPOWER_Holy_Avenger,
    ITEMPOWER_True_Sight,
    ITEMPOWER_Phantasmal,

    ITEMPOWER_Power_Drain,
    ITEMPOWER_Bless,
    ITEMPOWER_Lion_Heart,
    ITEMPOWER_Giant_Strength,
    ITEMPOWER_Planar_Travel,
    ITEMPOWER_Merging,
    ITEMPOWER_Righteousness,
    ITEMPOWER_Invulnerability,

    eItemPower_MAX
};

enum eItemPowerType ENUMSIZE8
{
    ITEMPOWERTYPE_Strength = 0,
    ITEMPOWERTYPE_Accuracy = 1,
    ITEMPOWERTYPE_Defense = 2,
    ITEMPOWERTYPE_Wizardry = 3,
    ITEMPOWERTYPE_Power = 4,
    ITEMPOWERTYPE_Speed = 5,
    ITEMPOWERTYPE_Protection = 6,
    ITEMPOWERTYPE_mutex_resist_elements = 7,
    ITEMPOWERTYPE_mutex_resist_magic = 8,
    ITEMPOWERTYPE_other_specials = 9,

    eItemPowerType_MAX,
    eItemPowerType__SIZE__ = 0xFF
} PACKED_ENUM;

enum eItem_Icon ENUMSIZE16
{
    Sword_1 = 0x00, Sword_2, Sword_3, Sword_4, Sword_5, Sword_6, Sword_7, Sword_8, Sword_9,
    Mace_01 = 0x09, Mace_02, Mace_03, Mace_04, Mace_05, Mace_06, Mace_07, Mace_08, Mace_09, Mace_10, Mace_11,
    Axe_1 = 0x14, Axe_2, Axe_3, Axe_4, Axe_5, Axe_6, Axe_7, Axe_8, Axe_9,
    Bow_1 = 0x1D, Bow_2, Bow_3, Bow_4, Bow_5, Bow_6, Bow_7, Bow_8, Bow_9,
    Staff_1 = 0x26, Staff_2, Staff_3, Staff_4, Staff_5, Staff_6, Staff_7, Staff_8, Staff_9,
    Chain_1 = 0x2F, Chain_2, Chain_3, Chain_4, Chain_5, Chain_6, Chain_7, Chain_8, 
    Plate_1 = 0x37, Plate_2, Plate_3, Plate_4, Plate_5, Plate_6, Plate_7,
    Shield_1 = 0x3E, Shield_2, Shield_3, Shield_4, Shield_5, Shield_6, Shield_7, Shield_8, Shield_9, Shield_10,
    Pendant_1 = 0x48, Pendant_2,
    Brooch_1 = 0x4A, Brooch_2, Brooch_3, Brooch_4,
    Ring_1 = 0x4E, Ring_2, Ring_3, Ring_4, Ring_5, Ring_6,
    Cloak_1 = 0x54, Cloak_2, Cloak_3, Cloak_4, Cloak_5, Cloak_6,
    Gauntlet_1 = 0x5A, Gauntlet_2, Gauntlet_3, Gauntlet_4,
    Helmet_1 = 0x5E, Helmet_2, Helmet_3, Helmet_4, Helmet_5, Helmet_6, Helmet_7,
    Orb_1 = 0x65, Orb_2, Orb_3, Orb_4, Orb_5, Orb_6,
    Wand_1 = 0x6B, Wand_2, Wand_3, Wand_4, Wand_5, Wand_6, Wand_7, Wand_8,

    eItem_Icon_MAX,
    eItem_Icon__SIZE__ = 0xFFFF
} PACKED_ENUM;


enum eItem_Type ENUMSIZE8
{
    ITEMTYPE_Sword,
    ITEMTYPE_Mace,
    ITEMTYPE_Axe,
    ITEMTYPE_Bow,
    ITEMTYPE_Staff,
    ITEMTYPE_Wand,
    ITEMTYPE_Various,
    ITEMTYPE_Shield,
    ITEMTYPE_Chain_Mail,
    ITEMTYPE_Plate_Mail,

    eItem_Type_MAX,
    eItem_Type__SIZE__ = 0xFF
} PACKED_ENUM;

enum eLand_Size ENUMSIZE16
{
    LANDSIZE_Small,
    LANDSIZE_Medium,
    LANDSIZE_Large,

    eLand_Size_MAX,
    eLand_Size__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eLand_Size140m ENUMSIZE16
{
    LANDSIZE140M_Small,
    LANDSIZE140M_Medium,
    LANDSIZE140M_Large,
    LANDSIZE140M_Huge,

    eLand_Size140m_MAX,
    eLand_Size140m__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eLand_Size150 ENUMSIZE16
{
    LANDSIZE150_Tiny,
    LANDSIZE150_Small,
    LANDSIZE150_Fair,
    LANDSIZE150_Large,
    LANDSIZE150_Huge,

    eLand_Size150_MAX,
    eLand_Size150__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eLand_Size60 ENUMSIZE16
{
    LANDSIZE60_Tiny,
    LANDSIZE60_Small,
    LANDSIZE60_Fair,
    LANDSIZE60_Large,
    LANDSIZE60_Huge,

    eLand_Size60_MAX,
    eLand_Size60__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eLevel ENUMSIZE8
{
    LEVEL_Level_1 = 0,
    LEVEL_Level_2 = 1,
    LEVEL_Level_3 = 2,
    LEVEL_Level_4 = 3,
    LEVEL_Level_5 = 4,
    LEVEL_Level_6 = 5,
    LEVEL_Level_7 = 6,
    LEVEL_Level_8 = 7,
    LEVEL_Level_9 = 8,

    eLevel_MAX
} PACKED_ENUM;

enum eMagic_Powerful ENUMSIZE16
{
    MAGICPOWERFUL_Weak,
    MAGICPOWERFUL_Normal,
    MAGICPOWERFUL_Powerful,

    eMagic_Powerful_MAX,
    eMagic_Powerful_Size__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eMagic_Powerful140m ENUMSIZE16
{
    MAGICPOWERFUL140M_0_5,
    MAGICPOWERFUL140M_1_0,
    MAGICPOWERFUL140M_1_5,
    MAGICPOWERFUL140M_2_0,
    MAGICPOWERFUL140M_2_5,

    eMagic_Powerful140m_MAX,
    eMagic_Powerful140m_Size__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eMagic_Powerful150 ENUMSIZE16
{
    MAGICPOWERFUL150_Weak,
    MAGICPOWERFUL150_Fair,
    MAGICPOWERFUL150_Good,
    MAGICPOWERFUL150_High,
    MAGICPOWERFUL150_Max,

    eMagic_Powerful150_MAX,
    eMagic_Powerful150_Size__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eMagic_Powerful60 ENUMSIZE16
{
    MAGICPOWERFUL60_Min,
    MAGICPOWERFUL60_Low,
    MAGICPOWERFUL60_Fair,
    MAGICPOWERFUL60_High,
    MAGICPOWERFUL60_Max,

    eMagic_Powerful60_MAX,
    eMagic_Powerful60_Size__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eMovement
{
    MOVEMENT_Unused,
    MOVEMENT_Walking,
    MOVEMENT_Forester,
    MOVEMENT_Mountaineer,
    MOVEMENT_Swimming,
    MOVEMENT_Sailing,

    eMovement_MAX
};

enum eTower_Node_Lair_Status ENUMSIZE8
{
    LAIRSTATUS_popped = 0,
    LAIRSTATUS_intact = 1,

    eTower_Node_Lair_Status_MAX
} PACKED_ENUM;

enum eTower_Node_Lair_Type ENUMSIZE8
{
    // As yet this list is incomplete
    LAIRTYPE_Tower = 0,             // random color if a book is awarded
    LAIRTYPE_Chaos_node = 1,
    LAIRTYPE_Nature_node = 2,
    LAIRTYPE_Sorcery_node = 3,
    LAIRTYPE_Mysterious_cave = 4,   // random color if a book is awarded
    LAIRTYPE_Dungeon = 5,           // death book if a book is awarded
    LAIRTYPE_Ancient_temple = 6,    // life book if a book is awarded
    LAIRTYPE_Abandoned_keep = 7,    // death book if a book is awarded
    LAIRTYPE_Monster_lair = 8,      // random color if a book is awarded
    LAIRTYPE_Ruins = 9,             // death book if a book is awarded
    LAIRTYPE_Fallen_temple = 10,    // life book if a book is awarded

    eTower_Node_Lair_Type_MAX
} PACKED_ENUM;

enum eNode_Type ENUMSIZE8
{
    NODETYPE_Sorcery = 0, 
    NODETYPE_Nature = 1,
    NODETYPE_Chaos = 2,

    eNode_Type_MAX
} PACKED_ENUM;

enum eObjective ENUMSIZE16
{
    OBJECTIVE_Pragmatist = 00,
    OBJECTIVE_Militarist = 01,
    OBJECTIVE_Theurgist = 02,
    OBJECTIVE_Perfectionist = 03,
    OBJECTIVE_Expansionist = 04,

    eObjective_MAX,
    eObjective__SIZE__ = 0xFFFF
} PACKED_ENUM;

// Owners are optional, as opposing to a player
enum eOwner ENUMSIZE8
{
    OWNER_None = 0,
    OWNER_YOU = 1,
    OWNER_1 = 2,
    OWNER_2 = 3,
    OWNER_3 = 4,
    OWNER_4 = 5,
    OWNER_Neutral = 6,

    eOwner_MAX,
    eOwner__SIZE__ = 0xFF
} PACKED_ENUM;

enum ePersonality ENUMSIZE16
{
    PERSONALITY_Maniacal = 00,
    PERSONALITY_Ruthless = 01,
    PERSONALITY_Aggressive = 02,
    PERSONALITY_Chaotic = 03,
    PERSONALITY_Lawful = 04,
    PERSONALITY_Peaceful = 05,

    ePersonality_MAX,
    ePersonality__SIZE__ = 0xFFFF
} PACKED_ENUM;

// A player is always given, as opposing to an owner,
enum ePlayer ENUMSIZE8
{
    PLAYER_Dismissed_Deceased = 0xFF,
    PLAYER_YOU = 0,
    PLAYER_1 = 1,
    PLAYER_2 = 2,
    PLAYER_3 = 3,
    PLAYER_4 = 4,
    PLAYER_NEUTRAL = 5,

    ePlayer_MAX,
    ePlayer__SIZE__ = 0xFF
} PACKED_ENUM;

enum ePlane ENUMSIZE8
{
    PLANE_Dismissed_Deceased = 0xFF,
    PLANE_Limbo = 9,    // To become undead

    PLANE_Arcanum = 0,
    PLANE_Myrror = 1,

    ePlane_MAX,
    ePlane__SIZE__ = 0xFF
} PACKED_ENUM;

enum ePlane16 ENUMSIZE16
{
    PLANE16_Arcanum = 0,
    PLANE16_Myrror = 1,

    ePlane16_MAX,
    ePlane16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum ePortrait ENUMSIZE8
{
    PORTRAIT_Merlin = 0,
    PORTRAIT_Raven = 1,
    PORTRAIT_Sharee = 2,
    PORTRAIT_Lo_Pan = 3,
    PORTRAIT_Jafar = 4,
    PORTRAIT_Oberic = 5,
    PORTRAIT_Rjak = 6,
    PORTRAIT_Sssra = 7,
    PORTRAIT_Tauron = 8,
    PORTRAIT_Freya = 9,
    PORTRAIT_Horus = 10,
    PORTRAIT_Ariel = 11,
    PORTRAIT_Tlaloc = 12,
    PORTRAIT_Kali = 13,
    PORTRAIT_Custom = 14,

    ePortrait_MAX,
    ePortrait_SIZE__ = 0xFF
} PACKED_ENUM;

enum eProducing ENUMSIZE16
{
    PRODUCING_Computer = 0xFFFC,
    PRODUCING_None = 0,
    
    PRODUCING_Trade_Goods = 1,
    PRODUCING_Housing = 2,
    PRODUCING_Barracks = 3,
    PRODUCING_Armory = 4,
    PRODUCING_Fighters_Guild = 5,
    PRODUCING_Armorers_Guild = 6,
    PRODUCING_War_College = 7,
    PRODUCING_Smithy = 8,
    PRODUCING_Stable = 9,
    PRODUCING_Animists_Guild = 10,
    PRODUCING_Fantastic_Stable = 11,
    PRODUCING_Shipwright_Guild = 12,
    PRODUCING_Ship_Yard = 13,
    PRODUCING_Maritime_Guild = 14,
    PRODUCING_Sawmill = 15,
    PRODUCING_Library = 16,
    PRODUCING_Sages_Guild = 17,
    PRODUCING_Oracle = 18,
    PRODUCING_Alchemist_Guild = 19,
    PRODUCING_University = 20,
    PRODUCING_Wizards_Guild = 21,
    PRODUCING_Shrine = 22,
    PRODUCING_Temple = 23,
    PRODUCING_Parthenon = 24,
    PRODUCING_Cathedral = 25,
    PRODUCING_Marketplace = 26,
    PRODUCING_Bank = 27,
    PRODUCING_Merchants_Guild = 28,
    PRODUCING_Granary = 29,
    PRODUCING_Farmers_Market = 30,
    PRODUCING_Foresters_Guild = 31,
    PRODUCING_Builders_Hall = 32,
    PRODUCING_Mechanicians_Guild = 33,
    PRODUCING_Miners_Guild = 34,
    PRODUCING_City_Walls = 35,
    eProducing_Building_MAX,

    //  * Inanimate units
    PRODUCING_Trireme = 135,
    PRODUCING_Galley = 136,
    PRODUCING_Catapult = 137,
    PRODUCING_Warship = 138,

    //  * 115 Racial units (Sorted by race, then by power)

    PRODUCING_Barbarian_Spearmen,
    PRODUCING_Barbarian_Swordsmen,
    PRODUCING_Barbarian_Bowmen,
    PRODUCING_Barbarian_Cavalry,
    PRODUCING_Barbarian_Shaman,
    PRODUCING_Barbarian_Settlers,
    PRODUCING_Barbarian_Berserkers,

    PRODUCING_Beastmen_Spearmen,
    PRODUCING_Beastmen_Swordsmen,
    PRODUCING_Beastmen_Halberdiers,
    PRODUCING_Beastmen_Bowmen,
    PRODUCING_Beastmen_Priests,
    PRODUCING_Beastmen_Magicians,
    PRODUCING_Beastmen_Engineers,
    PRODUCING_Beastmen_Settlers,
    PRODUCING_Beastmen_Centaurs,
    PRODUCING_Beastmen_Manticores,
    PRODUCING_Beastmen_Minotaurs,

    PRODUCING_Dark_Elf_Spearmen,
    PRODUCING_Dark_Elf_Swordsmen,
    PRODUCING_Dark_Elf_Halberdiers,
    PRODUCING_Dark_Elf_Cavalry,
    PRODUCING_Dark_Elf_Priests,
    PRODUCING_Dark_Elf_Settlers,
    PRODUCING_Dark_Elf_Nightblades,
    PRODUCING_Dark_Elf_Warlocks,
    PRODUCING_Dark_Elf_Nightmares,

    PRODUCING_Draconian_Spearmen,
    PRODUCING_Draconian_Swordsmen,
    PRODUCING_Draconian_Halberdiers,
    PRODUCING_Draconian_Bowmen,
    PRODUCING_Draconian_Shaman,
    PRODUCING_Draconian_Magicians,
    PRODUCING_Draconian_Engineers,
    PRODUCING_Draconian_Settlers,
    PRODUCING_Draconian_Doom_Drakes,
    PRODUCING_Draconian_Air_Ship,

    PRODUCING_Dwarven_Swordsmen,
    PRODUCING_Dwarven_Halberdiers,
    PRODUCING_Dwarven_Engineers,
    PRODUCING_Dwarven_Hammerhands,
    PRODUCING_Dwarven_Steam_Cannon,
    PRODUCING_Dwarven_Golem,
    PRODUCING_Dwarven_Settlers,

    PRODUCING_Gnoll_Spearmen,
    PRODUCING_Gnoll_Swordsmen,
    PRODUCING_Gnoll_Halberdiers,
    PRODUCING_Gnoll_Bowmen,
    PRODUCING_Gnoll_Settlers,
    PRODUCING_Gnoll_Wolf_Riders,

    PRODUCING_Halfling_Spearmen,
    PRODUCING_Halfling_Swordsmen,
    PRODUCING_Halfling_Bowmen,
    PRODUCING_Halfling_Shaman,
    PRODUCING_Halfling_Settlers,
    PRODUCING_Halfling_Slingers,

    PRODUCING_High_Elf_Spearmen,
    PRODUCING_High_Elf_Swordsmen,
    PRODUCING_High_Elf_Halberdiers,
    PRODUCING_High_Elf_Cavalry,
    PRODUCING_High_Elf_Magicians,
    PRODUCING_High_Elf_Settlers,
    PRODUCING_High_Elf_Longbowmen,
    PRODUCING_High_Elf_Elven_Lords,
    PRODUCING_High_Elf_Pegasai,

    PRODUCING_High_Men_Spearmen,
    PRODUCING_High_Men_Swordsmen,
    PRODUCING_High_Men_Bowmen,
    PRODUCING_High_Men_Cavalry,
    PRODUCING_High_Men_Priests,
    PRODUCING_High_Men_Magicians,
    PRODUCING_High_Men_Engineers,
    PRODUCING_High_Men_Settlers,
    PRODUCING_High_Men_Pikemen,
    PRODUCING_High_Men_Paladins,

    PRODUCING_Klackon_Spearmen,
    PRODUCING_Klackon_Swordsmen,
    PRODUCING_Klackon_Halberdiers,
    PRODUCING_Klackon_Engineers,
    PRODUCING_Klackon_Settlers,
    PRODUCING_Klackon_Stag_Beetle,

    PRODUCING_Lizardman_Spearmen,
    PRODUCING_Lizardman_Swordsmen,
    PRODUCING_Lizardman_Halberdiers,
    PRODUCING_Lizardman_Javelineers,
    PRODUCING_Lizardman_Shaman,
    PRODUCING_Lizardman_Settlers,
    PRODUCING_Lizardman_Dragon_Turtle,

    PRODUCING_Nomad_Spearmen,
    PRODUCING_Nomad_Swordsmen,
    PRODUCING_Nomad_Bowmen,
    PRODUCING_Nomad_Priests,
    PRODUCING_Nomad_Magicians,
    PRODUCING_Nomad_Settlers,
    PRODUCING_Nomad_Horsebowmen,
    PRODUCING_Nomad_Pikemen,
    PRODUCING_Nomad_Rangers,
    PRODUCING_Nomad_Griffins,

    PRODUCING_Orc_Spearmen,
    PRODUCING_Orc_Swordsmen,
    PRODUCING_Orc_Halberdiers,
    PRODUCING_Orc_Bowmen,
    PRODUCING_Orc_Cavalry,
    PRODUCING_Orc_Shaman,
    PRODUCING_Orc_Magicians,
    PRODUCING_Orc_Engineers,
    PRODUCING_Orc_Settlers,
    PRODUCING_Orc_Wyvern_Riders,

    PRODUCING_Troll_Spearmen,
    PRODUCING_Troll_Swordsmen,
    PRODUCING_Troll_Halberdiers,
    PRODUCING_Troll_Shaman,
    PRODUCING_Troll_Settlers,
    PRODUCING_Troll_War_Trolls,
    PRODUCING_Troll_War_Mammoths,
    
    eProducing_MAX,
    eProducing__SIZE__ = 0xFFFF
} PACKED_ENUM;


enum eRace ENUMSIZE8
{
    RACE_Barbarian = 0,
    RACE_Beastmen = 1,
    RACE_Dark_Elf = 2,
    RACE_Draconian = 3,
    RACE_Dwarven = 4,
    RACE_Gnoll = 5,
    RACE_Halfling = 6,
    RACE_High_Elf = 7,
    RACE_High_Men = 8,
    RACE_Klackon = 9,
    RACE_Lizardman = 10,
    RACE_Nomad = 11,
    RACE_Orc = 12,
    RACE_Troll = 13,         // Maximum in SaveGame and in Race_Data (gMAX_RACES-1)

    // Extra used in WizardsExe
    RACE_Generic = 0x0E,
    RACE_Arcane = 0x0F,
    RACE_Nature = 0x10,
    RACE_Sorcery = 0x11,
    RACE_Chaos = 0x12,
    RACE_Life = 0x13,
    RACE_Death = 0x14,

    eRace_MAX,
    eRace__SIZE__ = 0xFF
} PACKED_ENUM;

enum eRandomPickType ENUMSIZE16 {
    RANDOMPICK_Random_Ftr,
    RANDOMPICK_Random_Mage,
    RANDOMPICK_Random_Any,

    eRandomPickType_MAX,
    eRandomPickType__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eRanged_Type ENUMSIZE8 {
    RANGED_None = 0xFF,

    // 10-19 Rocks
    RANGED_Rock = 10,

    // 20-29 Missiles
    RANGED_Arrow = 20,
    RANGED_Bullet = 21,                     // Slingers

    // 30-39 Magic ranged attack
    RANGED_Chaos_Lightning = 30,            // Warlock, Chaos Warrior, Storm Giant
    RANGED_Chaos_Fireball = 31,             // Warrior Mage, Magician, Witch, Golden One, Unknown, Magicians, Warlocks
    RANGED_Sorcery_Illusion = 32,           // Sage, Illusionist
    RANGED_Chaos_Deathbolt = 33,            // Necromancer, Shadow_Demons, Demon_Lord
    RANGED_Nature_Icebolt = 34,             // Wind_Mage
    RANGED_Nature_Priest = 35,              // Healer, Priestess, Shamans, Priests
    RANGED_Chaos_Drow = 36,                 // Dark Elves (Spearmen, Swordsmen, Halberdiers, Cavalry, Nightmares)
    RANGED_Nature_Shimmer = 37,             // Sprites
    RANGED_Nature_Green = 38,               // Druid

    // 100-109 Thrown/Breath/Gaze
    RANGED_Thrown_Weapons = 100,
    RANGED_Fire_Breath = 101,               // (Chaos)
    RANGED_Lightning_Breath = 102,          // (Chaos)  Sky_Drake
    RANGED_Stoning_Gaze = 103,              // (Nature) Basilisk, Gorgons (resistance modifier in byte 17)
    RANGED_Multiple_Gaze = 104,             // (Chaos)  Chaos Spawn
                                            //          This is actually 3 gazes, including stoning and death
    RANGED_Death_Gaze = 105,                // (Death)  Night stalker (resistance modifier in byte 17)

    eRanged_Type_MAX
} PACKED_ENUM;

enum eRarity
{
    RARITY_Common = 0,
    RARITY_Uncommon = 1,
    RARITY_Rare = 2,
    RARITY_Very_Rare = 3,
    eRarity_MAX
};

enum eRealm_Type ENUMSIZE8
{
    REALM_None = 0xFF,

    REALM_Nature = 0,
    REALM_Sorcery = 1,
    REALM_Chaos = 2,
    REALM_Life = 3,
    REALM_Death = 4,
    REALM_Arcane = 5,

    eRealm_Type_MAX
} PACKED_ENUM;

enum eRealm_Type16 ENUMSIZE16
{
    REALM16_None = 0xFF,

    REALM16_Nature = 0,
    REALM16_Sorcery = 1,
    REALM16_Chaos = 2,
    REALM16_Life = 3,
    REALM16_Death = 4,
    REALM16_Arcane = 5,

    eRealm_Type16_MAX,
    eRealm_Type16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eResource
{
    RESOURCE_None,

    RESOURCE_Food,
    RESOURCE_Production,
    RESOURCE_Gold,
    RESOURCE_Power,
    RESOURCE_Mana,
    RESOURCE_Research,
    RESOURCE_Gray_Food,
    RESOURCE_Gray_Gold,
    RESOURCE_10_Food,
    RESOURCE_10_Production,
    RESOURCE_10_Gold,
    RESOURCE_10_Power,
    RESOURCE_10_Mana,
    RESOURCE_10_Research,
    RESOURCE_10_Gray_Food,
    RESOURCE_10_Gray_Gold,

    eResource_MAX
};

enum eReward_Specials ENUMSIZE8
{
    REWARD_No_specials = 0,
    REWARD_Common_Spell = 1,
    REWARD_Uncommon_Spell = 2,
    REWARD_Rare_Spell = 3,
    REWARD_Very_Rare_Spell = 4,
    REWARD_1_Spellbook_or_Skill = 5,    //5 = 1 Spellbook or Skill (74% - 26%)
    REWARD_2_Spellbooks_or_Skills = 6,   //6 = 2 Spellbooks or Skills (74% - 26%)
    eReward_Specials_MAX
} PACKED_ENUM;

BETTER_ENUM(eSkill, uint8_t,
    Alchemy,
    Warlord,
    Chaos_Master,
    Nature_Master,
    Sorcery_Master,
    Infernal_Power,
    Divine_Power,
    Sage_Master,
    Channeller,
    Myrran,
    Archmage,
    Mana_Focusing,
    Node_Mastery,
    Famous,
    Runemaster,
    Conjurer,
    Charismatic,
    Artificer
)

const unsigned eSkill_MAX = eSkill::_size();

enum eSkill16 ENUMSIZE16
{
    SKILL16_None = 0xFFFF,
    SKILL16_Alchemy = 0,
    SKILL16_Warlord,
    SKILL16_Chaos_Master,
    SKILL16_Nature_Master,
    SKILL16_Sorcery_Master,
    SKILL16_Infernal_Power,
    SKILL16_Divine_Power,
    SKILL16_Sage_Master,
    SKILL16_Channeller,
    SKILL16_Myrran,
    SKILL16_Archmage,
    SKILL16_Mana_Focusing,
    SKILL16_Node_Mastery,
    SKILL16_Famous,
    SKILL16_Runemaster,
    SKILL16_Conjurer,
    SKILL16_Charismatic,
    SKILL16_Artificer,
    eSkill16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eSlot_Type8 ENUMSIZE8
{
    SLOT8_Sword = 1,
    SLOT8_Bow = 2,
    SLOT8_Sword_Staff_Wand = 3,
    SLOT8_Staff_Wand = 4,
    SLOT8_Armor_Shield = 5,
    SLOT8_Amulet = 6,

    eSlot_Type8_MAX,
    eSlot_Type8__SIZE__ = 0xFF
} PACKED_ENUM;

enum eSlot_Type16 ENUMSIZE16
{
    SLOT16_Sword = 1,
    SLOT16_Bow = 2,
    SLOT16_Sword_Staff_Wand = 3,
    SLOT16_Staff_Wand = 4,
    SLOT16_Armor_Shield = 5,
    SLOT16_Amulet = 6,

    eSlot_Type16_MAX,
    eSlot_Type16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eSpellCategory ENUMSIZE8
{
    SPELLCATEGORY_Normal_summon = 0x00,     // normal summoning spell (all but hero/champion/torin). Byte 0x20 contains the creature summoned, 21-23 are blank.
    SPELLCATEGORY_Unit_enchantment = 0x01,  // enchantment that can be cast on a friendly unit. Bytes 0x20-0x23 contain the mask for the enchantment applied (same as bytes 0x18-0x1B in the unit data structure in the save file). While no current spells do this, it is possible to set more than one bit; doing so will apply both spells, and will also prevent casting if either spell is already on the unit.
    SPELLCATEGORY_Friendly_City_enchantment = 0x02,  // enchantment hits a friendly city. Byte 0x20 contains the code for the city enchantment, in the same order as for city enchantments in the save file, but starts at 0 (0x00 is wall of fire, 0x18 is altar of battle).
    SPELLCATEGORY_Hostile_City_enchantment = 0x03,  // enchantment hits a hostile city. Byte 0x20 is the same as above.
    SPELLCATEGORY_Fixed_damage = 0x04,      // non-moddable damaging spell. Byte 0x20 is the magnitude, 0x21 is the immunity flags that apply (same as for monsters), 0x22 and 0x23 are attack flags (same as for monsters, but for byte 0x23, 0x80, listed as no effect for monsters, makes it act like warp lightning, and 0x10 makes it act like fireball. Neither flag seems to apply to critter attacks, though I haven't tried all possible combinations. Not all attack flags work for spells).
    SPELLCATEGORY_Special = 0x05,           // unusual spells. I haven't been able to determine how they're parsed, and there are a lot of them.
    SPELLCATEGORY_Target_wizard = 0x06,     // spells that target one wizard. 0x20 appears to be the effect (there are only four spells in this category: spell blast, cruel unminding, drain power, subversion).
    SPELLCATEGORY_Global_enchantment = 0x09,// global enchantment. Byte 0x20 contains the code for the global enchantment, in the same order as for global enchantments in the save file, but starts at 0 (0x00 is eternal night, 0x17 is awareness).
    SPELLCATEGORY_Battle = 0x0a,            // combat globals (affect all units). Mostly, it's twice the number of the enchantment (if you want, you can add +1, and apply spells to the other side), but the instants (Call Chaos, Death Spell, Holy Word, Mass Healing, Flame Strike) would appear to be special case coded. If byte 0x21 is set, it seems to just make the spell not work in most cases (cloning flame strike does not work). Enchantment codes are:
                                            //    00: true light
                                            //    02: darkness
                                            //    04: warp reality
                                            //    06: black prayer
                                            //    08: wrack
                                            //    0a: metal fires
                                            //    0c: prayer
                                            //    0e: high prayer
                                            //    10: terror
                                            //    12: call lightning
                                            //    14: counter magic (which is a different bit; probably says moddable/not moddable)
                                            //    16: mass invisibility
                                            //    18: entangle
                                            //    1a: mana leak
                                            //    1c: blur
    SPELLCATEGORY_Create_item = 0x0b,       // enchant item and create artifact.
    SPELLCATEGORY_Destroy_unit = 0x0c,      // spells that destroy a unit. Byte 0x21 looks like it may be a normal immunity byte, as it's 0x22 for petrify, 0x20 for disintegrate, but there's missing information here.
    SPELLCATEGORY_Resistable_Combat_enchantment = 0x0d,// hostile (resisted) combat enchantments. Bytes 0x20-0x21 are a mask for what effect is created:
                                            //    0x0001: Warp Creature (Defense)
                                            //    0x0002: Warp Creature (Resistance)
                                            //    0x0004: Mind Twist, which is a status effect that uses the text for Mana Leak. The actual effect is apparently -1 to hit, -1 resistance (either won't reduce below 0, or the UI doesn't show negative attack modifiers).
                                            //    0x0008: Haste (yeah, it's not a negative effect). It looks like combat-only enchantments use a different set of bits.
                                            //    0x0010: Web. Applies the debuff, but either does nothing, or only removes flight (cannot determine on my test monster).
                                            //    0x0020: Creature Binding. Puts a counter on the unit, but doesn't actually change allegiance.
                                            //    0x0040: Possession. Puts a counter on the unit, but doesn't actually change allegiance.
                                            //    0x0080: No Apparent Effect
                                            //    0x0100: Vertigo
                                            //    0x0200: Confusion
                                            //    0x0400: Whirlwind (???) Puts a whirlwind counter on the unit, no apparent effect (may be related to magic vortex)
                                            //    0x0800: Mind Storm
                                            //    0x1000: Shatter
                                            //    0x2000: Weakness
                                            //    0x4000: Black Sleep
                                            //    0x8000: Warp Creature (-50% attack)
    SPELLCATEGORY_Unresistable_Combat_enchantment = 0x0e,   // hostile (un-resisted) combat enchantments. Otherwise as 0x0d
    SPELLCATEGORY_Mundane_Unit_enchantment = 0x0f,          // friendly unit enchantment that only works on mundane units. Otherwise identical to 0x01.
    SPELLCATEGORY_Mundane_Combat_enchantment = 0x10,        // hostile (resisted) combat enchantments that only work on mundane units.
    SPELLCATEGORY_Dispel = 0x12,            // dispel magic, dispel magic true. Max cost = base cost * 5.
    SPELLCATEGORY_Disenchant = 0x13,        // disenchant area, disenchant true. Max cost = base cost * 5.
    SPELLCATEGORY_Disjunction = 0x14,       // disjunction, disjunction true. Max cost = base cost * 5.
    SPELLCATEGORY_Counter = 0x15,           // counter magic. Max cost = base cost * 5. Functions otherwise like 0x0a, so it's likely that other area spells could be modded, though effects would likely be undefined.
    SPELLCATEGORY_Variable_damage = 0x16,   // variable cost damage spells; max cost = base cost * 5. Otherwise identical to 0x04. The extra effect per extra mana spent is probably determined by the flags, it isn't coded here.
                                            // Life Drain, Fire Bolt, Ice Bolt, Lightning Bolt, Psionic Blast, Fireball
                                            //      base spell strength + (mana - cost)
    SPELLCATEGORY_Banish = 0x17,

    eSpellCategory_MAX
} PACKED_ENUM;


enum eSpellKnown ENUMSIZE8
{
    SPELLKNOWN_unknown = 0,
    SPELLKNOWN_knowable = 1,
    SPELLKNOWN_currently_known = 2,
    SPELLKNOWN_researchable = 3,
    eSpellKnown_MAX
} PACKED_ENUM;

enum eSpellKnown16 ENUMSIZE16 
{
    SPELLKNOWN16_unknown = 0,
    SPELLKNOWN16_knowable = 1,
    SPELLKNOWN16_currently_known = 2,
    SPELLKNOWN16_researchable = 3,
    eSpellKnown16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eSpell_Type ENUMSIZE8
{
    SPELLTYPE_Special_Spells = 0,
    SPELLTYPE_Summoning = 1,
    SPELLTYPE_Enchantment = 2,
    SPELLTYPE_City_Spells = 3,
    SPELLTYPE_Unit_Spells = 4,
    SPELLTYPE_Combat_Spells = 5,
    eSpell_Type_MAX
} PACKED_ENUM;

enum eSpell ENUMSIZE8
{
    SPELL_None = 0,
    
    //    * NATURE
    SPELL_Earth_to_Mud = 1,
    SPELL_Resist_Elements = 2,
    SPELL_Wall_of_Stone = 3,
    SPELL_Giant_Strength = 4,
    SPELL_Web  = 5,
    SPELL_War_Bears  = 6,
    SPELL_Stone_Skin  = 7,
    SPELL_Water_Walking  = 8,
    SPELL_Sprites  = 9,
    SPELL_Earth_Lore  = 10,

    SPELL_Cracks_Call  = 11,
    SPELL_Natures_Eye  = 12,
    SPELL_Ice_Bolt  = 13,
    SPELL_Giant_Spiders  = 14,
    SPELL_Change_Terrain  = 15,
    SPELL_Path_Finding  = 16,
    SPELL_Cockatrices  = 17,
    SPELL_Transmute  = 18,
    SPELL_Natures_Cures  = 19,
    SPELL_Basilisk  = 20,

    SPELL_Elemental_Armor  = 21,
    SPELL_Petrify  = 22,
    SPELL_Stone_Giant  = 23,
    SPELL_Iron_Skin  = 24,
    SPELL_Ice_Storm  = 25,
    SPELL_Earthquake  = 26,
    SPELL_Gorgons  = 27,
    SPELL_Move_Fortress  = 28,
    SPELL_Gaias_Blessing  = 29,
    SPELL_Earth_Elemental  = 30,

    SPELL_Regeneration  = 31,
    SPELL_Behemoth  = 32,
    SPELL_Entangle  = 33,
    SPELL_Nature_Awareness  = 34,
    SPELL_Call_Lightning  = 35,
    SPELL_Colossus  = 36,
    SPELL_Earth_Gate  = 37,
    SPELL_Herb_Mastery  = 38,
    SPELL_Great_Wyrm  = 39,
    SPELL_Natures_Wrath  = 40,

    //    * SORCERY
    SPELL_Resist_Magic  = 41,
    SPELL_Dispel_Magic_True  = 42,
    SPELL_Floating_Island  = 43,
    SPELL_Guardian_Wind  = 44,
    SPELL_Phantom_Warriors  = 45,
    SPELL_Confusion  = 46,
    SPELL_Word_of_Recall  = 47,
    SPELL_Counter_Magic  = 48,
    SPELL_Nagas  = 49,
    SPELL_Psionic_Blast  = 50,

    SPELL_Blur  = 51,
    SPELL_Disenchant_True  = 52,
    SPELL_Vertigo  = 53,
    SPELL_Spell_Lock  = 54,
    SPELL_Enchant_Road  = 55,
    SPELL_Flight  = 56,
    SPELL_Wind_Mastery  = 57,
    SPELL_Spell_Blast  = 58,
    SPELL_Aura_of_Majesty  = 59,
    SPELL_Phantom_Beast  = 60,

    SPELL_Disjunction_True  = 61,
    SPELL_Invisibility  = 62,
    SPELL_Wind_Walking  = 63,
    SPELL_Banish  = 64,
    SPELL_Storm_Giant  = 65,
    SPELL_Air_Elemental  = 66,
    SPELL_Mind_Storm  = 67,
    SPELL_Stasis  = 68,
    SPELL_Magic_Immunity  = 69,
    SPELL_Haste  = 70,

    SPELL_Djinn  = 71,
    SPELL_Spell_Ward  = 72,
    SPELL_Creature_Binding  = 73,
    SPELL_Mass_Invisibility  = 74,
    SPELL_Great_Unsummoning  = 75,
    SPELL_Spell_Binding  = 76,
    SPELL_Flying_Fortress  = 77,
    SPELL_Sky_Drake  = 78,
    SPELL_Suppress_Magic  = 79,
    SPELL_Time_Stop  = 80,

    //    * CHAOS
    SPELL_Warp_Wood  = 81,
    SPELL_Disrupt  = 82,
    SPELL_Fire_Bolt  = 83,
    SPELL_Hell_Hounds  = 84,
    SPELL_Corruption  = 85,
    SPELL_Eldritch_Weapon  = 86,
    SPELL_Wall_of_Fire  = 87,
    SPELL_Shatter  = 88,
    SPELL_Warp_Creature  = 89,
    SPELL_Fire_Elemental  = 90,

    SPELL_Lightning_Bolt  = 91,
    SPELL_Fire_Giant  = 92,
    SPELL_Chaos_Channels  = 93,
    SPELL_Flame_Blade  = 94,
    SPELL_Gargoyles  = 95,
    SPELL_Fireball  = 96,
    SPELL_Doombat  = 97,
    SPELL_Raise_Volcano  = 98,
    SPELL_Immolation  = 99,
    SPELL_Chimeras  = 100,

    SPELL_Warp_Lightning  = 101,
    SPELL_Metal_Fires  = 102,
    SPELL_Chaos_Spawn  = 103,
    SPELL_Doom_Bolt  = 104,
    SPELL_Magic_Vortex  = 105,
    SPELL_Efreet  = 106,
    SPELL_Fire_Storm  = 107,
    SPELL_Warp_Reality  = 108,
    SPELL_Flame_Strike  = 109,
    SPELL_Chaos_Rift  = 110,

    SPELL_Hydra  = 111,
    SPELL_Disintegrate  = 112,
    SPELL_Meteor_Storm  = 113,
    SPELL_Great_Wasting  = 114,
    SPELL_Call_Chaos  = 115,
    SPELL_Chaos_Surge  = 116,
    SPELL_Doom_Mastery  = 117,
    SPELL_Great_Drake  = 118,
    SPELL_Call_The_Void  = 119,
    SPELL_Armageddon  = 120,

    //    * LIFE
    SPELL_Bless  = 121,
    SPELL_Star_Fires  = 122,
    SPELL_Endurance  = 123,
    SPELL_Holy_Weapon  = 124,
    SPELL_Healing  = 125,
    SPELL_Holy_Armor  = 126,
    SPELL_Just_Cause  = 127,
    SPELL_True_Light  = 128,
    SPELL_Guardian_Spirit  = 129,
    SPELL_Heroism  = 130,

    SPELL_True_Sight  = 131,
    SPELL_Plane_Shift  = 132,
    SPELL_Resurrection  = 133,
    SPELL_Dispel_Evil  = 134,
    SPELL_Planar_Seal  = 135,
    SPELL_Unicorns  = 136,
    SPELL_Raise_Dead  = 137,
    SPELL_Planar_Travel  = 138,
    SPELL_Heavenly_Light  = 139,
    SPELL_Prayer  = 140,

    SPELL_Lionheart  = 141,
    SPELL_Incarnation  = 142,
    SPELL_Invulnerability  = 143,
    SPELL_Righteousness  = 144,
    SPELL_Prosperity  = 145,
    SPELL_Altar_of_Battle  = 146,
    SPELL_Angel  = 147,
    SPELL_Stream_of_Life  = 148,
    SPELL_Mass_Healing  = 149,
    SPELL_Holy_Word  = 150,

    SPELL_High_Prayer  = 151,
    SPELL_Inspirations  = 152,
    SPELL_Astral_Gate  = 153,
    SPELL_Holy_Arms  = 154,
    SPELL_Consecration  = 155,
    SPELL_Life_Force  = 156,
    SPELL_Tranquility  = 157,
    SPELL_Crusade  = 158,
    SPELL_Arch_Angel  = 159,
    SPELL_Charm_of_Life  = 160,

    //    * DEATH
    SPELL_Skeletons  = 161,
    SPELL_Weakness  = 162,
    SPELL_Dark_Rituals  = 163,
    SPELL_Cloak_of_Fear  = 164,
    SPELL_Black_Sleep  = 165,
    SPELL_Ghouls  = 166,
    SPELL_Life_Drain  = 167,
    SPELL_Terror  = 168,
    SPELL_Darkness  = 169,
    SPELL_Mana_Leak  = 170,

    SPELL_Drain_Power  = 171,
    SPELL_Possession  = 172,
    SPELL_Lycantrophy  = 173,
    SPELL_Black_Prayer  = 174,
    SPELL_Black_Channels  = 175,
    SPELL_Night_Stalker  = 176,
    SPELL_Subversion  = 177,
    SPELL_Wall_of_Darkness  = 178,
    SPELL_Berserk  = 179,
    SPELL_Shadow_Demons  = 180,

    SPELL_Wraith_Form  = 181,
    SPELL_Wrack  = 182,
    SPELL_Evil_Presence  = 183,
    SPELL_Wraiths  = 184,
    SPELL_Cloud_Of_Shadow  = 185,
    SPELL_Warp_Node  = 186,
    SPELL_Black_Wind  = 187,
    SPELL_Zombie_Mastery  = 188,
    SPELL_Famine  = 189,
    SPELL_Cursed_Lands  = 190,

    SPELL_Cruel_Unminding  = 191,
    SPELL_Word_of_Death  = 192,
    SPELL_Death_Knights  = 193,
    SPELL_Death_Spell  = 194,
    SPELL_Animate_dead  = 195,
    SPELL_Pestilence  = 196,
    SPELL_Eternal_Night  = 197,
    SPELL_Evil_Omens  = 198,
    SPELL_Death_Wish  = 199,
    SPELL_Demon_Lord  = 200,

    //    * ARCANE = 1,
    SPELL_Magic_Spirit  = 201,
    SPELL_Dispel_Magic  = 202,
    SPELL_Summoning_Circle  = 203,
    SPELL_Disenchant_Area  = 204,
    SPELL_Recall_Hero  = 205,
    SPELL_Detect_Magic  = 206,
    SPELL_Enchant_Item  = 207,
    SPELL_Summon_Hero  = 208,
    SPELL_Awareness  = 209,
    SPELL_Disjunction  = 210,
    SPELL_Create_Artifact  = 211,
    SPELL_Summon_Champion  = 212,
    SPELL_Spell_Of_Mastery  = 213,
    SPELL_Spell_Of_Return  = 214,

    eSpell_MAX
} PACKED_ENUM;

enum eSpell16 ENUMSIZE16
{
    SPELL16_None = 0,

    //    * NATURE
    SPELL16_Earth_to_Mud = 1,
    SPELL16_Resist_Elements = 2,
    SPELL16_Wall_of_Stone = 3,
    SPELL16_Giant_Strength = 4,
    SPELL16_Web  = 5,
    SPELL16_War_Bears  = 6,
    SPELL16_Stone_Skin  = 7,
    SPELL16_Water_Walking  = 8,
    SPELL16_Sprites  = 9,
    SPELL16_Earth_Lore  = 10,

    SPELL16_Cracks_Call  = 11,
    SPELL16_Natures_Eye  = 12,
    SPELL16_Ice_Bolt  = 13,
    SPELL16_Giant_Spiders  = 14,
    SPELL16_Change_Terrain  = 15,
    SPELL16_Path_Finding  = 16,
    SPELL16_Cockatrices  = 17,
    SPELL16_Transmute  = 18,
    SPELL16_Natures_Cures  = 19,
    SPELL16_Basilisk  = 20,

    SPELL16_Elemental_Armor  = 21,
    SPELL16_Petrify  = 22,
    SPELL16_Stone_Giant  = 23,
    SPELL16_Iron_Skin  = 24,
    SPELL16_Ice_Storm  = 25,
    SPELL16_Earthquake  = 26,
    SPELL16_Gorgons  = 27,
    SPELL16_Move_Fortress  = 28,
    SPELL16_Gaias_Blessing  = 29,
    SPELL16_Earth_Elemental  = 30,

    SPELL16_Regeneration  = 31,
    SPELL16_Behemoth  = 32,
    SPELL16_Entangle  = 33,
    SPELL16_Nature_Awareness  = 34,
    SPELL16_Call_Lightning  = 35,
    SPELL16_Colossus  = 36,
    SPELL16_Earth_Gate  = 37,
    SPELL16_Herb_Mastery  = 38,
    SPELL16_Great_Wyrm  = 39,
    SPELL16_Natures_Wrath  = 40,

    //    * SORCERY
    SPELL16_Resist_Magic  = 41,
    SPELL16_Dispel_Magic_True  = 42,
    SPELL16_Floating_Island  = 43,
    SPELL16_Guardian_Wind  = 44,
    SPELL16_Phantom_Warriors  = 45,
    SPELL16_Confusion  = 46,
    SPELL16_Word_of_Recall  = 47,
    SPELL16_Counter_Magic  = 48,
    SPELL16_Nagas  = 49,
    SPELL16_Psionic_Blast  = 50,

    SPELL16_Blur  = 51,
    SPELL16_Disenchant_True  = 52,
    SPELL16_Vertigo  = 53,
    SPELL16_Spell_Lock  = 54,
    SPELL16_Enchant_Road  = 55,
    SPELL16_Flight  = 56,
    SPELL16_Wind_Mastery  = 57,
    SPELL16_Spell_Blast  = 58,
    SPELL16_Aura_of_Majesty  = 59,
    SPELL16_Phantom_Beast  = 60,

    SPELL16_Disjunction_True  = 61,
    SPELL16_Invisibility  = 62,
    SPELL16_Wind_Walking  = 63,
    SPELL16_Banish  = 64,
    SPELL16_Storm_Giant  = 65,
    SPELL16_Air_Elemental  = 66,
    SPELL16_Mind_Storm  = 67,
    SPELL16_Stasis  = 68,
    SPELL16_Magic_Immunity  = 69,
    SPELL16_Haste  = 70,

    SPELL16_Djinn  = 71,
    SPELL16_Spell_Ward  = 72,
    SPELL16_Creature_Binding  = 73,
    SPELL16_Mass_Invisibility  = 74,
    SPELL16_Great_Unsummoning  = 75,
    SPELL16_Spell_Binding  = 76,
    SPELL16_Flying_Fortress  = 77,
    SPELL16_Sky_Drake  = 78,
    SPELL16_Suppress_Magic  = 79,
    SPELL16_Time_Stop  = 80,

    //    * CHAOS
    SPELL16_Warp_Wood  = 81,
    SPELL16_Disrupt  = 82,
    SPELL16_Fire_Bolt  = 83,
    SPELL16_Hell_Hounds  = 84,
    SPELL16_Corruption  = 85,
    SPELL16_Eldritch_Weapon  = 86,
    SPELL16_Wall_of_Fire  = 87,
    SPELL16_Shatter  = 88,
    SPELL16_Warp_Creature  = 89,
    SPELL16_Fire_Elemental  = 90,

    SPELL16_Lightning_Bolt  = 91,
    SPELL16_Fire_Giant  = 92,
    SPELL16_Chaos_Channels  = 93,
    SPELL16_Flame_Blade  = 94,
    SPELL16_Gargoyles  = 95,
    SPELL16_Fireball  = 96,
    SPELL16_Doombat  = 97,
    SPELL16_Raise_Volcano  = 98,
    SPELL16_Immolation  = 99,
    SPELL16_Chimeras  = 100,

    SPELL16_Warp_Lightning  = 101,
    SPELL16_Metal_Fires  = 102,
    SPELL16_Chaos_Spawn  = 103,
    SPELL16_Doom_Bolt  = 104,
    SPELL16_Magic_Vortex  = 105,
    SPELL16_Efreet  = 106,
    SPELL16_Fire_Storm  = 107,
    SPELL16_Warp_Reality  = 108,
    SPELL16_Flame_Strike  = 109,
    SPELL16_Chaos_Rift  = 110,

    SPELL16_Hydra  = 111,
    SPELL16_Disintegrate  = 112,
    SPELL16_Meteor_Storm  = 113,
    SPELL16_Great_Wasting  = 114,
    SPELL16_Call_Chaos  = 115,
    SPELL16_Chaos_Surge  = 116,
    SPELL16_Doom_Mastery  = 117,
    SPELL16_Great_Drake  = 118,
    SPELL16_Call_The_Void  = 119,
    SPELL16_Armageddon  = 120,

    //    * LIFE
    SPELL16_Bless  = 121,
    SPELL16_Star_Fires  = 122,
    SPELL16_Endurance  = 123,
    SPELL16_Holy_Weapon  = 124,
    SPELL16_Healing  = 125,
    SPELL16_Holy_Armor  = 126,
    SPELL16_Just_Cause  = 127,
    SPELL16_True_Light  = 128,
    SPELL16_Guardian_Spirit  = 129,
    SPELL16_Heroism  = 130,

    SPELL16_True_Sight  = 131,
    SPELL16_Plane_Shift  = 132,
    SPELL16_Resurrection  = 133,
    SPELL16_Dispel_Evil  = 134,
    SPELL16_Planar_Seal  = 135,
    SPELL16_Unicorns  = 136,
    SPELL16_Raise_Dead  = 137,
    SPELL16_Planar_Travel  = 138,
    SPELL16_Heavenly_Light  = 139,
    SPELL16_Prayer  = 140,

    SPELL16_Lionheart  = 141,
    SPELL16_Incarnation  = 142,
    SPELL16_Invulnerability  = 143,
    SPELL16_Righteousness  = 144,
    SPELL16_Prosperity  = 145,
    SPELL16_Altar_of_Battle  = 146,
    SPELL16_Angel  = 147,
    SPELL16_Stream_of_Life  = 148,
    SPELL16_Mass_Healing  = 149,
    SPELL16_Holy_Word  = 150,

    SPELL16_High_Prayer  = 151,
    SPELL16_Inspirations  = 152,
    SPELL16_Astral_Gate  = 153,
    SPELL16_Holy_Arms  = 154,
    SPELL16_Consecration  = 155,
    SPELL16_Life_Force  = 156,
    SPELL16_Tranquility  = 157,
    SPELL16_Crusade  = 158,
    SPELL16_Arch_Angel  = 159,
    SPELL16_Charm_of_Life  = 160,

    //    * DEATH
    SPELL16_Skeletons  = 161,
    SPELL16_Weakness  = 162,
    SPELL16_Dark_Rituals  = 163,
    SPELL16_Cloak_of_Fear  = 164,
    SPELL16_Black_Sleep  = 165,
    SPELL16_Ghouls  = 166,
    SPELL16_Life_Drain  = 167,
    SPELL16_Terror  = 168,
    SPELL16_Darkness  = 169,
    SPELL16_Mana_Leak  = 170,

    SPELL16_Drain_Power  = 171,
    SPELL16_Possession  = 172,
    SPELL16_Lycantrophy  = 173,
    SPELL16_Black_Prayer  = 174,
    SPELL16_Black_Channels  = 175,
    SPELL16_Night_Stalker  = 176,
    SPELL16_Subversion  = 177,
    SPELL16_Wall_of_Darkness  = 178,
    SPELL16_Berserk  = 179,
    SPELL16_Shadow_Demons  = 180,

    SPELL16_Wraith_Form  = 181,
    SPELL16_Wrack  = 182,
    SPELL16_Evil_Presence  = 183,
    SPELL16_Wraiths  = 184,
    SPELL16_Cloud_Of_Shadow  = 185,
    SPELL16_Warp_Node  = 186,
    SPELL16_Black_Wind  = 187,
    SPELL16_Zombie_Mastery  = 188,
    SPELL16_Famine  = 189,
    SPELL16_Cursed_Lands  = 190,

    SPELL16_Cruel_Unminding  = 191,
    SPELL16_Word_of_Death  = 192,
    SPELL16_Death_Knights  = 193,
    SPELL16_Death_Spell  = 194,
    SPELL16_Animate_dead  = 195,
    SPELL16_Pestilence  = 196,
    SPELL16_Eternal_Night  = 197,
    SPELL16_Evil_Omens  = 198,
    SPELL16_Death_Wish  = 199,
    SPELL16_Demon_Lord  = 200,

    //    * ARCANE = 1,
    SPELL16_Magic_Spirit  = 201,
    SPELL16_Dispel_Magic  = 202,
    SPELL16_Summoning_Circle  = 203,
    SPELL16_Disenchant_Area  = 204,
    SPELL16_Recall_Hero  = 205,
    SPELL16_Detect_Magic  = 206,
    SPELL16_Enchant_Item  = 207,
    SPELL16_Summon_Hero  = 208,
    SPELL16_Awareness  = 209,
    SPELL16_Disjunction  = 210,
    SPELL16_Create_Artifact  = 211,
    SPELL16_Summon_Champion  = 212,
    SPELL16_Spell_Of_Mastery  = 213,
    SPELL16_Spell_Of_Return  = 214,

    eSpell16_MAX,
    eSpell16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eTax_Rate ENUMSIZE16
{
    TAX_Rate_1 = 0,
    TAX_Rate_2 = 1,
    TAX_Rate_3 = 2,
    TAX_Rate_4 = 3,
    TAX_Rate_5 = 4,
    TAX_Rate_6 = 5,
    TAX_Rate_7 = 6,
    eTax_Rate_MAX,
    eTax_Rate__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eTerrainBattle ENUMSIZE16
{
    TERRAINBATTLE_firstbasic = 0,
    TERRAINBATTLE_lastbasic = 47,
    TERRAINBATTLE_firstextra = 48,
    TERRAINBATTLE_lastextra = 57,

    eTerrainBattle_MAX,
    eTerrainBattle__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eTerrainBonusDeposit ENUMSIZE8
{
    DEPOSIT_no_deposit = 0x00,
    DEPOSIT_Iron_Ore = 0x01,            // (-5% unit cost)
    DEPOSIT_Coal = 0x02,                // (-10% unit cost)
    DEPOSIT_Silver_Ore = 0x03,          // (+2 gold)
    DEPOSIT_Gold_Ore = 0x04,            // (+3 gold)
    DEPOSIT_Gems = 0x05,                // (+5 gold)
    DEPOSIT_Mithril_Ore = 0x06,         // (+1 mana)
    DEPOSIT_Adamantium_Ore = 0x07,      // (+2 mana)
    DEPOSIT_Quork = 0x08,               // (+3 mana)
    DEPOSIT_Crysx = 0x09,               // (+5 mana)
    DEPOSIT_Orihalcon_Ore = 0x10,       // Unused in vanilla where it was 'Hunters Lodge', recycled in Caster of Magic
    DEPOSIT_Unk_20 = 0x20,
    DEPOSIT_Wild_Game = 0x40,           // (+2 food)
    DEPOSIT_Nightshade = 0x80,          // (50 strength dispel)

    eTerrainBonusDeposit_MAX
} PACKED_ENUM;

enum eTerrainCategory
{
    TERRAINCATEGORY_Ocean = 0,
    TERRAINCATEGORY_Grasslands = 1,
    TERRAINCATEGORY_Forest = 2,
    TERRAINCATEGORY_Mountain = 3,
    TERRAINCATEGORY_Desert = 4,
    TERRAINCATEGORY_Swamp = 5,
    TERRAINCATEGORY_Tundra = 6,
    TERRAINCATEGORY_Shore = 7,
    TERRAINCATEGORY_RiverMouth = 8,
    TERRAINCATEGORY_Hills = 9,
    TERRAINCATEGORY_Plains = 10,
    TERRAINCATEGORY_River = 11,
    TERRAINCATEGORY_Volcano = 12,
    TERRAINCATEGORY_SorceryNode = 13,
    TERRAINCATEGORY_NatureNode = 14,
    TERRAINCATEGORY_ChaosNode = 15,
    TERRAINCATEGORY_Lake = 16,

    eTerrainCategory_MAX
};

enum eTerrainChange
{
    TERRAINCHANGE_Volcano_you = 0,      // 07 //1 MP/turn for Wizard 1 (YOU) through 5, 0 = no one
    TERRAINCHANGE_Volcano_player1 = 1,  // 07
    TERRAINCHANGE_Volcano_player3 = 2,  // 07

    TERRAINCHANGE_Road = 3,             // 08
    TERRAINCHANGE_Enchanted_Road = 4,   // 10 // Only visible if 'road' is also set
    TERRAINCHANGE_Corruption = 5,       // 20
    TERRAINCHANGE_Unk_40 = 6,           // 40
    TERRAINCHANGE_NoRoad80 = 7,         // 80

    eTerrainChange_MAX
};

enum eTerrainType ENUMSIZE16
{
    ocean1 = 0x0000,
    grasslands0 = 0x0001,
    shore1_first = 0x0002,              //  160 various joining configurations
    shore1_last = 0x00A1,
    grasslands1 = 0x00A2,
    forest1 = 0x00A3,
    mountain1 = 0x00A4,
    desert1 = 0x00A5,
    swamp1 = 0x00A6,
    tundra1 = 0x00A7,
    grasslands_w_sorcery_node = 0x00A8,
    forest_w_nature_node = 0x00A9,
    volcano_w_chaos_node = 0x00AA,
    hills1 = 0x00AB,
    grasslands2 = 0x00AC,
    grasslands3 = 0x00AD,
    desert2 = 0x00AE,
    desert3 = 0x00AF,
    desert4 = 0x00B0,
    swamp2 = 0x00B1,
    swamp3 = 0x00B2,
    volcano = 0x00B3,
    grasslands4 = 0x00B4,
    tundra2 = 0x00B5,
    tundra3 = 0x00B6,
    forest2 = 0x00B7,
    forest3 = 0x00B8,
    river1a_first = 0x00B9,             //  12 river
    river1a_last = 0x00C4,
    lake1_first = 0x00C5,               //  36 lake
    lake1_last = 0x00E8,
    river2a_first = 0x00E9,             //  26 river
    river2a_last = 0x0102,
    mountain2_first = 0x0103,
    mountain2_last = 0x0112,
    hills2_first = 0x0113,
    hills2_last = 0x0123,
    desert5_first = 0x0124,
    desert5_last = 0x01C3,
    shore4_first = 0x01C4,
    shore4_last = 0x01D3,
    river3_first = 0x01D4,
    river3_last = 0x01D8,
    shore5_first = 0x01D9,
    shore5_last = 0x0258,
    ocean2 = 0x0259,
    tundra4_first = 0x025A,
    tundra4_last = 0x02F9,

    eTerrainType_MAX,
    eTerrainType__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eUnitAbility
{
    eUnitAbility_NONE = -1,

    // Movement_Flags
    UNITABILITY_Cavalry = 0,          // 01
    UNITABILITY_Sailing,              // 02
    UNITABILITY_Swimming,             // 04
    UNITABILITY_Flying,               // 08
    UNITABILITY_Teleporting,          // 10
    UNITABILITY_Forester,             // 20
    UNITABILITY_Mountaineer,          // 40
    UNITABILITY_Merging,              // 80

    // Zeroes

    // Immunity_Flags
    UNITABILITY_Fire_Immunity = 16,   // 01
    UNITABILITY_Stoning_Immunity,     // 02
    UNITABILITY_Missiles_Immunity,    // 04
    UNITABILITY_Illusions_Immunity,   // 08
    UNITABILITY_Cold_Immunity,        // 10
    UNITABILITY_Magic_Immunity,       // 20
    UNITABILITY_Death_Immunity,       // 40
    UNITABILITY_Poison_Immunity,      // 80

    // Attribute_Flags
    UNITABILITY_Weapon_Immunity = 24, // 01
    UNITABILITY_Flyer,                // 02
    UNITABILITY_Lucky,                // 04
    UNITABILITY_Summon_Demons_1,      // 08
    UNITABILITY_Summon_Demons_2,      // 10
    UNITABILITY_Caster_20_MP,         // 20
    UNITABILITY_Caster_40_MP,         // 40
    UNITABILITY_Standard,             // 80

    UNITABILITY_Healing_Spell,        // 01
    UNITABILITY_Fire_Ball_Spell,      // 02
    UNITABILITY_Doombolt_Spell,       // 04
    UNITABILITY_Immolation,           // 08
    UNITABILITY_Web_Spell,            // 10
    UNITABILITY_Cause_Fear_Spell,     // 20
    UNITABILITY_Resistance_to_All,    // 40
    UNITABILITY_Holy_Bonus,           // 80

    // Zeroes

    // Ability_Flags
    UNITABILITY_Summoned_Unit = 48,   // 01
    UNITABILITY_Large_Shield,         // 02
    UNITABILITY_Plane_Shift,          // 04
    UNITABILITY_Wall_Crusher,         // 08
    UNITABILITY_Healer,               // 10
    UNITABILITY_Create_Outpost,       // 20
    UNITABILITY_Invisibility,         // 40
    UNITABILITY_Create_Undead,        // 80

    UNITABILITY_Long_Range,           // 01
    UNITABILITY_Land_Corruption,      // 02
    UNITABILITY_Meld_With_Node,       // 04
    UNITABILITY_Non_Corporeal,        // 08
    UNITABILITY_Wind_Walking,         // 10
    UNITABILITY_Regeneration,         // 20
    UNITABILITY_Purify,               // 40
    UNITABILITY_Negate_First_Strike,  // 80

    // Attack_Flags
    UNITABILITY_Armor_Piercing = 64,  // 01
    UNITABILITY_First_Strike,         // 02
    UNITABILITY_Poison_attack,        // 04
    UNITABILITY_Life_Stealing,        // 08
    UNITABILITY_Automatic_Damage,     // 10   // Chaos Spawn, Black Sleep
    UNITABILITY_Destruction,          // 20
    UNITABILITY_Illusionary_attack,   // 40
    UNITABILITY_Stoning_Touch,        // 80

    UNITABILITY_No_effect05,          // 01
    UNITABILITY_Death_Touch,          // 02
    UNITABILITY_Power_Drain,          // 04
    UNITABILITY_Dispel_Evil,          // 08
    UNITABILITY_Ball_COMBAT,                // 10
    UNITABILITY_Simultaneous_Damage_COMBAT, // 20
    UNITABILITY_Eldritch_Weapon_COMBAT,     // 40
    UNITABILITY_Warp_Lightning_COMBAT,      // 80

    eUnitAbility_MAX
};

enum eUnitEnchantment
{
    UNITENCHANTMENT_Immolation = 0,   // 01
    UNITENCHANTMENT_Guardian_Wind,    // 02
    UNITENCHANTMENT_Berserk,          // 04
    UNITENCHANTMENT_Cloak_of_Fear,    // 08
    UNITENCHANTMENT_Black_Channels,   // 10
    UNITENCHANTMENT_Wraith_Form,      // 20
    UNITENCHANTMENT_Regeneration,     // 40
    UNITENCHANTMENT_Path_Finding,     // 80

    UNITENCHANTMENT_Water_Walking,    // 01
    UNITENCHANTMENT_Resist_Elements,  // 02
    UNITENCHANTMENT_Elemental_Armor,  // 04
    UNITENCHANTMENT_Stone_Skin,       // 08
    UNITENCHANTMENT_Iron_Skin,        // 10
    UNITENCHANTMENT_Endurance,        // 20
    UNITENCHANTMENT_Spell_Lock,       // 40
    UNITENCHANTMENT_Invisibility,     // 80

    UNITENCHANTMENT_Wind_Walking,     // 01
    UNITENCHANTMENT_Flight,           // 02
    UNITENCHANTMENT_Resist_Magic,     // 04
    UNITENCHANTMENT_Magic_Immunity,   // 08
    UNITENCHANTMENT_Flame_Blade,      // 10
    UNITENCHANTMENT_Eldritch_Weapon,  // 20
    UNITENCHANTMENT_True_Sight,       // 40
    UNITENCHANTMENT_Holy_Weapon,      // 80

    UNITENCHANTMENT_Heroism,          // 01
    UNITENCHANTMENT_Bless,            // 02
    UNITENCHANTMENT_Lionheart,        // 04
    UNITENCHANTMENT_Giant_Strength,   // 08
    UNITENCHANTMENT_Planar_Travel,    // 10
    UNITENCHANTMENT_Holy_Armor,       // 20
    UNITENCHANTMENT_Righteousness,    // 40
    UNITENCHANTMENT_Invulnerability,  // 80

    eUnitEnchantment_MAX
};

enum eUnitMutation
{
    // Left out WeaponType (2 bits)
    UNITMUTATION_Chaos_Channels_Demon_Skin = 2, // 04
    UNITMUTATION_Chaos_Channels_Demon_Wings,    // 08
    UNITMUTATION_Chaos_Channels_Fiery_Breath,   // 10
    UNITMUTATION_Undead,                        // 20
    UNITMUTATION_Stasis_initial,                // 40
    UNITMUTATION_Stasis_lingering,              // 80

    eUnitMutation_MAX
};

enum eUnit_Active ENUMSIZE8
{
    UNITACTIVE_awaiting_orders = 0,         //   00=awaiting orders / "I need attention" (will "jump to" this unit when hitting wait)
    UNITACTIVE_finished = 1,                //   01=finished (won't "jump to" this unit when hitting wait)

    eUnit_Active_MAX,
    eUnit_Active_SIZE__ = 0xFF
} PACKED_ENUM;

enum eUnit_Status8 ENUMSIZE8
{
    UNITSTATUS8_Ready = 0,                   //   00=ready
    UNITSTATUS8_Patrol = 1,                  //   01=patrol
    UNITSTATUS8_Building_road_in_place = 2,  //   02=building road (in place)
    UNITSTATUS8_Going_to_XY = 3,             //   03=going to X,Y destination (could be building road too, see 0x1C)
    UNITSTATUS8_Reached_destination = 4,     //   04=reached destination & expended (0x07 == 01)
                                             //      NOTE: it is a STATE MACHINE FAILURE to have 0x07 = 00 & 0x0B == 04!
    UNITSTATUS8_Wait = 5,                    //   05=wait until all other units have had a chance to go (then clears all flags)
                                             //      the SAVE game does not record if we are traversing UP or DOWN the units!
    UNITSTATUS8_Ink_6 = 6,
    UNITSTATUS8_Ink_7 = 7,
    UNITSTATUS8_Purify = 8,
    UNITSTATUS8_Meld = 9,
    UNITSTATUS8_Settle = 10,
    UNITSTATUS8_Seek_transport = 11,
    UNITSTATUS8_Unk_12 = 12,
    UNITSTATUS8_Unk_13 = 13,
    UNITSTATUS8_Unk_14 = 14,
    UNITSTATUS8_Unk_15 = 15,
    UNITSTATUS8_Move = 16,

    UNITSTATUS8_Finished_purifying = 111,

    eUnit_Status8_MAX,
    UNITSTATUS8_eUnit_Status_SIZE__ = 0xFF
} PACKED_ENUM;

enum eUnit_Type ENUMSIZE8
{
    //  * 35 Heroes (00=Brax; 0x22=Chosen One) => better heroes, higher number
    UNITTYPE_Dwarf = 0,
    HEROTYPE_FIRST = 0,
    UNITTYPE_Barbarian = 1,
    UNITTYPE_Sage = 2,
    UNITTYPE_Dervish = 3,
    UNITTYPE_Beastmaster = 4,
    UNITTYPE_Bard = 5,
    UNITTYPE_Orc_Warrior = 6,
    UNITTYPE_Healer = 7,
    UNITTYPE_Huntress = 8,
    UNITTYPE_Thief = 9,
    UNITTYPE_Druid = 10,
    UNITTYPE_War_Monk = 11,
    UNITTYPE_Warrior_Mage = 12,
    UNITTYPE_Magician = 13,
    UNITTYPE_Assassin = 14,
    UNITTYPE_Wind_Mage = 15,
    UNITTYPE_Ranger = 16,
    UNITTYPE_Draconian = 17,
    UNITTYPE_Witch = 18,
    UNITTYPE_Golden_One = 19,
    UNITTYPE_Ninja = 20,
    UNITTYPE_Rogue = 21,
    UNITTYPE_Amazon = 22,
    UNITTYPE_Warlock = 23,
    UNITTYPE_Unknown = 24,
    UNITTYPE_Illusionist = 25,
    UNITTYPE_Swordsman = 26,
    UNITTYPE_Priestess = 27,
    UNITTYPE_Paladin = 28,
    UNITTYPE_Black_Knight = 29,
    UNITTYPE_Elven_Archer = 30,
    UNITTYPE_Knight = 31,
    UNITTYPE_Necromancer = 32,
    UNITTYPE_Chaos_Warrior = 33,
    UNITTYPE_Chosen = 34,

    //  * Inanimate units (23=Trireme, 24=Galley, 25=Catapult, 26=Warship)
    UNITTYPE_Trireme = 35,
    UNITTYPE_FIRST = 35,
    UNITTYPE_Galley = 36,
    UNITTYPE_Catapult = 37,
    UNITTYPE_Warship = 38,
    
    //  * 0x27-0x99: 115 Racial units (Sorted by race, then by power)
    
    UNITTYPE_Barbarian_Spearmen,     // 39
    UNITTYPE_Barbarian_Swordsmen,
    UNITTYPE_Barbarian_Bowmen,
    UNITTYPE_Barbarian_Cavalry,
    UNITTYPE_Barbarian_Shaman,
    UNITTYPE_Barbarian_Settlers,
    UNITTYPE_Barbarian_Berserkers,

    UNITTYPE_Beastmen_Spearmen,      // 46
    UNITTYPE_Beastmen_Swordsmen,
    UNITTYPE_Beastmen_Halberdiers,
    UNITTYPE_Beastmen_Bowmen,
    UNITTYPE_Beastmen_Priests,
    UNITTYPE_Beastmen_Magicians,
    UNITTYPE_Beastmen_Engineers,
    UNITTYPE_Beastmen_Settlers,
    UNITTYPE_Beastmen_Centaurs,
    UNITTYPE_Beastmen_Manticores,
    UNITTYPE_Beastmen_Minotaurs,

    UNITTYPE_Dark_Elf_Spearmen,      // 57
    UNITTYPE_Dark_Elf_Swordsmen,
    UNITTYPE_Dark_Elf_Halberdiers,
    UNITTYPE_Dark_Elf_Cavalry,
    UNITTYPE_Dark_Elf_Priests,
    UNITTYPE_Dark_Elf_Settlers,
    UNITTYPE_Dark_Elf_Nightblades,
    UNITTYPE_Dark_Elf_Warlocks,
    UNITTYPE_Dark_Elf_Nightmares,

    UNITTYPE_Draconian_Spearmen,     // 66
    UNITTYPE_Draconian_Swordsmen,
    UNITTYPE_Draconian_Halberdiers,
    UNITTYPE_Draconian_Bowmen,
    UNITTYPE_Draconian_Shaman,
    UNITTYPE_Draconian_Magicians,
    UNITTYPE_Draconian_Engineers,
    UNITTYPE_Draconian_Settlers,
    UNITTYPE_Draconian_Doom_Drakes,
    UNITTYPE_Draconian_Air_Ship,

    UNITTYPE_Dwarven_Swordsmen,      // 76
    UNITTYPE_Dwarven_Halberdiers,
    UNITTYPE_Dwarven_Engineers,
    UNITTYPE_Dwarven_Hammerhands,
    UNITTYPE_Dwarven_Steam_Cannon,
    UNITTYPE_Dwarven_Golem,
    UNITTYPE_Dwarven_Settlers,

    UNITTYPE_Gnoll_Spearmen,         // 83
    UNITTYPE_Gnoll_Swordsmen,
    UNITTYPE_Gnoll_Halberdiers,
    UNITTYPE_Gnoll_Bowmen,
    UNITTYPE_Gnoll_Settlers,
    UNITTYPE_Gnoll_Wolf_Riders,

    UNITTYPE_Halfling_Spearmen,      // 89
    UNITTYPE_Halfling_Swordsmen,
    UNITTYPE_Halfling_Bowmen,
    UNITTYPE_Halfling_Shaman,
    UNITTYPE_Halfling_Settlers,
    UNITTYPE_Halfling_Slingers,

    UNITTYPE_High_Elf_Spearmen,      // 95
    UNITTYPE_High_Elf_Swordsmen,
    UNITTYPE_High_Elf_Halberdiers,
    UNITTYPE_High_Elf_Cavalry,
    UNITTYPE_High_Elf_Magicians,
    UNITTYPE_High_Elf_Settlers,
    UNITTYPE_High_Elf_Longbowmen,
    UNITTYPE_High_Elf_Elven_Lords,
    UNITTYPE_High_Elf_Pegasai,

    UNITTYPE_High_Men_Spearmen,      // 104
    UNITTYPE_High_Men_Swordsmen,
    UNITTYPE_High_Men_Bowmen,
    UNITTYPE_High_Men_Cavalry,
    UNITTYPE_High_Men_Priests,
    UNITTYPE_High_Men_Magicians,
    UNITTYPE_High_Men_Engineers,
    UNITTYPE_High_Men_Settlers,
    UNITTYPE_High_Men_Pikemen,
    UNITTYPE_High_Men_Paladins,

    UNITTYPE_Klackon_Spearmen,       // 114
    UNITTYPE_Klackon_Swordsmen,
    UNITTYPE_Klackon_Halberdiers,
    UNITTYPE_Klackon_Engineers,
    UNITTYPE_Klackon_Settlers,
    UNITTYPE_Klackon_Stag_Beetle,

    UNITTYPE_Lizardman_Spearmen,     // 120
    UNITTYPE_Lizardman_Swordsmen,
    UNITTYPE_Lizardman_Halberdiers,
    UNITTYPE_Lizardman_Javelineers,
    UNITTYPE_Lizardman_Shaman,
    UNITTYPE_Lizardman_Settlers,
    UNITTYPE_Lizardman_Dragon_Turtle,

    UNITTYPE_Nomad_Spearmen,         // 127
    UNITTYPE_Nomad_Swordsmen,
    UNITTYPE_Nomad_Bowmen,
    UNITTYPE_Nomad_Priests,
    UNITTYPE_Nomad_Magicians,
    UNITTYPE_Nomad_Settlers,
    UNITTYPE_Nomad_Horsebowmen,
    UNITTYPE_Nomad_Pikemen,
    UNITTYPE_Nomad_Rangers,
    UNITTYPE_Nomad_Griffins,

    UNITTYPE_Orc_Spearmen,           // 137
    UNITTYPE_Orc_Swordsmen,
    UNITTYPE_Orc_Halberdiers,
    UNITTYPE_Orc_Bowmen,
    UNITTYPE_Orc_Cavalry,
    UNITTYPE_Orc_Shaman,
    UNITTYPE_Orc_Magicians,
    UNITTYPE_Orc_Engineers,
    UNITTYPE_Orc_Settlers,
    UNITTYPE_Orc_Wyvern_Riders,

    UNITTYPE_Troll_Spearmen,         // 147
    UNITTYPE_Troll_Swordsmen,
    UNITTYPE_Troll_Halberdiers,
    UNITTYPE_Troll_Shaman,
    UNITTYPE_Troll_Settlers,
    UNITTYPE_Troll_War_Trolls,
    UNITTYPE_Troll_War_Mammoths,

    // * 0x9A-0xC5: 44 Magical (summoned) Creatures

    UNITTYPE_Arcane_Magic_Spirit,    // 154 0x9A

    UNITTYPE_Red_Hell_Hounds,        // 155 0x9B
    UNITTYPE_Red_Gargoyles,
    UNITTYPE_Red_Fire_Giant,
    UNITTYPE_Red_Fire_Elemental,
    UNITTYPE_Red_Chaos_Spawn,
    UNITTYPE_Red_Chimera,
    UNITTYPE_Red_Doom_Bat,
    UNITTYPE_Red_Efreet,
    UNITTYPE_Red_Hydra,
    UNITTYPE_Red_Great_Drake,

    UNITTYPE_Black_Skeletons,        // 165d
    UNITTYPE_Black_Ghouls,
    UNITTYPE_Black_Night_Stalker,
    UNITTYPE_Black_Werewolves,
    UNITTYPE_Black_Demon,
    UNITTYPE_Black_Wraiths,
    UNITTYPE_Black_Shadow_Demons,
    UNITTYPE_Black_Death_Knights,
    UNITTYPE_Black_Demon_Lord,
    UNITTYPE_Black_Zombies,

    UNITTYPE_White_Unicorns,         // 175d
    UNITTYPE_White_Guardian_Spirit,
    UNITTYPE_White_Angel,
    UNITTYPE_White_Arch_Angel,

    UNITTYPE_Green_War_Bears,        // 179d
    UNITTYPE_Green_Sprites,
    UNITTYPE_Green_Cockatrices,
    UNITTYPE_Green_Basilisk,
    UNITTYPE_Green_Giant_Spiders,
    UNITTYPE_Green_Stone_Giant,
    UNITTYPE_Green_Colossus,
    UNITTYPE_Green_Gorgons,
    UNITTYPE_Green_Earth_Elemental,
    UNITTYPE_Green_Behemoth,
    UNITTYPE_Green_Great_Wyrm,

    UNITTYPE_Blue_Floating_Island,
    UNITTYPE_Blue_Phantom_Beast,
    UNITTYPE_Blue_Phantom_Warriors,
    UNITTYPE_Blue_Storm_Giant,
    UNITTYPE_Blue_Air_Elemental,
    UNITTYPE_Blue_Djinn,
    UNITTYPE_Blue_Sky_Drake,
    UNITTYPE_Blue_Nagas,

    eUnit_Type_MAX                // 198 0xC6
} PACKED_ENUM;

/*
enum eUpkeep_Enchantment
{
    // Unit_Enchantments
    UPKEEPENCH_Immolation, // 0 Offset EXE:2B3B8
    UPKEEPENCH_Guardian_Wind, // 2 Offset EXE:2B3BA
    UPKEEPENCH_Berserk, // 4 Offset EXE:2B3BC     // Note(12)
    UPKEEPENCH_Cloak_of_Fear, // 6 Offset EXE:2B3BE
    UPKEEPENCH_Black_Channels, // 8 Offset EXE:2B3C0
    UPKEEPENCH_Wraithform, // A Offset EXE:2B3C2
    UPKEEPENCH_Regeneration, // C Offset EXE:2B3C4
    UPKEEPENCH_Pathfinding, // E Offset EXE:2B3C6
    UPKEEPENCH_Water_Walking, // 10 Offset EXE:2B3C8
    UPKEEPENCH_Resist_Elements, // 12 Offset EXE:2B3CA
    UPKEEPENCH_Elemental_Armor, // 14 Offset EXE:2B3CC
    UPKEEPENCH_Stone_Skin, // 16 Offset EXE:2B3CE
    UPKEEPENCH_Iron_Skin, // 18 Offset EXE:2B3D0
    UPKEEPENCH_Endurance, // 1A Offset EXE:2B3D2
    UPKEEPENCH_Spell_Lock, // 1C Offset EXE:2B3D4
    UPKEEPENCH_Invisibility, // 1E Offset EXE:2B3D6
    UPKEEPENCH_Wind_Walking, // 20 Offset EXE:2B3D8
    UPKEEPENCH_Flight, // 22 Offset EXE:2B3DA
    UPKEEPENCH_Resist_Magic, // 24 Offset EXE:2B3DC
    UPKEEPENCH_Magic_Immunity, // 26 Offset EXE:2B3DE
    UPKEEPENCH_Flame_Blade, // 28 Offset EXE:2B3E0
    UPKEEPENCH_Eldritch_Weapon, // 2A Offset EXE:2B3E2
    UPKEEPENCH_True_Sight, // 2C Offset EXE:2B3E4
    UPKEEPENCH_Holy_Weapon, // 2E Offset EXE:2B3E6
    UPKEEPENCH_Heroism, // 30 Offset EXE:2B3E8
    UPKEEPENCH_Bless, // 32 Offset EXE:2B3EA
    UPKEEPENCH_Lion_Heart, // 34 Offset EXE:2B3EC
    UPKEEPENCH_Giant_Strength, // 36 Offset EXE:2B3EE
    UPKEEPENCH_Planar_Travel, // 38 Offset EXE:2B3F0
    UPKEEPENCH_Holy_Armor, // 3A Offset EXE:2B3F2
    UPKEEPENCH_Righteousness, // 3C Offset EXE:2B3F4
    UPKEEPENCH_Invulnerability, // 3E Offset EXE:2B3F6

    // City_Enchantments
    UPKEEPENCH_Wall_of_Fire, // 40 Offset EXE:2B3F8
    UPKEEPENCH_Chaos_Rift, // 42 Offset EXE:2B3FA
    UPKEEPENCH_Dark_Rituals, // 44 Offset EXE:2B3FC
    UPKEEPENCH_Evil_Presence, // 46 Offset EXE:2B3FE
    UPKEEPENCH_Cursed_Lands, // 48 Offset EXE:2B400
    UPKEEPENCH_Pestilence, // 4A Offset EXE:2B402
    UPKEEPENCH_Cloud_of_Shadow, // 4C Offset EXE:2B404
    UPKEEPENCH_Famine, // 4E Offset EXE:2B406
    UPKEEPENCH_Flying_Fortress, // 50 Offset EXE:2B408
    UPKEEPENCH_Nature_Ward, // 52 Offset EXE:2B40A
    UPKEEPENCH_Sorcery_Ward, // 54 Offset EXE:2B40C
    UPKEEPENCH_Chaos_Ward, // 56 Offset EXE:2B40E
    UPKEEPENCH_Life_Ward, // 58 Offset EXE:2B410
    UPKEEPENCH_Death_Ward, // 5A Offset EXE:2B412
    UPKEEPENCH_Natures_Eye, // 5C Offset EXE:2B414
    UPKEEPENCH_Earth_Gate, // 5E Offset EXE:2B416
    UPKEEPENCH_Stream_of_Life, // 60 Offset EXE:2B418
    UPKEEPENCH_Gaias_Blessing, // 62 Offset EXE:2B41A
    UPKEEPENCH_Inspirations, // 64 Offset EXE:2B41C
    UPKEEPENCH_Prosperity, // 66 Offset EXE:2B41E
    UPKEEPENCH_Astral_Gate, // 68 Offset EXE:2B420
    UPKEEPENCH_Heavenly_Light, // 6A Offset EXE:2B422
    UPKEEPENCH_Consecration, // 6C Offset EXE:2B424
    UPKEEPENCH_Wall_of_Darkness, // 6E Offset EXE:2B426
    UPKEEPENCH_Altar_of_Battle, // 70 Offset EXE:2B428
    UPKEEPENCH_Nightshade, // 72 Offset EXE:2B42A

    // Global Enchantments
    UPKEEPENCH_Eternal_Night, // 74 Offset EXE:2B42C
    UPKEEPENCH_Evil_Omens, // 76 Offset EXE:2B42E
    UPKEEPENCH_Zombie_Mastery, // 78 Offset EXE:2B430
    UPKEEPENCH_Aura_of_Majesty, // 7A Offset EXE:2B432
    UPKEEPENCH_Wind_Mastery, // 7C Offset EXE:2B434
    UPKEEPENCH_Suppress_Magic, // 7E Offset EXE:2B436
    UPKEEPENCH_Time_Stop, // 80 Offset EXE:2B438
    UPKEEPENCH_Nature_Awareness, // 82 Offset EXE:2B43A
    UPKEEPENCH_Natures_Wrath, // 84 Offset EXE:2B43C
    UPKEEPENCH_Herb_Mastery, // 86 Offset EXE:2B43E
    UPKEEPENCH_Chaos_Surge, // 88 Offset EXE:2B440
    UPKEEPENCH_Doom_Mastery, // 8A Offset EXE:2B442
    UPKEEPENCH_Great_Wasting, // 8C Offset EXE:2B444
    UPKEEPENCH_Meteor_Storm, // 8E Offset EXE:2B446
    UPKEEPENCH_Armageddon, // 90 Offset EXE:2B448
    UPKEEPENCH_Tranquility, // 92 Offset EXE:2B44A
    UPKEEPENCH_Life_Force, // 94 Offset EXE:2B44C
    UPKEEPENCH_Crusade, // 96 Offset EXE:2B44E
    UPKEEPENCH_Just_Cause, // 98 Offset EXE:2B450
    UPKEEPENCH_Holy_Arms, // 9A Offset EXE:2B452
    UPKEEPENCH_Planar_Seal, // 9C Offset EXE:2B454
    UPKEEPENCH_Charm_of_Life, // 9E Offset EXE:2B456
    UPKEEPENCH_Detect_Magic, // A0 Offset EXE:2B458
    UPKEEPENCH_Awareness, // A2 Offset EXE:2B45A

    eUpkeep_Enchantment_MAX                // A4
};
*/
enum eWar_Status ENUMSIZE8
{
    WARSTATUS_None = 0,
    WARSTATUS_Wizard_Pact = 1,
    WARSTATUS_Alliance = 2,
    WARSTATUS_War = 3,

    eWar_Status_MAX
} PACKED_ENUM;

enum eWeaponType ENUMSIZE8
{
    WEAPON_normal = 0,
    WEAPON_magic = 1,
    WEAPON_mithril = 2,
    WEAPON_adamantium = 3,

    eWeaponType_MAX
} PACKED_ENUM;

enum eYesNo16 ENUMSIZE16
{
    YESNO16_No = 0,
    YESNO16_Yes = 1,

    eYesNo16_MAX,
    eYesNo16__SIZE__ = 0xFFFF
} PACKED_ENUM;

enum eYesNo8 ENUMSIZE8
{
    YESNO8_No = 0,
    YESNO8_Yes = 1,

    eYesNo8_MAX,
    eYesNo8__SIZE__ = 0xFF
} PACKED_ENUM;

//
// CONSTANTS
//

const unsigned gMAX_ARTIFACTS_IN_GAME = 250;
const unsigned gMAX_BATTLE_COLS = 21;
const unsigned gMAX_BATTLE_ROWS = 22;
const unsigned gMAX_BATTLE_UNITS = 18;
const unsigned gMAX_CITIES = 100;
const unsigned gMAX_FIGURES_IN_UNIT = 8;
const unsigned gMAX_HERO_TYPES = 35;
const unsigned gMAX_HIRED_HEROES = 6;
const unsigned gMAX_ITEMSLOTS = 3;
const unsigned gMAX_ITEMS_IN_GAME = 128;
const unsigned gMAX_ITEMS_VALID = 138;
const unsigned gMAX_LEVELS_HERO = 9;
const unsigned gMAX_LEVELS_UNIT = 6;
const unsigned gMAX_MAP_COLS = 60;
const unsigned gMAX_MAP_ROWS = 40;
const unsigned gMAX_NODES = 30;
const unsigned gMAX_NODES_LAIRS_TOWERS = 102;
const unsigned gMAX_RACES = 14;
const unsigned gMAX_UNITS = 1009;
const unsigned gMAX_UNITS_IN_LOCATION = 9;
const unsigned gMAX_VALID_WIZARDS = 5;
const unsigned gMAX_WIZARD_RECORDS = 6;

//
// STRUCTS / UNIONS
//

typedef struct PACKED_STRUCT // Skills 
{
    eYesNo8             eSkill_Alchemy;                // 00 (0 or 1)
    eYesNo8             eSkill_Warlord;
    eYesNo8             eSkill_Chaos_Master;
    eYesNo8             eSkill_Nature_Master;
    eYesNo8             eSkill_Sorcery_Master;
    eYesNo8             eSkill_Infernal_Power;         // 05
    eYesNo8             eSkill_Divine_Power;
    eYesNo8             eSkill_Sage_Master;
    eYesNo8             eSkill_Channeller;
    eYesNo8             eSkill_Myrran;
    eYesNo8             eSkill_Archmage;               // 0A
    eYesNo8             eSkill_Mana_Focusing;
    eYesNo8             eSkill_Node_Mastery;
    eYesNo8             eSkill_Famous;
    eYesNo8             eSkill_Runemaster;
    eYesNo8             eSkill_Conjurer;               // 0F
    eYesNo8             eSkill_Charismatic;
    eYesNo8             eSkill_Artificer;              // 11
                                // SIZE 12
} Skills;

typedef union // unionSkills
{
    Skills   s;
    eYesNo8     a[eSkill_MAX]; // (0 or 1)
} unionSkills; // <read=read_unionAbilities>;

typedef struct PACKED_STRUCT // Building_Data
{
    char                m_BuildingName[20];     // 00
    eBuilding           m_Prerequisite1;        // 14
    eBuilding           m_Prerequisite2;        // 16
    eBuilding           m_Replaces_building;    // 18
    int16_t             m_No_Effect_1A;         // 1A
    int16_t             m_No_Effect_1C;         // 1C
    int16_t             m_No_Effect_1E;         // 1E
    int16_t             m_Upkeep_yield;         // 20
    int16_t             m_No_Effect_22;         // 22
    int16_t             m_Zero_24;              // 24
    int16_t             m_No_Effect_26;         // 26
    int16_t             m_No_Effect_28;         // 28
    int16_t             m_No_Effect_2A;         // 2A
    int16_t             m_Building_cost;        // 2C
    int16_t             m_Zero_2E;              // 2E
    int16_t             m_Animation_related;    // 30
    eBuildingCategory   m_Building_category;    // 32
                                                // SIZE 34
} Building_Data;

typedef struct PACKED_STRUCT // Building_Status
{
    // TODO: Match with eBuilding
    eBuildingStatus     No_building;           // (keep at 0 "Replaced"!!!!!)

    eBuildingStatus     Trade_Goods;    // (keep at -1!!!!!)
    eBuildingStatus     Housing;        // (keep at -1!!!!!)

    eBuildingStatus     Barracks;
    eBuildingStatus     Armory;
    eBuildingStatus     Fighters_Guild;
    eBuildingStatus     Armorers_Guild;
    eBuildingStatus     War_College;
    eBuildingStatus     Smithy;
    eBuildingStatus     Stable;
    eBuildingStatus     Animists_Guild;
    eBuildingStatus     Fantastic_Stable;
    eBuildingStatus     Shipwright_Guild;
    eBuildingStatus     Ship_Yard;
    eBuildingStatus     Maritime_Guild;
    eBuildingStatus     Sawmill;
    eBuildingStatus     Library;
    eBuildingStatus     Sages_Guild;
    eBuildingStatus     Oracle;
    eBuildingStatus     Alchemist_Guild;
    eBuildingStatus     University;
    eBuildingStatus     Wizards_Guild;
    eBuildingStatus     Shrine;
    eBuildingStatus     Temple;
    eBuildingStatus     Parthenon;
    eBuildingStatus     Cathedral;
    eBuildingStatus     Marketplace;
    eBuildingStatus     Bank;
    eBuildingStatus     Merchants_Guild;
    eBuildingStatus     Granary;
    eBuildingStatus     Farmers_Market;
    eBuildingStatus     Foresters_Guild;
    eBuildingStatus     Builders_Hall;
    eBuildingStatus     Mechanicians_Guild;
    eBuildingStatus     Miners_Guild;
    eBuildingStatus     City_Walls;
                                                // SIZE 24
} Building_Status;

typedef union // unionBuilding_Status
{
    eBuildingStatus a[eBuilding_array_MAX];
    Building_Status s;
} unionBuilding_Status;

typedef struct PACKED_STRUCT // City_Enchantments
{
    // TODO: Match with eSpell
    eOwner      Wall_of_Fire;
    eOwner      _Chaos_Rift_;
    eOwner      Dark_Rituals;
    eOwner      _Evil_Presence_;
    eOwner      _Cursed_Lands_;
    eOwner      _Pestilence_;
    eOwner      Cloud_of_Shadow;
    eOwner      _Famine_;
    eOwner      Flying_Fortress;
    eOwner      Nature_Ward;
    eOwner      Sorcery_Ward;
    eOwner      Chaos_Ward;
    eOwner      Life_Ward;
    eOwner      Death_Ward;
    eOwner      Natures_Eye;
    eOwner      Earth_Gate;
    eOwner      Stream_of_Life;
    eOwner      Gaias_Blessing;
    eOwner      Inspirations;
    eOwner      Prosperity;
    eOwner      Astral_Gate;
    eOwner      Heavenly_Light;
    eOwner      Consecration;
    eOwner      Wall_of_Darkness;
    eOwner      Altar_of_Battle;
                                    // SIZE 18
} City_Enchantments;

typedef struct PACKED_STRUCT // CityQueueElement
{
    int8_t      m_CityNr;           // -1=Not applicable (anymore)
    uint8_t     m_Unused_01;
    eProducing  m_Producing;        // Negative?
                                    // SIZE 4
} CityQueueElement;

struct ClickableArea
{
    uint16_t                 left;                                    //   0 -1 0x10000400
    uint16_t                 top;                                     //   2 -1 0x10000400
    uint16_t                 right;                                   //   4 -1 0x10000400
    uint16_t                 bottom;                                  //   6 -1 0x10000400
    uint16_t                 type;                                    //   8 -1 0x10000400
    // 0X8 \n0=button\n7=clickable area
    uint16_t                 field_A_argument;                        //   A -1 0x10000400
    uint16_t                 fontNr;                                  //   C -1 0x10000400
    uint16_t                 fontPaletteSection;                      //   E -1 0x10000400
    uint8_t                  m_gap_16[6];                             //  10
    uint16_t                 field_16_text;                           //  16 -1 0x10000400
    uint8_t                  m_gap_24[4];                             //  18
    uint16_t                 field_1C_pic_attr;                       //  1C -1 0x10000400
    uint8_t                  m_gap_30[2];                             //  1E
    uint16_t                 pic_seg;                                 //  20 -1 0x10000400
    uint8_t                  m_gap_34[2];                             //  22
    uint8_t                  hotkey;                                  //  24 -1 0x400
    uint8_t                  field_25;                                //  25 -1 0x400
    // SIZE 0x26
};

typedef struct PACKED_STRUCT // Combat_Enchantment
{
    // TODO: Match with eSpell
    uint8_t     Vertigo:1;              // 01
    uint8_t     Confusion:1;            // 02
    uint8_t     Whirlwind:1;            // 04
    uint8_t     Mind_Storm:1;           // 08
    uint8_t     Shatter:1;              // 10
    uint8_t     Weakness:1;             // 20
    uint8_t     Black_Sleep:1;          // 40
    uint8_t     Warp_Creature_Attack:1; // 80

    uint8_t     Warp_Creature_Defense:1;    // 01
    uint8_t     Warp_Creature_Resistance:1; // 02
    uint8_t     Mind_Twist:1;           // 04
    uint8_t     Haste:1;                // 08
    uint8_t     Web:1;                  // 10
    uint8_t     Creature_Binding:1;     // 20
    uint8_t     Possession:1;           // 40
    uint8_t     No_Effect80:1;          // 80
} Combat_Enchantment;

typedef union // unionCombat_Enchantment
{
    Combat_Enchantment  s;
    uint16_t            bits; // <format=hex>;
} unionCombat_Enchantment;

typedef struct PACKED_STRUCT // Events_Status
{
    uint16_t    m_Turn_Event_triggered_GUESS ;         // 00
    uint16_t    m_Meteor_active      ;                 // 02    0=Not active, 1=Triggered, 2=Active, 3=Ending?
    uint16_t    m_Meteor_playerNr    ;                 // 04
    uint16_t    m_Meteor_cityNr      ;                 // 06
    uint16_t    m_Gift_of_the_Gods_active ;            // 08
    uint16_t    m_Gift_of_the_Gods_playerNr ;          // 0A
    uint16_t    m_Gift_of_the_Gods_itemNr_GUESS ;      // 0C
    uint16_t    m_Disjunction_active ;                 // 0E
    uint16_t    m_Diplomatic_Marriage_active ;         // 10
    uint16_t    m_Diplomatic_Marriage_playerNr ;       // 12
    uint16_t    m_Diplomatic_Marriage_newCityNr_GUESS ;// 14
    uint16_t    m_Diplomatic_Marriage_cityNr_GUESS ;   // 16
    uint16_t    m_Earthquake_active  ;                 // 18
    uint16_t    m_Earthquake_playerNr ;                // 1A
    uint16_t    m_Earthquake_cityNr  ;                 // 1C
    uint16_t    m_Pirates_active     ;                 // 1E
    uint16_t    m_Pirates_playerNr   ;                 // 20
    uint16_t    m_Pirates_gold_stolen ;                // 22
    uint16_t    m_Plague_active      ;                 // 24
    uint16_t    m_Plague_playerNr    ;                 // 26
    uint16_t    m_Plague_cityNr      ;                 // 28
    uint16_t    m_UNUSED01               ;             // 2A
    uint16_t    m_Rebellion_active   ;                 // 2C
    uint16_t    m_Rebellion_playerNr ;                 // 2E
    uint16_t    m_Rebellion_cityNr   ;                 // 30
    uint16_t    m_Donation_active    ;                 // 32
    uint16_t    m_Donation_playerNr  ;                 // 34
    uint16_t    m_Donation_gold_donated ;              // 36
    uint16_t    m_Depletion_active   ;                 // 38
    uint16_t    m_Depletion_playerNr ;                 // 3A
    uint16_t    m_UNUSED02               ;             // 3C
    uint16_t    m_Mine_found_active  ;                 // 3E
    uint16_t    m_Mine_found_playerNr ;                // 40
    uint16_t    m_UNUSED03               ;             // 42
    uint16_t    m_Population_Boom_active ;             // 44
    uint16_t    m_Population_Boom_CityNr ;             // 46
    uint16_t    m_Population_Boom_PlayerNr ;           // 48
    uint16_t    m_UNUSED04                         ;   // 4A
    uint16_t    m_Good_Moon_active   ;                 // 4C
    uint16_t    m_UNUSED05                         ;   // 4E
    uint16_t    m_Bad_Moon_active    ;                 // 50
    uint16_t    m_UNUSED06                         ;   // 52
    uint16_t    m_Conjunction_Sorcery_active ;         // 54
    uint16_t    m_Conjunction_Sorcery_Nr_GUESS     ;   // 56
    uint16_t    m_Conjunction_Nature_active ;          // 58
    uint16_t    m_Conjunction_Nature_Nr_GUESS    ;     // 5A
    uint16_t    m_Conjunction_Chaos_active ;           // 5C
    uint16_t    m_Conjunction_Chaos_Nr_GUESS    ;      // 5E
    uint16_t    m_Mana_Short_active  ;                 // 60
    uint16_t    m_UNUSED10 ;                           // 62
                                                       // SIZE 64
} Events_Status;

typedef struct PACKED_STRUCT // Game_Data_Save
{
    uint16_t        m_Number_of_Wizards;    // in Wizard Table (including YOU, 2 ... 5)
    eLand_Size      m_Land_Size_setting;
    eMagic_Powerful m_Magic_Powerful_setting;
    eDifficulty     m_Difficulty;
    uint16_t        m_Number_of_Cities;     // in City Table (0 ... 100, of out of a maximum of 100)
    uint16_t        m_Number_of_Units;      // in Unit Instance Table (0 ... 1000, out of a maximum of 1000)
    uint16_t        m_Current_Turn; // <read=read_Turn>;         // year is 1400 + Turn/12, month is Turn%12
    uint16_t        m_UnitNr_Active;
                                            // SIZE 16
} Game_Data_Save;


typedef struct PACKED_STRUCT // Global_Enchantments
{
    // TODO: Match with eSpell
    uint8_t     Eternal_Night;
    uint8_t     Evil_Omens;
    uint8_t     Zombie_Mastery;
    uint8_t     Aura_of_Majesty;
    uint8_t     Wind_Mastery;
    uint8_t     Suppress_Magic;
    uint8_t     Time_Stop;
    uint8_t     Nature_Awareness;
    uint8_t     Natures_Wrath;
    uint8_t     Herb_Mastery;
    uint8_t     Chaos_Surge;
    uint8_t     Doom_Mastery;
    uint8_t     Great_Wasting;
    uint8_t     Meteor_Storm;
    uint8_t     Armageddon;
    uint8_t     Tranquility;
    uint8_t     Life_Force;
    uint8_t     Crusade;
    uint8_t     Just_Cause;
    uint8_t     Holy_Arms;
    uint8_t     Planar_Seal;
    uint8_t     Charm_of_Life;
    uint8_t     Detect_Magic;
    uint8_t     Awareness;
                                    // SIZE 24
} Global_Enchantments;


typedef struct PACKED_STRUCT // Hero_Ability
{
    // TODO: Match... with which name?
    uint8_t     Leadership:1;       // 01
    uint8_t     Leadership_X:1;     // 02
    uint8_t     u1:1;               // 04
    uint8_t     Legendary:1;        // 08
    uint8_t     Legendary_X:1;      // 10
    uint8_t     u0:1;               // 20
    uint8_t     Blademaster:1;      // 40
    uint8_t     Blademaster_X:1;    // 80

    uint8_t     u4:1;               // 01
    uint8_t     Armsmaster:1;       // 02
    uint8_t     Armsmaster_X:1;     // 04
    uint8_t     u3:1;               // 08
    uint8_t     Constitution:1;     // 10
    uint8_t     Constitution_X:1;   // 20
    uint8_t     u2:1;               // 40
    uint8_t     Might:1;            // 80

    uint8_t     Might_X:1;          // 01
    uint8_t     u7:1;               // 02
    uint8_t     Arcane_Power:1;     // 04
    uint8_t     Arcane_Power_X:1;   // 08
    uint8_t     u6:1;               // 10
    uint8_t     Sage:1;             // 20
    uint8_t     Sage_X:1;           // 40
    uint8_t     u5:1;               // 80

    uint8_t     Prayermaster:1;     // 01
    uint8_t     Prayermaster_X:1;   // 02
    uint8_t     Agility_X:1;        // 04
    uint8_t     Lucky:1;            // 08
    uint8_t     Charmed:1;          // 10
    uint8_t     Noble:1;            // 20
    uint8_t     Female:1;           // 40
    uint8_t     Agility:1;          // 80
} Hero_Ability;


typedef union // unionHero_Abilities;
{
    Hero_Ability    s;      // <read=read_Hero_Ability>;
    uint32_t        bits;   // <format=hex>;
} unionHero_Abilities; // <read=read_unionHero_Abilities>;

    
typedef struct PACKED_STRUCT // Hired_Hero
{
    int16_t     m_Unit_Nr;              // 00 (see below)
    char        m_Hero_name[14];        // 02 (13 characters + '\0')
    int16_t     m_Items_In_Slot[3];     // 10 (see below)
    eSlot_Type16 m_Slot_Types[3];       // 16 (0-6; see below)
                                        // SIZE 1C
} Hired_Hero;    // <read=read_Hired_Hero>;


typedef struct PACKED_STRUCT // Hero_stats
{
    eHero_Level_Status  m_Level_Status;     // 00
    unionHero_Abilities m_Hero_Abilities;   // 02
    uint8_t     m_Hero_Casting_Skill;       // 06 <read=read_Hero_Casting_Skill>;
    eSpell      m_Spell[4];                 // 07-0A
    uint8_t     m_CoM_Special_Skill;        // 0B
                                            // SIZE 0C
} Hero_stats; // <read=read_Hero_stats>;

typedef struct PACKED_STRUCT // Hero_Stats_Initializer
{
    uint16_t    m_Nr_Random_picks;          // 00
    eRandomPickType    m_Random_pick_type;  // 02
    unionHero_Abilities m_Hero_Abilities;   // 04
    uint16_t    m_Hero_Casting_Skill;       // 08 <read=read_Hero_Casting_Skill>;
    eSpell16    m_Spell[4];                 // 0A-11
                                            // SIZE 12
} Hero_Stats_Initializer;


typedef struct PACKED_STRUCT // Item_Bonuses
{
    int8_t      Attack;                 // 00
    int8_t      To_Hit;                 // 01
    int8_t      Defense;                // 02
    int8_t      Movement_in_halves;     // 03
    int8_t      Resistance;             // 04
    int8_t      Spell_Points;           // 05
    int8_t      Spell_Save;             // 06
                                        // SIZE 07
} Item_Bonuses; // <read=read_Item_Bonuses>;


typedef struct PACKED_STRUCT // Item_Powers
{
    // TODO: Match with ItemPowLbx.m_Name
    uint8_t     Vampiric:1;             // 01
    uint8_t     Guardian_Wind:1;        // 02
    uint8_t     Lightning:1;            // 04
    uint8_t     Cloak_Of_Fear:1;        // 08
    uint8_t     Destruction:1;          // 10
    uint8_t     Wraith_Form:1;          // 20
    uint8_t     Regeneration:1;         // 40
    uint8_t     Pathfinding:1;          // 80

    uint8_t     Water_Walking:1;
    uint8_t     Resist_Elements:1;
    uint8_t     Elemental_Armour:1;
    uint8_t     Doom_equals_Chaos:1;
    uint8_t     Stoning:1;
    uint8_t     Endurance:1;
    uint8_t     Haste:1;
    uint8_t     Invisibility:1;

    uint8_t     Death:1;
    uint8_t     Flight:1;
    uint8_t     Resist_Magic:1;
    uint8_t     Magic_Immunity:1;
    uint8_t     Flaming:1;
    uint8_t     Holy_Avenger:1;
    uint8_t     True_Sight:1;
    uint8_t     Phantasmal:1;

    uint8_t     Power_Drain:1;
    uint8_t     Bless:1;
    uint8_t     Lion_Heart:1;
    uint8_t     Giant_Strength:1;
    uint8_t     Planar_Travel:1;
    uint8_t     Merging:1;
    uint8_t     Righteousness:1;
    uint8_t     Invulnerability:1;
} Item_Powers;


typedef union // unionItem_Powers
{
    Item_Powers s;
    uint32_t    bits; // <format=hex>;
} unionItem_Powers; // <read=read_unionItem_Powers>;


typedef struct PACKED_STRUCT // Item
{
    char                m_Item_Name[30];            // 00 Item's Name (30 characters + '\0')
    eItem_Icon          m_Icon;                     // 1E Icon (0-0x73; see below)
    eSlot_Type8         m_Slot_Required;            // 20 Slot Required (0-6; see above)
    eItem_Type          m_Item_Type;                // 21 Item Type (0-8; see below)
    int16_t             m_Cost;                     // 22 Cost
    Item_Bonuses        m_Bonuses;                  // 24 Bonuses
    eSpell              m_Spell_Number_Charged;     // 2B Spell Number Charged + 1 (-> 0 means no spell charge)
    int16_t             m_Number_Of_Charges;        // 2C Number Of Charges
    unionItem_Powers    m_Bitmask_Powers;           // 2E (bitmasks) Powers
                                                    // SIZE 32
} Item; // <read=read_Item>;


typedef struct PACKED_STRUCT // ItemDataLbx
{
    Item        m_Item;                             // 00   Movement is in full moves instead of half moves
    uint8_t     m_Required_Spellbooks[5];           // 32   Compared with spellbooks in Wizards structure
    uint8_t     m_Special_Item;                     // 37   0=not special, 1=special
                                                    // SIZE 38
} ItemDataLbx;


typedef struct PACKED_STRUCT // ItemPowLbx
{
    char                m_Name[18];                 // 00 Artifact special name (17 characters + '\0')
    uint16_t            m_EnchantibleItems;         // 12 Bitmask with (1 << eItem_Type)
    int16_t             m_Mana_cost_to_enchant;     // 14
    eItemPowerType      m_PowerType;                // 16
    eRealm_Type         m_Color;                    // 17
    int16_t             m_Required_Nr_Spell_Books;  // 18   (if a Stat special, it's the Stat bonus)
    unionItem_Powers    m_Bitmask_Powers;           // 1A
                                                    // SIZE 1E
} ItemPowLbx;


typedef struct PACKED_STRUCT // List_Hero_stats
{
    // TODO: Match with... what?
    //Hero_stats m_Hero_Stats[35];
    Hero_stats      Dwarf;
    Hero_stats      Barbarian;
    Hero_stats      Sage;
    Hero_stats      Dervish;
    Hero_stats      Beastmaster;
    Hero_stats      Bard;
    Hero_stats      Orc_Warrior;
    Hero_stats      Healer;
    Hero_stats      Huntress;
    Hero_stats      Thief;
    Hero_stats      Druid;
    Hero_stats      War_Monk;
    Hero_stats      Warrior_Mage;
    Hero_stats      Magician;
    Hero_stats      Assassin;
    Hero_stats      Wind_Mage;
    Hero_stats      Ranger;
    Hero_stats      Draconian;
    Hero_stats      Witch;
    Hero_stats      Golden_One;
    Hero_stats      Ninja;
    Hero_stats      Rogue;
    Hero_stats      Amazon;
    Hero_stats      Warlock;
    Hero_stats      Unknown;
    Hero_stats      Illusionist;
    Hero_stats      Swordsman;
    Hero_stats      Priestess;
    Hero_stats      Paladin;
    Hero_stats      Black_Knight;
    Hero_stats      Elven_Archer;
    Hero_stats      Knight;
    Hero_stats      Necromancer;
    Hero_stats      Chaos_Warrior;
    Hero_stats      Chosen;
} List_Hero_stats;


typedef union // unionList_Hero_stats
{
    Hero_stats      a[gMAX_HERO_TYPES];
    List_Hero_stats s;
} unionList_Hero_stats;


typedef struct PACKED_STRUCT // MapRow_Bonus
{
    eTerrainBonusDeposit   m_Bonus_Deposit[60];
} MapRow_Bonus;


typedef struct PACKED_STRUCT // MapRow_Exploration
{
    uint8_t         m_Explored[60];
} MapRow_Exploration;


typedef struct PACKED_STRUCT // MapRow_LandMassID
{
    int8_t          m_LandMassID[60];
} MapRow_LandMassID; // <read=read_MapRow_LandMassID>;


typedef struct PACKED_STRUCT // MapRow_Movement
{
    int8_t          m_Moves[60];    // -1 = impassible
} MapRow_Movement; // <read=read_MapRow_Movement>;


typedef struct PACKED_STRUCT // MapRow_Terrain
{
    eTerrainType    m_Tile[60];
} MapRow_Terrain;


typedef struct PACKED_STRUCT // Terrain_Changes
{
    uint8_t     Volcano_producing_for_Owner:3;  // 07 //1 MP/turn for Wizard 1 (YOU) through 5, 0 = no one
    uint8_t     road:1;                         // 08
    uint8_t     enchanted_road:1;               // 10   // Only visible if 'road' is also set
    uint8_t     corruption:1;                   // 20
    uint8_t     unknown40:1;                           // 40
    uint8_t     no_road80:1;                           // 80
} Terrain_Changes; // <read=read_Terrain_Changes>;


typedef struct PACKED_STRUCT // MapRow_Terrain_Changes
{
    Terrain_Changes   m_Terrain_Changes[60];
} MapRow_Terrain_Changes;


typedef struct PACKED_STRUCT // Map_Movement
{
    MapRow_Movement    m_Unused_Row[40];
    MapRow_Movement    m_Walking_Row[40];
    MapRow_Movement    m_Forester_Row[40];
    MapRow_Movement    m_Mountaineer_Row[40];
    MapRow_Movement    m_Swimming_Row[40];
    MapRow_Movement    m_Sailing_Row[40];
} Map_Movement;

typedef struct PACKED_STRUCT // Tower_Node_Lair_Flags
{
    uint8_t         m_Prisoner:1;
    uint8_t         m_YOU_aware_Inhabitant1:1;
    uint8_t         m_YOU_aware_Inhabitant2:1;
    uint8_t         m_Zero:5;
                                //01 unknown 
                                //02 YOU explored node and are aware of Inhabitant #1 
                                //04 YOU explored node and are aware of Inhabitant #2 
                                //other bits are not used and are set to zero  
} Tower_Node_Lair_Flags;

typedef union // unionTower_Node_Lair_Flags
{
    Tower_Node_Lair_Flags s;
    uint8_t bits;
} unionTower_Node_Lair_Flags;

typedef struct PACKED_STRUCT // Lair_Inhabitant
{
    eUnit_Type      m_Inhabitant;                       // 05 (monster index)
    uint8_t         m_Remaining_Nr_of_Inhabitant:4;     // 06 LSNybble
    uint8_t         m_Initial_Nr_of_Inhabitant:4;       // 06 MSNybble
} Lair_Inhabitant;

typedef struct PACKED_STRUCT // Tower_Node_Lair
{
    //uint8_t         m_Data[0x18];
    uint8_t         m_XPos;             // 00
    uint8_t         m_YPos;             // 01
    ePlane          m_Plane;            // 02
    eTower_Node_Lair_Status  m_Status;  // 03 (0 = popped, 1 = intact)
    eTower_Node_Lair_Type    m_Type;    // 04 (e.g., 0ATemple)
    Lair_Inhabitant m_Inhabitant1;      // 05-06
    Lair_Inhabitant m_Inhabitant2;      // 07-08
    uint8_t         m_Unk_09;           // 09
    uint16_t        m_Reward_Gold;      // 0A (0, 10 - 200 gold)
    uint16_t        m_Reward_Mana;      // 0C (0, 10 - 200 mana)
    eReward_Specials    m_Reward_Specials;   // 0E
                                //0 = No specials
                                //1 = Common Spell
                                //2 = Uncommon Spell
                                //3 = Rare Spell
                                //4 = Very Rare Spell
                                //5 = 1 Spellbook or Skill (74% - 26%)
                                //6 = 2 Spellbooks or Skills (74% - 26%)
    unionTower_Node_Lair_Flags m_Flags; // 0F
                                //01 Prisoner available
                                //02 YOU explored node and are aware of Inhabitant #1 
                                //04 YOU explored node and are aware of Inhabitant #2 
                                //other bits are not used and are set to zero  
    uint16_t        m_Item_Rewards;     // 10-11 (0 - 3 items)
    uint16_t        m_Item_Value[3];    // 12-17
                                        // SIZE 18
} Tower_Node_Lair; // <read=read_Tower_Node_Lair>;

typedef struct PACKED_STRUCT // Node_Attr
{
    int8_t          m_XPos;             // 0
    int8_t          m_YPos;             // 01
    ePlane          m_Plane;            // 02
    ePlayer         m_Owner;            // 03    // FF = Not owned by a player
    int8_t          m_Power_Node;       // 04
    int8_t          m_XPos_Mana[20];    // 05
    int8_t          m_YPos_Mana[20];    // 19
    eNode_Type      m_Node_Type;        // 2D
    uint8_t         m_Status;           // 2E    // 01=warped, 02=guardian spirit, 03=both
    uint8_t         m_Unk_2F;           // 2F
                                        // SIZE 30
} Node_Attr;

typedef struct PACKED_STRUCT // Fortress
{
    int8_t          m_XPos;             // 00
    int8_t          m_YPos;             // 01
    ePlane          m_Plane;            // 02
    int8_t          m_Active;           // 03   // 00 or 01=non-Eliminated or FF??=not-in-play??
} Fortress;

typedef struct PACKED_STRUCT // Tower_Attr
{
    int8_t          m_XPos;             // 00
    int8_t          m_YPos;             // 01
    uint8_t         m_Closed;           // 02   // 00=opened, FF=closed, sealed??
    uint8_t         m_Unk_03;           // 03   // unused ??
} Tower_Attr;

typedef struct PACKED_STRUCT // Unit_Weapon_Mutation
{
    //eWeaponType   Weapon_Type:2;
    uint8_t     Weapon_Type:2;
    uint8_t     Chaos_Channels_Demon_Skin:1;
    uint8_t     Chaos_Channels_Demon_Wings:1;
    uint8_t     Chaos_Channels_Fiery_Breath:1;
    uint8_t     Undead:1;
    uint8_t     Stasis_initial:1;
    uint8_t     Stasis_lingering:1;
} Unit_Weapon_Mutation; // <read=read_Unit_Weapon_Mutation>;

typedef union // unionUnit_Weapon_Mutation
{
    Unit_Weapon_Mutation  s;
    uint8_t                bits; // <format=hex>;
} unionUnit_Weapon_Mutation;

typedef struct PACKED_STRUCT // Unit_Enchantment
{
    // TODO: Match with eSpell
    uint8_t     Immolation:1;       // 01
    uint8_t     Guardian_Wind:1;    // 02
    uint8_t     Berserk:1;          // 04
    uint8_t     Cloak_of_Fear:1;    // 08
    uint8_t     Black_Channels:1;   // 10
    uint8_t     Wraith_Form:1;      // 20
    uint8_t     Regeneration:1;     // 40
    uint8_t     Path_Finding:1;     // 80

    uint8_t     Water_Walking:1;    // 01
    uint8_t     Resist_Elements:1;  // 02
    uint8_t     Elemental_Armor:1;  // 04
    uint8_t     Stone_Skin:1;       // 08
    uint8_t     Iron_Skin:1;        // 10
    uint8_t     Endurance:1;        // 20
    uint8_t     Spell_Lock:1;       // 40
    uint8_t     Invisibility:1;     // 80

    uint8_t     Wind_Walking:1;     // 01
    uint8_t     Flight:1;           // 02
    uint8_t     Resist_Magic:1;     // 04
    uint8_t     Magic_Immunity:1;   // 08
    uint8_t     Flame_Blade:1;      // 10
    uint8_t     Eldritch_Weapon:1;  // 20
    uint8_t     True_Sight:1;       // 40
    uint8_t     Holy_Weapon:1;      // 80

    uint8_t     Heroism:1;          // 01
    uint8_t     Bless:1;            // 02
    uint8_t     Lionheart:1;        // 04
    uint8_t     Giant_Strength:1;   // 08
    uint8_t     Planar_Travel:1;    // 10
    uint8_t     Holy_Armor:1;       // 20
    uint8_t     Righteousness:1;    // 40
    uint8_t     Invulnerability:1;  // 80
} Unit_Enchantment; // <read=read_Unit_Enchantment>;


typedef union // unionUnit_Enchantment
{
    Unit_Enchantment  s;
    uint32_t                bits; // <format=hex>;
} unionUnit_Enchantment; // <read=read_unionUnit_Enchantment>;

typedef struct PACKED_STRUCT // Spells_Cast_in_Battle
{
    // TODO: Match with eSpell
    uint8_t True_Light[2];          // 00-01    1=True Light (Battle), 2=Heavenly Light (City) / Cloud of Shadow (City), 3=Eternal Darkness (Global)
    uint8_t Darkness[2];            // 02-03
    uint8_t Warp_Reality[2];        // 04-05
    uint8_t Black_Prayer[2];        // 06-07
    uint8_t Wrack[2];               // 08-09
    uint8_t Metal_Fires[2];         // 0A-0B
    uint8_t Prayer[2];              // 0C-0D
    uint8_t High_Prayer[2];         // 0E-0F
    uint8_t Terror[2];              // 10-11
    uint8_t Call_Lightning[2];      // 12-13
    uint8_t Counter_Magic[2];       // 14-15
    uint8_t Mass_Invisibility[2];   // 16-17
    uint8_t Entangle[2];            // 18-19
    uint8_t Mana_Leak[2];           // 1A-1B
    uint8_t Blur[2];                // 1C-1D
                                    // SIZE 1E
} Spells_Cast_in_Battle;

typedef struct PACKED_STRUCT { // Spells_Known (order in file):
    // TODO: Match with eSpell
    eSpellKnown     No_spell;

    //    * NATURE
    eSpellKnown     Earth_to_Mud;
    eSpellKnown     Resist_Elements;
    eSpellKnown     Wall_of_Stone;
    eSpellKnown     Giant_Strength;
    eSpellKnown     Web;
    eSpellKnown     War_Bears;
    eSpellKnown     Stone_Skin;
    eSpellKnown     Water_Walking;
    eSpellKnown     Sprites;
    eSpellKnown     Earth_Lore;

    eSpellKnown     Cracks_Call;
    eSpellKnown     Natures_eye;
    eSpellKnown     Ice_Bolt;
    eSpellKnown     Giant_Spiders;
    eSpellKnown     Change_Terrain;
    eSpellKnown     Path_Finding;
    eSpellKnown     Cockatrices;
    eSpellKnown     Transmute;
    eSpellKnown     Natures_Cures;
    eSpellKnown     Basilisk;

    eSpellKnown     Elemental_Armor;
    eSpellKnown     Petrify;
    eSpellKnown     Stone_Giant;
    eSpellKnown     Iron_Skin;
    eSpellKnown     Ice_Storm;
    eSpellKnown     Earthquake;
    eSpellKnown     Gorgons;
    eSpellKnown     Move_Fortress;
    eSpellKnown     Gaias_Blessing;
    eSpellKnown     Earth_Elemental;

    eSpellKnown     Regeneration;
    eSpellKnown     Behemoth;
    eSpellKnown     Entangle;
    eSpellKnown     Nature_Awareness;
    eSpellKnown     Call_Lightning;
    eSpellKnown     Colossus;
    eSpellKnown     Earth_Gate;
    eSpellKnown     Herb_Mastery;
    eSpellKnown     Great_Wyrm;
    eSpellKnown     Natures_Wrath;

    //    * SORCERY
    eSpellKnown     Resist_Magic;
    eSpellKnown     Dispel_Magic_True;
    eSpellKnown     Floating_Island;
    eSpellKnown     Guardian_Wind;
    eSpellKnown     Phantom_Warriors;
    eSpellKnown     Confusion;
    eSpellKnown     Word_of_Recall;
    eSpellKnown     Counter_Magic;
    eSpellKnown     Nagas;
    eSpellKnown     Psionic_Blast;

    eSpellKnown     Blur;
    eSpellKnown     Disenchant_True;
    eSpellKnown     Vertigo;
    eSpellKnown     Spell_Lock;
    eSpellKnown     Enchant_Road;
    eSpellKnown     Flight;
    eSpellKnown     Wind_Mastery;
    eSpellKnown     Spell_Blast;
    eSpellKnown     Aura_of_Majesty;
    eSpellKnown     Phantom_Beast;

    eSpellKnown     Disjunction_True;
    eSpellKnown     Invisibility;
    eSpellKnown     Wind_Walking;
    eSpellKnown     Banish;
    eSpellKnown     Storm_Giant;
    eSpellKnown     Air_Elemental;
    eSpellKnown     Mind_Storm;
    eSpellKnown     Stasis;
    eSpellKnown     Magic_Immunity;
    eSpellKnown     Haste;

    eSpellKnown     Djinn;
    eSpellKnown     Spell_Ward;
    eSpellKnown     Creature_Binding;
    eSpellKnown     Mass_Invisibility;
    eSpellKnown     Great_Unsummoning;
    eSpellKnown     Spell_Binding;
    eSpellKnown     Flying_Fortress;
    eSpellKnown     Sky_Drake;
    eSpellKnown     Suppress_Magic;
    eSpellKnown     Time_Stop;

    //    * CHAOS
    eSpellKnown     Warp_Wood;
    eSpellKnown     Disrupt;
    eSpellKnown     Fire_Bolt;
    eSpellKnown     Hell_Hounds;
    eSpellKnown     Corruption;
    eSpellKnown     Eldritch_Weapon;
    eSpellKnown     Wall_of_Fire;
    eSpellKnown     Shatter;
    eSpellKnown     Warp_Creature;
    eSpellKnown     Fire_Elemental;

    eSpellKnown     Lightning_Bolt;
    eSpellKnown     Fire_Giant;
    eSpellKnown     Chaos_Channels;
    eSpellKnown     Flame_Blade;
    eSpellKnown     Gargoyles;
    eSpellKnown     Fireball;
    eSpellKnown     Doombat;
    eSpellKnown     Raise_Volcano;
    eSpellKnown     Immolation;
    eSpellKnown     Chimeras;

    eSpellKnown     Warp_Lightning;
    eSpellKnown     Metal_Fires;
    eSpellKnown     Chaos_Spawn;
    eSpellKnown     Doom_Bolt;
    eSpellKnown     Magic_Vortex;
    eSpellKnown     Efreet;
    eSpellKnown     Fire_Storm;
    eSpellKnown     Warp_Reality;
    eSpellKnown     Flame_Strike;
    eSpellKnown     Chaos_Rift;

    eSpellKnown     Hydra;
    eSpellKnown     Disintegrate;
    eSpellKnown     Meteor_Storm;
    eSpellKnown     Great_Wasting;
    eSpellKnown     Call_Chaos;
    eSpellKnown     Chaos_Surge;
    eSpellKnown     Doom_Mastery;
    eSpellKnown     Great_Drake;
    eSpellKnown     Call_The_Void;
    eSpellKnown     Armageddon;

    //    * LIFE
    eSpellKnown     Bless;
    eSpellKnown     Star_Fires;
    eSpellKnown     Endurance;
    eSpellKnown     Holy_Weapon;
    eSpellKnown     Healing;
    eSpellKnown     Holy_Armor;
    eSpellKnown     Just_Cause;
    eSpellKnown     True_Light;
    eSpellKnown     Guardian_Spirit;
    eSpellKnown     Heroism;

    eSpellKnown     True_Sight;
    eSpellKnown     Plane_Shift;
    eSpellKnown     Resurrection;
    eSpellKnown     Dispel_Evil;
    eSpellKnown     Planar_Seal;
    eSpellKnown     Unicorns;
    eSpellKnown     Raise_Dead;
    eSpellKnown     Planar_Travel;
    eSpellKnown     Heavenly_Light;
    eSpellKnown     Prayer;

    eSpellKnown     Lionheart;
    eSpellKnown     Incarnation;
    eSpellKnown     Invulnerability;
    eSpellKnown     Righteousness;
    eSpellKnown     Prosperity;
    eSpellKnown     Altar_of_Battle;
    eSpellKnown     Angel;
    eSpellKnown     Stream_of_Life;
    eSpellKnown     Mass_Healing;
    eSpellKnown     Holy_Word;

    eSpellKnown     High_Prayer;
    eSpellKnown     Inspirations;
    eSpellKnown     Astral_Gate;
    eSpellKnown     Holy_Arms;
    eSpellKnown     Consecration;
    eSpellKnown     Life_Force;
    eSpellKnown     Tranquility;
    eSpellKnown     Crusade;
    eSpellKnown     Arch_Angel;
    eSpellKnown     Charm_of_Life;

    //    * DEATH
    eSpellKnown     Skeletons;
    eSpellKnown     Weakness;
    eSpellKnown     Dark_Rituals;
    eSpellKnown     Cloak_of_Fear;
    eSpellKnown     Black_Sleep;
    eSpellKnown     Ghouls;
    eSpellKnown     Life_Drain;
    eSpellKnown     Terror;
    eSpellKnown     Darkness;
    eSpellKnown     Mana_Leak;

    eSpellKnown     Drain_Power;
    eSpellKnown     Possession;
    eSpellKnown     Lycantrophy;
    eSpellKnown     Black_Prayer;
    eSpellKnown     Black_Channels;
    eSpellKnown     Night_Stalker;
    eSpellKnown     Subversion;
    eSpellKnown     Wall_of_Darkness;
    eSpellKnown     Berserk;
    eSpellKnown     Shadow_Demons;

    eSpellKnown     Wraith_Form;
    eSpellKnown     Wrack;
    eSpellKnown     Evil_Presence;
    eSpellKnown     Wraiths;
    eSpellKnown     Cloud_of_Shadow;
    eSpellKnown     Warp_Node;
    eSpellKnown     Black_Wind;
    eSpellKnown     Zombie_Mastery;
    eSpellKnown     Famine;
    eSpellKnown     Cursed_Lands;

    eSpellKnown     Cruel_Unminding;
    eSpellKnown     Word_of_Death;
    eSpellKnown     Death_Knights;
    eSpellKnown     Death_Spell;
    eSpellKnown     Animate_dead;
    eSpellKnown     Pestilence;
    eSpellKnown     Eternal_Night;
    eSpellKnown     Evil_Omens;
    eSpellKnown     Death_Wish;
    eSpellKnown     Demon_Lord;

    //    * ARCANE
    eSpellKnown     Magic_Spirit;
    eSpellKnown     Dispel_Magic;
    eSpellKnown     Summoning_Circle;
    eSpellKnown     Disenchant_Area;
    eSpellKnown     Recall_Hero;
    eSpellKnown     Detect_Magic;
    eSpellKnown     Enchant_Item;
    eSpellKnown     Summon_Hero;
    eSpellKnown     Awareness;
    eSpellKnown     Disjunction;
    eSpellKnown     Create_Artifact;
    eSpellKnown     Summon_Champion;
    eSpellKnown     Spell_Of_Mastery;
    eSpellKnown     Spell_Of_Return;
                                        // SIZE D7
} Spells_Known;

typedef struct PACKED_STRUCT // Wizard_Diplomacy
{
    int8_t          m_Contacted[6];                     // 128
    int16_t         m_Consider_or_offer_Treaties[6];    // 12E
    int16_t         m_Offer_Peace[6];                   // 13A
    int16_t         m_Trade_Spells[6];                  // 146
    int8_t          m_Current_Wizard_Relations[6];      // 152
                                                        //    Notes:
                                                        //    Minimum -100 (if below, game resets to this).
                                                        //    Maximum 100 (if above, game resets to this).
                                                        //
                                                        //    Values:
                                                        //    Empty     -128...-120 (128-136)
                                                        //    Hate      -119...-101 (137-155)
                                                        //    Hate      -100...-81 (156-175)
                                                        //    Troubled  -80...-61 (176-195)
                                                        //    Tense     -60...-41 (196-215)
                                                        //    Restless  -40...-21 (216-235)
                                                        //    Unease    -20...-1 (236-255)
                                                        //    Neutral   0...19
                                                        //    Relaxed   20...39
                                                        //    Calm      40...59
                                                        //    Peaceful  60...79
                                                        //    Friendly  80...99
                                                        //    Harmony   100...119
                                                        //    No Treaty 120...127
    eWar_Status     m_War_Status[6];                    // 158 (0=None, 1=Wizard Pact, 2=Alliance, 3+=War)
    int16_t         m_Diplomatic_action_strength_AI[6]; // 15E
    int8_t          m_Diplomatic_action_AI[6];          // 16A
    int16_t         m_SpellID_in_diplomatic_action[6];  // 170
    int8_t          m_CityID_in_diplomatic_action[6];   // 17C
    int8_t          m_Unk_182_A04C[6];                  // 182
    int8_t          m_Initial_Contact_made[6];          // 188 (0 no contact, 1 contact made but not introduced yet, 2 already introduced)
    int8_t          m_Last_violated_treaty[6];          // 18E
    int16_t         m_Unk_194_A05E[6];                  // 194
    int8_t          m_Grudge_over_broken_treaties[6];   // 1A0 (aka Hidden Relation Modifier)
    int8_t          m_Unk_1A6_A070[0xB4];               // 1A6
} Wizard_Diplomacy;                                     // 25A

typedef struct PACKED_STRUCT // Wizard
{
    ePortrait       m_Portrait;                         // 000 (0-13)
    char            m_Name[20];                         // 001 (20 characters incl. '\0')
    eRace           m_Home_Race;                        // 015
    eBannerColor    m_BannerColor;                      // 016 (0-4)
    ePersonality    m_Personality;                      // 018 (0-5)
    eObjective      m_Objective;                        // 01A (0-4)
    uint8_t         m_Unk_01C[6];                       // 01C
    uint16_t        m_ResearchCost_SpellofMastery_GUESS;// 022
    uint16_t        m_Fame;                             // 024
    uint16_t        m_Power_Base;                       // 026
    uint16_t        m_Nr_controlled_volcanoes;          // 028
    uint8_t         m_Research_Percentage;              // 02A
    uint8_t         m_Mana_Percentage;                  // 02B
    uint8_t         m_Skill_Percentage;                 // 02C
    uint8_t         m_Mana_generated_by_volcanoes;      // 02D
    int16_t         m_X_Coordinate_of_Summoning_Circle; // 02E (0-59)
    int16_t         m_Y_Coordinate_of_Summoning_Circle; // 030 (0-59)
    ePlane          m_Plane_of_Summoning_Circle;        // 032 (0-1)
    uint8_t         m_Zero_033;
    eSpell16        m_Research_Spell_candidates[8];     // 034
    uint8_t         m_Unk_044[4];                       // 044
    int16_t         m_Garrison_average_strength;        // 048  // Average cost of units, units cheaper than half are dismissed
    uint16_t        m_Unk_04A;                          // 04A
    int16_t         m_Skill_Left_in_combat;             // 04C
    int16_t         m_Cost_Left_of_Spell_being_cast;    // 04E
    int16_t         m_Initial_Cost_of_Spell_being_cast; // 050
    eSpell16        m_Spell_being_cast;                 // 052
    int16_t         m_Unused_Casting_Skill_available_this_turn;     // 054
    int16_t         m_Nominal_Casting_Skill_available_this_turn;    // 056
    eTax_Rate       m_Tax_Rate;                         // 058
    int16_t         m_Number_of_Spellbooks_Nature;      // 05A (0-12) (Na-So-Ch-Li-De)
    int16_t         m_Number_of_Spellbooks_Sorcery;     //
    int16_t         m_Number_of_Spellbooks_Chaos;       //
    int16_t         m_Number_of_Spellbooks_Life;        //
    int16_t         m_Number_of_Spellbooks_Death;       //
    unionSkills     m_Skills;                           // 064
    Hired_Hero      m_Heroes_hired_by_wizard[gMAX_HIRED_HEROES];    // 076 six slots
    uint16_t        m_Unk_11E;                          // 11E
    int16_t         m_Items_in_Slots[4];                // 120 (See below)
    Wizard_Diplomacy m_Diplomacy_1;                     // 128
    int16_t         m_Researching_Left;                 // 25A
    int16_t         m_Mana_Crystals;                    // 25C
    int32_t         m_Wizard_Casting_Skill;             // 25E <read=read_Wizard_Casting_Skill>;
                                                        //     (actual skill is square root of value stored here, rounded up)
    eSpell          m_Researching_Spell;                // 262
    Spells_Known    m_Spells_Known;                     // 263 Spells (0-3) (None-Na-So-Ch-Li-De-Ar)
    int8_t          m_Unk_33A[26];                      // 33A
    uint16_t        m_Bitmask_banished_wizards;         // 354
    int16_t         m_Gold_Coins;                       // 356
    int16_t         m_Unk_358;                          // 358
    int16_t         m_Astrologer_Magic_Power;           // 35A (0-200)
    int16_t         m_Astrologer_Spell_Research;        // 35C (0-200)
    int16_t         m_Astrologer_Army_Strength;         // 35E (0-200)
    int16_t         m_Population_div_10000;             // 360
    int8_t          m_Historian[288];                   // 362
                                                        //  Values: 0...0xA0 (0...160) - Sum of Magic Power, Army Strength, and Spell Research
                                                        //  Notes: 0xA0 is barely above the graph
    Global_Enchantments  m_Global_Enchantments;         // 482
    uint16_t        m_Unk_49A_Power_Distribution;       // 49A
    uint16_t        m_Unk_49C;                          // 49C
    uint8_t         m_Hostility[6];                     // 49E
    uint8_t         m_Unk_4A4[6];                       // 4A4
    uint16_t        m_Hostility_Counter;                // 4AA
    uint8_t         m_Peace_Counter[6];                 // 4AC
    uint8_t         m_Unk_4B2[18];                      // 4B2
    eRealm_Type16   m_Primary_Book_Color;               // 4C4
    eRealm_Type16   m_Secondary_Book_Color;             // 4C6
                                                        // SIZE 4C8
} Wizard; // <read=read_Wizard>;

typedef struct PACKED_STRUCT // Wizard_Type_Data
{
    char            m_Wizard_Name[10];                  // 00
    uint16_t        m_Life_books;                       // 0A
    uint16_t        m_Sorcery_books;                    // 0C
    uint16_t        m_Nature_books;                     // 0E
    uint16_t        m_Death_books;                      // 10
    uint16_t        m_Chaos_books;                      // 12
    eSkill16        m_Skill;                            // 14
                                                        // SIZE 16
} Wizard_Type_Data;

typedef struct PACKED_STRUCT // City
{
    char            m_City_Name[14];            // 00
    eRace           m_Race;                     // 0E
    int8_t          m_XPos;                     // 0F (0-59)
    int8_t          m_YPos;                     // 10 (0-59)
    ePlane          m_Plane;                    // 11 (0 or 1)
    ePlayer         m_Owner;                    // 12 (0-4)
    eCity_Size      m_Size;                     // 13 (0-5; see below)
    int8_t          m_Population;               // 14 (in thousands) (0-25)
    int8_t          m_Number_of_farmers_allocated;  // 15 (should be <= population)
    eYesNo8         m_Building_sold;            // 16
    uint8_t         m_CoM_Power;                // 17
    int16_t         m_PopulationDekaPop;        // 18-19 (in tenths) (Outpost is 1-9, else 0-99)
    uint8_t         m_Player_as_bitmask_GUESS;  // 1A
    uint8_t         m_CoM_Gold;                 // 1B
    eProducing      m_Producing;                // 1C-1D (0-??; see below)
    uint8_t         m_Nr_buildings;             // 1E
    unionBuilding_Status    m_Building_Status;  // 1F-42
    City_Enchantments       m_City_Enchantments;// 43-5B (0 = not present, 1-5 = owner + 1)
    eYesNo8         m_Nightshade;               // 5C (0 or 1)
    int8_t          m_Hammers;                  // 5D
    int16_t         m_HammersAccumulated;       // 5E-5F
    int8_t          m_Coins;                    // 60
    int8_t          m_Maintenance;              // 61
    int8_t          m_Mana_cr;                  // 62
    int8_t          m_Research;                 // 63
    int8_t          m_Food_Produced;            // 64
    uint8_t         m_bitsetConnectedCities[13];// 65
                                                // SIZE 72
} City; // <read=read_City>;

typedef struct PACKED_STRUCT // Unit
{
    int8_t      m_XPos;                     // 00 X-Pos (0-59)
    int8_t      m_YPos;                     // 01 Y-Pos (0-39)
    ePlane      m_Plane;                    // 02 Plane
                                            //   00=Arcanus
                                            //   01=Myrror
                                            //   FF=Dismissed / Deceased
    ePlayer     m_Owner;                    // 03 'Owner'
                                            //   00=Player
                                            //   01=First Opponent
                                            //   02=Second Opponent
                                            //   03=Third Opponent
                                            //   04=Fourth Opponent
                                            //   05=Neutral Unit
                                            //   FF=Dismissed / Deceased
    int8_t      m_Moves_Total;              // 04 Moves Total (in 1/2 moves)
    eUnit_Type  m_Unit_Type;                // 05 Icon Number (0x00 - 0xC5)
    int8_t      m_Hero_Slot_Number;         // 06 Hero Slot Number (0-5; -1 (0xFF) means not a hero) (see below)
    eUnit_Active    m_Active;               // 07 Active
    int8_t      m_Moves_Left;               // 08 Moves Left (in 1/2 moves)
    int8_t      m_XPos_of_destination;      // 09 X-Pos of destination (should be 0 if status is not GOING TO)
    int8_t      m_YPos_of_destination;      // 0A Y-Pos of destination (should be 0 if status is not GOING TO)
    eUnit_Status8   m_Status;               // 0B Status (0-5)
    eLevel      m_Level;                    // 0C Level (Units: 0-5; Heroes: 0-8; Summon (other than Torin): 0)
    int8_t      m_Unk_0D;                   // 0D ?? 00
    int16_t     m_Experience;               // 0E-0F Experience (should match level or level will be lowered)
                                            //  (Units: 0-120; Heroes: 0-1000; Summon (other than Torin): 0)
    int8_t      m_Unk_10;                   // 10
    int8_t      m_Damage;                   // 11
    int8_t      m_Grouping;                 // 12 group=Me+Ra, +50 if transport, -1=invisible or dead, 0=active stack
    int8_t      m_Guess_Combat_Enchantment_Flag1; // 13
    int8_t      m_In_Tower_without_Seal;	// 14
    int8_t      m_Guess_Combat_Enchantment_Flag3; // 15
    int8_t      m_Scouting;                 // 16
    unionUnit_Weapon_Mutation   m_Weapon_Mutation;  // 17
    unionUnit_Enchantment       m_Unit_Enchantment; // 18-1B <format=hex>;
    int8_t      m_Road_Building_left_to_complete;   // 1C
    int8_t      m_XPos_Road_Building_from;  // 1D
    int8_t      m_YPos_Road_Building_from;  // 1E
    int8_t      m_Unk_1F;					// 1F
                                            // SIZE 20
} Unit; // <read=read_Unit>;

typedef struct PACKED_STRUCT // Upkeep_Enchantments
{
    // TODO: Match with eSpell
    // Unit_Enchantments
    uint16_t Immolation; // 0 Offset EXE:2B3B8
    uint16_t Guardian_Wind; // 2 Offset EXE:2B3BA
    uint16_t Berserk; // 4 Offset EXE:2B3BC     // Note(12)
    uint16_t Cloak_of_Fear; // 6 Offset EXE:2B3BE
    uint16_t Black_Channels; // 8 Offset EXE:2B3C0
    uint16_t Wraith_Form; // A Offset EXE:2B3C2
    uint16_t Regeneration; // C Offset EXE:2B3C4
    uint16_t Path_Finding; // E Offset EXE:2B3C6
    uint16_t Water_Walking; // 10 Offset EXE:2B3C8
    uint16_t Resist_Elements; // 12 Offset EXE:2B3CA
    uint16_t Elemental_Armor; // 14 Offset EXE:2B3CC
    uint16_t Stone_Skin; // 16 Offset EXE:2B3CE
    uint16_t Iron_Skin; // 18 Offset EXE:2B3D0
    uint16_t Endurance; // 1A Offset EXE:2B3D2
    uint16_t Spell_Lock; // 1C Offset EXE:2B3D4
    uint16_t Invisibility; // 1E Offset EXE:2B3D6
    uint16_t Wind_Walking; // 20 Offset EXE:2B3D8
    uint16_t Flight; // 22 Offset EXE:2B3DA
    uint16_t Resist_Magic; // 24 Offset EXE:2B3DC
    uint16_t Magic_Immunity; // 26 Offset EXE:2B3DE
    uint16_t Flame_Blade; // 28 Offset EXE:2B3E0
    uint16_t Eldritch_Weapon; // 2A Offset EXE:2B3E2
    uint16_t True_Sight; // 2C Offset EXE:2B3E4
    uint16_t Holy_Weapon; // 2E Offset EXE:2B3E6
    uint16_t Heroism; // 30 Offset EXE:2B3E8
    uint16_t Bless; // 32 Offset EXE:2B3EA
    uint16_t Lionheart; // 34 Offset EXE:2B3EC
    uint16_t Giant_Strength; // 36 Offset EXE:2B3EE
    uint16_t Planar_Travel; // 38 Offset EXE:2B3F0
    uint16_t Holy_Armor; // 3A Offset EXE:2B3F2
    uint16_t Righteousness; // 3C Offset EXE:2B3F4
    uint16_t Invulnerability; // 3E Offset EXE:2B3F6

    // City_Enchantments
    uint16_t Wall_of_Fire; // 40 Offset EXE:2B3F8
    uint16_t Chaos_Rift; // 42 Offset EXE:2B3FA
    uint16_t Dark_Rituals; // 44 Offset EXE:2B3FC
    uint16_t Evil_Presence; // 46 Offset EXE:2B3FE
    uint16_t Cursed_Lands; // 48 Offset EXE:2B400
    uint16_t Pestilence; // 4A Offset EXE:2B402
    uint16_t Cloud_of_Shadow; // 4C Offset EXE:2B404
    uint16_t Famine; // 4E Offset EXE:2B406
    uint16_t Flying_Fortress; // 50 Offset EXE:2B408
    uint16_t Nature_Ward; // 52 Offset EXE:2B40A
    uint16_t Sorcery_Ward; // 54 Offset EXE:2B40C
    uint16_t Chaos_Ward; // 56 Offset EXE:2B40E
    uint16_t Life_Ward; // 58 Offset EXE:2B410
    uint16_t Death_Ward; // 5A Offset EXE:2B412
    uint16_t Natures_Eye; // 5C Offset EXE:2B414
    uint16_t Earth_Gate; // 5E Offset EXE:2B416
    uint16_t Stream_of_Life; // 60 Offset EXE:2B418
    uint16_t Gaias_Blessing; // 62 Offset EXE:2B41A
    uint16_t Inspirations; // 64 Offset EXE:2B41C
    uint16_t Prosperity; // 66 Offset EXE:2B41E
    uint16_t Astral_Gate; // 68 Offset EXE:2B420
    uint16_t Heavenly_Light; // 6A Offset EXE:2B422
    uint16_t Consecration; // 6C Offset EXE:2B424
    uint16_t Wall_of_Darkness; // 6E Offset EXE:2B426
    uint16_t Altar_of_Battle; // 70 Offset EXE:2B428
    uint16_t Nightshade; // 72 Offset EXE:2B42A

    // Global Enchantments
    uint16_t Eternal_Night; // 74 Offset EXE:2B42C
    uint16_t Evil_Omens; // 76 Offset EXE:2B42E
    uint16_t Zombie_Mastery; // 78 Offset EXE:2B430
    uint16_t Aura_of_Majesty; // 7A Offset EXE:2B432
    uint16_t Wind_Mastery; // 7C Offset EXE:2B434
    uint16_t Suppress_Magic; // 7E Offset EXE:2B436
    uint16_t Time_Stop; // 80 Offset EXE:2B438
    uint16_t Nature_Awareness; // 82 Offset EXE:2B43A
    uint16_t Natures_Wrath; // 84 Offset EXE:2B43C
    uint16_t Herb_Mastery; // 86 Offset EXE:2B43E
    uint16_t Chaos_Surge; // 88 Offset EXE:2B440
    uint16_t Doom_Mastery; // 8A Offset EXE:2B442
    uint16_t Great_Wasting; // 8C Offset EXE:2B444
    uint16_t Meteor_Storm; // 8E Offset EXE:2B446
    uint16_t Armageddon; // 90 Offset EXE:2B448
    uint16_t Tranquility; // 92 Offset EXE:2B44A
    uint16_t Life_Force; // 94 Offset EXE:2B44C
    uint16_t Crusade; // 96 Offset EXE:2B44E
    uint16_t Just_Cause; // 98 Offset EXE:2B450
    uint16_t Holy_Arms; // 9A Offset EXE:2B452
    uint16_t Planar_Seal; // 9C Offset EXE:2B454
    uint16_t Charm_of_Life; // 9E Offset EXE:2B456
    uint16_t Detect_Magic; // A0 Offset EXE:2B458
    uint16_t Awareness; // A2 Offset EXE:2B45A
                        // SIZE A4
} Upkeep_Enchantments;

typedef struct PACKED_STRUCT // Hero_Choice
{
    char        m_Name[14];         // 00
    int16_t     m_Experience;       // 0E
                                    // SIZE 10
} Hero_Choice;

typedef struct PACKED_STRUCT // SaveGame
{
    unionList_Hero_stats    m_List_Hero_stats[gMAX_WIZARD_RECORDS];
    Game_Data_Save      m_Game_Data;
    Wizard              m_Wizards[gMAX_WIZARD_RECORDS];

    // Map section 1
    MapRow_Terrain      m_Arcanus_Map_Row[40];
    MapRow_Terrain      m_Myrror_Map_Row[40];
    uint16_t            m_Arcanus_UNK01[0x60];
    uint16_t            m_Myrror_UNK01[0x60];
    MapRow_LandMassID   m_Arcanus_LandMassID_Row[40];
    MapRow_LandMassID   m_Myrror_LandMassID_Row[40];

    Node_Attr           m_Arcanus_Node_Attr[16];
    Node_Attr           m_Myrror_Node_Attr[14];
    Fortress            m_Fortresses[gMAX_WIZARD_RECORDS];
    Tower_Attr          m_Tower_Attr[6];

    // Tower_Node_Lair m_Tower_Node_Lair[102];          // Only 99! Or can it be 102 max?
    //                                                  // When a city is reduced to rubble, an old popped lair is recycled
    Tower_Node_Lair     m_Arcanus_Towers[6];
    Tower_Node_Lair     m_Myrror_Towers[6];
    Tower_Node_Lair     m_Arcanus_Nodes[16];
    Tower_Node_Lair     m_Myrror_Nodes[14];
    Tower_Node_Lair     m_Normal_Lairs[25];
    Tower_Node_Lair     m_Weak_Lairs[32];
    Tower_Node_Lair     m_ExtraLairs_GUESS[3];

    Item                m_Items[128];
    Item                m_Item_Trashcan_GUESS[8];
    Item                m_Item_Found;                   // An item from ITEMDATA.LBX is parked here before inserting it into the game
    Item                m_Item_Created;                 // An item being created is parked here before inserting it into the game
    City                m_Cities[gMAX_CITIES];
    Unit                m_Unit[1000];
    Unit                m_Units_Trashcan_GUESS[9];

    // Map section 2
    MapRow_Bonus        m_Arcanus_Bonus_Row[40];
    MapRow_Bonus        m_Myrror_Bonus_Row[40];
    MapRow_Exploration  m_Arcanus_Exploration_Row[40];
    MapRow_Exploration  m_Myrror_Exploration_Row[40];
    Map_Movement        m_Arcanus_Movement;
    Map_Movement        m_Myrror_Movement;

    Events_Status       m_Events_Status;

    // Map section 3
    MapRow_Terrain_Changes  m_Arcanus_Terrain_Changes_Row[40];
    MapRow_Terrain_Changes  m_Myrror_Terrain_Changes_Row[40];

    eGrand_Vizier       m_Grand_Vizier;
    uint8_t             m_Artifacts_in_Game[gMAX_ARTIFACTS_IN_GAME];   // 00 = not in game, 01 = in game
    Hero_Choice         m_Chosen_Hero_Names[gMAX_HERO_TYPES];
} SaveGame;


typedef struct PACKED_STRUCT // SaveGame_CasterOfMagic
{
    unionList_Hero_stats    m_List_Hero_stats[gMAX_WIZARD_RECORDS];
    Game_Data_Save      m_Game_Data;
    Wizard              m_Wizards[gMAX_WIZARD_RECORDS];

    // Map section 1
    MapRow_Terrain      m_Arcanus_Map_Row[40];
    MapRow_Terrain      m_Myrror_Map_Row[40];
    uint16_t            m_Arcanus_UNK01[0x60];
    uint16_t            m_Myrror_UNK01[0x60];
    MapRow_LandMassID   m_Arcanus_LandMassID_Row[40];
    MapRow_LandMassID   m_Myrror_LandMassID_Row[40];

    Node_Attr           m_Arcanus_Node_Attr[17];
    Node_Attr           m_Myrror_Node_Attr[13];
    Fortress            m_Fortresses[gMAX_WIZARD_RECORDS];
    Tower_Attr          m_Tower_Attr[6];

    // Tower_Node_Lair m_Tower_Node_Lair[102];          // Only 99! Or can it be 102 max?
    //                                                  // When a city is reduced to rubble, an old popped lair is recycled
    Tower_Node_Lair     m_tn_dummy[1];
    Tower_Node_Lair     m_Arcanus_Towers[9];
    Tower_Node_Lair     m_Myrror_Towers[9];
    Tower_Node_Lair     m_Arcanus_Nodes[17];
    Tower_Node_Lair     m_Myrror_Nodes[13];
    Tower_Node_Lair     m_Normal_Lairs[26];
    Tower_Node_Lair     m_Weak_Lairs[27];
    Tower_Node_Lair     m_ExtraLairs_GUESS[1];

    Item                m_Items[128];
    Item                m_Item_Trashcan_GUESS[8];
    Item                m_Item_Found;                   // An item from ITEMDATA.LBX is parked here before inserting it into the game
    Item                m_Item_Created;                 // An item being created is parked here before inserting it into the game
    City                m_Cities[gMAX_CITIES];
    Unit                m_Unit[2000];
    Unit                m_Units_Trashcan_GUESS[18];

    // Map section 2
    MapRow_Bonus        m_Arcanus_Bonus_Row[40];
    MapRow_Bonus        m_Myrror_Bonus_Row[40];
    MapRow_Exploration  m_Arcanus_Exploration_Row[40];
    MapRow_Exploration  m_Myrror_Exploration_Row[40];
    Map_Movement        m_Arcanus_Movement;
    Map_Movement        m_Myrror_Movement;

    Events_Status       m_Events_Status;

    // Map section 3
    MapRow_Terrain_Changes  m_Arcanus_Terrain_Changes_Row[40];
    MapRow_Terrain_Changes  m_Myrror_Terrain_Changes_Row[40];

    eGrand_Vizier       m_Grand_Vizier;
    uint8_t             m_Artifacts_in_Game[gMAX_ARTIFACTS_IN_GAME];   // 00 = not in game, 01 = in game
    Hero_Choice         m_Chosen_Hero_Names[gMAX_HERO_TYPES];
} SaveGame_CasterOfMagic;

//---------------------------------------------

typedef struct PACKED_STRUCT // Movement_Flags
{
    uint8_t     Cavalry:1;     // 01
    uint8_t     Sailing:1;      // 02
    uint8_t     Swimming:1;     // 04
    uint8_t     Flying:1;       // 08
    uint8_t     Teleporting:1;      // 10
    uint8_t     Forester:1;     // 20
    uint8_t     Mountaineer:1;      // 40
    uint8_t     Merging:1;     // 80
} Movement_Flags;

typedef struct PACKED_STRUCT // Immunity_Flags
{
    uint8_t     Fire_Immunity:1;        // 01
    uint8_t     Stoning_Immunity:1;     // 02
    uint8_t     Missiles_Immunity:1;    // 04
    uint8_t     Illusions_Immunity:1;   // 08
    uint8_t     Cold_Immunity:1;        // 10
    uint8_t     Magic_Immunity:1;       // 20
    uint8_t     Death_Immunity:1;       // 40
    uint8_t     Poison_Immunity:1;      // 80
} Immunity_Flags;

typedef struct PACKED_STRUCT // Attribute_Flags
{
    uint8_t     Weapon_Immunity:1;      // 01
    uint8_t     Flyer:1;                // 02
    uint8_t     Lucky:1;                // 04
    uint8_t     Summon_Demons_1:1;      // 08
    uint8_t     Summon_Demons_2:1;      // 10
    uint8_t     Caster_20_MP:1;         // 20
    uint8_t     Caster_40_MP:1;         // 40
    uint8_t     Standard:1;             // 80

    uint8_t     Healing_Spell:1;        // 01
    uint8_t     Fire_Ball_Spell:1;      // 02
    uint8_t     Doombolt_Spell:1;       // 04
    uint8_t     Immolation:1;           // 08
    uint8_t     Web_Spell:1;            // 10
    uint8_t     Cause_Fear_Spell:1;     // 20
    uint8_t     Resistance_to_All:1;    // 40
    uint8_t     Holy_Bonus:1;           // 80
} Attribute_Flags;

typedef struct PACKED_STRUCT // Ability_Flags
{
    uint8_t     Summoned_Unit:1;        // 01
    uint8_t     Large_Shield:1;         // 02
    uint8_t     Plane_Shift:1;          // 04
    uint8_t     Wall_Crusher:1;         // 08
    uint8_t     Healer:1;               // 10
    uint8_t     Create_Outpost:1;       // 20
    uint8_t     Invisibility:1;         // 40
    uint8_t     Create_Undead:1;        // 80

    uint8_t     Long_Range:1;           // 01
    uint8_t     CoM_Quick_Casting:1;    // 02 The unused Land Corruption in MoM. Reused in CoM.
    uint8_t     Meld_With_Node:1;       // 04
    uint8_t     Non_Corporeal:1;        // 08
    uint8_t     Wind_Walking:1;         // 10
    uint8_t     Regeneration:1;         // 20
    uint8_t     Purify:1;               // 40
    uint8_t     Negate_First_Strike:1;  // 80
} Ability_Flags;

typedef struct PACKED_STRUCT // Attack_Flags
{
    uint8_t     Armor_Piercing:1;       // 01
    uint8_t     First_Strike:1;         // 02
    uint8_t     Poison_attack:1;        // 04
    uint8_t     Life_Stealing:1;        // 08
    uint8_t     Automatic_Damage:1;     // 10   // Chaos Spawn, Black Sleep
    uint8_t     Destruction:1;          // 20
    uint8_t     Illusionary_attack:1;   // 40
    uint8_t     Stoning_Touch:1;        // 80

    uint8_t     No_effect01:1;          // 01
    uint8_t     Death_Touch:1;          // 02
    uint8_t     Power_Drain:1;          // 04
    uint8_t     Dispel_Evil:1;          // 08
    uint8_t     Ball_COMBAT:1;          // 10
    uint8_t     CoM_Supernatural_COMBAT:1;   // 20 No effect in MoM. Reused in CoM.
    uint8_t     Eldritch_Weapon_COMBAT:1;   // 40
    uint8_t     Warp_Lightning_COMBAT:1;// 80
} Attack_Flags;

typedef union // unionMovement_Flags;
{
    Movement_Flags  s;
    uint8_t         bits;
} unionMovement_Flags;

typedef union // unionImmunity_Flags;
{
    Immunity_Flags  s;
    uint8_t         bits;
} unionImmunity_Flags;

typedef union // unionAttribute_Flags;
{
    Attribute_Flags s;
    uint16_t        bits;
} unionAttribute_Flags;

typedef union // unionAbility_Flags;
{
    Ability_Flags   s;
    uint16_t        bits;
} unionAbility_Flags;

typedef union // unionAttack_Flags;
{
    Attack_Flags    s;
    uint16_t        bits;
} unionAttack_Flags;

typedef struct PACKED_STRUCT // Difficulty_Table
{
    int16_t         m_City_Growth_multiplier;       // 00
    int16_t         m_Outpost_Growth_or_CoM_Overland_Casting_Cost_multiplier;    // 02
    int16_t         m_City_Production_multiplier;   // 04
    int16_t         m_City_Coin_multiplier;         // 06
    int16_t         m_City_Mana_multiplier;         // 08
    int16_t         m_City_Research_multiplier;     // 0A
    int16_t         m_City_Food_multiplier;         // 0C
    int16_t         m_Maintenance_multiplier;       // 0E
                                                    // SIZE 10
} Difficulty_Table;

typedef struct PACKED_STRUCT // Spell_Data
{
    char            m_SpellName[19];            // 00 ",    0X0,    0x50000400, 0x0,    19);
    int8_t          m_AI_Category;              // 13,   0x10000400, -1, 1);
    int8_t          m_AI_Desirability;          // 14,   0x000400,   -1, 1);
    eSpellCategory  m_Spell_Category;           // 15,   0x000400,   -1, 1);
    eSpell_Type     m_Section_in_spell_book;    // 16,   0x000400,   -1, 1);
    eRealm_Type     m_Magic_Realm;              // 17,   0x800400,   GetEnum("enum_RealmType"),  1);
    int8_t          m_Casting_eligibility;      // 18,   0x000400,   -1, 1);    
                                                //          -2,-1=combat-only,
                                                //          1=overland(non-combat) spells, 
                                                //          0,2,3,4=combat-or-overland,
                                                //          5=spell-of-return
    int8_t          m_Unk_19;                   // 19,   0x000400,   -1, 1);
    int16_t         m_Casting_cost;             // 1A,   0x10000400, -1, 2);
    int16_t         m_Research_cost;            // 1C,   0x10000400, -1, 2);
    int16_t         m_Sound_effect_when_casting_spell;  // 1E,   0x10000400, -1, 2);
    uint8_t         m_Parameters[4];            // 20-23
//    eUnit_Type      m_Unit_Summoned_or_Spell_Strength;            // 20,   0x000400,   -1, 1);
//    unionImmunity_Flags m_Immunity_Flags;       // 21,   0x800400,   GetEnum("enum_AtrributeFlags"), 1);
//    unionAttack_Flags   m_Attack_Flags;         // 22,   0x10800400, GetEnum("enum_AttackFlags"),    2);
                                                // SIZE 24
} Spell_Data;

typedef struct PACKED_STRUCT // Unit_Type_Data
{
    DS_Offset       m_PtrName;                  // 00-01  Pointer to name of unit type (note 1)
    uint8_t         m_Melee;                    // 02  Melee attack strength
    uint8_t         m_Ranged;                   // 03  Ranged attack strength
    eRanged_Type    m_Ranged_Type;              // 04  Ranged attack type (table 1)
    uint8_t         m_Ranged_Shots;             // 05  Ranged attack number of shots
    int8_t          m_To_Hit;                   // 06  Plus to hit
    uint8_t         m_Defense;                  // 07  Defense
    uint8_t         m_Resistance;               // 08  Resistance
    uint8_t         m_MoveHalves;               // 09  Movement rate (in units of 1/2 MP)
    uint16_t        m_Cost;                     // 0A-0B  Heroes: cost to hire (note 2)
                                                //     Normal units: cost to build
                                                //     Summoned units: cost to place in lair, node or rampaging force (note 3)
    uint8_t         m_Upkeep;                   // 0C  Summoned units: upkeep cost  Others: not used (note 4)
    eRace           m_Race_Code;                // 0D  Race code (table 2)
    uint8_t         m_Building1Required_or_PortraitLbxIndex;// 0E  Normal units: building required (table 3), Hero: ID Code, Summoned: 6
    eHero_TypeCode  m_Building2_or_HeroType;    // 0F  Heroes: type code, Regular units: Building required, Summoned: zero
    uint16_t        m_Unit_picture;             // 10  Unit picture
    uint8_t         m_Hitpoints;                // 12  Hit points (hearts) per figure
    uint8_t         m_Scouting;                 // 13  Scouting range
    uint8_t         m_Transport_Capacity;       // 14  Transport capacity (number of units carried)
    uint8_t         m_Nr_Figures;               // 15  Number of figures in the unit
    uint8_t         m_Construction;             // 16  Construction capacity (road building)
    int8_t          m_Gaze_Modifier;            // 17  Special attack or bonus strength
    unionMovement_Flags     m_Movement_Flags;   // 18  Movement flags (table 4)
    int8_t          m_CoM_Production_Priority_or_Fame_Req; // 19  00 in MoM. Reused in CoM.
    unionImmunity_Flags     m_Immunity_Flags;   // 1A  Immunity flags (table 5)
    unionAttribute_Flags    m_Attribute_Flags;  // 1B  Attribute flags (table 6)
                                                // 1C  Attribute flags (table 7)
    uint8_t                 m_CoM_Garrison_Priority;  // 1D  00 in MoM. Reused in CoM.
    unionAbility_Flags      m_Ability_Flags;    // 1E  Attribute flags (table 8)
                                                // 1F  Attribute flags (table 9)
    unionAttack_Flags       m_Attack_Flags;     // 20  Special attack flags (table 10)
                                                // 21  Special attack flags (table 11)
    uint8_t         m_Sound;                    // 22  Initialized only after starting the game
    uint8_t         m_Zero_23;                  // 23  00
                                                // SIZE 24
} Unit_Type_Data;

typedef struct PACKED_STRUCT // Battlefield
{
    eTerrainBattle  m_Terrain[462];             // 0        // Map is 22 rows by 11 columns, corresponding to 22 x 21 squares (lines of 21 bytes/words)
    uint8_t     m_TerrainGroupType[462];        // 039C     // 0=walkable, 1=rough, 2=walkable, 3=river, 4=sea
    uint8_t     m_Road[462];                    // 056A     // 0=no road, 81h=road, other=?
    int8_t      m_Movement_walking[462];        // 0738
    int8_t      m_Movement_merging_teleporting_fly[462];// 09D6
    int8_t      m_Movement_similar_to_walking[462];     // 0AD4
    int8_t      m_Movement_sailing[462];        // 0CA2
    uint16_t    m_Nr_tree_pics;                 // 0E70
    uint16_t    m_xpel_tree_pics[100];          // 0E72
    uint16_t    m_ypel_tree_pics[100];          // 0F3A
    uint16_t    m_tree_pics[100];               // 1002
    uint16_t    m_Nr_rock_pics;                 // 10CA
    uint16_t    m_xpel_rock_pics[100];          // 10CC
    uint16_t    m_ypel_rock_pics[100];          // 1194
    uint16_t    m_rock_pics[100];               // 125C
    uint8_t     m_Mud[462];                     // 1324     // 0=no mud, 1=mud, 2=?;  move->12
    eCentralStructure   m_Central_structure;    // 14F2
    uint16_t    m_Nr_houses;                    // 14F4
    uint16_t    m_xpos_houses[16];              // 14F6
    uint16_t    m_ypos_houses[16];              // 1516
    uint16_t    m_house_pics[16];               // 1536
    eYesNo16    m_City_Walls;                   // 1556     // 00=No City Walls, 01=City Walls
    uint16_t    m_Wall_present_4x4[16];         // 1558     // 00=No Wall, 01=Whole Wall, 02=Broken Wall
    eYesNo16    m_Wall_of_Fire;                 // 1578
    eYesNo16    m_Wall_of_Darkness;             // 157A
    int16_t     m_Plane;                        // 157C
    City_Enchantments m_City_Enchantments;      // 157E
                                                // SIZE 159C
} Battlefield;


typedef struct PACKED_STRUCT // Battle_Figure
{
    int16_t                 m_xpel;             // 0
    int16_t                 m_ypel;             // 02
    int16_t                 m_Pic;              // 04
    uint16_t                m_Unk_6_sound;      // 06
    uint16_t                m_Unk_8;            // 08
    uint16_t                m_Unk_A;            // 0A
    uint16_t                m_Unk_C;            // 0C
    ePlayer                 m_Owner;            // 0E
    uint16_t                m_Unk_10;           // 10
    uint16_t                m_Color_enchantment;// 12
    uint16_t                m_Unk_14_sound;     // 14
    uint16_t                m_Unk_16;           // 16
    uint16_t                m_Unk_18_battleunit_field_58;   // 18
    uint32_t                m_ZDepth;           // 1A
                                                // SIZE 1E
} Battle_Figure;

typedef struct PACKED_STRUCT // Battle_Unit
{
                                    // (sizeof=0x6E)
    // First part copied from Unit_Type_Data (starting at offset 2)

    uint8_t                 m_Melee;                        // 00  Melee attack strength
    uint8_t                 m_Ranged;                       // 01  Ranged attack strength
    eRanged_Type            m_Ranged_Type;                  // 02  Ranged attack type (table 1)
    uint8_t                 m_Ranged_Shots;                 // 03  Ranged attack number of shots
    uint8_t                 m_To_Hit;                       // 04  Plus to hit
    uint8_t                 m_Defense;                      // 05  Defense
    uint8_t                 m_Resistance;                   // 06  Resistance
    uint8_t                 m_MoveHalves;                   // 07  Movement rate (in units of 1/2 MP)
    uint16_t                m_Cost;                         // 08-09  Heroes: cost to hire (note 2)
                                                            //     Normal units: cost to build
                                                            //     Summoned units: cost to place in lair, node or rampaging force (note 3)
    uint8_t                 m_Upkeep;                       // 0A  Summoned units: upkeep cost  Others: not used (note 4)
    eRace                   m_Race_Code;                    // 0B  Race code (table 2)
    uint8_t                 m_Building1Required_or_PortraitLbxIndex;// 0C  Normal units: buildings required (table 3), heroes portraitIndex
                                                            //     Others: note 5
    uint8_t                 m_Current_figures;              // 0D
    int8_t                  m_BattleUnitNr;                 // 0E
    uint8_t                 m_Unk_0F;                       // 0F  00
    uint8_t                 m_Hitpoints_per_Figure;         // 10  Hit points (hearts) per figure
    uint8_t                 m_Scouting;                     // 11  Scouting range
    uint8_t                 m_Transport_Capacity_GUESS;     // 12  Transport capacity (number of units carried)
    uint8_t                 m_Max_figures;                // 13  Number of figures in the unit
    uint8_t                 m_Construction;                 // 14  Construction capacity (road building)
    int8_t                  m_Gaze_Modifier;                // 15  Special attack or bonus strength
    unionMovement_Flags     m_Movement_Flags;               // 16  Movement flags (table 4)
    uint8_t                 m_Zero_17;                      // 17  00
    unionImmunity_Flags     m_Immunity_Flags;               // 18  Immunity flags (table 5)
    unionAttribute_Flags    m_Attribute_Flags;              // 19  Attribute flags (table 6)
                                                            // 1A  Attribute flags (table 7)
    uint8_t                 m_Zero_1B;                      // 1B  00
    unionAbility_Flags      m_Ability_Flags;                // 1C  Attribute flags (table 8)
                                                            // 1D  Attribute flags (table 9)
    unionAttack_Flags       m_Attack_Flags;                 // 1E  Special attack flags (table 10)
                                                            // 1F  Special attack flags (table 11)

    uint16_t                m_Zero_20;                      // 20-21 ; unused
    unionCombat_Enchantment m_Flags_Combat_Enchantment;     // 22-23
    uint8_t                 m_Extra_ToHit_Melee;            // 24
    uint8_t                 m_Extra_ToHit_Ranged;           // 25
    uint8_t                 m_Extra_ToDefend;               // 26
    uint8_t                 m_Weapon_Type_Plus_1;           // 27
    unionAttack_Flags       m_Cur_Attack_Flags;             // 28-29
    unionAttack_Flags       m_Item_Attack_Flags;            // 2A-2B
    unionUnit_Enchantment   m_Item_UnitEnchantment;         // 2C-2F
    int16_t                 m_unitNr;                       // 30-31
    int8_t                  m_additional_life_per_figure;   // 32
    int8_t                  m_web_strength;                         // 33
    eBattleUnitActive       m_Active;                       // 34   Active (0=alive, 2=flee?, 4=dead, 5=undeaded, 6=removed)
    ePlayer                 m_Owner;                        // 35
    int8_t                  m_tracks_regular_damage;        // 36
    int8_t                  m_tracks_lifesteal_damage;      // 37
    int8_t                  m_tracks_instant_damage_like_stoning;// 38   stoning, destruction, dispel
    int8_t                  m_top_figure_damage;            // 39
    unionUnit_Enchantment   m_Flags1_UnitEnchantment;       // 3A-3D
    int8_t                  m_Suppression;                  // 3E
    int8_t                  m_Mana_points;                  // 3F
    int8_t                  m_Current_mana;                 // 40
    int8_t                  m_Item_nr_charges;              // 41
    int8_t                  m_Poison_strength;              // 42
    int8_t                  m_Target_BattleUnitID;          // 43
    int16_t                 m_xPos;                         // 44-45
    int16_t                 m_yPos;                         // 46-47
    int16_t                 m_xPosHeaded;                   // 48-49
    int16_t                 m_yPosHeaded;                   // 4A-4B
    uint16_t                m_Unk_4C_display;               // 4C
    uint16_t                m_Unk_4E;                       // 4E
    uint16_t                m_Unk_50;                       // 50
    uint16_t                m_Unk_52_sound;                 // 52
    eBattleUnitTactic       m_Tactic;                       // 54 Tactic (0=ready, 4=done, 100=melee, 101=shoot, ...)
    int8_t                  m_Confused_State;               // 56 (0=stunned?, 1=move randomly, 2=change allegiance, 3=?)
    uint8_t                 m_Unk_57;                       // 57
    uint16_t                m_Unk_58[6];                    // 58
                                                            // 59
                                                            // 5A
                                                            // 5B
                                                            // 5C
                                                            // 5D
                                                            // 5E
                                                            // 5F
                                                            // 60
                                                            // 61
                                                            // 62
                                                            // 63
    uint8_t                 m_Extra_Melee;                  // 64
    uint8_t                 m_Extra_Ranged;                 // 65
    uint8_t                 m_Extra_Defense;                // 66
    uint8_t                 m_Extra_Resistance;             // 67
    uint8_t                 m_Extra_Hitpoints;              // 68
    uint8_t                 m_Lost_Melee;                   // 69
    uint8_t                 m_Lost_Ranged;                  // 6A
    uint8_t                 m_Lost_Defense;                 // 6B
    uint8_t                 m_Lost_Resistance;              // 6C
    uint8_t                 m_Unused6D;                     // 6D db ?
                                                            // SIZE 6E
} Battle_Unit;


typedef struct PACKED_STRUCT // UnitView_Line
{
    char m_Line[30];
} UnitView_Line;

typedef struct PACKED_STRUCT // UnitView_Lines
{
    UnitView_Line   m_lines[40];                        // 000
    uint32_t        m_bitmask_unitenchantment[40];      // 4B0
    uint16_t        m_icon_pic[40];                     // 550
    uint16_t        m_helpIndex[40];                    // 5A0
                                                        // SIZE 5F0
} UnitView_Lines;

typedef struct PACKED_STRUCT // UnitView_HeroAbility
{
    uint32_t        m_bitmask;                  // 0
    int16_t         m_lbxIndex;                 // 4
    int16_t         m_helpIndex;                // 6
                                                // SIZE 8
} UnitView_HeroAbility;

typedef struct PACKED_STRUCT // UnitView_UnitData
{
    DS_Offset       m_offset_label;             // 0
    uint16_t        m_bitmask;                  // 2
    int16_t         m_lbxIndex;                 // 4
    int16_t         m_helpIndex;                // 6
                                                // SIZE 8
} UnitView_UnitData;

typedef struct PACKED_STRUCT // UnitView_Mutation
{
    DS_Offset       m_offset_label;             // 0
    uint8_t         m_bitmask;                  // 2
    int8_t          m_lbxIndex;                 // 3
    int16_t         m_helpIndex;                // 4
                                                // SIZE 6
} UnitView_Mutation;

typedef struct PACKED_STRUCT // UnitView_Ranged
{
    DS_Offset       m_offset_label;             // 0
    eRanged_Type    m_rangedType;               // 2
    int8_t          m_lbxIndex;                 // 3
    int16_t         m_helpIndex;                // 4
                                                // SIZE 6
} UnitView_Ranged;

typedef struct PACKED_STRUCT // UnitView_SpellData
{
    DS_Offset       m_offset_label;             // 0
    uint32_t        m_bitmask;                  // 2
    int16_t         m_lbxIndex;                 // 6
    int16_t         m_helpIndex;                // 8
                                                // SIZE A
} UnitView_SpellData;

typedef struct PACKED_STRUCT // UnitView_ItemText
{
    DS_Offset       m_offset_label;             // 0
    DS_Offset       m_offset_helpText;          // 2
    uint32_t        m_bitmask;                  // 4
                                                // SIZE 8
} UnitView_ItemText;

typedef struct PACKED_STRUCT // Unit_Data_Hero_Types
{
    // TODO: Match with... what?
    //Unit_Type_Data m_Hero_Types[35];
    Unit_Type_Data      Dwarf;
    Unit_Type_Data      Barbarian;
    Unit_Type_Data      Sage;
    Unit_Type_Data      Dervish;
    Unit_Type_Data      Beastmaster;
    Unit_Type_Data      Bard;
    Unit_Type_Data      Orc_Warrior;
    Unit_Type_Data      Healer;
    Unit_Type_Data      Huntress;
    Unit_Type_Data      Thief;
    Unit_Type_Data      Druid;
    Unit_Type_Data      War_Monk;
    Unit_Type_Data      Warrior_Mage;
    Unit_Type_Data      Magician;
    Unit_Type_Data      Assassin;
    Unit_Type_Data      Wind_Mage;
    Unit_Type_Data      Ranger;
    Unit_Type_Data      Draconian;
    Unit_Type_Data      Witch;
    Unit_Type_Data      Golden_One;
    Unit_Type_Data      Ninja;
    Unit_Type_Data      Rogue;
    Unit_Type_Data      Amazon;
    Unit_Type_Data      Warlock;
    Unit_Type_Data      Unknown;
    Unit_Type_Data      Illusionist;
    Unit_Type_Data      Swordsman;
    Unit_Type_Data      Priestess;
    Unit_Type_Data      Paladin;
    Unit_Type_Data      Black_Knight;
    Unit_Type_Data      Elven_Archer;
    Unit_Type_Data      Knight;
    Unit_Type_Data      Necromancer;
    Unit_Type_Data      Chaos_Warrior;
    Unit_Type_Data      Chosen;
} Unit_Data_Hero_Types;

typedef struct PACKED_STRUCT // Race_Data
{
    DS_Offset   m_PtrName;                          // 00-01  Pointer to name of race (note 1)
    uint16_t    m_Number_of_prohibited_buildings;   // 02-03  (0 to 7)
    eBuilding   m_Prohibited_buildings[7];          // 04-11  (table 3)
    int16_t     m_Outpost_growth_probability;       // 12-13  (percent chance to grow in each turn)
    int16_t     m_City_population_growth_modifier;  // 14-15  (in units of 10 people per turn)
    eHousing    m_Housing_picture;                  // 16-17  (0 wood frame house, 1 tree house, 2 mud hut)
                                                    // SIZE 18
} Race_Data;

typedef struct PACKED_STRUCT // WizardsExe_Game_Data
{                                           // ds:BD8E
    // Game status
    uint16_t        m_UnitNr_Active;        // 00
    uint16_t        m_Current_Turn;         // 02 year is 1400 + Turn/12, month is Turn%12
    uint16_t        m_Number_of_Units;      // 04 in Unit Instance Table (0 ... 1000, out of a maximum of 1000)
    uint16_t        m_Number_of_Cities;     // 06 in City Table (0 ... 100, of out of a maximum of 100)

    // Game choices
    eDifficulty     m_Difficulty;           // 08
    eMagic_Powerful m_Magic_Powerful_setting;   // 0A
    eLand_Size      m_Land_Size_setting;    // 0C
    uint16_t        m_Number_of_Wizards;    // 0E in Wizard Table (including YOU, 2 ... 5)
                                            // SIZE 10 ds:BD9E
} WizardsExe_Game_Data;

typedef struct PACKED_STRUCT // WizardsExe_Save_Name
{
    char        m_Name[20];
} WizardsExe_Save_Name;

typedef struct PACKED_STRUCT // Game_Settings
{
                                                // ds:BD9E  Maps 1-1 on the MAGIC.SET file (466 bytes)
    eYesNo16    m_Sound_Effects;                        // 000
    eYesNo16    m_Background_Music;
    eYesNo16    m_Event_Music;
    eYesNo16    m_City_Spell_Events;
    eYesNo16    m_Overland_Spell_Events;
    eYesNo16    m_Summoning_Events;
    eYesNo16    m_End_of_Turn_Summary;
    eYesNo16    m_Raze_City;
    eYesNo16    m_Random_Events;                        // 010
    eYesNo16    m_End_of_Turn_Wait;
    eYesNo16    m_Stratic_Combat_Only;
    eYesNo16    m_Auto_Unit_Information;
    eYesNo16    m_Enemy_Moves;
    eYesNo16    m_Enemy_Spells;
    eYesNo16    m_Spell_Book_Ordering;
    eYesNo16    m_Spell_Animations;
    eYesNo16    m_Show_Node_Owners;                     // 020
    eYesNo16    m_Expanding_Help;
    uint16_t    m_Unk_024;                              // 024
    uint16_t    m_Unk_026;                              // 026
    eDifficulty m_Difficulty;                           // 028
    uint16_t    m_Nr_Opponents;                         // 02A
    eLand_Size  m_Land_Size;                            // 02C
    eMagic_Powerful m_Magic_Powerful;                   // 02E
    eYesNo16    m_Slots_Saved[8];                       // 030
    WizardsExe_Save_Name   m_Save_Names[8];             // 040   // 8 slots with each 18 letters and 2 ending zeroes
    eYesNo16    m_Movement_Animations;                  // 0E0
    WizardsExe_Save_Name   m_Save_Wizard_Names[10];     // 0E2   // 10 (?) slots of each 20 bytes (incl. ending zero)
    uint16_t    m_Unk_1AA[20];                          // 1AA
                                                        // SIZE 1D2h = 466d
} Game_Settings;

typedef struct PACKED_STRUCT {   // Available_spell_page
    char        m_Page_Name[0x10];          // 00
    eSpell16    m_Spells_on_page[6];        // 10
    uint8_t     m_Nr_spells_on_page;        // 1C
    uint8_t     m_Zero_1D;                  // 1D
                                            // SIZE 1E
} Available_spell_page;

typedef struct PACKED_STRUCT {   // Lbx_ems_info
    char        m_name[9];                  // 00
    uint8_t     m_Unk_09;                   // 09
    uint16_t    m_Unk_0A;                   // 0A
                                            // SIZE 0C
} EMS_descriptor;

typedef struct // MoMDataSegment
{
    char        m_DataSegmentStart[0x0130];                     // ds:0 / EXE:294A0
    // "\0\0\0\0Borland C++ - Copyright 1991 Borland Intl.";

    DS_Offset   m_Offsets_SkillNames[eSkill_MAX];               // ds:0130
    uint16_t    m_Unk_0154;                                     // ds:0154
    DS_Offset   m_Offsets_CitySizeNames[eCity_Size_MAX];        // ds:0156
    DS_Offset   m_Offsets_CitySpecialNames[7];                  // ds:0162
    uint16_t    m_Unk_0170[22];                                 // ds:0170

    Unit_Data_Hero_Types    m_Hero_Types;                       // ds:019C / EXE:2963C
    Unit_Type_Data          m_Normal_Units[4 + 115];
    Unit_Type_Data          m_Summoned_Units[44];

    uint16_t    m_XP_Level_Table[gMAX_LEVELS_HERO];             // ds:1D74 / EXE:2B214

    Race_Data   m_Race_Data[14];                                // ds:1D86 / EXE:2B226

    uint16_t    m_Unk_1ED6;                                     // ds:1ED6 / EXE:2B376
    DS_Offset   m_Offset_Arcane;                                // ds:1ED8  // UNREFERENCED???
    DS_Offset   m_Offsets_RealmNames_5[5];                      // ds:1EDA
    DS_Offset   m_Offsets_CityEnchantmentNames[eCityEnchantments_MAX];   // ds:1EE4

    Upkeep_Enchantments m_Upkeep_Enchantments;                  // ds:1F18 / EXE:2B3B8

    DS_Offset   m_Offsets_PersonalityNames[ePersonality_MAX];   // ds:1FBC
    int16_t     m_Values_Personalities_GUESS[ePersonality_MAX]; // ds:1FC8
    DS_Offset   m_Offsets_ObjectiveNames[eObjective_MAX];       // ds:1FD4
    uint16_t    m_Unk_1FDE;                                     // ds:1FDE
    int16_t     m_Values_Objectives_GUESS[eObjective_MAX];      // ds:1FE0

    // 5 x 8 entries for each difficulty level
    Difficulty_Table m_DifficultyTable[eDifficulty_MAX];        // ds:1FEA / EXE:2B48A

    uint8_t     m_Unk_203A[72];                                 // ds:203A / EXE:2B4DA

    char        m_NameBuffer_2082[0x3A50 - 0x2082];             // ds:2082 / EXE:2B522

    UnitView_HeroAbility    m_UnitView_HeroAbility_data[23];    // ds:3A50
    UnitView_UnitData       m_UnitView_UnitAbility_data[17];    // ds:3B08
    UnitView_UnitData       m_UnitView_UnitImmunity_data[8];    // ds:3B90
    UnitView_UnitData       m_UnitView_UnitSpell_data[8];       // ds:3BD0
    UnitView_UnitData       m_UnitView_UnitAttack_data[11];     // ds:3C10
    UnitView_Mutation       m_UnitMutation_data[3];             // ds:3C68
    UnitView_Ranged         m_UnitRanged_data[6];               // ds:3C7A
    UnitView_SpellData      m_UnitEnchantment_data[32];         // ds:3C9E
    UnitView_SpellData      m_CombatEnchantment_data[15];       // ds:3DDE
    UnitView_SpellData      m_ItemPower_data[21];               // ds:3E74
    DS_Offset               m_Offsets_UnitLevelNames[gMAX_LEVELS_UNIT]; // ds:3F46
    DS_Offset               m_Offsets_HeroLevelNames[gMAX_LEVELS_HERO]; // ds:3F52
    UnitView_ItemText       m_ItemPower_text[32];               // ds:3F64

    char        m_NameBuffer_4064[0x5bd8 - 0x4064];             // ds:4064

    int16_t     m_BaseItemPowerCosts[eItemPowerType_MAX];       // dseg:5BD8

    char        m_NameBuffer_5BEC[0x5E92 - 0x5BEC];             // dseg:5BEC

    // Note: this can not be uint32_t because g++ will align it on a 32-bit boundary
    uint16_t    m_Next_Turn_seed_storage_lo;                    // ds:5E92
    uint16_t    m_Next_Turn_seed_storage_hi;                    // ds:5E94

    uint8_t     m_Unk_5E96[0x6E9E - 0x5E96];                    // ds:5E96

    uint16_t    m_Tax_Unrest_Table[eTax_Rate_MAX];              // ds:6E9E

    uint8_t     m_Unk_6EAC[0x7151 - 0x6EAC];                    // ds:6EAC

    char        m_Copyright_and_Version[41];                    // ds:7151  Offset version is at [34]

    uint8_t     m_Unk_717A[0x71E0 - 0x717A];                    // ds:717A

    uint32_t    m_BIOS_clock_snapshot;                          // ds:71E0

    uint8_t     m_Unk_71E4[0x760C - 0x71E4];                    // ds:71E4

    uint16_t    m_EMS_data_reference_point;                     // ds:760C

    uint8_t     m_Unk_760E[0x7846 - 0x760E];                    // ds:760E

    // Note: this can not be uint32_t because g++ will align it on a 32-bit boundary
    uint16_t    m_RNG_seed_lo;                                  // ds:7846
    uint16_t    m_RNG_seed_hi;                                  // ds:7848

    uint8_t     m_Unk_784A[0x7876 - 0x784A];                    // ds:784A

    uint16_t    m_DEBUG_Off;                                    // ds:7876

    uint8_t     m_Unk_7878[0x912C - 0x7878];                    // ds:7878

    EXE_Reloc   m_addr_Spell_Data;                              // 912C
    uint16_t    word_3FBD0  ; // 9130
    uint16_t    w_sound_x   ; // 9132
    uint16_t    word_3FBD4  ; // 9134
    EXE_Reloc   m_addr_Items;                                   // 9136
    EXE_Reloc   m_addr_Artifacts_in_Game;                       // 913A
    uint16_t    m_item_pics_116[116];                           // 913E
    EXE_Reloc   m_addr_Battle_Unit_View;                        // 9226
    EXE_Reloc   m_addr_Battle_Unit;                             // 922A
    EXE_Reloc   m_addr_Spells_Cast_in_Battle;                   // 922E
    EXE_Reloc   m_addr_Hero_stat[6];                            // 9232
    uint8_t w_AI_flees[20]  ; // 924A
    uint8_t word_3FCFE[10]  ; // 925E
    uint16_t    word_3FD08  ; // 9268
    uint16_t    word_3FD0A  ; // 926A
    uint16_t    word_3FD0C  ; // 926C
    uint16_t    word_3FD0E  ; // 926E
    uint16_t    word_3FD10  ; // 9270
    uint16_t    word_3FD12  ; // 9272
    EXE_Reloc   m_addr_Battlefield;                             // 9274
    uint16_t    word_3FD18  ; // 9278
    uint16_t    word_3FD1A  ; // 927A
    uint16_t    w_AI_on_the_move_GUESS  ; // 927C
    uint16_t    w_coo_X_Y_clicked   ; // 927E
    uint16_t    w_coo_Y_X_clicked   ; // 9280
    uint16_t    word_3FD22  ; // 9282
    int16_t     m_clash_place_type;                             // 9284         // -1=undef,0=overland,1=city,5=lair
    int16_t     m_clash_cityNr_or_lairNr;                       // 9286
    uint8_t word_3FD28[12]    ; // 9288
    int16_t     m_kyrub_dseg_9294  ;                            // 9294
    int16_t     m_kyrub_dseg_9296  ;                            // 9296
    uint8_t word_3FD38[20]  ; // 9298
    uint16_t    w_negat_encha_x17_flag_def  ; // 92AC
    uint16_t    w_aggre_spell_color_flag_att    ; // 92AE
    EXE_Reloc   dword_3FD50 ; // 92B0
    uint16_t    word_3FD54  ; // 92B4
    uint8_t word_3FD56[64]  ; // 92B6
    uint8_t word_3FD96[8]   ; // 92F6
    uint16_t    word_3FD9E  ; // 92FE
    uint16_t    word_3FDA0  ; // 9300
    uint16_t    word_3FDA2  ; // 9302
    uint16_t    word_3FDA4  ; // 9304
    uint16_t    word_3FDA6  ; // 9306
    uint16_t    word_3FDA8  ; // 9308
    uint16_t    word_3FDAA  ; // 930A
    uint16_t    word_3FDAC  ; // 930C
    uint16_t    word_3FDAE  ; // 930E
    uint16_t    word_3FDB0  ; // 9310
    uint16_t    word_3FDB2  ; // 9312
    uint16_t    word_3FDB4  ; // 9314
    uint16_t    word_3FDB6  ; // 9316
    uint8_t     word_3FDB8[44]  ; // 9318
    uint16_t    word_3FDE4  ; // 9344
    EXE_Reloc   addr_3FDE6  ; // 9346
    uint16_t    word_3FDEA  ; // 934A
    uint16_t    word_3FDEC  ; // 934C
    uint16_t    word_3FDEE  ; // 934E
    uint16_t    word_3FDF0  ; // 9350
    uint16_t    word_3FDF2  ; // 9352
    uint16_t    word_3FDF4  ; // 9354
    uint16_t    word_3FDF6  ; // 9356
    uint16_t    word_3FDF8  ; // 9358
    uint16_t    word_3FDFA  ; // 935A
    uint16_t    word_3FDFC  ; // 935C
    uint16_t    word_3FDFE  ; // 935E
    uint16_t    word_3FE00  ; // 9360
    uint16_t    word_3FE02  ; // 9362
    uint16_t    word_3FE04  ; // 9364
    uint16_t    word_3FE06  ; // 9366
    uint16_t    word_3FE08  ; // 9368
    uint16_t    word_3FE0A  ; // 936A
    uint16_t    word_3FE0C  ; // 936C
    uint16_t    word_3FE0E  ; // 936E
    uint16_t    word_3FE10  ; // 9370
    uint16_t    word_3FE12  ; // 9372
    uint16_t    word_3FE14  ; // 9374
    uint16_t    word_3FE16  ; // 9376
    uint16_t    word_3FE18  ; // 9378
    uint16_t    word_3FE1A  ; // 937A
    uint16_t    word_3FE1C  ; // 937C
    uint16_t    word_3FE1E  ; // 937E
    uint16_t    word_3FE20  ; // 9380
    uint16_t    word_3FE22  ; // 9382
    uint16_t    word_3FE24  ; // 9384
    uint16_t    word_3FE26  ; // 9386
    uint16_t    word_3FE28  ; // 9388
    uint16_t    word_3FE2A  ; // 938A
    EXE_Reloc   m_addr_Building_Data;               // 938C
    uint16_t    word_3FE30  ; // 9390
    EXE_Reloc   dword_3FE32 ; // 9392
    uint8_t dword_3FE36[126]    ; // 9396
    uint8_t word_3FEB4[6]   ; // 9414
    uint8_t word_3FEBA[18]  ; // 941A
    uint8_t word_3FECC[22]  ; // 942C
    uint16_t    word_3FEE2  ; // 9442
    uint16_t    w_all_units_moved_GUESS ; // 9444
    uint16_t    word_3FEE6  ; // 9446
    uint16_t    word_3FEE8  ; // 9448
    uint16_t    word_3FEEA  ; // 944A
    uint8_t word_3FEEC[6]   ; // 944C
    uint8_t word_3FEF2[14]  ; // 9452
    EXE_Reloc   word_3FF00  ; // 9460
    uint16_t    word_3FF04  ; // 9464
    uint16_t    word_3FF06  ; // 9466
    uint16_t    word_3FF08  ; // 9468
    uint16_t    word_3FF0A  ; // 946A
    uint16_t    word_3FF0C  ; // 946C
    uint16_t    word_3FF0E  ; // 946E
    uint8_t word_3FF10[14]  ; // 9470
    uint16_t    word_3FF1E  ; // 947E
    uint16_t    word_3FF20  ; // 9480
    uint16_t    word_3FF22  ; // 9482
    uint16_t    word_3FF24  ; // 9484
    uint16_t    word_3FF26  ; // 9486
    uint16_t    word_3FF28  ; // 9488
    uint16_t    word_3FF2A  ; // 948A
    uint16_t    word_3FF2C  ; // 948C
    uint8_t word_3FF2E[6]   ; // 948E
    uint16_t    word_3FF34  ; // 9494
    uint16_t    word_3FF36  ; // 9496
    uint16_t    word_3FF38  ; // 9498
    uint16_t    word_3FF3A  ; // 949A
    uint16_t    word_3FF3C  ; // 949C
    uint16_t    word_3FF3E  ; // 949E
    uint16_t    word_3FF40  ; // 94A0
    uint16_t    word_3FF42  ; // 94A2
    uint16_t    word_3FF44  ; // 94A4
    uint16_t    word_3FF46  ; // 94A6
    uint16_t    word_3FF48  ; // 94A8
    uint16_t    word_3FF4A  ; // 94AA
    uint8_t word_3FF4C[58]  ; // 94AC
    uint16_t    word_3FF86  ; // 94E6
    uint16_t    word_3FF88  ; // 94E8
    uint16_t    word_3FF8A  ; // 94EA
    uint16_t    word_3FF8C  ; // 94EC
    uint16_t    word_3FF8E  ; // 94EE
    uint8_t word_3FF90[30]  ; // 94F0
    uint16_t    word_3FFAE  ; // 950E
    uint16_t    word_3FFB0  ; // 9510
    uint16_t    word_3FFB2  ; // 9512
    uint16_t    word_3FFB4  ; // 9514
    uint16_t    word_3FFB6  ; // 9516
    uint16_t    word_3FFB8  ; // 9518
    uint16_t    word_3FFBA  ; // 951A
    uint16_t    word_3FFBC  ; // 951C
    EXE_Reloc   word_3FFBE  ; // 951E
    uint16_t    word_3FFC2  ; // 9522
    uint16_t    word_3FFC4  ; // 9524
    uint16_t    word_3FFC6  ; // 9526
    EXE_Reloc   word_3FFC8  ; // 9528
    uint16_t    word_3FFCC  ; // 952C
    uint8_t word_3FFCE[14]  ; // 952E
    uint16_t    word_3FFDC  ; // 953C
    uint16_t    word_3FFDE  ; // 953E
    uint8_t word_3FFE0[8]   ; // 9540
    uint8_t word_3FFE8[38]  ; // 9548
    uint16_t    word_4000E  ; // 956E
    uint16_t    word_40010  ; // 9570
    uint16_t    word_40012  ; // 9572
    uint16_t    word_40014  ; // 9574
    uint16_t    word_40016  ; // 9576
    uint16_t    word_40018  ; // 9578
    uint16_t    word_4001A  ; // 957A
    uint16_t    word_4001C  ; // 957C
    uint16_t    word_4001E  ; // 957E
    uint16_t    word_40020  ; // 9580
    uint16_t    word_40022  ; // 9582
    uint16_t    word_40024  ; // 9584
    uint16_t    word_40026  ; // 9586
    uint16_t    word_40028  ; // 9588
    uint16_t    word_4002A  ; // 958A
    uint16_t    word_4002C  ; // 958C
    uint8_t word_4002E[28]  ; // 958E
    uint16_t    word_4004A  ; // 95AA
    uint16_t    word_4004C  ; // 95AC
    uint16_t    word_4004E  ; // 95AE
    uint16_t    word_40050  ; // 95B0
    uint16_t    word_40052  ; // 95B2
    uint16_t    word_40054  ; // 95B4
    uint16_t    word_40056  ; // 95B6
    uint16_t    word_40058  ; // 95B8
    uint16_t    word_4005A  ; // 95BA
    uint16_t    word_4005C  ; // 95BC
    uint16_t    word_4005E  ; // 95BE
    uint16_t    word_40060  ; // 95C0
    uint8_t word_40062[12]  ; // 95C2
    uint8_t word_4006E[34]  ; // 95CE
    uint16_t    word_40090  ; // 95F0
    uint8_t word_40092[82]  ; // 95F2
    int16_t     m_cityNr_detailed;              // 9644
    uint8_t word_400E8[246] ; // 9646
    uint16_t    word_401DC  ; // 973C
    uint16_t    word_401DE  ; // 973E
    uint16_t    word_401E0  ; // 9740
    uint16_t    word_401E2  ; // 9742
    uint16_t    word_401E4  ; // 9744
    uint16_t    word_401E6  ; // 9746
    uint16_t    word_401E8  ; // 9748
    uint16_t    word_401EA  ; // 974A
    uint16_t    word_401EC  ; // 974C
    uint16_t    word_401EE  ; // 974E
    uint16_t    word_401F0  ; // 9750
    uint16_t    word_401F2  ; // 9752
    uint16_t    word_401F4  ; // 9754
    uint16_t    word_401F6  ; // 9756
    uint16_t    word_401F8  ; // 9758
    uint16_t    word_401FA  ; // 975A
    uint16_t    word_401FC  ; // 975C
    uint16_t    word_401FE  ; // 975E
    uint16_t    word_40200  ; // 9760
    uint16_t    word_40202  ; // 9762
    uint16_t    word_40204  ; // 9764
    uint16_t    word_40206  ; // 9766
    uint16_t    word_40208  ; // 9768
    uint16_t    word_4020A  ; // 976A
    uint8_t word_4020C[38]  ; // 976C
    uint16_t    word_40232  ; // 9792
    uint16_t    word_40234  ; // 9794
    uint16_t    word_40236  ; // 9796
    uint16_t    word_40238  ; // 9798
    uint16_t    word_4023A  ; // 979A
    uint16_t    word_4023C  ; // 979C
    uint16_t    word_4023E  ; // 979E
    uint16_t    word_40240  ; // 97A0
    uint8_t word_40242[18]  ; // 97A2
    uint16_t    word_40254  ; // 97B4
    uint16_t    word_40256  ; // 97B6
    uint16_t    word_40258  ; // 97B8
    uint8_t word_4025A[50]  ; // 97BA
    uint16_t    word_4028C  ; // 97EC
    uint8_t word_4028E[10]  ; // 97EE
    uint16_t    word_40298  ; // 97F8
    uint16_t    word_4029A  ; // 97FA
    uint16_t    word_4029C  ; // 97FC
    uint8_t word_4029E[34]  ; // 97FE
    uint8_t word_402C0[32]  ; // 9820
    uint16_t    word_402E0  ; // 9840
    uint8_t word_402E2[36]  ; // 9842
    uint8_t word_40306[120] ; // 9866
    uint8_t word_4037E[10]  ; // 98DE
    uint8_t word_40388[10]  ; // 98E8
    uint16_t    word_40392  ; // 98F2
    uint8_t word_40394[20]  ; // 98F4
    uint8_t word_403A8[6]   ; // 9908
    uint8_t word_403AE[10]  ; // 990E
    uint16_t    word_403B8  ; // 9918
    uint16_t    word_403BA  ; // 991A
    uint16_t    word_403BC  ; // 991C
    uint16_t    word_403BE  ; // 991E
    uint16_t    word_403C0  ; // 9920
    uint8_t word_403C2[20]  ; // 9922
    uint16_t    word_403D6  ; // 9936
    uint16_t    word_403D8  ; // 9938
    uint16_t    word_403DA  ; // 993A
    uint16_t    word_403DC  ; // 993C
    uint16_t    word_403DE  ; // 993E
    uint16_t    word_403E0  ; // 9940
    uint16_t    word_403E2  ; // 9942
    uint16_t    word_403E4  ; // 9944
    uint16_t    word_403E6  ; // 9946
    uint16_t    word_403E8  ; // 9948
    uint16_t    word_403EA  ; // 994A
    uint16_t    word_403EC  ; // 994C
    uint16_t    word_403EE  ; // 994E
    uint16_t    word_403F0  ; // 9950
    uint16_t    word_403F2  ; // 9952
    uint16_t    word_403F4  ; // 9954
    uint16_t    word_403F6  ; // 9956
    uint16_t    word_403F8  ; // 9958
    uint16_t    word_403FA  ; // 995A
    uint16_t    word_403FC  ; // 995C
    EXE_Reloc   word_403FE  ; // 995E
    uint16_t    word_40402  ; // 9962
    uint16_t    word_40404  ; // 9964
    uint16_t    word_40406  ; // 9966
    uint16_t    word_40408  ; // 9968
    uint16_t    word_4040A  ; // 996A
    uint16_t    word_4040C  ; // 996C
    uint16_t    word_4040E  ; // 996E
    uint16_t    word_40410  ; // 9970
    uint16_t    word_40412  ; // 9972
    uint16_t    word_40414  ; // 9974
    uint16_t    word_40416  ; // 9976
    uint16_t    word_40418  ; // 9978
    uint16_t    word_4041A  ; // 997A
    uint16_t    word_4041C  ; // 997C
    uint16_t    word_4041E  ; // 997E
    uint16_t    word_40420  ; // 9980
    uint16_t    word_40422  ; // 9982
    uint16_t    word_40424  ; // 9984
    uint16_t    word_40426  ; // 9986
    EXE_Reloc   word_40428  ; // 9988
    uint16_t    word_4042C  ; // 998C
    uint16_t    w_constant_GUESS    ; // 998E
    eGameState  m_Game_flow ;                                   // 9990
    uint16_t    word_40432  ; // 9992
    uint16_t    word_40434  ; // 9994
    uint16_t    word_40436  ; // 9996
    EXE_Reloc   m_addr_events ;                                 // 9998
    int16_t     m_nr_units_in_overland_stack;                   // 999C
    uint32_t    m_units_in_overland_stack[gMAX_UNITS_IN_LOCATION];  // 999E
    int8_t      m_nr_message_cityNames;                         // 99C2
    uint8_t     m_arr20_message_cityNames[20 * 14];             // 99C3
    int8_t      m_nr_message_cityNrs;                           // 9ADB
    int8_t      m_arr20_message_cityNrs[20];                    // 99DC
    int8_t      m_nr_city_queue;                                // 9AF0
    uint8_t     m_Unused_9AF1;                                  // 9AF1
    CityQueueElement    m_arr20_city_queue[20];                 // 9AF2
    uint8_t byte_405E2[82]  ; // 9B42
    uint8_t byte_40634[82]  ; // 9B94
    uint8_t byte_40686[82]  ; // 9BE6
    uint8_t byte_406D8[21]  ; // 9C38
    uint8_t byte_406ED[21]  ; // 9C4D
    uint8_t byte_40702[21]  ; // 9C62
    uint8_t byte_40717[21]  ; // 9C77
    uint16_t    byte_4072C  ; // 9C8C
    uint16_t    w_video_style_CL_BUG_GUESS  ; // 9C8E
    EXE_Reloc   dword_40730 ; // 9C90
    uint16_t    word_40734  ; // 9C94
    uint16_t    word_40736  ; // 9C96
    uint16_t    word_40738  ; // 9C98
    uint16_t    word_4073A  ; // 9C9A
    EXE_Reloc   dword_4073C ; // 9C9C
    EXE_Reloc   dword_40740 ; // 9CA0
    EXE_Reloc   m_addr_terrain_Movement_copy;       // 9CA4
    EXE_Reloc   dword_40748 ; // 9CA8
    EXE_Reloc   m_addr_Terrain_Movement;            // 9CAC
    EXE_Reloc   m_addr_Terrain_Explored;            // 9CB0
    EXE_Reloc   m_addr_Terrain_Changes;             // 9CB4
    EXE_Reloc   m_addr_Terrain_Bonuses;             // 9CB8
    EXE_Reloc   m_addr_Cities;                      // 9CBC
    EXE_Reloc   m_addr_Lairs_data;                  // 9CC0
    EXE_Reloc   m_addr_tower_attr;                  // 9CC4
    EXE_Reloc   m_addr_fortress_data;               // 9CC8
    EXE_Reloc   m_addr_Nodes_Attr;                  // 9CCC
    EXE_Reloc   m_addr_Terrain_LandMassID;          // 9CD0
    EXE_Reloc addr_40774_Terrain; // 9CD4
    EXE_Reloc addr_40778_Terrain; // 9CD8
    EXE_Reloc   m_addr_Terrain_Types;               // 9CDC
    EXE_Reloc   m_addr_Unrest_Table[gMAX_RACES];    // 9CE0
    uint16_t    word_407B8  ; // 9D18
    uint16_t    word_407BA  ; // 9D1A
    uint16_t    word_407BC  ; // 9D1C
    uint16_t    word_407BE  ; // 9D1E
    uint16_t    word_407C0  ; // 9D20
    uint16_t    m_Vizier_active   ; // 9D22
    uint8_t word_407C4[6]   ; // 9D24
    EXE_Reloc   dword_407CA ; // 9D2A
    EXE_Reloc   dword_407CE ; // 9D2E
    uint16_t    word_407D2  ; // 9D32
    uint16_t    word_407D4  ; // 9D34
    uint16_t    word_407D6  ; // 9D36
    uint16_t    word_407D8  ; // 9D38
    uint16_t    word_407DA  ; // 9D3A
    uint16_t    word_407DC  ; // 9D3C
    uint16_t    word_407DE  ; // 9D3E
    uint16_t    word_407E0  ; // 9D40
    uint16_t    word_407E2  ; // 9D42
    uint16_t    word_407E4  ; // 9D44
    uint16_t    word_407E6  ; // 9D46
    uint16_t    word_407E8  ; // 9D48
    uint16_t    w_AI_stack_nr   ; // 9D4A
    uint8_t unk_407EC[320]  ; // 9D4C
    EXE_Reloc   dw_Nr_in_stack  ; // 9E8C
    EXE_Reloc   dw_stack_place  ; // 9E90
    EXE_Reloc   dw_pla_stacks_loc   ; // 9E94
    EXE_Reloc   dw_Y_stacks_loc ; // 9E98
    EXE_Reloc   dw_X_stacks_loc ; // 9E9C
    EXE_Reloc   dw_bx_6160  ; // 9EA0
    EXE_Reloc   dw_bx_615C  ; // 9EA4
    EXE_Reloc   dw_bx_6176  ; // 9EA8
    EXE_Reloc   dw_bx_6154  ; // 9EAC
    uint16_t    word_40950  ; // 9EB0
    uint16_t    word_40952  ; // 9EB2
    uint16_t    word_40954  ; // 9EB4
    uint16_t    word_40956  ; // 9EB6
    uint16_t    word_40958  ; // 9EB8
    uint16_t    word_4095A  ; // 9EBA
    uint16_t    word_4095C  ; // 9EBC
    uint16_t    word_4095E  ; // 9EBE
    uint16_t    word_40960  ; // 9EC0
    EXE_Reloc   m_addr_Units;                       // 9EC2
    EXE_Reloc   m_addr_Chosen_Hero_Names;           // 9EC6

    Wizard      m_Wizards[gMAX_WIZARD_RECORDS];     // ds:9ECA / EXE:3336A

    uint8_t     m_Unk_BB7A[0xBD8E - 0xBB7A];        // ds:BB7A

    WizardsExe_Game_Data    m_Game_Data;            // ds:BD8E / EXE:3522E

    Game_Settings    m_Game_Settings;               // ds:BD9E / EXE:2523E

    uint8_t     m_Unk_BF70[0xC190 - 0xBF70];        // ds:BF70

    uint16_t    m_UnitView_nrLines;                 // ds:C190
    DS_Offset   m_Offset_UnitView_Lines;            // ds:C192
    unionUnit_Enchantment   m_UnitView_UnitEnchantment; // ds:C194

    uint8_t     m_Unk_C196[0xC292 - 0xC198];        // ds:C198

    int16_t     m_Location_Item;                    // dseg:C292
    int16_t     m_Selected_Item;                    // dseg:C294

    uint8_t     m_Unk_C296[0xC300 - 0xC296];        // ds:C296

    ePlane16    m_Plane_Item;                       // ds:C300
    int16_t     m_YPos_Item;                        // ds:C302
    int16_t     m_XPos_Item;                        // ds:C304

    uint8_t     m_Unk_C306[0xC51C - 0xC306];        // ds:C306

    int16_t     m_Combat_turn;                      // ds:C51C
    uint16_t    m_Unk_C51E;                         // ds:C51E
    eBattleCondition    m_External_battle_condition;// ds:C520
    uint16_t    m_Unk_C522;                         // ds:C522
    DS_Offset   m_Offsets_Battlefield_battleUnitIDs[22];    // ds:C524

    uint8_t     m_Unk_C550[0xC582 - 0xC550];        // ds:C550

    int16_t     m_BattleUnit_on_move;               // ds:C582
    int16_t     m_Defending_wizard;                 // ds:C584
    int16_t     m_Attacking_wizard;                 // ds:C586
    uint16_t    m_Nr_Battle_Units;                  // ds:C588
    int16_t     m_BattleUnit_pointed_at;            // ds:C58A

    uint8_t     m_Unk_C58A[0xC5C2 - 0xC58C];        // ds:C58C

    int16_t     m_Battle_winner;                    // ds:C5C2
    int16_t     m_Battle_loser;                     // ds:C5C4

    uint8_t     m_Unk_C5C6[0xC5DE - 0xC5C6];        // ds:C5C6

    int16_t     m_YPos_clash;                       // ds:C5DE
    int16_t     m_XPos_clash;                       // ds:C5E0

    uint8_t     m_Unk_C5E2[0xC8B4 - 0xC5E2];        // ds:C5E2

    uint16_t    m_Battlefield_flags;                // ds:C8B4

    uint8_t     m_Unk_C8B6[0xC8FA - 0xC8B6];        // ds:C8B6

    uint16_t    m_offset_Available_spell_pages;     // ds:C8FA

    uint8_t     m_Unk_C8FC[0xC910 - 0xC8FC];        // ds:C8FC

    EXE_Reloc   m_addr_Hero_Spells;                 // ds:C910
    uint16_t    m_Nr_available_spell_pages;         // ds:C914

    uint8_t     m_Unk_C916[0xC924 - 0xC916];        // ds:C916

    uint16_t    m_First_visible_available_spell_page;   // ds:C924

    uint8_t     m_Unk_C926[0xD15A - 0xC926];        // ds:C926

    EXE_Reloc   m_addr_Battle_figures_256;          // ds:D15A      // Battle_Figure
    int16_t     m_Nr_figures_on_battlefield;        // ds:D15E

    uint8_t     m_Unk_D160[0xD188 - 0xD160];        // ds:D160

    uint16_t    m_Battle_activetile_anim;           // ds:D188
    uint16_t    m_Battle_tileselection_anim;        // ds:D18A

    uint8_t     m_Unk_D18C[0xD49A - 0xD18C];        // ds:D18C

    uint8_t     m_NEAR_HEAP[0x1130];                // ds:D49A

    uint8_t     m_Unk_E5CA[0xE5FC - 0xE5CA];        // ds:E5CA

    EMS_descriptor    m_EMS_lbx_descriptors_0C[40]; // ds:E5FC
    uint16_t    m_required_EMS_memory;              // ds:E7DC

    uint8_t     m_Unk_E7DE[0xE800 - 0xE7DE];        // ds:E7DE

    uint8_t     m_arr16_font_palette[16];           // ds:E800
    uint8_t     m_byte_00h_from_fonts_style_data;   // ds:E810
    uint8_t     m_byte_14h_from_fonts_style_data;   // ds:E811
    uint8_t     m_byte_24h_from_fonts_style_data;   // ds:E812
    uint8_t     m_byte_34h_from_fonts_style_data;   // ds:E813
    uint16_t    m_Unk_E814;                         // ds:E814
    uint16_t    m_Unk_E816;                         // ds:E816
    int16_t     m_font_palette_section_index;       // ds:E818
    int16_t     m_cur_font_nr;                      // ds:E81A
    int16_t     m_display_text_xpel;                // ds:E81C
    int16_t     m_display_text_ypel;                // ds:E81E
    uint16_t    m_seg_fonts_style_data;             // ds:E820
    uint16_t    m_seg_fonts_border_style_data;      // ds:E822

    uint8_t     m_Unk_E824[0xE860 - 0xE824];        // ds:E824

    EMS_descriptor    m_EMS_lbx_descriptor;         // ds:E860
    uint8_t     m_Unk_E86C[0xE8A2 - 0xE86C];        // ds:E86C
    int16_t     m_clickable_area_nr;                // ds:E8A2
    uint8_t     m_Unk_E8A4[0xE8AC - 0xE8A4];        // ds:E8A4
    EXE_Reloc   m_addr_ClickableArea;               // ds:E8AC

    uint8_t     m_Unk_E8B0[0xEA54 - 0xE8B0];        // ds:E8B0

    // Offset ds:EA54  END DATA SEGMENT (DS)

    uint8_t     m_PARALIGN09[0x0C];                 // ds:EA54 / EXE:37EF4

    // STACK SEGMENT (SS)
    uint16_t    m_StackSegment[0x40];               // CS:35500 / EXE:37F00

} MoMDataSegment;

typedef struct PACKED_STRUCT // MoMMagicDataSegment
{
    char            m_DataSegmentStart[47];         // ds:0 / cs:21D10 / EXE:24510
    // "\0\0\0\0Borland C++ - Copyright 1991 Borland Intl.";

    uint8_t         m_Unk_002F[0x2AD0 - 47];        // ds:002F

    Wizard_Type_Data    m_Wizard_Types[ePortrait_MAX];  // ds:2AD0

    int16_t         m_Unk_2C1A[10];					// ds:2C1A

    int16_t         m_Nr_spell_choices[10];			// ds:2C2E
    uint8_t         m_Unk_2C40[0x2C6A - 0x2C42];    // ds:2C42
    eSpell16        m_Preselected_spell_choices[65];// ds:2C6A

    uint8_t         m_Unk_2CEC[0x3067 - 0x2CEC];    // ds:2CEC

    char            m_Copyright1_and_Version[41];   // ds:3067  Offset version is at [34]

    uint8_t         m_Unk_3090[0x35D0 - 0x3090];    // ds:3090

    Hero_Stats_Initializer  m_Hero_Stats_Initializers[gMAX_HERO_TYPES];  // ds:35D0

    uint8_t         m_Unk_3846[0x389A - 0x3846];    // ds:3846

    char            m_Copyright2_and_Version[41];   // ds:389A  Offset version is at [34]

    uint8_t         m_Unk_38C3[0x3CB8 - 0x38C3];    // ds:38C3

    uint32_t        m_BIOS_clock_snapshot;          // ds:3CB8

    uint8_t         m_Unk_3CBC[0x40E4 - 0x3CBC];    // ds:3CBC

    uint16_t        m_EMS_data_reference_point;     // ds:40E4

    uint8_t         m_Unk_40E6[0x431E - 0x40E6];    // ds:40E6

    // Duplicated from Wizards datasegement, where it could not be uint32_t because g++ would align it on a 32-bit boundary
    uint16_t        m_RNG_seed_lo;                  // ds:431E
    uint16_t        m_RNG_seed_hi;                  // ds:4320

    uint8_t         m_Unk_4322[0x434E - 0x4322];    // ds:4322

    uint16_t        m_DEBUG_Off;                    // ds:434E

    uint8_t         m_Unk_4350[0x4D44 - 0x4350];    // ds:4350

    int16_t         m_Want_input_GUESS;             // ds:4D44

    uint8_t         m_Unk_4D46[0x52EA - 0x4D46];    // ds:4D46

    int16_t         m_Button_New_game_ID;           // ds:52EA

    uint8_t         m_Unk_52EC[0x6900 - 0x52EC];    // ds:52EC

    Wizard          m_Wizards[6];                   // ds:6900

    eSpell16        m_Spells_selected_of_Realm[150];// ds:85B0

    uint8_t         m_Unk_86DC[0x87C4 - 0x86DC];    // ds:86DC

    WizardsExe_Game_Data    m_Game_Data;            // ds:87C4

    Game_Settings   m_Game_Settings;                // ds:87D4

    uint8_t         m_Unk_89A6[0x8A14 - 0x89A6];    // ds:89A6

    eSpell          m_Spell_Selected[eSpell_MAX];   // ds:8A14

    uint8_t         m_Unk_8AEB[0x8D52 - 0x8AEB];    // ds:8AEB

    uint16_t        m_Spells_Known_Realm[66];       // ds:8D52
    uint16_t        m_Unk_8DD6;                     // ds:8DD6
    uint16_t        m_Sorcery_Picks_Divider;        // ds:8DD8
    uint16_t        m_Nature_Picks_Divider;         // ds:8DDA
    uint16_t        m_Chaos_Picks_Divider;          // ds:8DDC
    uint16_t        m_Death_Picks_Divider;          // ds:8DDE
    uint16_t        m_Life_Picks_Divider;           // ds:8DE0

    uint16_t        m_Unk_8DE2[2];                  // ds:8DE2

    uint16_t        m_Spells_Known_Realm_Indexed[15];    // ds:8DE6

    uint8_t         m_Unk_8E04[0x8E0C - 0x8E04];    // ds:8E04

    int16_t         m_Button_IDs[30];               // ds:8E0C
    ePortrait       m_Wizard_portrait_shown;        // ds:8E48

    uint8_t         m_Unk_8E49[0x8E92 - 0x8E49];    // ds:8E49

    int16_t         m_Button_OK_ID;                 // ds:8E92
    uint16_t        m_Total_Picks_Left;             // ds:8E94

    uint8_t         m_Unk_8E96[0xA8A2 - 0x8E96];    // ds:8E96

    EXE_Reloc       m_addr_ClickableArea;           // ds:A8A2

    uint8_t         m_Unk_A8A6[0xAA4A - 0xA8A6];    // ds:A8A6

                                                    // ds:AA4A  END DATA SEGMENT (DS)

    //    uint8_t     m_PARALIGN09[0x0C];                 // Offset EXE:37EF4 / ds:EA54

    uint8_t         m_Unk_AA4A[0xFE68 - 0xAA4A];    // ds:AA4A

    char            m_Wizards_Name_Shown_1[11];     // ds:FE68

    uint8_t         m_Unk_FE73[0xFEC2 - 0xFE73];    // ds:FE73

    char            m_Wizards_Name_Shown_2[11];     // ds:FEC2
} MoMMagicDataSegment;


typedef struct PACKED_STRUCT // HelpLBXentry
{
    char        title[30];                          // 00
    char        lbxFile[14];                        // 1E
    uint16_t    lbxIndex;                           // 2C
    uint16_t    zero_2E;                            // 2E
    char        description[1000];                  // 30
                                                    // SIZE 418?
} HelpLBXentry;

typedef struct PACKED_STRUCT // HlpEntryLbx
{
    uint16_t    lbxIndex;                           // 00
    int16_t     left;                               // 02
    int16_t     top;                                // 04
    int16_t     right;                              // 06
    int16_t     bottom;                             // 08
                                                    // SIZE 0A
} HlpEntryLbx;

typedef struct PACKED_STRUCT // MoMFontsCharacterWidths
{
    int8_t      m_characterWidths[0x60];
} MoMFontsCharacterWidths;

typedef struct PACKED_STRUCT // MoMFontsGlyphOffsets
{
    uint16_t    m_glyphOffsets[0x60];
} MoMFontsGlyphOffsets;

typedef struct PACKED_STRUCT // MoMFontsStyleData
{
    uint8_t     m_Unk_0000[0x0010];             // 0000
    int16_t     m_curFontHeight;                // 0010
    uint8_t     m_Unk_0012[0x0036];             // 0012
    int16_t     m_curHorizontalSpacing;         // 0048
    int8_t      m_curCharacterWidths[0x60];     // 004A
    uint16_t    m_curGlyphOffsets[0x60];        // 00AA

    int16_t     m_allFontHeights[8];            // 016A
    int16_t     m_allHorizontalSpacings[8];     // 017A
    int16_t     m_allLineSpacings[8];           // 018A
    MoMFontsCharacterWidths m_allCharacterWidths[8];// 019A
    MoMFontsGlyphOffsets    m_allGlyphOffsets[8];   // 049A

    uint8_t     m_glyphData[1];                 // 0A9A
                                                // SIZE 4574
} MoMFontsStyleData;

typedef struct PACKED_STRUCT // LBX_ImageHeader
{
    uint16_t    width;                              // 00
    uint16_t    height;                             // 02
    uint16_t    compression;                        // 04   0=RLE, 0xDE0A=uncompressed
    uint16_t    nframes;                            // 06   0=uncompressed, 1+=RLE
    uint16_t    frameDelay;                         // 08
    uint16_t    flags;                              // 0A
    uint16_t    reserved_0C;                        // 0C
    uint32_t    paletteInfoOffset;                  // 0E
                                                    // SIZE 12
} MoMImageHeaderLbx;

typedef struct PACKED_STRUCT // LBX_PaletteInfo
{
    uint16_t    paletteOffset;                      // 00
    uint16_t    firstPaletteColorIndex;             // 02
    uint16_t    paletteColorCount;                  // 04
                                                    // SIZE 6
} MoMPaletteInfoLbx;

}

#undef ENUMSIZE8
#undef ENUMSIZE16

#if defined(_MSC_VER)       // Compiler MS Visual Studio
  #pragma pack(pop)
#elif defined(__MINGW32__)  // Compiler MinGW
  #pragma pack(pop)
#endif
#undef PACKED_STRUCT

#endif // MOMTEMPLATE_H
