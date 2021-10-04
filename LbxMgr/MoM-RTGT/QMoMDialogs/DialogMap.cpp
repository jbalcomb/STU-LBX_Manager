// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2012-03-12
// ---------------------------------------------------------------------------

#include "DialogMap.h"
#include "ui_DialogMap.h"

#include <QGraphicsScene>
#include <QMenu>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <cmath>
#include <fstream>
#include <vector>

#include "DialogCalculatorAddress.h"
#include "MainWindow.h"
#include "MoMCombat.h"
#include "MoMController.h"
#include "MoMGenerated.h"
#include "MoMExeWizards.h"
#include "MoMTemplate.h"
#include "MoMTerrain.h"
#include "MoMUtility.h"
#include "QMoMAnimationTile.h"
#include "QMoMMapScene.h"
#include "QMoMMapTile.h"
#include "QMoMResources.h"
#include "QMoMSettings.h"
#include "QMoMTreeItemWidget.h"
#include "QMoMUnitTile.h"

namespace MoM
{

DialogMap::DialogMap(QWidget *parent) :
    QMoMDialogBase(parent),
    m_sceneArcanus(new QMoMMapScene(MoM::PLANE_Arcanum, false)),
    m_sceneMyrror(new QMoMMapScene(MoM::PLANE_Myrror, false)),
    m_sceneBattle(new QMoMMapScene(MoM::ePlane_MAX, true)),
    m_timer(new QTimer),
    ui(new Ui::DialogOverlandMap)
{
    ui->setupUi(this);

    postInitialize();


//    QMoMGamePtr game= MainWindow::getInstance()->getGame();
//    if ((0 != game) && (game->isBattleInProgress()))
//    {
//        ui->comboBox_Plane->setCurrentIndex(3);
//    }
//    else
//    {
//        ui->comboBox_Plane->setCurrentIndex(1);
//    }

    // Update view when checkbox is clicked
    QObject::connect(ui->checkBox_Cities, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_EnemyUnits, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_ExploredOnly, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_Lairs, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_Terrain, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_TerrainBonuses, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_TerrainChanges, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));
    QObject::connect(ui->checkBox_YourUnits, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));

    // Update view when items are inspected
    QObject::connect(m_sceneArcanus, SIGNAL(signal_tileChanged(MoM::MoMLocation)), this, SLOT(slot_tileChanged(MoM::MoMLocation)));
    QObject::connect(m_sceneMyrror, SIGNAL(signal_tileChanged(MoM::MoMLocation)), this, SLOT(slot_tileChanged(MoM::MoMLocation)));
    QObject::connect(m_sceneBattle, SIGNAL(signal_tileChanged(MoM::MoMLocation)), this, SLOT(slot_tileChanged(MoM::MoMLocation)));

    QObject::connect(m_sceneArcanus, SIGNAL(signal_tileSelected(MoM::MoMLocation)), this, SLOT(slot_tileSelected(MoM::MoMLocation)));
    QObject::connect(m_sceneMyrror, SIGNAL(signal_tileSelected(MoM::MoMLocation)), this, SLOT(slot_tileSelected(MoM::MoMLocation)));
    QObject::connect(m_sceneBattle, SIGNAL(signal_tileSelected(MoM::MoMLocation)), this, SLOT(slot_tileSelected(MoM::MoMLocation)));

    QObject::connect(m_sceneArcanus, SIGNAL(signal_tileDragged(MoM::MoMLocation,MoM::MoMLocation)), this, SLOT(slot_tileDragged(MoM::MoMLocation,MoM::MoMLocation)));
    QObject::connect(m_sceneMyrror, SIGNAL(signal_tileDragged(MoM::MoMLocation,MoM::MoMLocation)), this, SLOT(slot_tileDragged(MoM::MoMLocation,MoM::MoMLocation)));
    QObject::connect(m_sceneBattle, SIGNAL(signal_tileDragged(MoM::MoMLocation,MoM::MoMLocation)), this, SLOT(slot_tileDragged(MoM::MoMLocation,MoM::MoMLocation)));

    // Connect timers
    QObject::connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(slot_timerAnimations()));

    // Start timer
    m_timer->start(250);
}

DialogMap::~DialogMap()
{
    preFinalize();

    delete ui;
    delete m_sceneMyrror;
    delete m_sceneArcanus;
}

void DialogMap::addBattleUnitSubtree(QTreeWidget* treeWidget, Battle_Unit* battleUnit)
{
    assert(0 != battleUnit);
    int battleUnitNr = (int)(battleUnit - m_game->getBattleUnit(0));
    Unit* unit = m_game->getUnit(battleUnit->m_unitNr);
    if (0 == unit)
        return;
    QString text = QString("%0").arg(prettyQStr(unit->m_Unit_Type));
    if (battleUnit->m_Weapon_Type_Plus_1 > 1)
    {
        text += ", " + prettyQStr((eWeaponType)(battleUnit->m_Weapon_Type_Plus_1 - 1));
    }
    QMoMTreeItemWidgetBase* qtreeUnit = new QMoMTreeItemWidgetBase(m_game,
                                 QString("battle[%0] unit[%1]").arg(battleUnitNr).arg(battleUnit->m_unitNr),
                                 text);
    treeWidget->addTopLevelItem(qtreeUnit);

    qtreeUnit->addChild(new EnumTreeItem<eUnit_Type>(m_game, "Unit Type", &unit->m_Unit_Type, eUnit_Type_MAX));
    qtreeUnit->addChild(new EnumTreeItem<ePlayer>(m_game, "Owner", &battleUnit->m_Owner, ePlayer_MAX));
    qtreeUnit->addChild(new EnumTreeItem<eBattleUnitActive>(m_game, "Active", &battleUnit->m_Active, eBattleUnitActive_MAX));
    qtreeUnit->addChild(new EnumTreeItem<eBattleUnitTactic>(m_game, "Status", &battleUnit->m_Tactic, eBattleUnitTactic_MAX));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "TargetID", &battleUnit->m_Target_BattleUnitID));
    qtreeUnit->addChild(new NumberTreeItem<int16_t>(m_game, "XPosHeaded", &battleUnit->m_xPosHeaded));
    qtreeUnit->addChild(new NumberTreeItem<int16_t>(m_game, "YPosHeaded", &battleUnit->m_yPosHeaded));
    qtreeUnit->addChild(new NumberTreeItem<uint8_t>(m_game, "HalfMovesLeft", &battleUnit->m_MoveHalves));
    qtreeUnit->addChild(new NumberTreeItem<uint8_t>(m_game, "HP figure", &battleUnit->m_Hitpoints_per_Figure));
    qtreeUnit->addChild(new NumberTreeItem<uint8_t>(m_game, "CurFigures", &battleUnit->m_Current_figures));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "TopFigDamage", &battleUnit->m_top_figure_damage));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Suppression", &battleUnit->m_Suppression));
    qtreeUnit->addChild(new NumberTreeItem<uint8_t>(m_game, "WeaponType", &battleUnit->m_Weapon_Type_Plus_1));
    if (battleUnit->m_Current_mana > 0)
    {
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Mana", &battleUnit->m_Current_mana));
    }
    if (battleUnit->m_Gaze_Modifier != 0)
    {
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Gaze/Poison", &battleUnit->m_Gaze_Modifier));
    }
}

void DialogMap::addCitySubtree(QTreeWidget *treeWidget, MoMTerrain &momTerrain)
{
    MoM::City* city = momTerrain.getCity();
    if (0 != city)
    {
        int cityNr = (int)(city - m_game->getCity(0));
        Fortress* fortresses = m_game->getFortresses();
        Fortress* fortress = 0;
        if ((0 != fortresses) && equalLocation(fortresses[city->m_Owner], momTerrain.getLocation()))
        {
            fortress = &fortresses[city->m_Owner];
        }

        QString text = QString("%0 \"%1\"").arg(prettyQStr(city->m_Race)).arg(city->m_City_Name);
        if (0 != fortress)
        {
            text += ", fortress";
        }
        QMoMTreeItemWidgetBase* qtreeItem = new QMoMTreeItemWidgetBase(m_game,
            QString("City[%0]").arg(cityNr),
            text);
        treeWidget->addTopLevelItem(qtreeItem);

//        ptree->appendChild("m_City_Name", new QMoMTreeItem<char[14]>(rhs->m_City_Name));
        qtreeItem->addChild(new EnumTreeItem<eRace>(m_game, "Race", &city->m_Race, (eRace)gMAX_RACES));
        qtreeItem->addChild(new EnumTreeItem<ePlayer>(m_game, "Owner", &city->m_Owner, ePlayer_MAX));
        qtreeItem->addChild(new NumberTreeItem<int8_t>(m_game, "Population", &city->m_Population));
        qtreeItem->addChild(new EnumTreeItem<eCity_Size>(m_game, "Size", &city->m_Size, eCity_Size_MAX));

//        ptree->appendChild("m_Number_of_farmers_allocated", new QMoMTreeItem<uint8_t>(&rhs->m_Number_of_farmers_allocated));
//        ptree->appendChild("m_PopulationDekaPop", new QMoMTreeItem<uint8_t>(&rhs->m_PopulationDekaPop));
//        ptree->appendChild("m_Player_as_bitmask_GUESS", new QMoMTreeItem<uint8_t>(&rhs->m_Player_as_bitmask_GUESS));
        qtreeItem->addChild(new EnumTreeItem<eProducing>(m_game, "Producing", &city->m_Producing, eProducing_MAX));
//        ptree->appendTree(constructTreeItem(&rhs->m_Building_Status, "m_Building_Status"), "");
//        ptree->appendTree(constructTreeItem(&rhs->m_City_Enchantments, "m_City_Enchantments"), "");
//        ptree->appendChild("m_Nightshade", new QMoMTreeItem<eYesNo8>(&rhs->m_Nightshade));
//        ptree->appendChild("m_Hammers", new QMoMTreeItem<uint8_t>(&rhs->m_Hammers));
        qtreeItem->addChild(new NumberTreeItem<int16_t>(m_game, "HammersAccumulated", &city->m_HammersAccumulated));
//        ptree->appendChild("m_Coins", new QMoMTreeItem<uint8_t>(&rhs->m_Coins));
//        ptree->appendChild("m_Maintenance", new QMoMTreeItem<uint8_t>(&rhs->m_Maintenance));
//        ptree->appendChild("m_Mana_cr", new QMoMTreeItem<uint8_t>(&rhs->m_Mana_cr));
//        ptree->appendChild("m_Research", new QMoMTreeItem<uint8_t>(&rhs->m_Research));
//        ptree->appendChild("m_Food_Produced", new QMoMTreeItem<uint8_t>(&rhs->m_Food_Produced));
//        ptree->appendChild("m_Road_Connection_GUESS", new QMoMTreeItem<int8_t>(&rhs->m_Road_Connection_GUESS));
    }
}

void DialogMap::addLairSubtree(QTreeWidget *treeWidget, MoMTerrain &momTerrain)
{
    MoM::Tower_Node_Lair* lair = momTerrain.getLair();
    if (0 != lair)
    {
        int lairNr = (int)(lair - m_game->getLair(0));
        MoM::Node_Attr* nodeAttr = MoM::MoMController(m_game.data()).findNodeAttrAtLocation(MoM::MoMLocation(*lair));

        QString text = QString("%0").arg(prettyQStr(lair->m_Type));
        if ((0 != nodeAttr) && (MoM::PLAYER_Dismissed_Deceased != nodeAttr->m_Owner))
        {
            text += QString(",player %0").arg(prettyQStr(nodeAttr->m_Owner));
        }
        QMoMTreeItemWidgetBase* qtreeItem = new QMoMTreeItemWidgetBase(m_game,
            QString("Lair[%0]").arg(lairNr),
            text);
        treeWidget->addTopLevelItem(qtreeItem);

        qtreeItem->addChild(new EnumTreeItem<eTower_Node_Lair_Status>(m_game, "Status", &lair->m_Status, eTower_Node_Lair_Status_MAX));
        qtreeItem->addChild(new EnumTreeItem<eTower_Node_Lair_Type>(m_game, "Type", &lair->m_Type, eTower_Node_Lair_Type_MAX));

        qtreeItem->addChild(new EnumTreeItem<eUnit_Type>(m_game, "Inhabitant1", &lair->m_Inhabitant1.m_Inhabitant, eUnit_Type_MAX));
        qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Initial1", (uint16_t*)&lair->m_Inhabitant1, 0xF000));
        qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Remaining1", (uint16_t*)&lair->m_Inhabitant1, 0x0F00));
        qtreeItem->addChild(new NumberTreeItem<uint8_t>(m_game, "Aware1", (uint8_t*)&lair->m_Flags, 0x02));
        if ((eUnit_Type)0 != lair->m_Inhabitant2.m_Inhabitant)
        {
            qtreeItem->addChild(new EnumTreeItem<eUnit_Type>(m_game, "Inhabitant2", &lair->m_Inhabitant2.m_Inhabitant, eUnit_Type_MAX));
            qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Initial2", (uint16_t*)&lair->m_Inhabitant2, 0xF000));
            qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Remaining2", (uint16_t*)&lair->m_Inhabitant2, 0x0F00));
            qtreeItem->addChild(new NumberTreeItem<uint8_t>(m_game, "Aware2", (uint8_t*)&lair->m_Flags, 0x04));
        }
        if (0 != lair->m_Reward_Gold)
        {
            qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Reward Gold", &lair->m_Reward_Gold));
        }
        if (0 != lair->m_Reward_Mana)
        {
            qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, "Reward Mana", &lair->m_Reward_Mana));
        }
        qtreeItem->addChild(new EnumTreeItem<eReward_Specials>(m_game, "Reward Specials", &lair->m_Reward_Specials, eReward_Specials_MAX));
        qtreeItem->addChild(new NumberTreeItem<uint8_t>(m_game, "PrisonerPresent", (uint8_t*)&lair->m_Flags, 0x01));
        for (unsigned i = 0; (i < lair->m_Item_Rewards) && (i < 3); ++i)
        {
            if (lair->m_Item_Value[i] != 0)
            {
                qtreeItem->addChild(new NumberTreeItem<uint16_t>(m_game, QString("Item Value[%0]").arg(i), &lair->m_Item_Value[i]));
            }
        }

        if (0 != nodeAttr)
        {
            qtreeItem->addChild(new EnumTreeItem<ePlayer>(m_game, "Owner", &nodeAttr->m_Owner, ePlayer_MAX));
            qtreeItem->addChild(new NumberTreeItem<int8_t>(m_game, "Power node", &nodeAttr->m_Power_Node));
        }
    }
}

void DialogMap::addTerrainSubtree(QTreeWidget* treeWidget, MoMTerrain& momTerrain)
{
    const MoMLocation& loc = momTerrain.getLocation();

    treeWidget->addTopLevelItem(new QMoMTreeItemWidgetBase(m_game,
        "Location",
        QString("%0:(%1,%2)").arg(prettyQStr(loc.m_Plane)).arg(loc.m_XPos).arg(loc.m_YPos)));

    QMoMTreeItemWidgetBase* qtreeTerrain = new QMoMTreeItemWidgetBase(m_game, "Terrain", "");
    ui->treeWidget_Tile->addTopLevelItem(qtreeTerrain);

    if (MoMLocation::MAP_overland == loc.m_Map)
    {
        if (0 != m_game->getTerrainType(loc))
        {
            MoM::eTerrainType terrainType = *m_game->getTerrainType(loc);
            QString text = QString("%0 (%1)")
                    .arg(prettyQStr(MoM::MoMTerrain::getTerrainCategory(terrainType)))
                    .arg((int)terrainType);
            if ((0 != m_game->getTerrainBonus(loc)) && (MoM::DEPOSIT_no_deposit != *m_game->getTerrainBonus(loc)))
            {
                text += ", " + prettyQStr(*m_game->getTerrainBonus(loc));
            }
            qtreeTerrain->setText(1, text);
        }

        if (0 != m_game->getTerrainType(loc))
        {
            qtreeTerrain->addChild(new EnumTreeItem<MoM::eTerrainType>(m_game, "TerrainType", m_game->getTerrainType(loc), MoM::eTerrainType_MAX));
        }
        if (0 != m_game->getTerrainBonus(loc))
        {
            qtreeTerrain->addChild(new EnumTreeItem<MoM::eTerrainBonusDeposit>(m_game, "TerrainBonus", m_game->getTerrainBonus(loc), MoM::eTerrainBonusDeposit_MAX));
        }
        if (0 != m_game->getTerrainChange(loc))
        {
            qtreeTerrain->addChild(new BitmaskTreeItem<uint8_t, MoM::eTerrainChange>(m_game, "TerrainChange", (uint8_t*)m_game->getTerrainChange(loc), (MoM::eTerrainChange)0, MoM::eTerrainChange_MAX));
        }
        if (0 != m_game->getTerrainExplored(loc))
        {
            qtreeTerrain->addChild(new NumberTreeItem<uint8_t>(m_game, "Explored", m_game->getTerrainExplored(loc)));
        }
        if (0 != m_game->getTerrainLandMassID(loc))
        {
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "LandMassID", m_game->getTerrainLandMassID(loc)));
        }
//        Map_Movement* terrainMovement = m_game->getTerrain_Movements();
        int8_t* terrainMovementUnused = m_game->getTerrainMovement(loc, MoM::MOVEMENT_Unused);
//        Map_Movement* terrainMovementCopy = m_game->getTerrain_Movements_copy();
        //qDebug("TerrMove %p TerrMoveUnused %p TerrMoveCopy %p", terrainMovement, terrainMovementUnused, terrainMovementCopy);
        if (0 == terrainMovementUnused)
        {
            qtreeTerrain->addChild(new QMoMTreeItemWidgetBase(m_game, "Moves", "(Not accessible->no road effect)"));
        }
        else
        {
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveUnused", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Unused)));
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveWalk", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Walking)));
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveForester", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Forester)));
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveMountaineer", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Mountaineer)));
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveSwimming", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Swimming)));
            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveSailing", m_game->getTerrainMovement(loc, MoM::MOVEMENT_Sailing)));
        }
//        if (0 == terrainMovementCopy)
//        {
//            qtreeTerrain->addChild(new QMoMTreeItemWidgetBase(m_game, "MovesCopy", "(Not accessible)"));
//        }
//        else
//        {
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCUnused", &terrainMovementCopy[loc.m_Plane].m_Unused_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCWalk", &terrainMovementCopy[loc.m_Plane].m_Walking_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCForester", &terrainMovementCopy[loc.m_Plane].m_Forester_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCMountaineer", &terrainMovementCopy[loc.m_Plane].m_Mountaineer_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCSwimming", &terrainMovementCopy[loc.m_Plane].m_Swimming_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//            qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "MoveCSailing", &terrainMovementCopy[loc.m_Plane].m_Sailing_Row[loc.m_YPos].m_Moves[loc.m_XPos]));
//        }

    }
    else if (0 != m_game->getBattlefield())
    {
        Battlefield* battlefield = m_game->getBattlefield();
        int index = loc.m_YPos * gMAX_BATTLE_COLS + loc.m_XPos;

        qtreeTerrain->addChild(new EnumTreeItem<MoM::eTerrainBattle>(m_game, "TerrainBattle", &battlefield->m_Terrain[index], MoM::eTerrainBattle_MAX));
        qtreeTerrain->addChild(new NumberTreeItem<uint8_t>(m_game, "TerrainGroup", &battlefield->m_TerrainGroupType[index]));
        qtreeTerrain->addChild(new NumberTreeItem<uint8_t>(m_game, "Road", &battlefield->m_Road[index]));
        qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "WalkMove", &battlefield->m_Movement_walking[index]));
        qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "Merge/Tport/Fly", &battlefield->m_Movement_merging_teleporting_fly[index]));
        qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "WalkSimilar", &battlefield->m_Movement_similar_to_walking[index]));
        qtreeTerrain->addChild(new NumberTreeItem<int8_t>(m_game, "SailMove", &battlefield->m_Movement_sailing[index]));
        if (battlefield->m_Mud[index] != 0)
        {
            qtreeTerrain->addChild(new NumberTreeItem<uint8_t>(m_game, "Mud", &battlefield->m_Mud[index]));
        }
    }
}

void DialogMap::addUnitSubtree(QTreeWidgetItem *treeWidgetItem, Unit* unit)
{
    assert(0 != unit);
    int unitNr = (int)(unit - m_game->getUnit(0));
    QString text = QString("%0").arg(prettyQStr(unit->m_Unit_Type));
    if (0 != unit->m_Weapon_Mutation.s.Weapon_Type)
    {
        text += ", " + prettyQStr((eWeaponType)unit->m_Weapon_Mutation.s.Weapon_Type);
    }
    QMoMTreeItemWidgetBase* qtreeUnit = new QMoMTreeItemWidgetBase(m_game,
        QString("unit[%0]").arg(unitNr),
        text);
    treeWidgetItem->addChild(qtreeUnit);

    qtreeUnit->addChild(new EnumTreeItem<eUnit_Type>(m_game, "Unit Type", &unit->m_Unit_Type, eUnit_Type_MAX));
    qtreeUnit->addChild(new EnumTreeItem<ePlayer>(m_game, "Owner", &unit->m_Owner, ePlayer_MAX));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "HalfMovesTotal", &unit->m_Moves_Total));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "HalfMovesLeft", &unit->m_Moves_Left));
    qtreeUnit->addChild(new EnumTreeItem<eUnit_Active>(m_game, "Active", &unit->m_Active, eUnit_Active_MAX));
    qtreeUnit->addChild(new EnumTreeItem<eUnit_Status8>(m_game, "Status", &unit->m_Status, eUnit_Status8_MAX));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "destXPos", &unit->m_XPos_of_destination));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "destYPos", &unit->m_YPos_of_destination));
    qtreeUnit->addChild(new EnumTreeItem<eLevel>(m_game, "Level", &unit->m_Level, eLevel_MAX));
    qtreeUnit->addChild(new NumberTreeItem<int16_t>(m_game, "Experience", &unit->m_Experience));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Damage", &unit->m_Damage));
    qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Grouping", &unit->m_Grouping));
    if (1 != unit->m_Scouting)
    {
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "Scouting", &unit->m_Scouting));
    }
//    ptree->appendTree(constructTreeItem(&rhs->m_Weapon_Mutation, "m_Weapon_Mutation"), "");
    qtreeUnit->addChild(new BitmaskTreeItem<uint32_t, eUnitEnchantment>(m_game, "Enchantments", &unit->m_Unit_Enchantment.bits, (eUnitEnchantment)0, eUnitEnchantment_MAX));
    if (unit->m_Road_Building_left_to_complete > 0)
    {
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "roadLeft", &unit->m_Road_Building_left_to_complete));
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "fromXPos", &unit->m_XPos_Road_Building_from));
        qtreeUnit->addChild(new NumberTreeItem<int8_t>(m_game, "fromYPos", &unit->m_YPos_Road_Building_from));
    }
}

void DialogMap::updateGraying()
{
    ui->pushButton_RestoreBookmark->setEnabled((0 != m_game) && (ui->graphicsView->scene() == m_sceneBattle));
    ui->pushButton_SaveBookmark->setEnabled((0 != m_game) && (ui->graphicsView->scene() == m_sceneBattle));
}

void DialogMap::on_comboBox_Plane_currentIndexChanged(int index)
{
    switch (index)
    {
    case 1:
        ui->graphicsView->setScene(m_sceneArcanus);
        break;
    case 2:
        ui->graphicsView->setScene(m_sceneMyrror);
        break;
    case 3:
        ui->graphicsView->setScene(m_sceneBattle);
        break;
    default:
        ui->graphicsView->setScene(0);
        break;
    }
    updateGraying();
}

void DialogMap::on_treeWidget_Tile_customContextMenuRequested(const QPoint &pos)
{
    //qDebug() << "on_treeWidget_Tile_customContextMenuRequested" << pos;
    QTreeWidgetItem* pItem = ui->treeWidget_Tile->currentItem();

    QMenu contextMenu;
    QAction* action = contextMenu.addAction("Address calculator");
    action->connect(action, SIGNAL(triggered()), this, SLOT(slot_addressCalculator()));

    QMoMTreeItemWidgetBase* pMoMItem = dynamic_cast<QMoMTreeItemWidgetBase*>(pItem);
    if (0 != pMoMItem)
    {
        QList<QAction*> actions = pMoMItem->requestActions(&contextMenu);
        if (!actions.isEmpty())
        {
            contextMenu.addSeparator();
        }
        contextMenu.addActions(actions);
        foreach(QAction* action, actions)
        {
            connect(action, SIGNAL(triggered()), this, SLOT(slot_itemAction()));
        }
    }

    contextMenu.exec(ui->treeWidget_Tile->mapToGlobal(pos));
    updateGraying();
}

void DialogMap::on_verticalSlider_Zoom_valueChanged(int value)
{
    double scale = 1.0 + value / 100.0;
    if (value < 0)
    {
        scale = std::pow(2.0, value / 100.0);
    }

    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scale, scale);
}

void DialogMap::slot_addressCalculator()
{
    DialogCalculatorAddress* dialog = new DialogCalculatorAddress(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
//    QObject::connect(this, SIGNAL(signal_addressChanged(const void*)), dialog, SLOT(slot_addressChanged(const void*)));
    dialog->show();

    QTreeWidgetItem* pItem = ui->treeWidget_Tile->currentItem();
    QMoMTreeItemWidgetBase* pMoMItem = dynamic_cast<QMoMTreeItemWidgetBase*>(pItem);
    if (0 != pMoMItem)
    {
        dialog->slot_addressChanged(pMoMItem->getMoMPointer());
    }
}

void DialogMap::slot_gameChanged(const QMoMGamePtr& game)
{
    m_game = game;

    slot_gameUpdated();
}

void DialogMap::slot_gameUpdated()
{
    // Reset game
    m_sceneArcanus->clear();
    m_sceneMyrror->clear();
    m_sceneBattle->clear();

    if (m_game.isNull())
        return;

    QMoMMapScene* scene[MoM::ePlane_MAX] = { m_sceneArcanus, m_sceneMyrror };

    // Show overland terrain
    for (MoM::ePlane plane = (MoM::ePlane)0; MoM::toUInt(plane) < MoM::ePlane_MAX; MoM::inc(plane))
    {
        for (int y = 0; y < (int)MoM::gMAX_MAP_ROWS; ++y)
        {
            for (int x = 0; x < (int)MoM::gMAX_MAP_COLS; ++x)
            {
                MoM::QMoMMapTile* mapTile = new MoM::QMoMMapTile(MoMLocation(x, y, plane, MoMLocation::MAP_overland));
                if (ui->checkBox_Terrain->isChecked())
                {
                    mapTile->setTerrainType(m_game->getTerrainType(plane, x, y));
                }
                if (ui->checkBox_TerrainBonuses->isChecked())
                {
                    mapTile->setTerrainBonus(m_game->getTerrainBonus(plane, x, y));
                }
                if (ui->checkBox_TerrainChanges->isChecked())
                {
                    mapTile->setTerrainChange(m_game->getTerrainChange(plane, x, y));
                }
                if (ui->checkBox_ExploredOnly->isChecked())
                {
                    mapTile->setTerrainExplored(m_game->getTerrainExplored(plane, x, y));
                }
                scene[plane]->addItemAtLocation(mapTile, mapTile->getLocation());
            }
        }
    }


    // Update items that are not fixed

    // Show units
    for (int unitNr = 0; (unitNr < m_game->getNrUnits()) && (MoM::toUInt(unitNr) < MoM::gMAX_UNITS); ++unitNr)
    {
        MoM::Unit* unit = m_game->getUnit(unitNr);
        if ((0 != unit)
                && MoM::inRange(unit->m_Unit_Type, MoM::eUnit_Type_MAX)
                && MoM::inRange(unit->m_Plane, MoM::ePlane_MAX))
        {
            if (!ui->checkBox_YourUnits->isChecked() && (unit->m_Owner == MoM::PLAYER_YOU))
                continue;
            if (!ui->checkBox_EnemyUnits->isChecked() && (unit->m_Owner != MoM::PLAYER_YOU))
                continue;

            QMoMUnitPtr momUnit(new MoMUnit(m_game.data()));
            momUnit->changeUnit(unit);

            MoM::QMoMUnitTile* unitTile = new MoM::QMoMUnitTile;
            unitTile->setGame(m_game);
            unitTile->setUnit(momUnit);

            scene[unit->m_Plane]->addItemAtLocation(unitTile, MoMLocation(*unit, MoMLocation::MAP_overland));
            unitTile->setToolTip(momUnit->getDisplayName().c_str());
        }
    }

    // Show lairs
    if (ui->checkBox_Lairs->isChecked())
    {
        QRectF rectfTile = MoM::QMoMMapTile(MoMLocation()).boundingRect();
        for (int lairNr = 0; (MoM::toUInt(lairNr) < MoM::gMAX_NODES_LAIRS_TOWERS); ++lairNr)
        {
            MoM::Tower_Node_Lair* lair = m_game->getLair(lairNr);
            if (0 == lair)
                continue;
            QPixmap pixmapLair = MoM::QMoMResources::instance().getPixmap(lair->m_Type, 1, true);
            if (MoM::inRange(lair->m_Type, MoM::eTower_Node_Lair_Type_MAX)
                    && MoM::inRange(lair->m_Plane, MoM::ePlane_MAX)
                    && !pixmapLair.isNull())
            {
                QGraphicsItem* mapLairItem = scene[lair->m_Plane]->addPixmapAtLocation(pixmapLair, MoMLocation(*lair, MoMLocation::MAP_overland));
                mapLairItem->setScale(rectfTile.width() / pixmapLair.width());
                mapLairItem->setToolTip(prettyQStr(lair->m_Type));
            }
        }
    }

    // Show cities
    if (ui->checkBox_Cities->isChecked())
    {
        QRectF rectfTile = MoM::QMoMMapTile(MoMLocation()).boundingRect();
        for (int cityNr = 0; (cityNr < m_game->getNrCities()) && (MoM::toUInt(cityNr) < MoM::gMAX_CITIES); ++cityNr)
        {
            MoM::City* city = m_game->getCity(cityNr);
            Wizard* wizard = m_game->getWizard(city->m_Owner);
            if ((0 == city) || (0 == wizard))
                continue;
            QPixmap pixmapCity = MoM::QMoMResources::instance().getPixmap(city->m_Size, 1, wizard->m_BannerColor);
            if (MoM::inRange(city->m_Plane, MoM::ePlane_MAX)
                    && !pixmapCity.isNull())
            {
                QGraphicsPixmapItem* mapCityItem = scene[city->m_Plane]->addPixmapAtLocation(pixmapCity, MoMLocation(*city, MoMLocation::MAP_overland));
                // city pixmap is about twice as big as a regular tile
                QPointF offset = mapCityItem->offset();
                offset.rx() += (rectfTile.width() - pixmapCity.width()) / 2;
                offset.ry() += (rectfTile.height() - pixmapCity.height()) / 2;
                mapCityItem->setOffset(offset);
                mapCityItem->setToolTip(QString("%0 \"%1\"").arg(prettyQStr(city->m_Race)).arg(city->m_City_Name));
            }
        }
    }

    for (int nodeNr = 0; toUInt(nodeNr) < gMAX_NODES; ++nodeNr)
    {
        Node_Attr* nodeAttr = m_game->getNodeAttr(nodeNr);
        if (0 == nodeAttr)
            break;
        eBannerColor bannerColor = BANNER_Brown;
        QMoMAnimation animation;
        Wizard* wizard = m_game->getWizard(nodeAttr->m_Owner);
        if (0 != wizard)
        {
            bannerColor = wizard->m_BannerColor;
            animation = MoM::QMoMResources::instance().getAnimation(LBXRecordID("MAPBACK", 63 + bannerColor));
        }
        else
        {
            switch (nodeAttr->m_Node_Type)
            {
            case NODETYPE_Sorcery:  bannerColor = BANNER_Blue; break;
            case NODETYPE_Nature:   bannerColor = BANNER_Green; break;
            case NODETYPE_Chaos:    bannerColor = BANNER_Red; break;
            case eNode_Type_MAX:    break;
            }
            animation = MoM::QMoMResources::instance().getAnimation(LBXRecordID("MAPBACK", 63 + bannerColor));
            if (!animation.empty())
            {
                animation.resize(1);
            }
        }
        for (int auraNr = 0; auraNr < nodeAttr->m_Power_Node; ++auraNr)
        {
            QMoMAnimationTile* auraTile = new QMoMAnimationTile(animation);
            scene[nodeAttr->m_Plane]->addItemAtLocation(auraTile,
                                                        MoMLocation(nodeAttr->m_XPos_Mana[auraNr], nodeAttr->m_YPos_Mana[auraNr], nodeAttr->m_Plane, MoMLocation::MAP_overland));
            auraTile->setToolTip(prettyQStr(nodeAttr->m_Node_Type));
        }
    }

    Battlefield* battlefield = m_game->getBattlefield();
    MoM::Battle_Unit* battleUnits = m_game->getBattleUnit(0);
    if ((0 != battlefield) && (0 != battleUnits))
    {
        // Update plane of battlefield
        m_sceneBattle->setPlane((ePlane)battlefield->m_Plane);

        // Show battle terrain
        for (int y = 0; y < (int)MoM::gMAX_BATTLE_ROWS; ++y)
        {
            for (int x = 0; x < (int)MoM::gMAX_BATTLE_COLS; ++x)
            {
                MoM::QMoMMapTile* mapTile = new MoM::QMoMMapTile(MoMLocation(x, y, (ePlane)battlefield->m_Plane, MoMLocation::MAP_battle));
                if (ui->checkBox_Terrain->isChecked())
                {
                    int index = x + y * gMAX_BATTLE_COLS;
                    mapTile->setTerrainBattle(&battlefield->m_Terrain[index]);
                }
                m_sceneBattle->addItemAtLocation(mapTile, mapTile->getLocation());
            }
        }

        // Show central structure
        QPixmap pixmapStructure = MoM::QMoMResources::instance().getPixmap(battlefield->m_Central_structure);
        QGraphicsPixmapItem* itemStructure = m_sceneBattle->addPixmapAtLocation(pixmapStructure, MoMLocation(6, 11, (ePlane)battlefield->m_Plane, MoMLocation::MAP_battle));
        QPointF offset = itemStructure->offset();
        if (MoM::CENTRALSTRUCTURE_city_grid == battlefield->m_Central_structure)
        {
            offset.ry() += 2 * 16;
        }
        else if ((MoM::CENTRALSTRUCTURE_sorcery_node == battlefield->m_Central_structure)
                 || (MoM::CENTRALSTRUCTURE_nature_node == battlefield->m_Central_structure))
        {
            offset.ry() += 16;
        }
        else
        {
            int zvalue = m_sceneBattle->convertScenePosToZValue(itemStructure->pos());
            itemStructure->setZValue(zvalue);
        }
        itemStructure->setOffset(offset);

        // Show battle units
        int nrBattleUnits = m_game->getNrBattleUnits();
        for (int battleUnitNr = 0; battleUnitNr < nrBattleUnits; ++battleUnitNr)
        {
            MoM::Battle_Unit* battleUnit = m_game->getBattleUnit(battleUnitNr);
            if (0 == battleUnit)
                break;
            MoMLocation loc(battleUnit->m_xPos, battleUnit->m_yPos, (ePlane)battlefield->m_Plane, MoMLocation::MAP_battle);

            QMoMUnitPtr momUnit(new MoMUnit(m_game.data()));
            momUnit->changeUnit(battleUnit);

            MoM::QMoMUnitTile* unitTile = new MoM::QMoMUnitTile(true);
            unitTile->setGame(m_game);
            unitTile->setUnit(momUnit);

            m_sceneBattle->addItemAtLocation(unitTile, loc);
            int zvalue = m_sceneBattle->convertScenePosToZValue(unitTile->pos());
            unitTile->setZValue(zvalue);
            unitTile->setToolTip(momUnit->getDisplayName().c_str());

            QPointF pos;
            m_sceneBattle->convertLocationToScenePos(loc, pos);
            pos.rx() -= 16;
            pos.ry() -= 30;
            QGraphicsSimpleTextItem* textItem = m_sceneBattle->addSimpleText(QString("%0").arg(battleUnitNr));
            textItem->setFont(QMoMResources::g_FontSmall);
            textItem->setPos(pos);

            QString text = prettyQStr(battleUnit->m_Tactic);
            textItem = m_sceneBattle->addSimpleText(QString("%0").arg(text[0]));
            textItem->setFont(QMoMResources::g_FontSmall);
            pos.rx() += textItem->boundingRect().width() + 4;
            textItem->setPos(pos);

            if (MoM::toUInt(battleUnit->m_Target_BattleUnitID) < (unsigned)nrBattleUnits)
            {
                MoM::Battle_Unit* targetUnit = &battleUnits[battleUnit->m_Target_BattleUnitID];
                MoMLocation locTarget(targetUnit->m_xPos, targetUnit->m_yPos, (ePlane)battlefield->m_Plane, MoMLocation::MAP_battle);
                QPointF ptBegin;
                QPointF ptEnd;
                m_sceneBattle->convertLocationToScenePos(loc, ptBegin);
                m_sceneBattle->convertLocationToScenePos(locTarget, ptEnd);
                ptBegin.ry() -= 8;
                ptEnd.ry() -= 8;
                m_sceneBattle->addLine(QLineF(ptBegin, ptEnd), QPen(Qt::darkRed));
            }
        }

        // Central structure at (6,11)
        // City walls between (5,10) and (8,13)
        // Gate at (8,12)

        static const MoMLocation wallLocations[14] =
        {
            MoMLocation(5, 10),
            MoMLocation(5, 11),
            MoMLocation(5, 12),
            MoMLocation(5, 13),
            MoMLocation(6, 10),
            MoMLocation(7, 10),
            MoMLocation(8, 10),
            MoMLocation(6, 13),
            MoMLocation(7, 13),
            MoMLocation(8, 13),
            MoMLocation(8, 11),
            MoMLocation(8, 12), // Gate

            MoMLocation(5, 13), // Extra for fire/darkness
            MoMLocation(8, 10), // Extra for fire/darkness
        };

        if (battlefield->m_City_Walls)
        {
            for (int i = 0; i < 12; ++i)
            {
                MoM::eCityWall wall = (MoM::eCityWall)i;
                MoMLocation loc = wallLocations[i];
                int wholeIndex = (loc.m_XPos - 5) + (loc.m_YPos - 10) * 4;
                if (0 != battlefield->m_Wall_present_4x4[wholeIndex])
                {
                    bool broken = (2 == battlefield->m_Wall_present_4x4[wholeIndex]);
                    QPixmap pixmapWall = MoM::QMoMResources::instance().getPixmap(wall, 1, broken);
                    (void)m_sceneBattle->addPixmapAtLocation(pixmapWall, wallLocations[i]);
                }
            }
        }
        if (battlefield->m_Wall_of_Fire)
        {
            for (int i = 0; i < 14; ++i)
            {
                MoM::eCityWall wall = (MoM::eCityWall)(i + CITYWALL_walloffire_first);
                QPixmap pixmapWall = MoM::QMoMResources::instance().getPixmap(wall);
                (void)m_sceneBattle->addPixmapAtLocation(pixmapWall, wallLocations[i]);
            }
        }
        if (battlefield->m_Wall_of_Darkness)
        {
            for (int i = 0; i < 14; ++i)
            {
                MoM::eCityWall wall = (MoM::eCityWall)(i + CITYWALL_wallofdarkness_first);
                QPixmap pixmapWall = MoM::QMoMResources::instance().getPixmap(wall);
                (void)m_sceneBattle->addPixmapAtLocation(pixmapWall, wallLocations[i]);
            }
        }

        // Show battlefield menubar (numbers from IDA)
        m_sceneBattle->addRect(m_sceneBattle->sceneRect(), QPen(Qt::black));
        QPixmap pixmapMenubar = MoM::QMoMResources::instance().getPixmap(MoM::LBXRecordID("BACKGRND", 3));
        QGraphicsPixmapItem* itemMenubar = m_sceneBattle->addPixmap(pixmapMenubar);
        itemMenubar->setPos(0, 164);
    }
    updateGraying();
}

void DialogMap::slot_itemAction()
{
    QTreeWidgetItem* pItem = ui->treeWidget_Tile->currentItem();
    QMoMTreeItemWidgetBase* pMoMItem = dynamic_cast<QMoMTreeItemWidgetBase*>(pItem);
    if (0 != pMoMItem)
    {
        pMoMItem->slotAction();
    }
    updateGraying();
}

void DialogMap::slot_tileChanged(const MoM::MoMLocation& loc)
{
    ui->label_Location->setText(QString("Location: (%1,%2)").arg(loc.m_XPos).arg(loc.m_YPos));
}

void DialogMap::slot_tileDragged(const MoMLocation &locFrom, const MoMLocation &locTo)
{
    //qDebug() << QString("slot_tileDragged(%0:(%1,%2) -> (%3,%4)")
    //            .arg(prettyQStr(locFrom.m_Plane)).arg(locFrom.m_XPos).arg(locFrom.m_YPos)
    //            .arg(locTo.m_XPos).arg(locTo.m_YPos);

    MoM::MoMTerrain terrainFrom(m_game.data(), locFrom);
    MoM::MoMTerrain terrainTo(m_game.data(), locTo);

    // TODO: Figure out the action:
    //       1. Move (sub) stack of selected units
    //       2. Attack with (sub) stack of selected units
    //       3. Copy or move terrain feature
    //       4. Copy or move terrain tile

    if (locFrom.m_Map == MoMLocation::MAP_overland)
    {
        std::vector<int> unitsFrom = terrainFrom.getUnits();
        std::vector<int> unitsTo = terrainTo.getUnits();

        //qDebug() << QString("%0 units versus %1 units")
        //            .arg(unitsFrom.size()).arg(unitsTo.size());

        if ((unitsFrom.size() > 0) && (unitsTo.size() <= 0))
        {
            // Move stack
            for (size_t i = 0; i < unitsFrom.size(); ++i)
            {
                Unit* unit = m_game->getUnit(unitsFrom[i]);
                if (0 != unit)
                {
                    Unit newUnit = *unit;
                    newUnit.m_XPos = locTo.m_XPos;
                    newUnit.m_YPos = locTo.m_YPos;
                    newUnit.m_Plane = locTo.m_Plane;
                    (void)m_game->commitData(unit, &newUnit, sizeof(newUnit));
                }
            }
            slot_gameUpdated();
        }
        else if ((unitsFrom.size() > 0) && (unitsTo.size() > 0))
        {
            // Simulate combat
            MoMCombat::StackUnits attackers(unitsFrom.size());
            MoMCombat::StackUnits defenders(unitsTo.size());

            for (size_t i = 0; i < attackers.size(); ++i)
            {
                attackers[i] = CombatUnit(m_game.data());
                CombatUnit& attacker = attackers[i];
                attacker.changeUnit(m_game->getUnit(unitsFrom[i]));
            }
            for (size_t i = 0; i < defenders.size(); ++i)
            {
                defenders[i] = CombatUnit(m_game.data());
                CombatUnit& defender = defenders[i];
                defender.changeUnit(m_game->getUnit(unitsTo[i]));
            }

            MoMCombat combat;
            int result = 0;
            std::string ostr = combat.full_combat(attackers, defenders, result);

            std::cout << ostr << std::endl;
            (void)QMessageBox::information(this,
                tr("Battle simulation"),
                ostr.c_str());
        }
    }
    else
    {
        Battle_Unit* unitFrom = terrainFrom.getBattleUnit();
        Battle_Unit* unitTo = terrainTo.getBattleUnit();

        if ((0 != unitFrom) && ((0 == unitTo) || (unitTo->m_Active != BATTLEUNITACTIVE_alive)))
        {
            // Move unit
            Battle_Unit newUnit = *unitFrom;
            newUnit.m_xPos = locTo.m_XPos;
            newUnit.m_yPos = locTo.m_YPos;
            (void)m_game->commitData(unitFrom, &newUnit, sizeof(newUnit));
            slot_gameUpdated();
        }
        else if ((0 != unitFrom) && (unitFrom->m_Active == BATTLEUNITACTIVE_alive)
                 && (0 != unitTo) && (unitTo->m_Active == BATTLEUNITACTIVE_alive))
        {
            // Simulate combat
            CombatUnit attacker(m_game.data());
            CombatUnit defender(m_game.data());
            attacker.changeUnit(unitFrom);
            defender.changeUnit(unitTo);

            MoMCombat combat;
            std::string ostr = combat.combat_round(attacker, defender);
            std::cout << ostr << std::endl;
            (void)QMessageBox::information(this,
                tr("Combat round simulation"),
                ostr.c_str());
        }
    }
    updateGraying();
}

void DialogMap::slot_tileSelected(const MoM::MoMLocation &loc)
{
    //qDebug() << QString("slot_tileSelected(%0:(%1,%2)").arg(prettyQStr(loc.m_Plane)).arg(loc.m_XPos).arg(loc.m_YPos);

    ui->treeWidget_Tile->clear();

    MoM::MoMTerrain momTerrain(m_game.data(), loc);

    addTerrainSubtree(ui->treeWidget_Tile, momTerrain);
    addLairSubtree(ui->treeWidget_Tile, momTerrain);
    addCitySubtree(ui->treeWidget_Tile, momTerrain);

    std::vector<int> units = momTerrain.getUnits();
    if (units.size() > 0)
    {
        QMoMTreeItemWidgetBase* qtreeItem = new QMoMTreeItemWidgetBase(m_game,
            QString("Units"), 
            QString("%0 (player %1)").arg(units.size()).arg(prettyQStr(m_game->getUnit(units[0])->m_Owner)));
        ui->treeWidget_Tile->addTopLevelItem(qtreeItem);

        for (size_t i = 0; i < units.size(); ++i)
        {
            addUnitSubtree(qtreeItem, m_game->getUnit(units[i]));
        }
    }

    if (loc.m_Map == MoMLocation::MAP_battle)
    {
        for (int battleUnitNr = 0; battleUnitNr < m_game->getNrBattleUnits(); ++battleUnitNr)
        {
            Battle_Unit* battleUnit = m_game->getBattleUnit(battleUnitNr);
            if (0 == battleUnit)
                break;
            if ((loc.m_XPos == battleUnit->m_xPos) && (loc.m_YPos == battleUnit->m_yPos))
            {
                addBattleUnitSubtree(ui->treeWidget_Tile, battleUnit);
            }
        }
    }
    updateGraying();
}

void DialogMap::slot_timerAnimations()
{
    if (m_game.isNull() || (0 == m_game->getGameData_WizardsExe()))
        return;
    int unitNrActive = m_game->getGameData_WizardsExe()->m_UnitNr_Active;
    MoM::Unit* unit = m_game->getUnit(unitNrActive);
    if (0 == unit)
        return;

    // TODO
//    QGraphicsItem* qItem = lookup(unit);
//    if (0 != qItem)
//    {
//        qItem->setVisible(!qItem->isVisible());
//    }
}

}

// TODO: Move to dedicated .cpp and .h file
class MoMBookmark
{
public:
    MoMBookmark(MoM::MoMGameBase* game);

    bool save(std::ostream& os);
    bool restore(std::istream& is);

private:
    template<typename T>
    void saveField(const T* t, size_t nitems = 1);

    template<typename T>
    void restoreField(T* t, size_t nitems = 1);

    static const std::string sFILETYPE_BATTLEBOOKMARK;

    MoM::MoMGameBase* m_game;
    std::istream* m_is;
    std::ostream* m_os;
    bool m_ok;
};

const std::string MoMBookmark::sFILETYPE_BATTLEBOOKMARK("BATTLEBOOKMARK\r\n\x1A");

MoMBookmark::MoMBookmark(MoM::MoMGameBase* game) :
    m_game(game),
    m_is(),
    m_os(),
    m_ok()
{
}

template<typename T>
void MoMBookmark::saveField(const T* t, size_t nitems)
{
    if (!m_ok)
        return;
    if ((0 != m_os) && (0 != t))
    {
        m_os->write((const char*)t, nitems * sizeof(T));
        m_ok = m_os->good();
    }
}

template<typename T>
void MoMBookmark::restoreField(T* t, size_t nitems)
{
    if (!m_ok)
        return;
    std::vector<T> newValue(nitems);
    if ((0 != m_is) && (0 != t))
    {
        m_is->read((char*)&newValue[0], nitems * sizeof(T));
        m_ok = m_is->good();
    }
    if (m_ok)
    {
        m_ok = m_game->commitData(t, &newValue[0], nitems * sizeof(T));
    }
}

bool MoMBookmark::save(std::ostream& os)
{
    if (0 == m_game)
        return false;
    m_is = 0;
    m_os = &os;
    m_ok = true;

    MoM::MoMDataSegment* dseg = m_game->getDataSegment();
    assert(0 != dseg);

    // File type marker
    saveField(sFILETYPE_BATTLEBOOKMARK.c_str(), sFILETYPE_BATTLEBOOKMARK.size());

    // Battle turn
    saveField(&dseg->m_Combat_turn);

    // Defending wizard's skill left
    // Defending wizard's mana
    MoM::Wizard* wizard = m_game->getWizard(dseg->m_Defending_wizard);
    if (0 == wizard)
    {
        m_ok = false;
    }
    else
    {
        saveField(&wizard->m_Skill_Left_in_combat);
        saveField(&wizard->m_Mana_Crystals);
    }

    // Attacking wizard's skill left
    // Attacking wizard's mana
    wizard = m_game->getWizard(dseg->m_Attacking_wizard);
    if (0 == wizard)
    {
        m_ok = false;
    }
    else
    {
        saveField(&wizard->m_Skill_Left_in_combat);
        saveField(&wizard->m_Mana_Crystals);
    }

    // Nr of battle units
    // Battle unit data (always 18 units)
    saveField(&dseg->m_Nr_Battle_Units);
    saveField(m_game->getBattleUnit(0), MoM::gMAX_BATTLE_UNITS);

    // Spells cast in battle
    saveField(m_game->getSpells_Cast_in_Battle());

    // Broken wall (16 positions)
    MoM::Battlefield* battlefield = m_game->getBattlefield();
    if (0 == battlefield)
    {
        m_ok = false;
    }
    else
    {
        saveField(battlefield->m_Wall_present_4x4, sizeof(battlefield->m_Wall_present_4x4));
    }

    return m_ok;
}

bool MoMBookmark::restore(std::istream& is)
{
    if (0 == m_game)
        return false;
    m_is = &is;
    m_os = 0;
    m_ok = true;

    MoM::MoMDataSegment* dseg = m_game->getDataSegment();
    assert(0 != dseg);

    // File type marker
    std::string filetype(sFILETYPE_BATTLEBOOKMARK.size(), '\0');
    m_is->read(&filetype[0], filetype.size());
    if (filetype != sFILETYPE_BATTLEBOOKMARK)
    {
        m_ok = false;
    }

    // Battle turn
    restoreField(&dseg->m_Combat_turn);

    // Defending wizard's skill left
    // Defending wizard's mana
    MoM::Wizard* wizard = m_game->getWizard(dseg->m_Defending_wizard);
    if (0 == wizard)
    {
        m_ok = false;
    }
    else
    {
        restoreField(&wizard->m_Skill_Left_in_combat);
        restoreField(&wizard->m_Mana_Crystals);
    }

    // Attacking wizard's skill left
    // Attacking wizard's mana
    wizard = m_game->getWizard(dseg->m_Attacking_wizard);
    if (0 == wizard)
    {
        m_ok = false;
    }
    else
    {
        restoreField(&wizard->m_Skill_Left_in_combat);
        restoreField(&wizard->m_Mana_Crystals);
    }

    // Nr of battle units
    // Battle unit data (always 18 units)
    restoreField(&dseg->m_Nr_Battle_Units);
    restoreField(m_game->getBattleUnit(0), MoM::gMAX_BATTLE_UNITS);

    // Spells cast in battle
    restoreField(m_game->getSpells_Cast_in_Battle());

    // Broken wall (16 positions)
    MoM::Battlefield* battlefield = m_game->getBattlefield();
    if (0 == battlefield)
    {
        m_ok = false;
    }
    else
    {
        restoreField(battlefield->m_Wall_present_4x4, sizeof(battlefield->m_Wall_present_4x4));
    }

    return m_ok;
}

void MoM::DialogMap::on_pushButton_SaveBookmark_clicked()
{
    if ((0 == m_game) || (0 == m_game->getDataSegment()))
        return;

    QFileDialog saveBookMark;

    saveBookMark.setObjectName("saveBookMark");
    saveBookMark.setWindowTitle(tr("Save battle bookmark"));
    saveBookMark.setNameFilter(tr("Battle bookmark files (*.battlebookmark);;All files (*.*)"));
    saveBookMark.setDefaultSuffix("battlebookmark");
    saveBookMark.setAcceptMode(QFileDialog::AcceptSave);
    saveBookMark.setFileMode(QFileDialog::AnyFile);
    saveBookMark.setViewMode(QFileDialog::Detail);
    saveBookMark.setDirectory(m_game->getGameDirectory().c_str());

    if (!saveBookMark.exec())
        return;

    QString filename = saveBookMark.selectedFiles().first();

    std::ofstream ofs(filename.toUtf8().data(), std::ios_base::binary);
    MoMBookmark bookmark(m_game.data());
    if (bookmark.save(ofs))
    {
        QMessageBox::information(this, "Save battle bookmark", "Saved bookmark in '" + filename + "'");
    }
    else
    {
        QMessageBox::warning(this, "Save battle bookmark", "Failed to save correctly in '" + filename + "'");
    }
}

void MoM::DialogMap::on_pushButton_RestoreBookmark_clicked()
{
    if ((0 == m_game) || (0 == m_game->getDataSegment()))
        return;

    QFileDialog restoreBookMark;

    restoreBookMark.setObjectName("restoreBookMark");
    restoreBookMark.setWindowTitle(tr("Restore battle bookmark"));
    restoreBookMark.setNameFilter(tr("Battle bookmark files (*.battlebookmark *.BATTLEBOOKMARK);;All files (*)"));
    restoreBookMark.setAcceptMode(QFileDialog::AcceptOpen);
    restoreBookMark.setFileMode(QFileDialog::ExistingFile);
    restoreBookMark.setViewMode(QFileDialog::Detail);

    if (!restoreBookMark.exec())
        return;

    QString filename = restoreBookMark.selectedFiles().first();

    std::ifstream ifs(filename.toUtf8().data(), std::ios_base::binary);
    MoMBookmark bookmark(m_game.data());
    if (bookmark.restore(ifs))
    {
        QMessageBox::information(this, "Restore battle bookmark", "Restored bookmark from '" + filename + "'");
    }
    else
    {
        QMessageBox::warning(this, "Restore battle bookmark", "Failed to restore from '" + filename + "'");
    }
}
