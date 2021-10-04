#include <QtCore/QString>
#include <QtTest/QtTest>

#include <cstdio>
#include <fstream>

#include <MoMGameSave.h>
#include "test_utility.h"

namespace
{
const std::string gRefGameFile = "REFTEST.GAM";
const std::string gNotExistingGameFile = "NOTEXIST.GAM";
const std::string gTestGameFile = "SAVETEST.GAM";
}

class Test_MoMGameBase : public QObject
{
    Q_OBJECT
    
public:
    Test_MoMGameBase();
    
private Q_SLOTS:
    void testCase_Failures();
    void load();
    void save();
};

Test_MoMGameBase::Test_MoMGameBase()
{
}

void Test_MoMGameBase::testCase_Failures()
{
    MoM::MoMGameSave game;
    QVERIFY(!game.load(gNotExistingGameFile.c_str()));
    QCOMPARE(game.errorString().c_str(), "Could not open file 'NOTEXIST.GAM' for reading");
}

void Test_MoMGameBase::load()
{
    // TODO: Check each possible MoMVersion

    MoM::MoMGameSave game;
    QVERIFY(game.load(gRefGameFile.c_str()));
    QCOMPARE(game.errorString().c_str(), "");
}

void Test_MoMGameBase::save()
{
    // TODO: Check each possible type of file (.LBX, MAGIC.EXE, etcetera)

    // Setup: clean up and load a file
    (void)std::remove(gTestGameFile.c_str());
    MoM::MoMGameSave game;
    QVERIFY(game.load(gRefGameFile.c_str()));

    // Test: save file and compare it with reference
    QVERIFY(game.save(gTestGameFile.c_str()));
    QCOMPARE(game.errorString().c_str(), "");
    QCOMPARE(diff(gRefGameFile, gTestGameFile), 0);

    // Clean up
    QCOMPARE(std::remove(gTestGameFile.c_str()), 0);
}

int runTests_MoMGameBase(int argc, char *argv[])
{
    Test_MoMGameBase tc;
    return QTest::qExec(&tc, argc, argv);
}

#include "tst_momgamebase.moc"
