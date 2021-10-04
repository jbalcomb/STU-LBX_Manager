// ---------------------------------------------------------------------------
// Copyright:   2014 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2013-01-01
// ---------------------------------------------------------------------------

#include "DialogManageCities.h"
#include "ui_DialogManageCities.h"

#include <QMenu>

#include <math.h>

#include "MainWindow.h"
#include "MoMCity.h"
#include "MoMController.h"
#include "MoMManageCities.h"
#include "MoMUtility.h"
#include "MoMGenerated.h"
#include "QMoMSettings.h"
#include "QMoMTableItem.h"

using namespace MoM;

/////////////////////////////////////////

//! Item tracking the MoMCityState that can be inserted in a table.
class CityTargetTableItem : public QMoMTableItemBase
{
public:
    enum eDisplay { DISPLAY_State, DISPLAY_Target };
    CityTargetTableItem(const QMoMGamePtr& game, MoMManageCities::MoMCityState* cityState, eDisplay display);

    virtual QList<QAction*> requestActions(QObject* parent);
    virtual void slotAction();
    virtual QString toString() const;

protected:
    template<typename Enum>
    void addAction(Enum e);

    QList<QAction*> getActions()
    {
        assert(0 != m_actionGroup);
        return m_actionGroup->actions();
    }

    void createActionGroup(QObject* parent)
    {
        // Ownership is transfered to the parent, who will delete it.
        m_actionGroup = new QActionGroup(parent);
    }

private:
    // Configuration
    MoMManageCities::MoMCityState* m_cityState;
    eDisplay m_display;

    // Status

    // Keep track of the action group
    // m_actionGroup is deleted by its parent (the context menu)
    QActionGroup* m_actionGroup;
};

CityTargetTableItem::CityTargetTableItem(const QMoMGamePtr& game, MoMManageCities::MoMCityState* cityState, eDisplay display) :
    QMoMTableItemBase(game),
    m_cityState(cityState),
    m_display(display),
    m_actionGroup()
{
    QTableWidgetItem::setData(Qt::EditRole, toString());
}

template<typename Enum>
void CityTargetTableItem::addAction(Enum e)
{
    assert(m_actionGroup != 0);

    QString name = MoM::QMoMResources::instance().getName(e);
    QAction* action = new QAction(name, m_actionGroup);
    QMoMIconPtr iconPtr = MoM::QMoMResources::instance().getIcon(e);
    if (!iconPtr.isNull())
    {
        action->setIcon(*iconPtr);
    }
    action->setCheckable(true);
    action->setData(QVariant((unsigned)e));
    if (m_cityState->getTarget() == (MoMManageCities::MoMCityState::eTarget)e)
    {
        action->setChecked(true);
    }
}

QList<QAction*> CityTargetTableItem::requestActions(QObject* parent)
{
    this->createActionGroup(parent);

    const MoMCity& momCity = m_cityState->getMoMCity();
    MOM_FOREACH(eProducing, produce, eProducing_MAX)
    {
        if ((toUInt(produce) < eBuilding_array_MAX) && momCity.isBuildingPresent((eBuilding)produce))
            continue;
        if (momCity.isProductionAllowed(produce))
        {
            addAction(produce);
        }
    }

    for (eAdditionalCityTargets target = CITYTARGET_FIRST_ADDITIONAL_TARGET; target < eAdditionalCityTargets_MAX; inc(target))
    {
        addAction(target);
    }

    return this->getActions();
}

void CityTargetTableItem::slotAction()
{
    QAction* action = m_actionGroup->checkedAction();
    m_cityState->setTarget((MoMManageCities::MoMCityState::eTarget)action->data().toUInt());
    setData(Qt::EditRole, toString());
}

QString CityTargetTableItem::toString() const
{
    QString str;
    if (m_display == DISPLAY_State)
    {
        str = prettyQStr(m_cityState->getState());
    }
    else if (m_display == DISPLAY_Target)
    {
        MoMManageCities::MoMCityState::eTarget target = m_cityState->getTarget();
        if (target < eProducing_MAX)
        {
            str = prettyQStr((eProducing)target);
        }
        else
        {
            str = prettyQStr((eAdditionalCityTargets)target);
        }
    }
    return str;
}

/////////////////////////////////////////

DialogManageCities::DialogManageCities(QWidget *parent) :
    QMainWindow(parent),
    m_game(),
    m_updating(false),
    m_columnFarmers(-1),
    m_columnBuy(-1),
    ui(new Ui::DialogManageCities)
{
    ui->setupUi(this);

    setCentralWidget(ui->tableWidget_Cities);

    ui->dockWidget_Summary->setParent(ui->tableWidget_Cities);
    addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidget_Summary);
    ui->menubar->addAction(ui->dockWidget_Summary->toggleViewAction());

    ui->dockWidget_QueueDefinition->setParent(ui->tableWidget_Cities);
    addDockWidget(Qt::BottomDockWidgetArea, ui->dockWidget_QueueDefinition);
    ui->menubar->addAction(ui->dockWidget_QueueDefinition->toggleViewAction());

    //    ui->splitter->setSizes(QList<int>() << 100 << 680);

    ui->tableWidget_QueueDefinition->setColumnWidth(0, 140);
    ui->tableWidget_QueueDefinition->setColumnWidth(1, 420);

    QStringList labelsQueue;
    labelsQueue << "Build" << "Conditions";
    ui->tableWidget_QueueDefinition->setColumnCount(labelsQueue.size());
    ui->tableWidget_QueueDefinition->setHorizontalHeaderLabels(labelsQueue);

    int row = 0;
    ui->tableWidget_QueueDefinition->setRowCount(26);

    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Garrison"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("Garrison < 1 and not producing(Garrison)"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Garrison"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("producing(Garrison)"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Builder's Hall"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
//    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("buy(Builder's Hall)"));
//    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("Completion >= 50%"));
//    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Granary"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
//    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("buy(Granary)"));
//    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("Completion >= 50%"));
//    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Smithy"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Marketplace"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Farmer's Market"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Housing"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("Pop < 4"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Shrine"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Sawmill"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Foresters Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Library"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Sages Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Temple"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Alchemist Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("University"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Bank"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Miners Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Stable"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("if allowed(Animists Guild)"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Animists Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Parthenon"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Shipwright Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("if allowed(Merchants Guild)"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Ship Yard"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem("if allowed(Merchants Guild)"));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Merchants Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Cathedral"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->setItem(row, 0, new QTableWidgetItem("Wizards Guild"));
    ui->tableWidget_QueueDefinition->setItem(row, 1, new QTableWidgetItem(""));
    row++;
    ui->tableWidget_QueueDefinition->resizeRowsToContents();

    QStringList labelsCities;
    labelsCities << "Nr"
           << "Name" << "Race" << "Pop" << "Farmers" << "Workers" << "Rebels" << "Food" << "Conn" << "Gold" << "Prod" << "Completion" << "Producing"
           << "Time" << "Garrison" << "Buy" << "Factor" << "State" << "Target";
    ui->tableWidget_Cities->setColumnCount(labelsCities.size());
    ui->tableWidget_Cities->setHorizontalHeaderLabels(labelsCities);
    ui->tableWidget_Cities->setIconSize(QSize(24, 14));
    ui->tableWidget_Cities->sortByColumn(0, Qt::AscendingOrder);

    ui->tableWidget_Summary->setColumnCount(2);
    QStringList labelsSummary;
    labelsSummary << "Treasury" << "Magic Reserve" << "Casting Skill" << "Gold/turn" << "Prod/turn" << "Food/turn"
                  << "Power Base" << "Mana/turn" << "Skill to pool" << "Research/turn" << "Researching";
    ui->tableWidget_Summary->setRowCount(labelsSummary.size());
    ui->tableWidget_Summary->setVerticalHeaderLabels(labelsSummary);
    for (int row = 0; row < ui->tableWidget_Summary->rowCount(); ++row)
    {
        ui->tableWidget_Summary->setItem(row, 0, new QMoMTableItemBase(m_game, ""));
    }
    row = 0;
    ui->tableWidget_Summary->setIconSize(QSize(24, 14));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_10_Gold, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_10_Mana, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_10_Power, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Gold, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Production, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Food, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Power, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Mana, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Power, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Research, 2));
    ui->tableWidget_Summary->item(row++, 0)->setIcon(*QMoMResources::instance().getIcon(RESOURCE_Research, 2));
    assert(ui->tableWidget_Summary->rowCount() == row);
    ui->tableWidget_Summary->resizeRowsToContents();

    // From QMoMDialogBase
    setAttribute(Qt::WA_DeleteOnClose);

//    setFont(MoM::QMoMResources::g_Font);
//    m_font = MoM::QMoMResources::g_Font;
//    m_font.setPointSize(16);

    QMoMSettings::readSettingsWindow(this);

    QObject::connect(MainWindow::getInstance(), SIGNAL(signal_gameChanged(QMoMGamePtr)), this, SLOT(slot_gameChanged(QMoMGamePtr)));
    QObject::connect(MainWindow::getInstance(), SIGNAL(signal_gameUpdated()), this, SLOT(slot_gameUpdated()));
    slot_gameChanged(MainWindow::getInstance()->getGame());
}

DialogManageCities::~DialogManageCities()
{
    QMoMSettings::writeSettingsWindow(this);
    delete ui;
}

QIcon DialogManageCities::getResourceIcon(eResource resource, int value)
{
    switch (resource)
    {
    case RESOURCE_Food:         resource = (value <= -10 ? RESOURCE_10_Gray_Food : value < 0 ? RESOURCE_Gray_Food : value < 10 ? RESOURCE_Food : RESOURCE_10_Food); break;
    case RESOURCE_Production:   resource = (value < 10 ? RESOURCE_Production : RESOURCE_10_Production); break;
    case RESOURCE_Gold:         resource = (value <= -10 ? RESOURCE_10_Gray_Gold : value < 0 ? RESOURCE_Gray_Gold : value < 10 ? RESOURCE_Gold : RESOURCE_10_Gold); break;
    case RESOURCE_Power:        resource = (value < 10 ? RESOURCE_Production : RESOURCE_10_Production); break;
    case RESOURCE_Mana:         resource = (value < 10 ? RESOURCE_Production : RESOURCE_10_Production); break;
    case RESOURCE_Research:     resource = (value < 10 ? RESOURCE_Production : RESOURCE_10_Production); break;
    default:                    ;
    }
    return *QMoMResources::instance().getIcon(resource, 2);
}

void DialogManageCities::update()
{
    int nrCities = 0;
    if (0 != m_game)
    {
        nrCities = m_game->getNrCities();
        if (m_game->isBattleInProgress())
        {
            nrCities = 0;
        }
    }

    MoM::MoMController momController(m_game.data());

    ui->tableWidget_Cities->setSortingEnabled(false);

    int row = 0;
    for (int cityNr = 0; (cityNr < nrCities) && (cityNr < (int)MoM::gMAX_CITIES); ++cityNr)
    {
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            break;
        if (MoM::PLAYER_YOU != city->m_Owner)
            continue;
        MoMCity momCity(m_game.data(), city);

        int curMaxPop = momCity.calcMaxPopCurrent();
        int topMaxPop = momCity.calcMaxPopTop();
        QString population = QString("%0 / %1").arg((int)(city->m_Population), 2).arg(curMaxPop);
        if (topMaxPop > curMaxPop)
        {
            population += QString("(%0)").arg(topMaxPop);
        }
        int buildingCost = momCity.getCostToProduce(city->m_Producing);
        int timeCompletion = momCity.getTimeToComplete(city->m_Producing);
        int garrisonSize = momController.countGarrison(MoMLocation(*city));
        int costToBuy = momCity.getCostToBuy(city->m_Producing);

        if (row >= ui->tableWidget_Cities->rowCount())
        {
            ui->tableWidget_Cities->insertRow(row);
        }
        int col = 0;
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("%0").arg(cityNr, 2)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(city->m_City_Name));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
                                            *QMoMResources::instance().getIcon(city->m_Race),
                                            prettyQStr(city->m_Race)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(population));
        m_columnFarmers = col;
        ui->tableWidget_Cities->setItem(row, col, new NumberTableItem<int8_t>(m_game, &city->m_Number_of_farmers_allocated, 2, SHOWNUMBER_normal));
        ui->tableWidget_Cities->item(row, col)->setIcon(*QMoMResources::instance().getIcon(LBXRecordID("BACKGRND", 59 + toUInt(city->m_Race)), 2));
        ui->tableWidget_Cities->item(row, col++)->setBackground(Qt::cyan);
        ui->tableWidget_Cities->setItem(row, col, new QTableWidgetItem(QString("%0").arg(momCity.calcNrWorkers(), 2)));
        ui->tableWidget_Cities->item(row, col++)->setIcon(*QMoMResources::instance().getIcon(LBXRecordID("BACKGRND", 45 + toUInt(city->m_Race)), 2));
        ui->tableWidget_Cities->setItem(row, col, new QTableWidgetItem(QString("%0").arg(momCity.calcNrRebels(), 2)));
        ui->tableWidget_Cities->item(row, col++)->setIcon(*QMoMResources::instance().getIcon(LBXRecordID("BACKGRND", 74 + toUInt(city->m_Race)), 2));
//        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
//                                            getResourceIcon(RESOURCE_Food, momCity.calcFoodProduced() - city->m_Population),
//                                            QMoMTableItemBase::formatNumber(momCity.calcFoodProduced() - city->m_Population, SHOWNUMBER_alwaysPlus, 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
                                            getResourceIcon(RESOURCE_Food, city->m_Food_Produced - city->m_Population),
                                            QMoMTableItemBase::formatNumber(city->m_Food_Produced - city->m_Population, SHOWNUMBER_alwaysPlus, 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("%0").arg(momCity.countConnectedCities(), 2)));
//        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
//                                            getResourceIcon(RESOURCE_Gold, momCity.calcGoldProduced() - city->m_Maintenance),
//                                            QMoMTableItemBase::formatNumber(momCity.calcGoldProduced() - city->m_Maintenance, SHOWNUMBER_alwaysPlus, 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
                                            getResourceIcon(RESOURCE_Gold, city->m_Coins - city->m_Maintenance),
                                            QMoMTableItemBase::formatNumber(city->m_Coins - city->m_Maintenance, SHOWNUMBER_alwaysPlus, 3)));
//        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
//                                            getResourceIcon(RESOURCE_Production, momCity.calcHammersProduced()),
//                                            QString("%0").arg(momCity.calcHammersProduced(), 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(
                                            getResourceIcon(RESOURCE_Production, city->m_Hammers),
                                            QString("%0").arg((int)(city->m_Hammers), 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("%0 /%1").arg((int)city->m_HammersAccumulated, 4).arg(buildingCost, 4)));
        QList<eProducing> listCanProduce;
        MOM_FOREACH(eProducing, produce, eProducing_MAX)
        {
            if (momCity.canProduce(produce))
            {
                listCanProduce << produce;
            }
        }
        ui->tableWidget_Cities->setItem(row, col, new EnumTableItemList<eProducing>(m_game, &city->m_Producing, listCanProduce));
        ui->tableWidget_Cities->item(row, col++)->setBackground(Qt::cyan);
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("%0").arg(timeCompletion, 3)));
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("%0").arg(garrisonSize)));
        m_columnBuy = col;
        ui->tableWidget_Cities->setItem(row, col, new QTableWidgetItem(*QMoMResources::instance().getIcon(RESOURCE_Gold, 2),
                                                                       QString("%0").arg(costToBuy, 4)));
        ui->tableWidget_Cities->item(row, col++)->setBackground(Qt::cyan);
        ui->tableWidget_Cities->setItem(row, col++, new QTableWidgetItem(QString("x %0").arg(momCity.getBuyFactor())));
        ui->tableWidget_Cities->setItem(row, col++, new CityTargetTableItem(m_game, MainWindow::getInstance()->getManageCities()->getCityState(cityNr), CityTargetTableItem::DISPLAY_State));
        ui->tableWidget_Cities->setItem(row, col, new CityTargetTableItem(m_game, MainWindow::getInstance()->getManageCities()->getCityState(cityNr), CityTargetTableItem::DISPLAY_Target));
        ui->tableWidget_Cities->item(row, col++)->setBackground(Qt::cyan);

        assert(ui->tableWidget_Cities->columnCount() == col);

        for (int col = 0; col < ui->tableWidget_Cities->columnCount(); ++col)
        {
            ui->tableWidget_Cities->item(row, col)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        }

        row++;
    }

    ui->tableWidget_Cities->setRowCount(row);
    for (int row = 0; row < ui->tableWidget_Cities->rowCount(); ++row)
    {
        ui->tableWidget_Cities->setVerticalHeaderItem(row, new QTableWidgetItem(""));
    }
    ui->tableWidget_Cities->resizeRowsToContents();
    ui->tableWidget_Cities->setSortingEnabled(true);

    
    if ((0 != m_game) && (0 != m_game->getWizard(PLAYER_YOU)))
    {
        Wizard* wizard  = m_game->getWizard(PLAYER_YOU);
        int goldIncome      = momController.calcGoldIncome(PLAYER_YOU);
        int goldUpkeep      = momController.calcGoldUpkeep(PLAYER_YOU);
        int foodProduced    = momController.calcFoodProduced(PLAYER_YOU);
        int foodUpkeep      = momController.calcFoodUpkeep(PLAYER_YOU);
        int production      = momController.calcProduction(PLAYER_YOU);
        int manaUpkeep      = momController.calcManaUpkeep(PLAYER_YOU);
        int researchTurns   = 0;
        int mana            = 0;
        int research        = 0;
        int skill           = 0;
        momController.calcManaSkillResearch(PLAYER_YOU, mana, skill, research);
        int totalGold       = wizard->m_Gold_Coins;
        int totalMana       = wizard->m_Mana_Crystals;
        int powerBase       = wizard->m_Power_Base;
        int researchLeft    = wizard->m_Researching_Left;
        int castingSkillBase = momController.calcCastingSkillBase(PLAYER_YOU);
        int castingSkillTotal = momController.calcCastingSkillTotal(PLAYER_YOU);
        int skillLeft       = wizard->m_Unused_Casting_Skill_available_this_turn;
        int skillFromPool   = Round(sqrt((double)wizard->m_Wizard_Casting_Skill));
        int skillBeforeInc  = Sqr(skillFromPool) + skillFromPool + 1 - wizard->m_Wizard_Casting_Skill;
        if (research > 0)
        {
            researchTurns       = (researchLeft + research - 1) / research;
        }

        row = 0;
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 GP").arg(totalGold, 5));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 MP").arg(totalMana, 5));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 / %1(%2) %3 %4 before inc")
                                                         .arg(skillLeft).arg(castingSkillBase).arg(castingSkillTotal)
                                                         .arg(sqrt((double)wizard->m_Wizard_Casting_Skill), 0, 'g', 2).arg(skillBeforeInc));
        ui->tableWidget_Summary->item(row, 0)->setText(QString("%0 GP").arg(goldIncome - goldUpkeep, 5));
        ui->tableWidget_Summary->setItem(row++, 1, new EnumTableItem<eTax_Rate>(m_game, &wizard->m_Tax_Rate, eTax_Rate_MAX, SHOWENUM_normal));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 Hammers").arg(production));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 Food").arg(foodProduced - foodUpkeep, 4));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 Power").arg(powerBase, 4));
        ui->tableWidget_Summary->item(row, 0)->setText(QString("%0 MP").arg(mana - manaUpkeep, 5));
        ui->tableWidget_Summary->setItem(row++, 1, new NumberTableItem<uint8_t>(m_game, &wizard->m_Mana_Percentage, 3, SHOWNUMBER_normal));
        ui->tableWidget_Summary->item(row, 0)->setText(QString("%0 SP to pool").arg(skill, 3));
        ui->tableWidget_Summary->setItem(row++, 1, new NumberTableItem<uint8_t>(m_game, &wizard->m_Skill_Percentage, 3, SHOWNUMBER_normal));
        ui->tableWidget_Summary->item(row, 0)->setText(QString("%0 RP").arg(research, 4));
        ui->tableWidget_Summary->setItem(row++, 1, new NumberTableItem<uint8_t>(m_game, &wizard->m_Research_Percentage, 3, SHOWNUMBER_normal));
        ui->tableWidget_Summary->item(row++, 0)->setText(QString("%0 RP left (%1 turns)").arg(researchLeft, 5).arg(researchTurns));
    }
}

void DialogManageCities::on_buttonBox_clicked(QAbstractButton* button)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    MainWindow* controller = MainWindow::getInstance();
    if (0 == controller)
        return;

    if (QDialogButtonBox::ApplyRole == ui->buttonBox->buttonRole(button))
    {
        controller->applyBuildQueues();
        update();
    }
}

void DialogManageCities::on_tableWidget_Cities_cellChanged(int row, int column)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    if (0 == m_game)
        return;
    if (column == m_columnFarmers)
    {
        int cityNr = ui->tableWidget_Cities->item(row, 0)->text().toInt();
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            return;
        MoMCity momCity(m_game.data(), city);
        int8_t foodProduced = momCity.calcFoodProduced();
        int8_t hammersProduced = momCity.calcHammersProduced();
        if (foodProduced != city->m_Food_Produced)
        {
            //qDebug() << QString("Updating food produced in city '%0' from %1 to %2")
            //            .arg(city->m_City_Name).arg((int)city->m_Food_Produced).arg((int)foodProduced);
            if (!m_game->commitData(&city->m_Food_Produced, &foodProduced, sizeof(city->m_Food_Produced)))
            {
                //qDebug() << "Failed to commit food change";
            }
        }
        if (hammersProduced != city->m_Hammers)
        {
            //qDebug() << QString("Updating hammers produced in city '%0' from %1 to %2")
            //            .arg(city->m_City_Name).arg((int)city->m_Hammers).arg((int)hammersProduced);
            if (!m_game->commitData(&city->m_Hammers, &hammersProduced, sizeof(city->m_Hammers)))
            {
                //qDebug() << "Failed to commit production change";
            }
        }
    }

    // Force update game to see all updated values
    if (m_game->readData())
    {
        update();
    }
}

void DialogManageCities::on_tableWidget_Cities_clicked(const QModelIndex &index)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    if (0 == m_game)
        return;
    if (index.column() == m_columnBuy)
    {
        int cityNr = ui->tableWidget_Cities->item(index.row(), 0)->text().toInt();
        City* city = m_game->getCity(cityNr);
        if (0 == city)
            return;
        MoMCity momCity = MoMCity(m_game.data(), city);
        int ret = QMessageBox::question(this, "Buy production",
                                        QString("Do you wish to spend %0 gold by purchasing a %1?")
                                        .arg(momCity.getCostToBuy())
                                        .arg(prettyQStr(city->m_Producing)),
                                        "Buy", "Cancel", "", 0, 1);
        if (ret == 0)
        {
            (void)MoMController(m_game.data()).buyProduction(city);
        }
    }
}

void DialogManageCities::on_tableWidget_Cities_customContextMenuRequested(const QPoint &pos)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    QTableWidgetItem* pItem = ui->tableWidget_Cities->currentItem();

    QMenu contextMenu;
    QMoMTableItemBase* pMoMItem = dynamic_cast<QMoMTableItemBase*>(pItem);
    if (0 != pMoMItem)
    {
        QList<QAction*> actions = pMoMItem->requestActions(&contextMenu);
        if (!actions.empty())
        {
            contextMenu.addSeparator();
            contextMenu.addActions(actions);
        }
        foreach(QAction* action, actions)
        {
            connect(action, SIGNAL(triggered()), this, SLOT(slot_ItemActionCities()));
        }
    }

    contextMenu.exec(ui->tableWidget_Cities->viewport()->mapToGlobal(pos));
}

void DialogManageCities::on_tableWidget_Summary_customContextMenuRequested(const QPoint &pos)
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    QTableWidgetItem* pItem = ui->tableWidget_Summary->currentItem();

    QMenu contextMenu;
    QMoMTableItemBase* pMoMItem = dynamic_cast<QMoMTableItemBase*>(pItem);
    if (0 != pMoMItem)
    {
        QList<QAction*> actions = pMoMItem->requestActions(&contextMenu);
        if (!actions.empty())
        {
            contextMenu.addSeparator();
            contextMenu.addActions(actions);
        }
        foreach(QAction* action, actions)
        {
            connect(action, SIGNAL(triggered()), this, SLOT(slot_ItemActionSummary()));
        }
    }

    contextMenu.exec(ui->tableWidget_Summary->viewport()->mapToGlobal(pos));
}

void DialogManageCities::slot_gameChanged(const QMoMGamePtr& game)
{
    // TODO: Recover from an open context menu, while the game is changed
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    m_game = game;

    update();
}

void DialogManageCities::slot_gameUpdated()
{
    if (m_updating)
        return;
    MoM::UpdateLock lock(m_updating);

    update();
}

void DialogManageCities::slot_ItemActionCities()
{
    QTableWidgetItem* pItem = ui->tableWidget_Cities->currentItem();
    QMoMTableItemBase* pMoMItem = dynamic_cast<QMoMTableItemBase*>(pItem);
    if (0 != pMoMItem)
    {
        pMoMItem->slotAction();
    }
}

void DialogManageCities::slot_ItemActionSummary()
{
    QTableWidgetItem* pItem = ui->tableWidget_Summary->currentItem();
    QMoMTableItemBase* pMoMItem = dynamic_cast<QMoMTableItemBase*>(pItem);
    if (0 != pMoMItem)
    {
        pMoMItem->slotAction();

        MoMController momController(m_game.data());
        momController.updateTaxAndPowerDivision(PLAYER_YOU);

        if (m_game->readData())
        {
            update();
        }
    }
}
