// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include "DialogTables.h"
#include "ui_DialogTables.h"

#include <QClipboard>
#include <QKeyEvent>
#include <QMenu>
#include <QMimeData>
#include <QProgressDialog>

#include <string>

#include "MoMCity.h"
#include "MoMController.h"
#include "MoMExeWizards.h"
#include "MoMGenerated.h"
#include "MoMLevelBonuses.h"
#include "MoMTemplate.h"
#include "MoMUtility.h"
#include "MainWindow.h"
#include "QMoMSettings.h"
#include "QMoMTableItem.h"

using namespace MoM;

DialogTables::DialogTables(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogTables)
{
    ui->setupUi(this);

    postInitialize();
}

DialogTables::~DialogTables()
{
    preFinalize();
    delete ui;
}

void DialogTables::keyPressEvent(QKeyEvent* event)
{
    //if there is a control-C event copy data to the global clipboard
    if ((event->key() == Qt::Key_C) && (event->modifiers() & Qt::ControlModifier))
    {
        slot_Copy();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

QColor DialogTables::getRealmColor(MoM::eRealm_Type realm) const
{
    QColor color = Qt::gray;
    switch (realm)
    {
    case MoM::REALM_None:      color = Qt::black;       break;
    case MoM::REALM_Nature:    color = Qt::darkGreen;   break;
    case MoM::REALM_Sorcery:   color = Qt::blue;        break;
    case MoM::REALM_Chaos:     color = Qt::darkRed;     break;
    case MoM::REALM_Life:      color = Qt::darkGray;    break;
    case MoM::REALM_Death:     color = Qt::black;       break;
    case MoM::REALM_Arcane:    color = Qt::darkYellow;  break;
    default:                   color = Qt::gray;        break;
    }
    return color;
}

void DialogTables::buildTable(const QStringList &labels, int rows, const char* slotFunction)
{
    initializeTable(labels, rows);

    QProgressDialog progress("Building table...", "Abort", 0, rows, this);
    progress.setWindowModality(Qt::WindowModal);

    connect(this, SIGNAL(signal_addRow(int)), this, slotFunction);

    for (int row = 0; row < rows; ++row)
    {
        progress.setValue(row);
        if (progress.wasCanceled())
            break;

        emit signal_addRow(row);
    }

    disconnect(this, SIGNAL(signal_addRow(int)), this, slotFunction);

    finalizeTable();
}

void DialogTables::initializeTable(const QStringList &labels, int rows)
{
    ui->tableWidget->setColumnCount(labels.size());
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->setRowCount(rows);

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        ui->tableWidget->setVerticalHeaderItem(row, new QTableWidgetItem(""));
    }
}

void DialogTables::finalizeTable()
{
    ui->tableWidget->resizeColumnsToContents();
    ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
    ui->tableWidget->setSortingEnabled(true);
}

void DialogTables::update_BuildingData()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = MoM::eBuilding_array_MAX;
    }

    QStringList labels;
    labels << "Nr";
    labels << "BuildingName" << "Prerequisite1" << "Prerequisite2" << "Replaces" << "Upkeep"
           << "Cost" << "Category" << "Animation";

    buildTable(labels, ndata, SLOT(slot_addRow_to_BuildingData(int)));
}

void DialogTables::slot_addRow_to_BuildingData(int row)
{
    MoM::eBuilding building = (MoM::eBuilding)row;
    MoM::Building_Data* data = m_game->getBuildingData(building);
    if (0 == data)
        return;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_BuildingName, sizeof(data->m_BuildingName)));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding>(m_game, &data->m_Prerequisite1, MoM::eBuilding_extra_MAX, SHOWENUM_minusOneAndnoZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding>(m_game, &data->m_Prerequisite2, MoM::eBuilding_extra_MAX, SHOWENUM_minusOneAndnoZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding>(m_game, &data->m_Replaces_building, MoM::eBuilding_extra_MAX, SHOWENUM_minusOneAndnoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Upkeep_yield, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Building_cost, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuildingCategory>(m_game, &data->m_Building_category, MoM::eBuildingCategory_MAX));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Animation_related, 2, SHOWNUMBER_noZero));
}

void DialogTables::update_Cities()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = m_game->getNrCities();
    }

    QStringList labels;
    labels << "Nr";
    labels << "Owner" << "CityName" << "Race" << "Pop" << "Farmers" << "Gold" << "Maint" << "Mana" << "Research" << "Food"
           << "Prd" << "Producing" << "Produced";
    labels << "Cost" << "Time" << "Garrison";

    buildTable(labels, ndata, SLOT(slot_addRow_to_Cities(int)));
}

void DialogTables::slot_addRow_to_Cities(int row)
{
    int cityNr = row;
    City* data = m_game->getCity(cityNr);
    if (0 == data)
        return;
    MoMCity momCity(m_game.data(), data);

    int buildingCost = momCity.getCostToProduce(data->m_Producing);
    int timeCompletion = momCity.getTimeToComplete(data->m_Producing);
    int garrisonSize = MoM::MoMController(m_game.data()).countGarrison(MoMLocation(*data));

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::ePlayer>(m_game, &data->m_Owner, MoM::ePlayer_MAX, SHOWENUM_normal));
    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_City_Name, sizeof(data->m_City_Name)));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eRace>(m_game, &data->m_Race, (eRace)MoM::gMAX_RACES, SHOWENUM_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Population, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Number_of_farmers_allocated, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Coins, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Maintenance, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Mana_cr, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Research, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Food_Produced, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Hammers, 2, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eProducing>(m_game, &data->m_Producing, MoM::eProducing_MAX, SHOWENUM_normal));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_HammersAccumulated, 3, SHOWNUMBER_normal));

    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(buildingCost, 4)));
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(timeCompletion, 3)));
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(garrisonSize, 1)));
}

void DialogTables::update_HeroData()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        if (0 != m_game->getHeroStatsInitializer((MoM::eUnit_Type)0))
        {
            ndata = MoM::gMAX_HERO_TYPES;
        }
        else if (0 != m_game->getHeroStats(MoM::PLAYER_YOU, (MoM::eUnit_Type)0))
        {
            ndata = MoM::gMAX_VALID_WIZARDS * MoM::gMAX_HERO_TYPES;
        }
        else
        {
        }
    }

    QStringList labels;
    labels << "Nr";
    labels << "Player" << "HeroType";
    labels << "Abilities" << "Casting" << "Spell 1" << "Spell 2" << "Spell 3" << "Spell 4"
           << "Picks" << "PickType" << "Status" << "XP";
    labels << "UnitNr" << "HeroName" << "Slot 1" << "Slot 2" << "Slot 3" << "Item 1" << "Item 2" << "Item 3";

    buildTable(labels, ndata, SLOT(slot_addRow_to_HeroData(int)));
}

void DialogTables::slot_addRow_to_HeroData(int row)
{
    MoM::ePlayer playerNr = (MoM::ePlayer)(row / MoM::gMAX_HERO_TYPES);
    MoM::eUnit_Type unitTypeNr = (MoM::eUnit_Type)(row % MoM::gMAX_HERO_TYPES);

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    if (0 != m_game->getHeroStatsInitializer(unitTypeNr))
    {
        MoM::Hero_Stats_Initializer* data = m_game->getHeroStatsInitializer(unitTypeNr);

        // PlayerNr
        col++;
        // HeroType
        ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, prettyQStr(unitTypeNr)));
        ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eHeroAbility>(
                                     m_game, &data->m_Hero_Abilities.bits, (MoM::eHeroAbility)0, MoM::eHeroAbility_MAX));
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, &data->m_Hero_Casting_Skill, 2, SHOWNUMBER_noZero));
        // Spells
        for (size_t i = 0; i < ARRAYSIZE(data->m_Spell); ++i)
        {
            ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpell16>(m_game, &data->m_Spell[i], MoM::eSpell16_MAX, SHOWENUM_minusOneAndnoZero));
        }
        // Picks
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, &data->m_Nr_Random_picks, 2, SHOWNUMBER_noZero));
        // PickType
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eRandomPickType>(m_game, &data->m_Random_pick_type, MoM::eRandomPickType_MAX, SHOWENUM_normal));
        // Status
        col++;
        // XP
        col++;
    }
    else if (0 != m_game->getHeroStats(playerNr, unitTypeNr))
    {
        MoM::Hero_stats* data = m_game->getHeroStats(playerNr, unitTypeNr);

        // PlayerNr
        ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, prettyQStr(playerNr)));
        // HeroType
        ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, prettyQStr(unitTypeNr)));
        ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eHeroAbility>(
                                     m_game, &data->m_Hero_Abilities.bits, (MoM::eHeroAbility)0, MoM::eHeroAbility_MAX));
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Hero_Casting_Skill, 2, SHOWNUMBER_noZero));
        // Spells
        for (size_t i = 0; i < ARRAYSIZE(data->m_Spell); ++i)
        {
            ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpell>(m_game, &data->m_Spell[i], MoM::eSpell_MAX, SHOWENUM_minusOneAndnoZero));
        }
        // Picks
        col++;
        // PickType
        col++;
        // Status
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eHero_Level_Status>(
                                     m_game, &data->m_Level_Status,
                                     MoM::HEROLEVELSTATUS_Active_in_Wizards_army, MoM::Succ(MoM::HEROLEVELSTATUS_Level_9),
                                     SHOWENUM_normal));
        // XP
        col++;
    }
    else
    {
        col = 13;
    }

    if (0 != m_game->getHiredHero(playerNr, unitTypeNr))
    {
        MoM::Hired_Hero* data = m_game->getHiredHero(playerNr, unitTypeNr);

        ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Unit_Nr, 3, SHOWNUMBER_normal));
        ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Hero_name, sizeof(data->m_Hero_name)));
        // Slots
        for (size_t i = 0; i < ARRAYSIZE(data->m_Slot_Types); ++i)
        {
            ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSlot_Type16>(m_game, &data->m_Slot_Types[i], MoM::eSlot_Type16_MAX, SHOWENUM_minusOneAndnoZero));
        }
        for (size_t i = 0; i < ARRAYSIZE(data->m_Slot_Types); ++i)
        {
            ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Items_In_Slot[i], 2, SHOWNUMBER_normal));
        }
    }
    else
    {
        // UnitNr
        col++;

        // HeroName
        MoM::Hero_Choice* heroChoices = m_game->getChosen_Hero_Names();
        MoM::Hero_stats* heroStats = m_game->getHeroStats(playerNr, unitTypeNr);
        if ((playerNr == MoM::PLAYER_YOU) && (0 != heroChoices) && (0 != heroStats) && (heroStats->m_Level_Status != 0))
        {
            MoM::Hero_Choice* data = &heroChoices[unitTypeNr];

            // Retro actively fill in column XP
            ui->tableWidget->setItem(row, 12, new NumberTableItem<int16_t>(m_game, &data->m_Experience, 4, SHOWNUMBER_normal));

            // Hero name
            ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Name, sizeof(data->m_Name)));
        }
        else
        {
            col++;
        }

        // Slots
        MoM::Unit_Type_Data* unitType = m_game->getUnitTypeData(unitTypeNr);
        if (0 != unitType)
        {
            MoM::eSlot_Type16 heroSlotTypes[3];
            m_game->getHeroSlotTypes(unitType->m_Building2_or_HeroType, heroSlotTypes);
            for (size_t i = 0; i < ARRAYSIZE(heroSlotTypes); ++i)
            {
                ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, prettyQStr(heroSlotTypes[i])));
            }
        }
        else
        {
            col += 3;
        }
    }
}

void DialogTables::update_ItemData()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = m_game->getNrItemDataLbx();
    }

    QStringList labels;
    labels << "Nr";
    labels << "ItemName" << "Icon" << "Slot" << "Item Type" << "Cost";
    labels << "Att" << "ToHit" << "Def" << "Move" << "Resist" << "Mana" << "Save";
    labels << "Spell" << "NrCharges" << "Powers";
    labels << "Nature" << "Sorcery" << "Chaos" << "Life" << "Death" << "Artifact" << "InGame";

    buildTable(labels, ndata, SLOT(slot_addRow_to_ItemData(int)));
}

void DialogTables::slot_addRow_to_ItemData(int row)
{
    ItemDataLbx* dataLbx = m_game->getItemDataLbx(row);
    if (0 == dataLbx)
        return;
    MoM::Item* data = &dataLbx->m_Item;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Item_Name, sizeof(data->m_Item_Name)));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eItem_Icon>(m_game, &data->m_Icon, MoM::eItem_Icon_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSlot_Type8>(m_game, &data->m_Slot_Required, MoM::eSlot_Type8_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eItem_Type>(m_game, &data->m_Item_Type, MoM::eItem_Type_MAX));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Cost, 5));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Attack, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.To_Hit, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Defense, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Movement_in_halves, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Resistance, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Spell_Points, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Spell_Save, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpell>(m_game, &data->m_Spell_Number_Charged, MoM::eSpell_MAX, SHOWENUM_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Number_Of_Charges, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eItemPower>(
                                  m_game, &data->m_Bitmask_Powers.bits, (MoM::eItemPower)0, MoM::eItemPower_MAX));

    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Required_Spellbooks[0], 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Required_Spellbooks[1], 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Required_Spellbooks[2], 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Required_Spellbooks[3], 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Required_Spellbooks[4], 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &dataLbx->m_Special_Item, 1, SHOWNUMBER_noZero));

    uint8_t* artifactsInGame = m_game->getArtifacts_in_game();
    if (0 != artifactsInGame)
    {
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &artifactsInGame[row], 1, SHOWNUMBER_noZero));
    }
    else
    {
        col++;
    }
}

void DialogTables::update_ItemPowers()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = m_game->getNrItemPowLbx();
    }

    QStringList labels;
    labels << "Nr";
    labels << "PowerName" << "Enchantible" << "Mana" << "Power Type" << "Color" << "Bonus/NrBooks" << "Powers";

    buildTable(labels, ndata, SLOT(slot_addRow_to_ItemPowers(int)));
}

void DialogTables::slot_addRow_to_ItemPowers(int row)
{
    MoM::ItemPowLbx* data = m_game->getItemPowLbx(row);
    if (0 == data)
        return;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Name, sizeof(data->m_Name)));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eItem_Type>(
                                 m_game, &data->m_Bitmask_Powers.bits, (MoM::eItem_Type)0, MoM::eItem_Type_MAX));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Mana_cost_to_enchant, 5));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eItemPowerType>(m_game, &data->m_PowerType, MoM::eItemPowerType_MAX));
    if ((row >= 33) && (row < 66))  // Hard coded in MoM
    {
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eRealm_Type>(m_game, &data->m_Color, MoM::eRealm_Type_MAX));
    }
    else
    {
        col++;
    }
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Required_Nr_Spell_Books, 2));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eItemPower>(
                                 m_game, &data->m_Bitmask_Powers.bits, (MoM::eItemPower)0, MoM::eItemPower_MAX));
}

void DialogTables::update_ItemsInGame()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = m_game->getNrItemsValid();
    }

    QStringList labels;
    labels << "Nr";
    labels << "ItemName" << "Icon" << "Slot" << "Item Type" << "Cost";
    labels << "Att" << "ToHit" << "Def" << "Move" << "Resist" << "Mana" << "Save";
    labels << "Spell" << "NrCharges" << "Powers";

    buildTable(labels, ndata, SLOT(slot_addRow_to_ItemsInGame(int)));
}

void DialogTables::slot_addRow_to_ItemsInGame(int row)
{
    static int itemNr;

    if (0 == row)
    {
        itemNr = -1;
    }

    // Find next item
    MoM::Item* data = 0;
    for (++itemNr; toUInt(itemNr) < MoM::gMAX_ITEMS_VALID; ++itemNr)
    {
        data = m_game->getItem(itemNr);
        if ((0 != data) && (data->m_Cost > 0))
            break;
    }
    if (0 == data)
        return;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(itemNr, 3)));

    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Item_Name, sizeof(data->m_Item_Name)));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eItem_Icon>(m_game, &data->m_Icon, MoM::eItem_Icon_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSlot_Type8>(m_game, &data->m_Slot_Required, MoM::eSlot_Type8_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eItem_Type>(m_game, &data->m_Item_Type, MoM::eItem_Type_MAX));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Cost, 5));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Attack, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.To_Hit, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Defense, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Movement_in_halves, 2, SHOWNUMBER_halfMove));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Resistance, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Spell_Points, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Bonuses.Spell_Save, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpell>(m_game, &data->m_Spell_Number_Charged, MoM::eSpell_MAX, SHOWENUM_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Number_Of_Charges, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eItemPower>(
                                 m_game, &data->m_Bitmask_Powers.bits, (MoM::eItemPower)0, MoM::eItemPower_MAX));
}

void DialogTables::update_LevelBonus()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        MoMLevelBonuses momLevelBonuses(m_game.data());
        if (!momLevelBonuses.isCodeInstalled())
        {
            if (QMessageBox::Yes != QMessageBox::question(this,
                tr("Install level bonus code"),
                tr("Level bonus code is not installed yet into WIZARDS.EXE.\nInstall it now?"),
                (QMessageBox::Yes | QMessageBox::No)) )
            {
                return;
            }
            if (!momLevelBonuses.installCode())
            {
                (void)QMessageBox::warning(this,
                     tr("Install level bonus code"),
                     tr("Failed to install level bonus code"));
                return;
            }
            else
            {
                (void)QMessageBox::information(this,
                     tr("Install level bonus code"),
                     tr("Successfully installed level bonus code"));
            }
        }

        // 6 unit levels + 9 hero levels
        ndata = gMAX_LEVELS_UNIT + gMAX_LEVELS_HERO;
    }

    QStringList labels;
    // labels << "Nr";
    //  Me Ra    Sh Th Df Re Mv                 CFg       Hp Sc Tr Fg Cs Ga
    labels << "Level" << "Me" << "Ra" << "Shots" << "ToHit" << "Df" << "Re";
    // DOES NOT WORK:
    // - Move has no effect overland nor in battle
    // - Hp has no effect overland nor in battle
    // - CurFig/MaxFig works times 3
    // - Holy Bonus (Gaze) works times 3
    // - Poison does not work
    //labels << "Move" << "CurFig" << "Hp" << "MaxFig" << "Pois/Gaze";

    buildTable(labels, ndata, SLOT(slot_addRow_to_LevelBonus(int)));
}

void DialogTables::slot_addRow_to_LevelBonus(int row)
{
    int level = 0;
    Battle_Unit* data = 0;
    QString name;
    if (row < 6)
    {
        // Unit level
        level = row;
        data = m_game->getLevelBonusUnit(level);
        name = "Unit";
    }
    else
    {
        // Hero level
        level = row - 6;
        data = m_game->getLevelBonusHero(level);
        name = "Hero";
    }
    if (0 == data)
        return;

    int col = 0;
//    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0 %1").arg(name).arg(1 + level)));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Melee, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Ranged, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Ranged_Shots, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_To_Hit, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Defense, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Resistance, 2, SHOWNUMBER_plusAndNoZero));
    // SEE LABELS
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_MoveHalves, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Current_figures, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Hitpoints_per_Figure, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Scouting, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Transport_Capacity_GUESS, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Max_figures, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Construction, 2, SHOWNUMBER_plusAndNoZero));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Gaze_Modifier, 2, SHOWNUMBER_plusAndNoZero));
}

void DialogTables::update_RaceData()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        ndata = MoM::gMAX_RACES;
    }

    QStringList labels;
    labels << "Nr";
    labels << "RaceName" << "Nr prohibited";
    for (size_t i = 0; i < ARRAYSIZE(((MoM::Race_Data*)0)->m_Prohibited_buildings); ++i)
    {
        labels << QString("Prohibited%0").arg(1 + i);
    }
    labels << "Outpost growth (%)" << "City growth (10/turn)" << "Housing";

    buildTable(labels, ndata, SLOT(slot_addRow_to_RaceData(int)));
}

void DialogTables::slot_addRow_to_RaceData(int row)
{
    MoM::eRace race = (MoM::eRace)row;
    MoM::Race_Data* data = m_game->getRaceData(race);
    if (0 == data)
        return;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    char* ptrName = const_cast<char*>(m_game->getNameByOffset(data->m_PtrName));
    ui->tableWidget->setItem(row, col, new TextTableItem(m_game, ptrName, strlen(ptrName) + 1));
    ui->tableWidget->item(row, col)->setForeground(getRealmColor(m_game->getRealmRace(race)));
    ui->tableWidget->item(row, col++)->setToolTip(MoM::QMoMResources::instance().getHelpText(race).c_str());

    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, &data->m_Number_of_prohibited_buildings, 2));
    for (size_t i = 0; i < ARRAYSIZE(data->m_Prohibited_buildings); ++i)
    {
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding>(m_game, &data->m_Prohibited_buildings[i], MoM::eBuilding_extra_MAX, SHOWENUM_noZero));
    }

    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Outpost_growth_probability, 2));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_City_population_growth_modifier, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eHousing>(m_game, &data->m_Housing_picture, MoM::eHousing_MAX));
}

void DialogTables::update_SpellData()
{
    int ndata = 0;
    if (!m_game.isNull())
    {
        if (0 != m_game->getSpellData((MoM::eSpell)0))
        {
            ndata = (int)MoM::eSpell_MAX - 1;
        }
    }

    QStringList labels;
    labels << "Nr";
    labels << "SpellName" << "AI Category" << "Desirability" << "Category" << "Section" << "Realm" << "Elegibility"
            << "Casting" << "Research" << "Parameter" << "Immunities" << "Attack Flags";
    labels<< "Upkeep";
    labels<< "Save";

    buildTable(labels, ndata, SLOT(slot_addRow_to_SpellData(int)));

    MoM::MoMExeWizards* wizardsExe = 0;
    uint8_t* ovl112 = 0;
    if (!m_game.isNull())
    {
        wizardsExe = m_game->getWizardsExe();
        ovl112 = m_game->getWizardsOverlay(112);
    }
    if ((0 != wizardsExe) && (0 != ovl112))
    {
        int col = ui->tableWidget->columnCount() - 1;

        for (size_t i = 0; i < wizardsExe->getNrSpellSaves(); ++i)
        {
            int saveSpellNr = (int)ovl112[ wizardsExe->getSpellSave(i).spellOffset ];
            int8_t *pSaveModifier = 0;
            if (0 != wizardsExe->getSpellSave(i).saveOffset)
            {
                pSaveModifier = (int8_t*)&ovl112[ wizardsExe->getSpellSave(i).saveOffset ];
            }
            int rowSpell = saveSpellNr - 1;
            if ((rowSpell >= 0) && (rowSpell < ndata) && (0 != pSaveModifier))
            {
                ui->tableWidget->setItem(rowSpell, col, new NumberTableItem<int8_t>(m_game, pSaveModifier, 4, SHOWNUMBER_alwaysPlus));
            }
        }
    }
}

void DialogTables::slot_addRow_to_SpellData(int row)
{
    MoM::Upkeep_Enchantments* upkeepEnchantments = 0;
    if (!m_game.isNull())
    {
        if (0 != m_game->getDataSegment())
        {
            upkeepEnchantments = &m_game->getDataSegment()->m_Upkeep_Enchantments;
        }
    }

    int spellNr = row + 1;
    MoM::eSpell spell = (MoM::eSpell)spellNr;
    MoM::Spell_Data* data = m_game->getSpellData(spell);

    MoM::eRealm_Type realm = (MoM::eRealm_Type)((spellNr - 1) / 40);
    QColor color = getRealmColor(realm);

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(spellNr, 3)));

    ui->tableWidget->setItem(row, col, new TextTableItem(m_game, data->m_SpellName, sizeof(data->m_SpellName)));
    ui->tableWidget->item(row, col)->setForeground(QBrush(color));
    ui->tableWidget->item(row, col++)->setToolTip(MoM::QMoMResources::instance().getHelpText(spell).c_str());

    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_AI_Category, 4));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_AI_Desirability, 4));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpellCategory>(m_game, &data->m_Spell_Category, MoM::eSpellCategory_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eSpell_Type>(m_game, &data->m_Section_in_spell_book, MoM::eSpell_Type_MAX));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eRealm_Type>(m_game, &data->m_Magic_Realm, MoM::eRealm_Type_MAX));

    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Casting_eligibility, 4));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Casting_cost, 5));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Research_cost, 5));

    QMoMTableItemBase* itemParameters[4] = { 0, 0, 0, 0 };
    switch (data->m_Spell_Category)
    {
    case MoM::SPELLCATEGORY_Normal_summon:
        itemParameters[0] = new QMoMTableItemBase(m_game, prettyQStr((MoM::eUnit_Type)data->m_Parameters[0]));
        break;
    case MoM::SPELLCATEGORY_Unit_enchantment:
    case MoM::SPELLCATEGORY_Mundane_Unit_enchantment:
        itemParameters[0] = new BitmaskTableItem<uint32_t, MoM::eUnitEnchantment>(
                    m_game, (uint32_t*)&data->m_Parameters[0],
                    (MoM::eUnitEnchantment)0, MoM::eUnitEnchantment_MAX);
        itemParameters[1] = new QMoMTableItemBase(m_game, QString());
        itemParameters[2] = new QMoMTableItemBase(m_game, QString());
        break;
    case MoM::SPELLCATEGORY_Friendly_City_enchantment:
    case MoM::SPELLCATEGORY_Hostile_City_enchantment:
        break;
    case MoM::SPELLCATEGORY_Fixed_damage:
        itemParameters[0] = new NumberTableItem<uint8_t>(m_game, &data->m_Parameters[0], 4, SHOWNUMBER_normal);
        itemParameters[1] = new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                                     m_game, &data->m_Parameters[1],
                                     MoM::UNITABILITY_Fire_Immunity, MoM::UNITABILITY_Weapon_Immunity);
        itemParameters[2] = new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                                     m_game, (uint16_t*)&data->m_Parameters[2],
                                     MoM::UNITABILITY_Armor_Piercing, MoM::eUnitAbility_MAX);
        break;
    case MoM::SPELLCATEGORY_Variable_damage:
        itemParameters[0] = new NumberTableItem<uint8_t>(m_game, &data->m_Parameters[0], 4, SHOWNUMBER_normal);
        itemParameters[1] = new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                                     m_game, &data->m_Parameters[1],
                                     MoM::UNITABILITY_Fire_Immunity, MoM::UNITABILITY_Weapon_Immunity);
        itemParameters[2] = new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                                     m_game, (uint16_t*)&data->m_Parameters[2],
                                     MoM::UNITABILITY_Armor_Piercing, MoM::eUnitAbility_MAX);
        break;
    case MoM::SPELLCATEGORY_Special:
        break;
    case MoM::SPELLCATEGORY_Target_wizard:
        itemParameters[0] = new NumberTableItem<uint8_t>(m_game, &data->m_Parameters[0], 4, SHOWNUMBER_normal);
        break;
    case MoM::SPELLCATEGORY_Global_enchantment:
        break;
    case MoM::SPELLCATEGORY_Battle:
        itemParameters[0] = new NumberTableItem<uint8_t>(m_game, &data->m_Parameters[0], 4, SHOWNUMBER_normal);
        itemParameters[1] = new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                                     m_game, &data->m_Parameters[1],
                                     MoM::UNITABILITY_Fire_Immunity, MoM::UNITABILITY_Weapon_Immunity);
        itemParameters[2] = new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                                     m_game, (uint16_t*)&data->m_Parameters[2],
                                     MoM::UNITABILITY_Armor_Piercing, MoM::eUnitAbility_MAX);
        break;
    case MoM::SPELLCATEGORY_Create_item:
        break;
    case MoM::SPELLCATEGORY_Destroy_unit:
        itemParameters[1] = new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                                     m_game, &data->m_Parameters[1],
                                     MoM::UNITABILITY_Fire_Immunity, MoM::UNITABILITY_Weapon_Immunity);
        break;
    case MoM::SPELLCATEGORY_Resistable_Combat_enchantment:
    case MoM::SPELLCATEGORY_Unresistable_Combat_enchantment:
    case MoM::SPELLCATEGORY_Mundane_Combat_enchantment:
        itemParameters[0] = new BitmaskTableItem<uint16_t, MoM::eCombatEnchantment>(
                    m_game, (uint16_t*)&data->m_Parameters[0],
                    (MoM::eCombatEnchantment)0, MoM::eCombatEnchantment_MAX);
        itemParameters[1] = new QMoMTableItemBase(m_game, QString());
        break;
    case MoM::SPELLCATEGORY_Dispel:
    case MoM::SPELLCATEGORY_Disenchant:
    case MoM::SPELLCATEGORY_Disjunction:
    case MoM::SPELLCATEGORY_Counter:
    case MoM::SPELLCATEGORY_Banish:
    default:
        break;
    }

    for (int i = 0; i < 3; ++i)
    {
        if (0 != itemParameters[i])
        {
            ui->tableWidget->setItem(row, col++, itemParameters[i]);
        }
        else if (i < 2)
        {
            ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, (uint8_t*)&data->m_Parameters[i], 6, SHOWNUMBER_noZero));
        }
        else
        {
            ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, (uint16_t*)&data->m_Parameters[i], 6, SHOWNUMBER_noZero));
        }
    }

    uint16_t* pUpkeep = 0;
    ui->tableWidget->setItem(row, col, new QMoMTableItemBase(m_game));
    if (0 != upkeepEnchantments)
    {
        switch (spell)
        {
        // Unit_Enchantments
        case MoM::SPELL_Guardian_Wind: pUpkeep = &upkeepEnchantments->Guardian_Wind; break;
        case MoM::SPELL_Berserk: pUpkeep = &upkeepEnchantments->Berserk; break;
        case MoM::SPELL_Cloak_of_Fear: pUpkeep = &upkeepEnchantments->Cloak_of_Fear; break;
        case MoM::SPELL_Black_Channels: pUpkeep = &upkeepEnchantments->Black_Channels; break;
        case MoM::SPELL_Wraith_Form: pUpkeep = &upkeepEnchantments->Wraith_Form; break;
        case MoM::SPELL_Regeneration: pUpkeep = &upkeepEnchantments->Regeneration; break;
        case MoM::SPELL_Path_Finding: pUpkeep = &upkeepEnchantments->Path_Finding; break;
        case MoM::SPELL_Water_Walking: pUpkeep = &upkeepEnchantments->Water_Walking; break;
        case MoM::SPELL_Resist_Elements: pUpkeep = &upkeepEnchantments->Resist_Elements; break;
        case MoM::SPELL_Elemental_Armor: pUpkeep = &upkeepEnchantments->Elemental_Armor; break;
        case MoM::SPELL_Stone_Skin: pUpkeep = &upkeepEnchantments->Stone_Skin; break;
        case MoM::SPELL_Iron_Skin: pUpkeep = &upkeepEnchantments->Iron_Skin; break;
        case MoM::SPELL_Endurance: pUpkeep = &upkeepEnchantments->Endurance; break;
        case MoM::SPELL_Spell_Lock: pUpkeep = &upkeepEnchantments->Spell_Lock; break;
        case MoM::SPELL_Invisibility: pUpkeep = &upkeepEnchantments->Invisibility; break;
        case MoM::SPELL_Wind_Walking: pUpkeep = &upkeepEnchantments->Wind_Walking; break;
        case MoM::SPELL_Flight: pUpkeep = &upkeepEnchantments->Flight; break;
        case MoM::SPELL_Resist_Magic: pUpkeep = &upkeepEnchantments->Resist_Magic; break;
        case MoM::SPELL_Magic_Immunity: pUpkeep = &upkeepEnchantments->Magic_Immunity; break;
        case MoM::SPELL_Flame_Blade: pUpkeep = &upkeepEnchantments->Flame_Blade; break;
        case MoM::SPELL_Eldritch_Weapon: pUpkeep = &upkeepEnchantments->Eldritch_Weapon; break;
        case MoM::SPELL_True_Sight: pUpkeep = &upkeepEnchantments->True_Sight; break;
        case MoM::SPELL_Holy_Weapon: pUpkeep = &upkeepEnchantments->Holy_Weapon; break;
        case MoM::SPELL_Heroism: pUpkeep = &upkeepEnchantments->Heroism; break;
        case MoM::SPELL_Bless: pUpkeep = &upkeepEnchantments->Bless; break;
        case MoM::SPELL_Lionheart: pUpkeep = &upkeepEnchantments->Lionheart; break;
        case MoM::SPELL_Giant_Strength: pUpkeep = &upkeepEnchantments->Giant_Strength; break;
        case MoM::SPELL_Planar_Travel: pUpkeep = &upkeepEnchantments->Planar_Travel; break;
        case MoM::SPELL_Holy_Armor: pUpkeep = &upkeepEnchantments->Holy_Armor; break;
        case MoM::SPELL_Righteousness: pUpkeep = &upkeepEnchantments->Righteousness; break;
        case MoM::SPELL_Invulnerability: pUpkeep = &upkeepEnchantments->Invulnerability; break;

        // City Enchantments
        case MoM::SPELL_Wall_of_Fire: pUpkeep = &upkeepEnchantments->Wall_of_Fire; break;
        case MoM::SPELL_Chaos_Rift: pUpkeep = &upkeepEnchantments->Chaos_Rift; break;
        case MoM::SPELL_Dark_Rituals: pUpkeep = &upkeepEnchantments->Dark_Rituals; break;
        case MoM::SPELL_Evil_Presence: pUpkeep = &upkeepEnchantments->Evil_Presence; break;
        case MoM::SPELL_Cursed_Lands: pUpkeep = &upkeepEnchantments->Cursed_Lands; break;
        case MoM::SPELL_Pestilence: pUpkeep = &upkeepEnchantments->Pestilence; break;
        case MoM::SPELL_Cloud_Of_Shadow: pUpkeep = &upkeepEnchantments->Cloud_of_Shadow; break;
        case MoM::SPELL_Famine: pUpkeep = &upkeepEnchantments->Famine; break;
        case MoM::SPELL_Flying_Fortress: pUpkeep = &upkeepEnchantments->Flying_Fortress; break;
        // TODO: SPELL_Spell_Ward
//            case MoM::SPELL_Nature_Ward: upkeep = upkeepEnchantments->Nature_Ward; break;
//            case MoM::SPELL_Sorcery_Ward: upkeep = upkeepEnchantments->Sorcery_Ward; break;
//            case MoM::SPELL_Chaos_Ward: upkeep = upkeepEnchantments->Chaos_Ward; break;
//            case MoM::SPELL_Life_Ward: upkeep = upkeepEnchantments->Life_Ward; break;
//            case MoM::SPELL_Death_Ward: upkeep = upkeepEnchantments->Death_Ward; break;
        case MoM::SPELL_Natures_Eye: pUpkeep = &upkeepEnchantments->Natures_Eye; break;
        case MoM::SPELL_Earth_Gate: pUpkeep = &upkeepEnchantments->Earth_Gate; break;
        case MoM::SPELL_Stream_of_Life: pUpkeep = &upkeepEnchantments->Stream_of_Life; break;
        case MoM::SPELL_Gaias_Blessing: pUpkeep = &upkeepEnchantments->Gaias_Blessing; break;
        case MoM::SPELL_Inspirations: pUpkeep = &upkeepEnchantments->Inspirations; break;
        case MoM::SPELL_Prosperity: pUpkeep = &upkeepEnchantments->Prosperity; break;
        case MoM::SPELL_Astral_Gate: pUpkeep = &upkeepEnchantments->Astral_Gate; break;
        case MoM::SPELL_Heavenly_Light: pUpkeep = &upkeepEnchantments->Heavenly_Light; break;
        case MoM::SPELL_Consecration: pUpkeep = &upkeepEnchantments->Consecration; break;
        case MoM::SPELL_Wall_of_Darkness: pUpkeep = &upkeepEnchantments->Wall_of_Darkness; break;
        case MoM::SPELL_Altar_of_Battle: pUpkeep = &upkeepEnchantments->Altar_of_Battle; break;
        // TODO: ?
//            case MoM::SPELL_Nightshade: upkeep = upkeepEnchantments->Nightshade; break;

        // Global Enchantments
        case MoM::SPELL_Eternal_Night: pUpkeep = &upkeepEnchantments->Eternal_Night; break;
        case MoM::SPELL_Evil_Omens: pUpkeep = &upkeepEnchantments->Evil_Omens; break;
        case MoM::SPELL_Zombie_Mastery: pUpkeep = &upkeepEnchantments->Zombie_Mastery; break;
        case MoM::SPELL_Aura_of_Majesty: pUpkeep = &upkeepEnchantments->Aura_of_Majesty; break;
        case MoM::SPELL_Wind_Mastery: pUpkeep = &upkeepEnchantments->Wind_Mastery; break;
        case MoM::SPELL_Suppress_Magic: pUpkeep = &upkeepEnchantments->Suppress_Magic; break;
        case MoM::SPELL_Time_Stop: pUpkeep = &upkeepEnchantments->Time_Stop; break;
        case MoM::SPELL_Nature_Awareness: pUpkeep = &upkeepEnchantments->Nature_Awareness; break;
        case MoM::SPELL_Natures_Wrath: pUpkeep = &upkeepEnchantments->Natures_Wrath; break;
        case MoM::SPELL_Herb_Mastery: pUpkeep = &upkeepEnchantments->Herb_Mastery; break;
        case MoM::SPELL_Chaos_Surge: pUpkeep = &upkeepEnchantments->Chaos_Surge; break;
        case MoM::SPELL_Doom_Mastery: pUpkeep = &upkeepEnchantments->Doom_Mastery; break;
        case MoM::SPELL_Great_Wasting: pUpkeep = &upkeepEnchantments->Great_Wasting; break;
        case MoM::SPELL_Meteor_Storm: pUpkeep = &upkeepEnchantments->Meteor_Storm; break;
        case MoM::SPELL_Armageddon: pUpkeep = &upkeepEnchantments->Armageddon; break;
        case MoM::SPELL_Tranquility: pUpkeep = &upkeepEnchantments->Tranquility; break;
        case MoM::SPELL_Life_Force: pUpkeep = &upkeepEnchantments->Life_Force; break;
        case MoM::SPELL_Crusade: pUpkeep = &upkeepEnchantments->Crusade; break;
        case MoM::SPELL_Just_Cause: pUpkeep = &upkeepEnchantments->Just_Cause; break;
        case MoM::SPELL_Holy_Arms: pUpkeep = &upkeepEnchantments->Holy_Arms; break;
        case MoM::SPELL_Planar_Seal: pUpkeep = &upkeepEnchantments->Planar_Seal; break;
        case MoM::SPELL_Charm_of_Life: pUpkeep = &upkeepEnchantments->Charm_of_Life; break;
        case MoM::SPELL_Detect_Magic: pUpkeep = &upkeepEnchantments->Detect_Magic; break;
        case MoM::SPELL_Awareness: pUpkeep = &upkeepEnchantments->Awareness; break;
        default: break;
        }

        if (0 != pUpkeep)
        {
            ui->tableWidget->setItem(row, col, new NumberTableItem<uint16_t>(m_game, pUpkeep, 5));
        }
    }
    col++;

    // Spell save
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game));
}

void DialogTables::update_UnitTypes()
{
    int ndata = 0;

    if ((0 != m_game) && (0 != m_game->getUnitTypeData((MoM::eUnit_Type)0)))
	{
		ndata = MoM::eUnit_Type_MAX;
	}

    QStringList labels;
    labels << "Nr" << "Race" << "UnitName"
            << "Fig" << "Me" << "Ra" << "Df" << "Re" << "Hp" << "Th" << "Move"
            << "RangedType" << "Shots" << "Gaze/Poison"
            << "Cost" << "Upkeep" << "Parm1" << "Parm2"
            << "Movement" << "Immunities" << "Attributes" << "Abilities" << "Attacks"
            << "TypeCode" << "Scout" << "Transport" << "Construction";

    buildTable(labels, ndata, SLOT(slot_addRow_to_UnitTypes(int)));
}

void DialogTables::slot_addRow_to_UnitTypes(int row)
{
    int unitTypeNr = row;
    MoM::Unit_Type_Data* data = m_game->getUnitTypeData((MoM::eUnit_Type)unitTypeNr);
    if (0 == data)
        return;

    QColor raceColor = getRealmColor(m_game->getRealmRace(data->m_Race_Code));
    QColor rangedColor = getRealmColor(m_game->getRealmRangedType(data->m_Ranged_Type));

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(unitTypeNr, 3)));

    ui->tableWidget->setItem(row, col, new EnumTableItem<MoM::eRace>(m_game, &data->m_Race_Code, MoM::eRace_MAX));
    ui->tableWidget->item(row, col++)->setForeground(raceColor);

    ui->tableWidget->setItem(row, col, new QMoMTableItemBase(m_game, m_game->getNameByOffset(data->m_PtrName)));
    ui->tableWidget->item(row, col++)->setForeground(raceColor);

    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Nr_Figures, 3));
    ui->tableWidget->setItem(row, col, new NumberTableItem<uint8_t>(m_game, &data->m_Melee, 3, SHOWNUMBER_noZero));
    ui->tableWidget->item(row, col++)->setForeground(raceColor);
    ui->tableWidget->setItem(row, col, new NumberTableItem<uint8_t>(m_game, &data->m_Ranged, 3, SHOWNUMBER_noZero));
    ui->tableWidget->item(row, col++)->setForeground(rangedColor);
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Defense, 3));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Resistance, 3));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Hitpoints, 3));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_To_Hit, 2, SHOWNUMBER_plusAndNoZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_MoveHalves, 5, SHOWNUMBER_halfMove));
    ui->tableWidget->setItem(row, col, new EnumTableItem<MoM::eRanged_Type>(m_game, &data->m_Ranged_Type, MoM::eRanged_Type_MAX, SHOWENUM_minusOne));
    ui->tableWidget->item(row, col++)->setForeground(rangedColor);
    ui->tableWidget->setItem(row, col, new NumberTableItem<uint8_t>(m_game, &data->m_Ranged_Shots, 3, SHOWNUMBER_noZero));
    ui->tableWidget->item(row, col++)->setForeground(rangedColor);
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Gaze_Modifier, 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, &data->m_Cost, 5));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Upkeep, 3));

    if (unitTypeNr < (int)MoM::gMAX_HERO_TYPES)
    {
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Building1Required_or_PortraitLbxIndex, 4));
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eHero_TypeCode>(m_game, (MoM::eHero_TypeCode*)&data->m_Building2_or_HeroType, MoM::eHero_TypeCode_MAX));
    }
    else if (unitTypeNr < MoM::UNITTYPE_Arcane_Magic_Spirit)
    {
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding8>(m_game, (MoM::eBuilding8*)&data->m_Building1Required_or_PortraitLbxIndex, MoM::eBuilding8_array_MAX, SHOWENUM_noZero));
        ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBuilding8>(m_game, (MoM::eBuilding8*)&data->m_Building2_or_HeroType, MoM::eBuilding8_array_MAX, SHOWENUM_noZero));
    }
    else
    {
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Building1Required_or_PortraitLbxIndex, 4, SHOWNUMBER_noZero));
        ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, (uint8_t*)&data->m_Building2_or_HeroType, 4, SHOWNUMBER_noZero));
    }

    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                              m_game, &data->m_Movement_Flags.bits,
                              MoM::UNITABILITY_Cavalry, MoM::Succ(MoM::UNITABILITY_Merging)));

    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint8_t, MoM::eUnitAbility>(
                              m_game, &data->m_Immunity_Flags.bits,
                              MoM::UNITABILITY_Fire_Immunity, MoM::UNITABILITY_Weapon_Immunity));

    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                              m_game, &data->m_Attribute_Flags.bits,
                              MoM::UNITABILITY_Weapon_Immunity, MoM::Succ(MoM::UNITABILITY_Holy_Bonus)));

    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                              m_game, &data->m_Ability_Flags.bits,
                              MoM::UNITABILITY_Summoned_Unit, MoM::UNITABILITY_Armor_Piercing));

    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint16_t, MoM::eUnitAbility>(
                              m_game, &data->m_Attack_Flags.bits,
                              MoM::UNITABILITY_Armor_Piercing, MoM::eUnitAbility_MAX));

    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint16_t>(m_game, &data->m_Unit_picture, 3));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Scouting, 3));

    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Transport_Capacity, 3, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<uint8_t>(m_game, &data->m_Construction, 3, SHOWNUMBER_noZero));
}

void DialogTables::update_UnitsInGame()
{
    int ndata = 0;
    if ((0 != m_game) && (0 != m_game->getUnit(0)))
    {
        ndata = m_game->getNrUnits();
    }

    QStringList labels;
    labels << "Nr";
    labels << "XPos" << "YPos" << "Plane" << "Owner" << "MaxMove" << "Type" << "HeroSlot"  << "Active"  << "CurMove" << "XDest" << "YDest"
           << "Status" << "Level" << "Experience" << "Lifedrain??" << "Damage" << "Grouping" << "Combat??" << "TowerPass" << "Combat??"
           << "Scout" << "Weapon" << "Mutation" << "Enchantment" << "RoadLeft" << "XRoad" << "YRoad";

    buildTable(labels, ndata, SLOT(slot_addRow_to_UnitsInGame(int)));
}

void DialogTables::slot_addRow_to_UnitsInGame(int row)
{
    MoM::Unit* data = m_game->getUnit(row);
    if (0 == data)
        return;

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_XPos, 2));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_YPos, 2));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::ePlane>(m_game, &data->m_Plane, MoM::ePlane_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::ePlayer>(m_game, &data->m_Owner, MoM::ePlayer_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Moves_Total, 5, SHOWNUMBER_halfMove));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eUnit_Type>(m_game, &data->m_Unit_Type, MoM::eUnit_Type_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Hero_Slot_Number, 5, SHOWNUMBER_normal));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eUnit_Active>(m_game, &data->m_Active, MoM::eUnit_Active_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Moves_Left, 5, SHOWNUMBER_halfMove));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_XPos_of_destination, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_YPos_of_destination, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eUnit_Status8>(m_game, &data->m_Status, MoM::eUnit_Status8_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eLevel>(m_game, &data->m_Level, MoM::eLevel_MAX, SHOWENUM_minusOne));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int16_t>(m_game, &data->m_Experience, 4));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Unk_10, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Damage, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Grouping, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Guess_Combat_Enchantment_Flag1, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_In_Tower_without_Seal, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Guess_Combat_Enchantment_Flag3, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Scouting, 2));
    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eWeaponType>(m_game, (MoM::eWeaponType*)&data->m_Weapon_Mutation.bits, MoM::eWeaponType_MAX, SHOWENUM_noZero, 0x3));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint8_t, MoM::eUnitMutation>(
                              m_game, &data->m_Weapon_Mutation.bits,
                              (MoM::eUnitMutation)0, MoM::eUnitMutation_MAX, 0xFC));
    ui->tableWidget->setItem(row, col++, new BitmaskTableItem<uint32_t, MoM::eUnitEnchantment>(
                              m_game, &data->m_Unit_Enchantment.bits,
                              (MoM::eUnitEnchantment)0, MoM::eUnitEnchantment_MAX));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_Road_Building_left_to_complete, 2));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_XPos_Road_Building_from, 2, SHOWNUMBER_noZero));
    ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data->m_YPos_Road_Building_from, 2, SHOWNUMBER_noZero));

}

void DialogTables::update_UnrestTable()
{
    int8_t* unrestTable = 0;
    int ndata = 0;

    if (!m_game.isNull())
    {
        unrestTable = m_game->getUnrest_Table(MoM::RACE_Barbarian);
    }
    if (0 != unrestTable)
    {
        ndata = (int)MoM::gMAX_RACES;
    }

    QStringList labels;
    labels << "Nr" << "HomeRace";
    for (MoM::eRace race = (MoM::eRace)0; MoM::toUInt(race) < MoM::gMAX_RACES; MoM::inc(race))
    {
        labels << prettyQStr(race);
    }

    buildTable(labels, ndata, SLOT(slot_addRow_to_UnrestTable(int)));
}

void DialogTables::slot_addRow_to_UnrestTable(int row)
{
    int raceNr = row;
    MoM::eRace homeRace = (MoM::eRace)raceNr;
    int8_t* data = m_game->getUnrest_Table(homeRace);

    int col = 0;
    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(raceNr, 2)));
    ui->tableWidget->setItem(row, col, new QMoMTableItemBase(m_game, prettyQStr(homeRace)));
    ui->tableWidget->item(row, col++)->setToolTip(MoM::QMoMResources::instance().getHelpText(homeRace).c_str());

    for (MoM::eRace cityRace = (MoM::eRace)0; MoM::toUInt(cityRace) < MoM::gMAX_RACES; MoM::inc(cityRace))
    {
        ui->tableWidget->setItem(row, col++, new NumberTableItem<int8_t>(m_game, &data[cityRace], 3, SHOWNUMBER_plusAndNoZero));
    }
}

//void DialogTables::update_Wizards()
//{
//    int ndata = 0;
//    if (!m_game.isNull())
//    {
//        ndata = m_game->getNrWizards();
//    }

//    QStringList labels;
//    labels << "Nr";
//    labels << "Name" << "Banner" << "Power" << "SkillCombat" << "Mana" << "Gold" << "Skills" << "Spells known" << "Global enchantments";

//    buildTable(labels, ndata, SLOT(slot_addRow_to_Wizards(int)));
//}

//void DialogTables::slot_addRow_to_Wizards(int row)
//{
//    MoM::Wizard* data = m_game->getWizard(row);
//    if (0 == data)
//        return;

//    int col = 0;
//    ui->tableWidget->setItem(row, col++, new QMoMTableItemBase(m_game, QString("%0").arg(row, 3)));

//    ui->tableWidget->setItem(row, col++, new TextTableItem(m_game, data->m_Name, sizeof(data->m_Name)));
//    ui->tableWidget->setItem(row, col++, new EnumTableItem<MoM::eBannerColor>(m_game, data->m_BannerColor, MoM::eBannerColor_MAX));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<MoM::uint16_t>(m_game, data->m_Power_Base, 4));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<MoM::int16_t>(m_game, data->m_Skill_Left_in_combat, 4));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<MoM::int16_t>(m_game, data->m_Mana_Crystals, 5));
//    ui->tableWidget->setItem(row, col++, new NumberTableItem<MoM::int16_t>(m_game, data->m_Gold_Coins, 5));

////    MoM::unionSkills     m_Skills;                           // 064
////    MoM::Spells_Known    m_Spells_Known;                     // 263 Spells (0-3) (None-Na-So-Ch-Li-De-Ar)
////    MoM::Global_Enchantments  m_Global_Enchantments;         // 482

//}

void DialogTables::on_comboBox_Table_currentIndexChanged(QString newIndex)
{
    if (ui->tableWidget->rowCount() > 0)
    {
        // Remember the settings of the table view, but only if there was actual data in it
        QMoMSettings::writeSettingsControl(ui->tableWidget);
    }

    setWindowTitle(newIndex);

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setObjectName(newIndex);

    if ("Building Data" == newIndex)
    {
        update_BuildingData();
    }
    else if ("Cities" == newIndex)
    {
        update_Cities();
    }
    else if ("Hero Data" == newIndex)
    {
        update_HeroData();
    }
    else if ("Item Data" == newIndex)
    {
        update_ItemData();
    }
    else if ("Item Powers" == newIndex)
    {
        update_ItemPowers();
    }
    else if ("Items in game" == newIndex)
    {
        update_ItemsInGame();
    }
    else if ("Level Bonus" == newIndex)
    {
        update_LevelBonus();
    }
    else if ("Race Data" == newIndex)
    {
        update_RaceData();
    }
    else if ("Spell Data" == newIndex)
    {
        update_SpellData();
    }
    else if ("Unit Types" == newIndex)
    {
        update_UnitTypes();
    }
    else if ("Units in game" == newIndex)
    {
        update_UnitsInGame();
    }
    else if ("Unrest Table" == newIndex)
    {
        update_UnrestTable();
    }
    else
    {
        // Nothing to show
    }

    // Retrieve the settings of the table view (if there are any)
    QMoMSettings::readSettingsControl(ui->tableWidget);
}

void DialogTables::on_tableWidget_clicked(QModelIndex)
{
    ui->tableWidget->resizeRowsToContents();
}

void DialogTables::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
    //qDebug() << "on_tableWidget_customContextMenuRequested" << pos;
    QTableWidgetItem* pItem = ui->tableWidget->currentItem();

    QMenu contextMenu;
    contextMenu.addAction("Copy", this, SLOT(slot_Copy()), QKeySequence("Ctrl+C"));

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
            connect(action, SIGNAL(triggered()), this, SLOT(slot_ItemAction()));
        }
    }

    contextMenu.exec(ui->tableWidget->viewport()->mapToGlobal(pos));
}

void DialogTables::slot_Copy()
{
    QByteArray byteArray;

    int leftColumn = ui->tableWidget->columnCount();
    int rightColumn = 0;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item->isSelected())
            {
                leftColumn = std::min(col, leftColumn);
                rightColumn = std::max(col, rightColumn);
            }
        }
    }

    for (int col = leftColumn; col <= rightColumn; ++col)
    {
        byteArray.append(ui->tableWidget->horizontalHeaderItem(col)->text());
        byteArray.append("\t");
    }
    byteArray.append("\r\n");

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        bool selectedItems = false;
        for (int col = leftColumn; col <= rightColumn; ++col)
        {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item->isSelected())
            {
                byteArray.append(item->text());
                byteArray.append("\t");
                selectedItems = true;
           }
        }
        if (selectedItems)
        {
            byteArray.append("\r\n");
        }
    }

    QMimeData * mimeData = new QMimeData();
    mimeData->setData("text/plain", byteArray);
    QApplication::clipboard()->setMimeData(mimeData);
}

void DialogTables::slot_ItemAction()
{
    QTableWidgetItem* pItem = ui->tableWidget->currentItem();
    QMoMTableItemBase* pMoMItem = dynamic_cast<QMoMTableItemBase*>(pItem);
    if (0 != pMoMItem)
    {
        pMoMItem->slotAction();
    }
}

void DialogTables::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;

    // Retrigger generation of selected table
    on_comboBox_Table_currentIndexChanged(ui->comboBox_Table->currentText());

    slot_gameUpdated();
}

void DialogTables::slot_gameUpdated()
{
}
