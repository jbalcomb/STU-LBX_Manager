// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <assert.h>
#include <stdio.h>

#include "MoMController.h"
#include "MoMGenerated.h"
#include "MoMUnit.h"
#include "MoMUtility.h"

#include "MoMCatnip.h"

namespace MoM
{

MoMCatnip::MoMCatnip()
{
}

bool MoMCatnip::apply(MoMGameBase* game)
{
    assert(0 != game);

    m_errorString.clear();

    bool ok = game->readData();
    if (!ok)
    {
        m_errorString = "failed to reread the process memory";
    }

    if (ok)
    {
        ok = (0 != game->getUnitTypeData((eUnit_Type)0));
        if (!ok)
        {
            m_errorString = "cannot read unit type data";
        }
    }

    if (ok)
    {
        // Update set_hero_slots() based on hero_type
        // Target is that a hero_type of 0-3 works as before (Fig, Bow, FigWiz, Wiz) -> (1-5-6, 2-5-6, 3-5-6, 4-6-6)
        // But that otherwise the hero_type can be of the form 0x0RQP, where P, Q, R are the 3 slots-types (1-6)
        // This means that a hero_type of 0x0652 is equivalent to a hero_type of 1 (Bow), yielding Bow-Armor-Ring

        static uint8_t new_set_hero_slots[] =
        {
//                8                      ; int __cdecl __far set_hero_slot_types(int unitType, int hero_slot_types)
//                9 0000                     set_hero_slot_types      proc far       ; CODE XREF: j_set_hero_slot_types
//               10
//               11       =0006              unitType               = word ptr  6
//               12       =0008              hero_slot_types        = word ptr  8
//               13
//               14       =01AB              offset_hero_type       = 01ABh
//               15       =SI                si_hero_slot_types     = si
//               16       =CX                cx_unitType            = cx
//               17
//               18                      ; ovr130:0000
          0x55,                         // 0000  push   bp
          0x8B,  0xEC,                  // 0001  mov    bp,    sp
          0x56,                         // 0003  push   si_hero_slot_types
          0x8B,  0x4E, 0x06,            // 0004  mov    cx_unitType, [bp+unitType]
          0x8B,  0x76, 0x08,            // 0007  mov    si_hero_slot_types,    [bp+hero_slot_types]
          0x8B,  0xC1,                  // 000A  mov    ax,    cx_unitType
          0xBA,  0x24, 0x00,            // 000C  mov    dx,    24h
          0xF7,  0xEA,                  // 000F  imul   dx
          0x8B,  0xD8,                  // 0011  mov    bx,    ax
//               28
//               29                      ; ovr130:0013
          0x8A,  0x87, 0xAB, 0x01,      // 0013  mov   al, offset_hero_type[bx]   ;    AX = 0-3 as before, or 6 + P + 6 * (Q + 6 * R), where P,Q,R are the slot-types - 1
          0x32,  0xE4,                  // 0017  xor   ah, ah
          0x3D,  0x06, 0x00,            // 0019  cmp   ax, 6
          0x7D,  0x16,                  // 001C  jge   new_method
//               35
          0x40,                         // 001E  inc   ax
          0x89,  0x04,                  // 001F  mov   [si], ax         ; Set slot1 to 1-4 (Sword, Bow, Sword/Staff, Staff)
          0xBA,  0x05, 0x00,            // 0021  mov   dx, 5
          0x89,  0x54, 0x02,            // 0024  mov   [si+2], dx       ; Set slot2 to 5 (Armor)
          0x42,                         // 0027  inc   dx
          0x89,  0x54, 0x04,            // 0028  mov   [si+4], dx       ; Set slot3 to 6 (Ring)
          0x3C,  0x04,                  // 002B  cmp   al, 4
          0x75,  0x21,                  // 002D  jnz   done
          0x89,  0x54, 0x02,            // 002F  mov   [si+2], dx       ; If AX is 4 (Wizard), overrule slot2 to 6 (Ring)
          0xEB,  0x1C,                  // 0032  jmp   short done
//               46
//               47 0034                 new_method:
          0xB1,  0x06,                  // 0034  mov   cl, 6
          0x2A,  0xC1,                  // 0036  sub   al, cl           ; AX = P + 6 * (Q + 6 * R), where P,Q,R are the slot-types - 1
          0x99,                         // 0038  cwd
          0xF6,  0xF9,                  // 0039  idiv  cl               ; AH = P, AL = Q + 6 * R
          0x8A,  0xD4,                  // 003B  mov   dl, ah
          0x42,                         // 003D  inc   dx
          0x89,  0x14,                  // 003E  mov   [si], dx         ; Set slot 1 to <P>
          0x32,  0xE4,                  // 0040  xor   ah, ah
          0xF6,  0xF9,                  // 0042  idiv  cl               ; AH = Q, AL = R
          0x8A,  0xD4,                  // 0044  mov   dl, ah
          0x42,                         // 0046  inc   dx
          0x89,  0x54, 0x02,            // 0047  mov   [si+2], dx       ; Set slot 2 to <Q>
          0x8A,  0xD0,                  // 004A  mov   dl, al
          0x42,                         // 004C  inc   dx
          0x89,  0x54, 0x04,            // 004D  mov   [si+4], dx       ; Set slot 3 to <R>
//               63
//               64 0050                 done:
          0x5E,                         // 0050  pop   si
          0x5D,                         // 0051  pop   bp
          0xCB,                         // 0052  retf
//               68
//               69                      ; nops
//               70 0053                         endp

                // ...          nops
                //              endp
                // ovr130:0071
        };

        uint8_t* code = game->getWizardsOverlay(130);

        if (0 != code)
        {
            memcpy(code, new_set_hero_slots, sizeof(new_set_hero_slots));
            memset(code + sizeof(new_set_hero_slots), '\0', 0x0071 - sizeof(new_set_hero_slots));

            ok = game->commitData(code, code, 0x0071);
            if (!ok)
            {
                m_errorString = "failed to commit code changes";
            }
        }
    }

    // Set hero attributes
    if (ok)
    {
        // HEROES2
        // Copied from http://realmsbeyond.net/forums/showthread.php?t=4303 at 2012-03-25
        //

        // TODO: W/B=Warrior/Bow???
        // TODO: specials

        Unit_Type_Data* data = 0;           // old style
        MoMUnit momUnit(game);              // new style
        Hero_stats newHeroStats;            // new style
        Unit_Type_Data newUnitTypeData;     // new style

        //Barbarian      0     -     -     -     -     -     -     -     -     -
        // specials: Thrown 3, Blade Master
        //  item slots: W+W+A
        //   spells:

        // old style
//        data = game->getUnit_Type_Data(MoM::UNITTYPE_Barbarian);
//        data->m_Hero_TypeCode_or_Building2 = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield);

        // new style
        momUnit.changeUnit(MoM::UNITTYPE_Barbarian);

        newUnitTypeData = momUnit.getUnitTypeData();
        newUnitTypeData.m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield);
        newUnitTypeData.m_Upkeep = 4;
        ok |= momUnit.setUnitTypeData(newUnitTypeData);

        newHeroStats = momUnit.getHeroStats();
        newHeroStats.m_Hero_Abilities.s.Armsmaster = true;
        ok |= momUnit.setHeroStats(newHeroStats);

        //Bard           0     -     -     -     -     -     -     -     -     -
        // specials: Legendary, Charmed, Lucky
        //  item slots: W+A+R
        //   spells:
//        data = game->getUnit_Type_Data(MoM::UNITTYPE_Bard);
//        data->m_Hero_TypeCode_or_Building2 = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        momUnit.changeUnit(MoM::UNITTYPE_Bard);

        newUnitTypeData = momUnit.getUnitTypeData();
        newUnitTypeData.m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);
        ok |= momUnit.setUnitTypeData(newUnitTypeData);

        newHeroStats = momUnit.getHeroStats();
        newHeroStats.m_Hero_Abilities.s.Leadership = false;
        newHeroStats.m_Hero_Abilities.s.Leadership_X = true;
        ok |= momUnit.setHeroStats(newHeroStats);

        //Beastmaster    0     -     -     -     -     -     -     -     -     -
        // specials: Pathfinding, caster 5
        //  item slots: W+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Beastmaster);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Dervish        0     -     -     -     -     -     -     -     -     -
        // specials: Noble, 1x Random
        //  item slots: W/B+W+R
        //   spells:
        // TODO: Warrior/Bow???
//        data = game->getUnit_Type_Data(MoM::UNITTYPE_Dervish);
//        data->m_Hero_TypeCode_or_Building2 = game->getHeroTypeCode(MoM::SLOT16_Bow, MoM::SLOT16_Sword, MoM::SLOT16_Amulet);

        momUnit.changeUnit(MoM::UNITTYPE_Dervish);

        newUnitTypeData = momUnit.getUnitTypeData();
        newUnitTypeData.m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Bow, MoM::SLOT16_Sword, MoM::SLOT16_Amulet);
        ok |= momUnit.setUnitTypeData(newUnitTypeData);

        newHeroStats = momUnit.getHeroStats();
        newHeroStats.m_Hero_Abilities.s.Leadership = true;
        ok |= momUnit.setHeroStats(newHeroStats);

        //Dwarf          0     -     -     -     -     -     -     -     -     -
        // specials: Constitution, Mountaineer, Wall Crushing
        //  item slots: W+A+A
        //   spells:
//        data = game->getUnit_Type_Data(MoM::UNITTYPE_Dwarf);
//        data->m_Hero_TypeCode_or_Building2 = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Armor_Shield);

        momUnit.changeUnit(MoM::UNITTYPE_Dwarf);

        newUnitTypeData = momUnit.getUnitTypeData();
        newUnitTypeData.m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Armor_Shield);
        newUnitTypeData.m_Upkeep = 4;
        ok |= momUnit.setUnitTypeData(newUnitTypeData);

        newHeroStats = momUnit.getHeroStats();
        newHeroStats.m_Hero_Abilities.s.Armsmaster = true;
        ok |= momUnit.setHeroStats(newHeroStats);

        //Healer         0     -     -     -     -     -     -     -     -     -
        // specials: Healer, Caster 7.5
        //  item slots: W+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Healer);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Huntress       0     -     -     -     -     -     -     -     -     -
        // specials: Blademaster, Forester, Long Range
        //  item slots: W/B+R+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Huntress);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Bow, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);

        //Orc Warrior    0     -     -     -     -     -     -     -     -     -
        // specials: Might, First strike
        //  item slots: W+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Orc_Warrior);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Sage           0     -     -     -     -     -     -     -     -     -
        // specials: Sage, Caster 7.5
        //  item slots: S+R+R
        //   spells:
//        data = game->getUnit_Type_Data(MoM::UNITTYPE_Sage);
//        data->m_Hero_TypeCode_or_Building2 = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);

        momUnit.changeUnit(MoM::UNITTYPE_Sage);

        newUnitTypeData = momUnit.getUnitTypeData();
        newUnitTypeData.m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);
        newUnitTypeData.m_Upkeep = 4;
        ok |= momUnit.setUnitTypeData(newUnitTypeData);

        newHeroStats = momUnit.getHeroStats();
        newHeroStats.m_Hero_Abilities.s.Sage = false;
        newHeroStats.m_Hero_Abilities.s.Sage_X = true;
        ok |= momUnit.setHeroStats(newHeroStats);

        //Thief          0     -     -     -     -     -     -     -     -     -
        // specials: Armour Piercing, Agility
        //  item slots: W+R+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Thief);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);

        //Assassin       5     -     -     -     -     -     -     -     -     -
        // specials: Poison, Blade Master, Web, 1x Random
        //  item slots: W+W+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Assassin);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword, MoM::SLOT16_Sword, MoM::SLOT16_Amulet);

        //Druid          5     -     -     -     -     -     -     -     -     -
        // specials: Purification, Regeneration, Caster 7.5
        //  item slots: S+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Druid);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Magician       5     -     -     -     -     -     -     -     -     -
        // specials: Arcane Power, Caster 7.5
        //  item slots: S+S+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Magician);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Staff_Wand, MoM::SLOT16_Amulet);

        //War Monk       5     -     -     -     -     -     -     -     -     -
        // specials: Super Agility, First Strike, Negate First Strike, Blademaster
        //  item slots: R+R+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_War_Monk);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Amulet, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);

        //Warrior Mage   5     -     -     -     -     -     -     -     -     -
        // specials:
        //  item slots: S/W+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Warrior_Mage);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword_Staff_Wand, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Draconian     10     -     -     -     -     -     -     -     -     -
        // specials: Fire Breath 5, Flying, Might, 1x Random
        //  item slots: S+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Draconian);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Golden One    10     -     -     -     -     -     -     -     -     -
        // specials: Caster 5, 3x Random
        //  item slots: W/S+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Golden_One);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Sword_Staff_Wand, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Ranger        10     -     -     -     -     -     -     -     -     -
        // specials: Pathfinding, Caster 5, , Scouting III, 1x Random
        //  item slots: W/B+A+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Ranger);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Bow, MoM::SLOT16_Armor_Shield, MoM::SLOT16_Amulet);

        //Wind Mage     10     -     -     -     -     -     -     -     -     -
        // specials: Windwalking, caster 7.5, 1x Random
        //  item slots: S+R+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Wind_Mage);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);

        //Witch         10     -     -     -     -     -     -     -     -     -
        // specials: Caster 10, Life Steal
        //  item slots: S+R+R
        //   spells:
        data = game->getUnitTypeData(MoM::UNITTYPE_Witch);
        data->m_Building2_or_HeroType = game->getHeroTypeCode(MoM::SLOT16_Staff_Wand, MoM::SLOT16_Amulet, MoM::SLOT16_Amulet);


        //
        // Non-hero units
        //

        // TODO: Large Shield is +3 or +4

        DS_Offset spareNameOffset = 0xEA60;   // Spare room in Stack Segment
        uint16_t size = 0;
        char* spareNameBuffer = const_cast<char*>(game->getNameByOffset(spareNameOffset));
        char* spareNames = spareNameBuffer;

        uint16_t offsetClerics = spareNameOffset;
        size = 1 + sprintf(spareNames, "%s", "Clerics");
        spareNameOffset += size;
        spareNames += size;
        uint16_t offsetEvokers = spareNameOffset;
        size += 1 + sprintf(spareNames, "%s", "Evokers");
        spareNameOffset += size;
        spareNames += size;
        uint16_t offsetSorcerers = spareNameOffset;
        size += 1 + sprintf(spareNames, "%s", "Sorcerers");
        spareNameOffset += size;
        spareNames += size;
        uint16_t offsetEnchanters = spareNameOffset;
        size += 1 + sprintf(spareNames, "%s", "Enchanters");
        spareNameOffset += size;
        spareNames += size;
        uint16_t offsetSummoners = spareNameOffset;
        size += 1 + sprintf(spareNames, "%s", "Summoners");
        spareNameOffset += size;
        spareNames += size;
        uint16_t offsetWizards = spareNameOffset;
        size += 1 + sprintf(spareNames, "%s", "Wizards");
        spareNameOffset += size;
        spareNames += size;

        ok = game->commitData(spareNameBuffer, spareNameBuffer, (size_t)(spareNames - spareNameBuffer));

        for (int unitTypeNr = 0; MoM::toUInt(unitTypeNr) < MoM::eUnit_Type_MAX; ++unitTypeNr)
        {
            MoM::eUnit_Type unitType = static_cast<MoM::eUnit_Type>(unitTypeNr);
            MoM::Unit_Type_Data* data = game->getUnitTypeData(unitType);
            if (0 == data)
            {
                ok = false;
                break;
            }
            std::string unitName = toStr(unitType);

//            bool isFootUnit = (-1 != QRegExp("Spearmen").indexIn(unitName)
//                    || -1 != QRegExp("Swordsmen").indexIn(unitName)
//                    || -1 != QRegExp("Bowmen").indexIn(unitName)
//                    || -1 != QRegExp("Swordsmen").indexIn(unitName)
//                    || -1 != QRegExp("Halberdiers").indexIn(unitName)
//                    || -1 != QRegExp("Pikemen").indexIn(unitName)
//                    || -1 != QRegExp("Berserkers").indexIn(unitName)
//                    || -1 != QRegExp("Minotaurs").indexIn(unitName)
//                    || -1 != QRegExp("Nightblades").indexIn(unitName)
//                    || -1 != QRegExp("Hammerhands").indexIn(unitName)
//                    || -1 != QRegExp("Rangers").indexIn(unitName)
//                    || -1 != QRegExp("War_Trolls").indexIn(unitName)
//                    );

//            bool isCavalry = (QRegExp("Cavalry").indexIn(unitName)
//                    || QRegExp("Horsebowmen").indexIn(unitName)
//                    || QRegExp("Centaurs").indexIn(unitName)
//                    || QRegExp("Nightmares").indexIn(unitName)
//                    || QRegExp("Wolf_Riders").indexIn(unitName)
//                    || QRegExp("Elven_Lords").indexIn(unitName)
//                    || QRegExp("Pegasai").indexIn(unitName)
//                    || QRegExp("Paladins").indexIn(unitName)
//                    || QRegExp("Stag_Beetle").indexIn(unitName)
//                    || QRegExp("Dragon_Turtle").indexIn(unitName)
//                    || QRegExp("Griffins").indexIn(unitName)
//                    || QRegExp("Wyvern_Riders").indexIn(unitName)
//                    || QRegExp("War_Mammoths").indexIn(unitName)
//                    );

            //Clerics are a new unit type which is available to high men, beastmen and halflings.
            //They replace priests and shaman respectively.
            bool isCleric = ((MoM::UNITTYPE_Beastmen_Priests == unitType) || (MoM::UNITTYPE_Halfling_Shaman == unitType) || (MoM::UNITTYPE_High_Men_Priests == unitType));

            //
            // All foot units get 2 moves and 2 hearts, cavalry gets 4 moves and 5 hearts, catapults stay at 1 move and 10 hearts.
            //

//            if (isFootUnit)
//            {
//                data->m_MoveHalves = 4;
//                data->m_Hitpoints = 2;
//            }
//            else if (-1 != QRegExp("Cavalry").indexIn(unitName))
//            {
//                data->m_MoveHalves = 8;
//                data->m_Hitpoints = 5;
//            }
//			else if (data->m_Movement_Flags.s.Cavalry)
//			{
//                data->m_MoveHalves = 8;
//			}

            data->m_MoveHalves += 1;

            //
            // Normal units
            //

            if (std::string::npos != unitName.find("Spearmen"))
            {
//                //Spearmen: Cost 20, 7 fig, 2 off, 3 def, negate first strike
//                data->m_Cost = 20;
//                data->m_Nr_Figures = 7;
//                data->m_Melee = 2;
//                data->m_Defense = 3;
//                data->m_Ability_Flags.s.Negate_First_Strike = true;
            }
            else if (std::string::npos != unitName.find("Swordsmen"))
            {
                //Swordsmen: Cost 30, 7 fig, 4 off, 2 def, large shield (hacked to give +3 or possibly +4 ranged def)
//                data->m_Cost = 30;
//                data->m_Nr_Figures = 7;
//                data->m_Melee = 4;
//                data->m_Defense = 2;
//                data->m_Ability_Flags.s.Large_Shield = true;
            }
            else if (std::string::npos != unitName.find("Bowmen"))
            {
                //Bowmen: Cost 40, 6 fig, 1 off, 1 def, 2 rng, 6 ammo
//                data->m_Cost = 40;
//                data->m_Nr_Figures = 6;
//                data->m_Melee = 1;
//                data->m_Defense = 1;
//                data->m_Ranged = 2;
//                data->m_Ranged_Shots = 6;
                data->m_Ranged += 1;
            }
            else if (std::string::npos != unitName.find("Halberdiers"))
            {
                //Halberdiers: Cost 50, 6 fig, 5 off, 3 def, armour piercing
//                data->m_Cost = 50;
//                data->m_Nr_Figures = 6;
//                data->m_Melee = 5;
//                data->m_Defense = 3;
//                data->m_Attack_Flags.s.Armor_Piercing = true;
                data->m_Attack_Flags.s.Armor_Piercing = true;
            }
            else if (std::string::npos != unitName.find("Cavalry"))
            {
                //Cavalry: Cost 60, 4 fig, 7 off, 2 def, first strike
//                data->m_Cost = 60;
//                data->m_Nr_Figures = 4;
//                data->m_Melee = 7;
//                data->m_Defense = 2;
//                data->m_Ranged = 2;
//                data->m_Attack_Flags.s.First_Strike = true;
            }
            else if (isCleric)
            {
                //Clerics: Cost 100, 3 fig, 6 off, 5 def, healing
                data->m_PtrName = offsetClerics;
//                data->m_Cost = 100;
//                data->m_Nr_Figures = 3;
//                data->m_Melee = 6;
//                data->m_Defense = 5;
//                data->m_Ability_Flags.s.Healer = false;
//                data->m_Ability_Flags.s.Purify = false;
//                data->m_Attribute_Flags.s.Healing_Spell = true;
                data->m_Attribute_Flags.s.Healing_Spell = true;
            }
            else if (std::string::npos != unitName.find("Shaman"))
            {
                //Shaman: Cost 80, 3 fig, 2 off, 4 def, 3 rng, 4 ammo, purify, healer
//                data->m_Cost = 80;
//                data->m_Nr_Figures = 3;
//                data->m_Melee = 2;
//                data->m_Defense = 4;
//                data->m_Ranged = 3;
//                data->m_Ranged_Shots = 4;
//                data->m_Ability_Flags.s.Purify = true;
//                data->m_Ability_Flags.s.Healer = true;
//                data->m_Attribute_Flags.s.Healing_Spell = false;
            }
            else if (std::string::npos != unitName.find("Priests"))
            {
                //Priests: Cost 120, 3 fig, 3 off, 4 def, 4 rng, 4 ammo, magic immunity
//                data->m_Cost = 120;
//                data->m_Nr_Figures = 3;
//                data->m_Melee = 3;
//                data->m_Defense = 4;
//                data->m_Ranged = 4;
//                data->m_Ranged_Shots = 4;
//                data->m_Ability_Flags.s.Purify = false;
//                data->m_Ability_Flags.s.Healer = false;
//                data->m_Attribute_Flags.s.Healing_Spell = false;
//                data->m_Immunity_Flags.s.Magic_Immunity = true;
                data->m_Immunity_Flags.s.Magic_Immunity = true;
            }
            else if (std::string::npos != unitName.find("Magicians"))
            {
                //Magicians: stats (and name) vary from race to race

                // 3 figures (default is 4)
//                data->m_Nr_Figures = 3;
                // Fire Ball Spell is no longer a "given"
//                data->m_Attribute_Flags.s.Fire_Ball_Spell = false;
                // Default Ranged strength is 5 for Magicians and 7 for Warlocks
                // Default Ranged Shots is 4

                switch (unitType)
                {
                case MoM::UNITTYPE_Beastmen_Magicians:
                    //        Beastmen Evokers
                    //        Fireball, strength 5 ranged attack, 6 ammo
                    data->m_PtrName = offsetEvokers;
//                    data->m_Attribute_Flags.s.Fire_Ball_Spell = true;
//                    data->m_Ranged = 5;
                    data->m_Ranged_Shots += 2;
                    break;

                case MoM::UNITTYPE_Draconian_Magicians:
                    //        Draconian Sorcerers
                    //        Caster 40, no ranged attack, fire breath and decent melee
                    data->m_PtrName = offsetSorcerers;
//                    data->m_Attribute_Flags.s.Caster_40_MP = true;
////                    data->m_Race_Code = MoM::RACE_Chaos;            // Has to be a color (not arcane) to make Caster_40_MP work
////                    data->m_Attribute_Flags.s.Standard = false;     // Do not display the color-prefix for the name
//                    data->m_Ranged_Type = MoM::RANGED_None;
//                    data->m_Ranged = 2;
//                    data->m_Ranged_Shots = 0;
                    break;

                case MoM::UNITTYPE_Dark_Elf_Warlocks:
                    // No changes
                    break;

                case MoM::UNITTYPE_High_Elf_Magicians:
                    //        High Elf Enchanters
                    //        Caster 20, weak ranged attack and good melee
                    data->m_PtrName = offsetEnchanters;
//                    data->m_Attribute_Flags.s.Caster_20_MP = true;
////                    data->m_Race_Code = MoM::RACE_Nature;           // Has to be a color (not arcane) to make Caster_40_MP work
////                    data->m_Attribute_Flags.s.Standard = false;     // Do not display the color-prefix for the name
//                    data->m_Melee = 2;
//                    data->m_Ranged = 3;
                    break;

                case MoM::UNITTYPE_High_Men_Magicians:
                    //        High Men Magicians
                    //        Strong ranged attack, 4 figures (others are 3)
//                    data->m_PtrName = offsetMagicians;
//                    data->m_Nr_Figures = 4;
                    data->m_Ranged += 2;
                    break;

                case MoM::UNITTYPE_Nomad_Magicians:
                    data->m_PtrName = offsetWizards;
                    data->m_Attribute_Flags.s.Web_Spell = true;
                    break;

                case MoM::UNITTYPE_Orc_Magicians:
                    //        Orc Summoners
                    //        Summon Demon, ranged attack
                    data->m_PtrName = offsetSummoners;
                    data->m_Attribute_Flags.s.Summon_Demons_1 = true;
                    break;

                default:
                    break;
                }
            }
            else if (std::string::npos != unitName.find("Catapult"))
            {
                //Catapult: Cost 40, 1 fig, 0 off, 1 def, 8 rng, 10 ammo, long range, wall crusher, large shield
//                data->m_Cost = 40;
//                data->m_Nr_Figures = 1;
//                data->m_Melee = 0;
//                data->m_Defense = 1;
//                data->m_Ranged = 8;
//                data->m_Ranged_Shots = 10;
//                data->m_Ability_Flags.s.Long_Range = true;
//                data->m_Ability_Flags.s.Wall_Crusher = true;
//                data->m_Ability_Flags.s.Large_Shield = true;
            }
        }

        ok = game->commitData(game->getUnitTypeData((eUnit_Type)0), game->getUnitTypeData((eUnit_Type)0), sizeof(MoM::Unit_Type_Data) * MoM::eUnit_Type_MAX);
        if (!ok)
        {
            m_errorString = "failed to commit unit data";
        }
    }

    return ok;
}

}
