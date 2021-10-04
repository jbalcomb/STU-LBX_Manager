// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2012-03-12
// ---------------------------------------------------------------------------

#include "DialogBuildingTree.h"
#include "ui_DialogBuildingTree.h"

#include <QGraphicsScene>
#include <QMenu>
#include <QTimer>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <cmath>
#include <fstream>
#include <regex>
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

// TODO: Put in its own file. Let it clash if accidentally duplicated.
class MoMBuilding
{
public:
    MoMBuilding(MoMGameBase* game, eBuilding building) : m_game(game), m_buildingEnum(building), m_buildingData(nullptr)
    {
        if (m_game != nullptr)
            m_buildingData = m_game->getBuildingData(m_buildingEnum);
    }
    eBuilding getBuildingEnum() const { return m_buildingEnum; }
    std::string getName() const
    {
        if (m_buildingData != nullptr)
        {
            return m_buildingData->m_BuildingName;
        }
        auto name = std::regex_replace(toStr(m_buildingEnum), std::regex("^[A-Z]+_"), "");
        name = std::regex_replace(name, std::regex("\\s*\\(\\d+\\)$"), "");
        return replaceUnderscoresBySpaces(name);
    }
    std::vector<eBuilding> getRequiredBuildings() const
    {
        if (m_buildingData == nullptr)
            return {};
        auto buildings = std::vector<eBuilding>();
        for (auto preq: {m_buildingData->m_Prerequisite1, m_buildingData->m_Prerequisite2})
        {
            if (preq != BUILDING_None)
                buildings.push_back(preq);
        }
        return buildings;
    }
private:
    MoMGameBase* m_game;
    eBuilding m_buildingEnum;
    Building_Data* m_buildingData;
};

namespace
{

class QMoMBuildingTile : public QGraphicsItem
{
    const int lineHeight = 16;
public:
    QMoMBuildingTile(eBuilding buildingEnum, const QMoMGamePtr& game)
        : QGraphicsItem(),
          m_game(game),
          m_momBuilding(game.data(), buildingEnum)
    {}

    QRectF boundingRect() const override
    {
        int height = 1;
        height += MoM::QMoMResources::instance().getPixmap(m_momBuilding.getBuildingEnum(), 1).height();
        height += m_momBuilding.getRequiredBuildings().size() * lineHeight;
        height += m_prohibitedRaces.size() * lineHeight;
        height += m_unitsPossible.count() * (MoM::QMoMResources::instance().getPixmap(
                                                 UNITTYPE_High_Men_Spearmen, 1).height());
        height += 2;
        return QRectF(0, 0, 150, height);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*) override
    {
        painter->fillRect(boundingRect(), Qt::white);
        painter->drawRect(boundingRect());

        const int xLeft = 2;
        int y = 1;
        int x;

        auto animationBuilding = MoM::QMoMResources::instance().getAnimation(m_momBuilding.getBuildingEnum());
        auto rectBuilding = QRect(0, 0, 32, 32);
        if (!animationBuilding.empty())
        {
            rectBuilding = animationBuilding.first()->rect();
            animationBuilding.crop();
            auto rect = animationBuilding.first()->rect();
            painter->drawImage(xLeft + (rectBuilding.width() - rect.width()) / 2,
                               y + (rectBuilding.height() - rect.height()) / 2,
                               *animationBuilding.first());
        }
        x = xLeft + rectBuilding.width();
        painter->drawText(x, y, boundingRect().width() - x, rectBuilding.height(), Qt::AlignVCenter,
                          m_momBuilding.getName().c_str());
        y += rectBuilding.height();
        for (auto preqEnum : m_momBuilding.getRequiredBuildings())
        {
            auto preqBuilding = MoMBuilding(m_game.data(), preqEnum);
            painter->drawText(xLeft, y, boundingRect().width() - xLeft, lineHeight, Qt::AlignVCenter,
                              QString("Prequisite: %0").arg(preqBuilding.getName().c_str()));
            y += lineHeight;
        }
        for (auto raceEnum : m_prohibitedRaces)
        {
            painter->drawText(xLeft, y, boundingRect().width() - xLeft, lineHeight, Qt::AlignVCenter,
                              QString("Prohibited: %0").arg(m_game->getRaceName(raceEnum).c_str()));
            y += lineHeight;
        }
        for (auto unit : m_unitsPossible)
        {
            QPixmap pixmapUnit = MoM::QMoMResources::instance().getPixmap(unit.getUnitTypeNr(), 1);
            painter->drawPixmap(xLeft, y, pixmapUnit);
            x = xLeft + pixmapUnit.width();
            painter->drawText(x, y, boundingRect().width() - x, pixmapUnit.height(), Qt::AlignVCenter,
                              unit.getDisplayName().c_str());
            y += pixmapUnit.height();
        }
    }

    eBuilding buildingEnum() const { return m_momBuilding.getBuildingEnum(); }

    void addPrerequisite(QMoMBuildingTile* preqTile) { m_prerequisiteTiles.push_back(preqTile); }
    QVector<QMoMBuildingTile*> getPrerequisites() { return m_prerequisiteTiles; }

    void addProhibitedRace(eRace race) { m_prohibitedRaces.push_back(race); }
    QVector<eRace> getProhibitedRaces() { return m_prohibitedRaces; }

    void addUnit(const MoMUnit& momUnit) { return m_unitsPossible.push_back(momUnit); }
    QVector<MoMUnit> getUnits() const { return m_unitsPossible; }

    void setTier(int value) { m_col = value; }
    int getTier() const { return m_col; }

private:
    QMoMGamePtr m_game;
    MoMBuilding m_momBuilding;
    QVector<QMoMBuildingTile*> m_prerequisiteTiles;
    QVector<eRace> m_prohibitedRaces;
    QVector<MoMUnit> m_unitsPossible;
    int m_col = -1;
};

void positionBuildingTiles(QMap<eBuilding, QMoMBuildingTile*>& buildingTiles)
{
    QVector<int> firstFreeRow(buildingTiles.count());
    for (auto buildingTile : buildingTiles)
    {
        auto col = buildingTile->getTier();
        auto row = firstFreeRow[col];
        firstFreeRow[col] += buildingTile->boundingRect().height() + 20;
        buildingTile->setPos(buildingTile->getTier() * (buildingTile->boundingRect().width() + 40), row);
        buildingTile->setZValue(1);
    }
}

int recurseDependency(QMoMBuildingTile& tile)
{
    // TODO: Better way to find buildings that are already available
    if ((tile.buildingEnum() == BUILDING_Trade_Goods) || (tile.buildingEnum() == BUILDING_Housing))
    {
        tile.setTier(0);
    }
    if (tile.getTier() != -1)
        return tile.getTier();

    int maxPreqCol = 0;
    for (auto preqTile : tile.getPrerequisites())
    {
        int col = recurseDependency(*preqTile);
        maxPreqCol = Max(maxPreqCol, col);
    }
    tile.setTier(maxPreqCol + 1);
    return tile.getTier();
}

}

class QMoMBuildingScene : public QGraphicsScene
{
public:
    QMoMBuildingScene() : QGraphicsScene() {}

    void addDependency(QMoMBuildingTile* tile, QMoMBuildingTile* preq)
    {
        auto line = addLine(QLineF(preq->x() + preq->boundingRect().right(),
                                   preq->y() + preq->boundingRect().height() / 2,
                                   tile->x(),
                                   tile->y() + tile->boundingRect().height() / 2),
                            QPen(Qt::black, 2));
        line->setZValue(0);
    }
};

DialogBuildingTree::DialogBuildingTree(QWidget *parent) :
    QMoMDialogBase(parent),
    m_scene(std::make_unique<QMoMBuildingScene>()),
    m_timer(new QTimer),
    ui(std::make_unique<Ui::DialogBuildTree>())
{
    ui->setupUi(this);

    postInitialize();

    // Update view when checkbox is clicked
//    QObject::connect(ui->checkBox_Cities, SIGNAL(clicked()), this, SLOT(slot_gameUpdated()));

    // Update view when items are inspected
//    QObject::connect(m_scene, SIGNAL(signal_tileChanged(MoM::MoMLocation)), this, SLOT(slot_tileChanged(MoM::MoMLocation)));
//    QObject::connect(m_scene, SIGNAL(signal_tileSelected(MoM::MoMLocation)), this, SLOT(slot_tileSelected(MoM::MoMLocation)));
//    QObject::connect(m_scene, SIGNAL(signal_tileDragged(MoM::MoMLocation,MoM::MoMLocation)), this, SLOT(slot_tileDragged(MoM::MoMLocation,MoM::MoMLocation)));

    // Connect timers
    QObject::connect(m_timer.data(), SIGNAL(timeout()), this, SLOT(slot_timerAnimations()));

    // Start timer
//    m_timer->start(250);

    ui->graphicsView->setScene(m_scene.get());
}

DialogBuildingTree::~DialogBuildingTree()
{
    preFinalize();
}

//void DialogBuildingTree::addBattleUnitSubtree(QTreeWidget* treeWidget, Battle_Unit* battleUnit)
//{
//    assert(0 != battleUnit);
//    int battleUnitNr = (int)(battleUnit - m_game->getBattleUnit(0));
//    Unit* unit = m_game->getUnit(battleUnit->m_unitNr);
//    if (0 == unit)
//        return;
//    QString text = QString("%0").arg(prettyQStr(unit->m_Unit_Type));
//    if (battleUnit->m_Weapon_Type_Plus_1 > 1)
//    {
//        text += ", " + prettyQStr((eWeaponType)(battleUnit->m_Weapon_Type_Plus_1 - 1));
//    }
//    QMoMTreeItemWidgetBase* qtreeUnit = new QMoMTreeItemWidgetBase(m_game,
//                                 QString("battle[%0] unit[%1]").arg(battleUnitNr).arg(battleUnit->m_unitNr),
//                                 text);
//    treeWidget->addTopLevelItem(qtreeUnit);

//    qtreeUnit->addChild(new EnumTreeItem<eUnit_Type>(m_game, "Unit Type", &unit->m_Unit_Type, eUnit_Type_MAX));
//    qtreeUnit->addChild(new EnumTreeItem<ePlayer>(m_game, "Owner", &battleUnit->m_Owner, ePlayer_MAX));
//    qtreeUnit->addChild(new EnumTreeItem<eBattleUnitActive>(m_game, "Active", &battleUnit->m_Active, eBattleUnitActive_MAX));
//    qtreeUnit->addChild(new NumberTreeItem<uint8_t>(m_game, "WeaponType", &battleUnit->m_Weapon_Type_Plus_1));
//}

void DialogBuildingTree::on_comboBox_Plane_currentIndexChanged(int index)
{
//    switch (index)
//    {
//    case 1:
//        ui->graphicsView->setScene(m_scene.get());
//        break;
//    default:
//        ui->graphicsView->setScene(0);
//        break;
//    }
}

void DialogBuildingTree::on_treeWidget_Tile_customContextMenuRequested(const QPoint &pos)
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
}

void DialogBuildingTree::on_verticalSlider_Zoom_valueChanged(int value)
{
    double scale = 1.0 + value / 100.0;
    if (value < 0)
    {
        scale = std::pow(2.0, value / 100.0);
    }

    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scale, scale);
}

void DialogBuildingTree::slot_addressCalculator()
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

void DialogBuildingTree::slot_gameChanged(const QMoMGamePtr& game)
{
    m_game = game;

    slot_gameUpdated();
}

void DialogBuildingTree::slot_gameUpdated()
{
    // Reset game
    m_scene->clear();

    if (m_game.isNull())
        return;
    if (m_game->getBuildingData(BUILDING_None) == nullptr)
        return;

    // Create tile for each building
    QMap<eBuilding, QMoMBuildingTile*> buildingTiles;
    for (int buildingNr = 1; buildingNr < eBuilding_array_MAX; ++buildingNr)
    {
        auto buildingEnum = static_cast<eBuilding>(buildingNr);
        if (m_game->getBuildingData(buildingEnum) == nullptr)
            break;
        auto tile = new QMoMBuildingTile(buildingEnum, m_game);
        m_scene->addItem(tile);
        buildingTiles[buildingEnum] = tile;
    }

    // Register prerequisite tiles for every building tile
    for(auto buildingTile : buildingTiles)
    {
       for (auto preq : MoMBuilding(m_game.data(), buildingTile->buildingEnum()).getRequiredBuildings())
       {
           auto it = buildingTiles.find(preq);
           if (it != buildingTiles.end())
           {
               buildingTile->addPrerequisite(it.value());
           }
       }
    }

    // List prohibited races for each building
    for (int raceNr = 0; raceNr < (int)gMAX_RACES; ++raceNr)
    {
        auto race = (eRace)raceNr;
        const auto raceData = m_game->getRaceData(race);
        if (raceData == nullptr)
            break;
        for (size_t i = 0; (i < raceData->m_Number_of_prohibited_buildings) && (i < ARRAYSIZE(raceData->m_Prohibited_buildings)); ++i)
        {
             auto buildingEnum = raceData->m_Prohibited_buildings[i];
             auto it = buildingTiles.find(buildingEnum);
             if (it != buildingTiles.end())
             {
                 it.value()->addProhibitedRace(race);
             }
        }
    }

    // Find tier for each building tile
    for (auto buildingTile : buildingTiles)
    {
        recurseDependency(*buildingTile);
    }

    // Position each building tile
    positionBuildingTiles(buildingTiles);

    // List units at the highest tier buildings they require
    for (int unitTypeNr = 0; unitTypeNr < eUnit_Type_MAX; ++unitTypeNr)
    {
        MoMUnit momUnit(m_game.data(), static_cast<eUnit_Type>(unitTypeNr));
        auto listBuildings = momUnit.getRequiredBuildings();
        auto highestTierValue = QPointF();
        QMoMBuildingTile* highestTierTile = nullptr;
        for (auto buildingEnum : listBuildings)
        {
            auto it = buildingTiles.find(buildingEnum);
            if (it != buildingTiles.end())
            {
                auto tile = it.value();
                if ((tile->x() > highestTierValue.x())
                        || ((tile->x() == highestTierValue.x()) && (tile->y() > highestTierValue.y())))
                {
                    highestTierTile = tile;
                    highestTierValue = tile->pos();
                }
            }
        }
        if (highestTierTile != nullptr)
        {
            highestTierTile->addUnit(momUnit);
        }
    }

    // Redo positioning of each building tile to give space to added units
    positionBuildingTiles(buildingTiles);

    // Add connectors for prequisite tiles to the scene
    for (auto buildingTile : buildingTiles)
    {
        for (auto preqTile : buildingTile->getPrerequisites())
        {
            m_scene->addDependency(buildingTile, preqTile);
        }
    }
}

void DialogBuildingTree::slot_itemAction()
{
    QTreeWidgetItem* pItem = ui->treeWidget_Tile->currentItem();
    QMoMTreeItemWidgetBase* pMoMItem = dynamic_cast<QMoMTreeItemWidgetBase*>(pItem);
    if (0 != pMoMItem)
    {
        pMoMItem->slotAction();
    }
}

void DialogBuildingTree::slot_tileChanged(const MoM::MoMLocation& loc)
{
}

void DialogBuildingTree::slot_tileDragged(const MoMLocation &locFrom, const MoMLocation &locTo)
{
}

void DialogBuildingTree::slot_tileSelected(const MoM::MoMLocation &loc)
{
}

void DialogBuildingTree::slot_timerAnimations()
{
}

}
