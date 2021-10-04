#include <QtCore/QString>
#include <QtTest/QtTest>

#include <cmath>
#include <cstdio>
#include <fstream>

#include <MoMCombat.h>
#include <MoMController.h>
#include <MoMGameMemory.h>
#include "test_utility.h"

class Test_MoMCombat : public QObject
{
    Q_OBJECT
    
public:
    Test_MoMCombat();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void testCase_Nominal();

    void testCase_Abilities();
    void testCase_AbilityFlying();
    void testCase_AbilityInvisible();
    void testCase_Items();
    void testCase_ItemFlying();
    void testCase_ItemInvisible();
//    void testCase_ItemInvulnerability();
    void testCase_Level();

    void testCase_ShootArrows();
    void testCase_ShootMagic();

//    void testCase_SpecialPreBreath();
//    void testCase_SpecialPreGaze();
//    void testCase_SpecialPreThrown();
//    void testCase_SpecialPreGazeDefend();
//    void testCase_SpecialPreMultipleGaze();

//    void testCase_SpecialImmolation();
//    void testCase_SpecialPoison();
//    void testCase_SpecialTouch();
//    void testCase_SpecialFirstStrike();

//    void testCase_Spells();
    void testCase_SpellFlying();
//    void testCase_SpellInvisibility();
//    void testCase_SpellInvulnerability();
    void testCase_WeaponType();

private:
    void createHero(MoM::ePlayer playerNr, int slotNr, int unitNr);

    double prec1(double v)
    {
        // Format the number v to have 3 digit max after the decimal point.
        return std::floor(v * 10 + 0.5) / 10;
    }

    void test_combat_attack(const std::string& testName);
    void test_shoot_attack(const std::string& testName, int distance);

private:
    MoM::MoMGameMemory m_gameMemory;
    MoM::MoMGameMemory* m_game;
    MoM::MoMCombat  m_combat;
    MoM::CombatUnit m_attacker;
    MoM::CombatUnit m_defender;
};

Test_MoMCombat::Test_MoMCombat() :
    m_gameMemory(),
    m_game(),
    m_combat(),
    m_attacker(),
    m_defender()
{
}

void Test_MoMCombat::initTestCase()
{
    // General set up, which is connecting to a game, since we need the unit data.
    std::unique_ptr<MoM::MoMProcess> momProcess(new MoM::MoMProcess);
    QVERIFY(momProcess->findProcessAndData());
    QVERIFY(m_gameMemory.openGame(momProcess));
    m_game = &m_gameMemory;
}

void Test_MoMCombat::cleanupTestCase()
{
}

void Test_MoMCombat::init()
{
    m_combat = MoM::MoMCombat();
    m_attacker = MoM::CombatUnit(m_game);
    m_defender = MoM::CombatUnit(m_game);
}

void Test_MoMCombat::cleanup()
{
}

void Test_MoMCombat::createHero(MoM::ePlayer playerNr, int slotNr, int unitNr)
{
    MoM::Hired_Hero* hiredHero = m_game->getHiredHero(playerNr, slotNr);
    MoM::Unit* unit = m_game->getUnit(unitNr);

    QVERIFY(0 != hiredHero);
    QVERIFY(0 != unit);
    memset(hiredHero, '\0', sizeof(*hiredHero));
    memset(unit, '\0', sizeof(*unit));

    hiredHero->m_Unit_Nr = unitNr;
    hiredHero->m_Items_In_Slot[0] = -1;
    hiredHero->m_Items_In_Slot[1] = -1;
    hiredHero->m_Items_In_Slot[2] = -1;
    unit->m_Owner = playerNr;
    unit->m_Hero_Slot_Number = slotNr;
}

void Test_MoMCombat::test_combat_attack(const std::string& testName)
{
    std::ofstream ofs(("out/" + testName + ".out").c_str());

    ofs << testName << "\n";
    ofs << "Unit " << m_attacker.getDisplayName() << "\n"
        << "attacks unit " <<  m_defender.getDisplayName() << "\n\n";

    std::string ostr = m_combat.combat_attack(m_attacker, m_defender);

    ofs << ostr;
    ofs.close();

    QVERIFY(0 == diff("out/" + testName + ".out", "out/" + testName + ".ref"));
}

void Test_MoMCombat::test_shoot_attack(const std::string& testName, int distance)
{
    std::ofstream ofs(("out/" + testName + ".out").c_str());

    ofs << testName << "\n";
    ofs << "Unit " << m_attacker.getDisplayName() << "\n"
        << "shoot unit " <<  m_defender.getDisplayName() << "\n"
        << "at distance " <<  distance << "\n\n";

    std::string ostr = m_combat.shoot_attack(m_attacker, m_defender, distance);

    ofs << ostr;
    ofs.close();

    QVERIFY(0 == diff("out/" + testName + ".out", "out/" + testName + ".ref"));
}

void Test_MoMCombat::testCase_Nominal()
{
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Swordsmen);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_combat_attack("testCase_Nominal");
}

void Test_MoMCombat::testCase_Abilities()
{
    // TODO
    // First Strike
    // Armor Piercing
    // Breath
    // Gaze
    // Touch
    // ...
}

void Test_MoMCombat::testCase_AbilityFlying()
{

    // Cannot attack flying creature
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Swordsmen);
    m_defender.changeUnit(MoM::UNITTYPE_Green_Gorgons);
    test_combat_attack("testCase_AbilityFlying1");

    // Flying creature can attack ground unit in which case the ground unit can retaliate
    m_attacker.changeUnit(MoM::UNITTYPE_Green_Gorgons);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_combat_attack("testCase_AbilityFlying2");
}

void Test_MoMCombat::testCase_AbilityInvisible()
{
    // Attack invisible
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Swordsmen);
    m_defender.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    test_combat_attack("testCase_AbilityInvisible1");

    // Defend against invisible
    m_attacker.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Swordsmen);
    test_combat_attack("testCase_AbilityInvisible2");
}

void Test_MoMCombat::testCase_Items()
{
    // TODO
    // Vampiric
    // ...
}

void Test_MoMCombat::testCase_ItemFlying()
{
    int attUnitNr = 0;
    int defUnitNr = 1;

    // Can't attack flying hero
    createHero(MoM::PLAYER_YOU, 0, attUnitNr);
    MoM::Unit* attUnit = m_game->getUnit(attUnitNr);
    MoM::Hired_Hero* attHero = m_game->getHiredHero(attUnit);
    attUnit->m_Unit_Type = MoM::UNITTYPE_Dwarf;
    m_attacker.changeUnit(attUnit);

    createHero(MoM::PLAYER_1, 0, defUnitNr);
    MoM::Unit* defUnit = m_game->getUnit(defUnitNr);
    MoM::Hired_Hero* defHero = m_game->getHiredHero(defUnit);
    defUnit->m_Unit_Type = MoM::UNITTYPE_Barbarian;
    defHero->m_Items_In_Slot[2] = 0;
    MoM::Item* item = m_game->getItem(0);
    memset(item, '\0', sizeof(*item));
    item->m_Bitmask_Powers.s.Flight = true;
    m_defender.changeUnit(defUnit);

    test_combat_attack("testCase_ItemFlying1");

    // Flying hero can attack flying hero
    attHero->m_Items_In_Slot[2] = 0;
    test_combat_attack("testCase_ItemFlying2");
}

void Test_MoMCombat::testCase_ItemInvisible()
{
}

void Test_MoMCombat::testCase_Level()
{
    MoM::Unit attUnit = m_attacker.getUnitInGame();
    attUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Swordsmen;
    attUnit.m_Level = MoM::LEVEL_Level_5;   // Super-Elite
    m_attacker.changeUnit(&attUnit);

    MoM::Unit defUnit = m_defender.getUnitInGame();
    defUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Spearmen;
    defUnit.m_Level = MoM::LEVEL_Level_2;   // Regular
    m_defender.changeUnit(&defUnit);

    test_combat_attack("testCase_Level");
}

void Test_MoMCombat::testCase_ShootArrows()
{
    // Regular shooting arrows
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Bowmen);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_shoot_attack("testCase_ShootArrows_Arrows", 5);

    // Bullets
    m_attacker.changeUnit(MoM::UNITTYPE_Halfling_Slingers);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_shoot_attack("testCase_ShootArrows_Bullets", 10);

    // Rocks
    m_attacker.changeUnit(MoM::UNITTYPE_Catapult);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_shoot_attack("testCase_ShootArrows_Rocks", 10);

    // Shoot invisible not allowed at a distance
    m_attacker.changeUnit(MoM::UNITTYPE_Catapult);
    m_defender.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    test_shoot_attack("testCase_ShootArrows_Invis1", 2);

    // Shoot invisible at distance 1
    m_attacker.changeUnit(MoM::UNITTYPE_Catapult);
    m_defender.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    test_shoot_attack("testCase_ShootArrows_Invis2", 1);
}

void Test_MoMCombat::testCase_ShootMagic()
{
    // Shoot magic
    m_attacker.changeUnit(MoM::UNITTYPE_Blue_Storm_Giant);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_shoot_attack("testCase_ShootMagic_Unit", 5);

    // Use mana
    MoM::Unit attUnit = m_attacker.getUnitInGame();
    attUnit.m_Unit_Type = MoM::UNITTYPE_Healer;
    attUnit.m_Level = MoM::LEVEL_Level_4;
    m_attacker.changeUnit(&attUnit);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_shoot_attack("testCase_ShootMagic_Hero", 5);

    // Shoot invisible not allowed at a distance
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Priests);
    m_defender.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    test_shoot_attack("testCase_ShootMagic_Invis1", 2);

    // Shoot invisible at distance 1
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Priests);
    m_defender.changeUnit(MoM::UNITTYPE_Dark_Elf_Nightblades);
    test_shoot_attack("testCase_ShootMagic_Invis2", 1);
}

void Test_MoMCombat::testCase_SpellFlying()
{
    // Cannot attack flying creature
    m_attacker.changeUnit(MoM::UNITTYPE_High_Men_Swordsmen);
    MoM::Unit defUnit = m_defender.getUnitInGame();
    defUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Spearmen;
    defUnit.m_Unit_Enchantment.s.Flight = true;
    m_defender.changeUnit(&defUnit);
    test_combat_attack("testCase_SpellFlying1");

    // Flying creature can attack ground unit
    MoM::Unit attUnit = m_attacker.getUnitInGame();
    attUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Swordsmen;
    attUnit.m_Unit_Enchantment.s.Flight = true;
    m_attacker.changeUnit(&attUnit);
    m_defender.changeUnit(MoM::UNITTYPE_High_Men_Spearmen);
    test_combat_attack("testCase_SpellFlying2");
}

void Test_MoMCombat::testCase_WeaponType()
{
    MoM::Unit attUnit = m_attacker.getUnitInGame();
    attUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Swordsmen;
    attUnit.m_Weapon_Mutation.s.Weapon_Type = MoM::WEAPON_magic;
    m_attacker.changeUnit(&attUnit);

    MoM::Unit defUnit = m_defender.getUnitInGame();
    defUnit.m_Unit_Type = MoM::UNITTYPE_High_Men_Spearmen;
    defUnit.m_Weapon_Mutation.s.Weapon_Type = MoM::WEAPON_adamantium;
    m_defender.changeUnit(&defUnit);

    test_combat_attack("testCase_WeaponType");
}

int runTests_MoMCombat(int argc, char *argv[])
{
    Test_MoMCombat tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_momcombat.moc"
