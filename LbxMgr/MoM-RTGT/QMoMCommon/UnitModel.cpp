// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

#include <QDateTime>
#include <QtGui>

#include <sstream>

#include <MoMController.h>
#include <MoMGameCustom.h>
#include <MoMGenerated.h>
#include <MoMLbxBase.h>
#include <MoMUnit.h>
#include <MoMUtility.h>
#include <MoMExeWizards.h>
#include <QMoMGenerated.h>
#include <QMoMUtility.h>

#include "QMoMResources.h"

#include "UnitModel.h"

using namespace MoM;
using MoM::QMoMResources;

QString toQStr(const QModelIndex& index)
{
	return QString("QModelIndex row=%0 col=%1").arg(index.row()).arg(index.column());
}

UnitModel::UnitModel(QObject *parent) :
    QAbstractItemModel(parent),
    m_rootItem(new QMoMTreeItemModelBase),
	m_abortUpdate(false)
{
}

UnitModel::~UnitModel()
{
	try
	{
		abortUpdate();
	}
	catch (...)
	{
		// Empty
	}
}

//void UnitModel::setGame(const QMoMGamePtr& game)
//{
//    setupModelData(game);
//}

int UnitModel::columnCount(const QModelIndex& /*parent*/) const
{
    return m_columnCount;
}

QVariant UnitModel::data(const QModelIndex& index, int role) const
{
//    qDebug() << QString("UnitModel::data(%0, role=%1)").arg(toQStr(index)).arg((Qt::ItemDataRole)role);

    if (!index.isValid())
        return QVariant();

    QMoMTreeItemModelBase* item = reinterpret_cast<QMoMTreeItemModelBase*> (index.internalPointer());

    return item->data(role);
}

Qt::ItemFlags UnitModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    QMoMTreeItemModelBase* item = reinterpret_cast<QMoMTreeItemModelBase*> (index.internalPointer());
    return item->flags();
}

QVariant UnitModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant header;
    if ((orientation == Qt::Horizontal) && (role == Qt::DisplayRole))
    {
        switch (section)
        {
        case 0:
            header = tr("Feature");
            break;
        case 1:
            header = tr("Value");
            break;
        case 2:
            header = tr("Comment");
            break;
        default:
            break;
        }
    }
    else
    {
        // Nothing to do
    }
    return header;
}

QModelIndex UnitModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    QMoMTreeItemModelBase *parentItem;

    if (!parent.isValid())
        parentItem = m_rootItem.data();
    else
        parentItem = static_cast<QMoMTreeItemModelBase*> (parent.internalPointer());

    QMoMTreeItemModelBase *childItem = parentItem->child(row, column);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex UnitModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    QMoMTreeItemModelBase *childItem = static_cast<QMoMTreeItemModelBase*> (index.internalPointer());
    QMoMTreeItemModelBase *parentItem = childItem->parent();

    if (parentItem == m_rootItem.data())
    {
        return QModelIndex();
    }
    else
    {
        return createIndex(parentItem->row(), 0, parentItem);
    }
}

int UnitModel::rowCount(const QModelIndex &parent) const
{
    QMoMTreeItemModelBase *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootItem.data();
    else
        parentItem = static_cast<QMoMTreeItemModelBase*> (parent.internalPointer());

    if (0 == parentItem)
    {
        return 0;
    }
    else
    {
        return parentItem->rowCount();
    }
}

bool UnitModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
//    qDebug() << QString("UnitModel: setData(%0, %1, role=%2)").arg(toQStr(index)).arg(value.value<QString>()).arg((Qt::ItemDataRole)role);
    if (index.isValid() && role == Qt::EditRole)
    {
        QMoMTreeItemModelBase* item = reinterpret_cast<QMoMTreeItemModelBase*> (index.internalPointer());
        item->setData(value, role);

//        qDebug() << QString("UnitModel: emit dataChanged(%0, %1)").arg(toQStr(index)).arg(toQStr(index));
        emit dataChanged(index, index);
//        qDebug() << QString("UnitModel: after emit dataChanged(%0, %1)").arg(toQStr(index)).arg(toQStr(index));

        return true;
    }
    return false;
}

void UnitModel::slot_gameChanged(const QMoMGamePtr &game)
{
//    qDebug() << getDateTimeStr() << ">UnitModel::slot_gameChanged() begin";
    // Abort an old update if any
    abortUpdate();

    // Destroy all references to the old game and make sure they are complete
    QMoMGamePtr emptyGame;
    QMoMTreeItemModelBase::setGame(emptyGame);
    startUpdate();
    waitForUpdate();

    // Install new game
    MoM::QMoMResources::instance().setGame(game);	// Load resources if they are changed
    QMoMTreeItemModelBase::setGame(game);
    startUpdate();

//    qDebug() << getDateTimeStr() << "<UnitModel::slot_gameChanged() end";
}

void UnitModel::slot_gameUpdated()
{
    // Abort an old update if any
    abortUpdate();

    // Start a new update asynchronously
    startUpdate();
}

void UnitModel::slot_selectionChanged(const QModelIndex &index)
{
    // Check if a unit was clicked.
    // If so, emit a signal
    QMoMTreeItemModelBase* itemBase = reinterpret_cast<QMoMTreeItemModelBase*> (index.internalPointer());

    checkUnitChanged<MoM::Battle_Unit>(itemBase);
    checkUnitChanged<MoM::Hero_stats>(itemBase);
    checkUnitChanged<MoM::Hired_Hero>(itemBase);
    checkUnitChanged<MoM::Unit>(itemBase);
    checkUnitChanged<MoM::Unit_Type_Data>(itemBase);
    checkUnitChanged<MoM::Hero_Stats_Initializer>(itemBase);

    // Emit a signal for the change in pointer to a MoM item
    emit signal_addressChanged(itemBase->getVoidMoMPointer());
}

template<class T>
void UnitModel::checkUnitChanged(QMoMTreeItemModelBase* itemBase)
{
    QMoMTreeItemModelSubtree<T>* itemSubtree = dynamic_cast< QMoMTreeItemModelSubtree<T>* >(itemBase);
    if (0 != itemSubtree)
    {
        T* t = itemSubtree->getMoMPointer();
        QMoMUnitPtr momUnit(new MoM::MoMUnit(QMoMTreeItemModelBase::game().data()));
        momUnit->changeUnit(t);

        emit signal_unitChanged(momUnit);
    }
}

//void UnitModel::update()
//{
//    setupModelData(QMoMTreeItemBase::game());
//}

void UnitModel::removeUnusedRows(int toprow, QMoMTreeItemModelBase* ptree, int firstUnusedRow)
{
    if (ptree->rowCount() > firstUnusedRow)
    {
        beginRemoveRows(this->createIndex(toprow, 0, ptree), firstUnusedRow, ptree->rowCount() - 1);
        ptree->removeRows(firstUnusedRow, ptree->rowCount() - 1);
        endRemoveRows();
    }
}


void update_Battlefield(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;
    MoM::Battlefield* pBattlefield = game->getBattlefield();

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(pBattlefield, ""));
    }
    ptree->child(row, 0)->setData(toQStr("Battlefield"), Qt::UserRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString(), Qt::EditRole);

    ++row;
}

void update_Battle_Unit_View(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;
    MoM::Battle_Unit* pBattle_Unit_View = game->getBattleUnitViewed();
    if (0 == pBattle_Unit_View)
        return;

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(pBattle_Unit_View, ""));
    }
    QString id;
    int unitNr = pBattle_Unit_View->m_unitNr;
    if ((unitNr >= 0) && (unitNr < game->getNrUnits()) && (unitNr < (int)MoM::gMAX_UNITS))
    {
        MoM::Unit* unit = game->getUnit(unitNr);
        if (0 != unit)
        {
            id = toQStr(unit->m_Unit_Type);
        }
    }
    else
    {
        id = toQStr(pBattle_Unit_View->m_Race_Code);
    }
    ptree->child(row, 0)->setData(id, Qt::UserRole);
    ptree->child(row, 1)->setData(toQStr(pBattle_Unit_View->m_Owner), Qt::UserRole);
    ptree->child(row, 2)->setData(QString(), Qt::EditRole);

    ++row;
}

void update_Battle_Units(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;
    MoM::Battle_Unit* pBattleUnits = game->getBattleUnit(0);
    if (0 == pBattleUnits)
        return;
    int nrBattle_Units = game->getNrBattleUnits();

    if (row >= ptree->rowCount())
    {
        ptree->appendChild("m_Number_of_Battle_Units", new QMoMTreeItemModel<uint16_t>(game->getNumber_of_Battle_Units()));
    }
    row++;

    for (int i = 0; (i < nrBattle_Units) && (i < (int)MoM::gMAX_BATTLE_UNITS); ++i)
    {
        MoM::Battle_Unit& battleUnit = pBattleUnits[i];

        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(&battleUnit, ""));
        }
		//QMoMTreeItemSubtree<MoM::Battle_Unit>* pnode = dynamic_cast< QMoMTreeItemSubtree<MoM::Battle_Unit>* >(ptree->child(row, 0));
		//if (0 == pnode)
		//{
		//	pnode = new QMoMTreeItemSubtree<MoM::Battle_Unit>(0);
		//	ptree->setChild(row, 0, pnode);
		//}
		//constructTreeItem(pnode, &battleUnit, "");

        QString id;
        int unitNr = battleUnit.m_unitNr;
        if ((unitNr >= 0) && (unitNr < game->getNrUnits()) && (unitNr < (int)MoM::gMAX_UNITS))
        {
            MoM::Unit* unit = game->getUnit(unitNr);
            if (0 != unit)
            {
                id = toQStr(unit->m_Unit_Type);
            }
        }
        else
        {
            id = toQStr(battleUnit.m_Race_Code);
        }
        ptree->child(row, 0)->setData(id, Qt::UserRole);
        ptree->child(row, 1)->setData(toQStr(battleUnit.m_Owner), Qt::UserRole);
        ptree->child(row, 2)->setData(QString("BattleUnit[%0]").arg(i), Qt::EditRole);

        ++row;
    }
}

//class QMoMTreeItem_Buildings : public QMoMTreeItemBase
//{
//public:
//    explicit QMoMTreeItem_Buildings(T* ptr, const QString& data = QString(), const QIcon& icon = QIcon()) :
//        QMoMTreeItemBase(data, icon),
//        m_ptr(ptr)
//    {
//    }

//    T* getMoMPointer()
//    {
//        return m_ptr;
//    }

//public slots:

//signals:

//private:
//    T* m_ptr;
//};

void UnitModel::update_Buildings(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (MoM::eBuilding building = (MoM::eBuilding)0; (0 != game) && (building < MoM::eBuilding_array_MAX); MoM::inc(building))
    {
        MoM::Building_Data* buildingData = game->getBuildingData(building);
        if (0 == buildingData)
            break;
        if (building >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(buildingData, ""));

            ptree->child(row, 0)->setData(buildingData->m_BuildingName, Qt::EditRole);
            ptree->child(row, 0)->setLazyIcon(building);
            QString toolTip = MoM::QMoMResources::instance().getHelpText(building).c_str();
            ptree->child(row, 0)->setToolTip(toolTip);
//            ptree->child(row, 1)->setData(QString("(%0, %1, %2) %3")
//                                                    .arg(lair->m_XPos).arg(lair->m_YPos).arg(lair->m_Plane)
//                                                    .arg(prettyQStr(lair->m_Status)), Qt::UserRole);
            ptree->child(row, 2)->setData(QString("Building[%0]").arg((int)building), Qt::EditRole);
        }
        ++row;
    }
}


//class QMoMTreeItemSubtree_Building_Data : public QMoMTreeItemBase
//{
//public:
//    explicit QMoMTreeItemSubtree_Building_Data(MoM::Building_Data* ptr, const QString& data = QString(), const QIcon& icon = QIcon());

//    MoM::Building_Data* getMoMPointer()
//    {
//        return m_ptr;
//    }

//public slots:

//signals:

//private:
//    MoM::Building_Data* m_ptr;
//};

//QMoMTreeItemSubtree_Building_Data::QMoMTreeItemSubtree_Building_Data(MoM::Building_Data* ptr, const QString& data, const QIcon& icon) :
//    QMoMTreeItemBase(data, icon),
//    m_ptr(ptr)
//{
//    appendChild("m_BuildingName", new QMoMTreeItem<char[20]>(m_ptr->m_BuildingName));
//	appendChild("m_Prerequisite1", new QMoMTreeItem<MoM::eBuilding>(&m_ptr->m_Prerequisite1));
//    appendChild("m_Prerequisite2", new QMoMTreeItem<MoM::eBuilding>(&m_ptr->m_Prerequisite2));
//    appendChild("m_Replaces_building", new QMoMTreeItem<MoM::eBuilding>(&m_ptr->m_Replaces_building));
//    appendChild("m_Produces_Regulars", new QMoMTreeItem<MoM::eYesNo16>(&m_ptr->m_Produces_Regulars));
//    appendChild("m_Produces_Veterans", new QMoMTreeItem<MoM::eYesNo16>(&m_ptr->m_Produces_Veterans));
//    appendChild("m_Produces_Magic_Weapons", new QMoMTreeItem<MoM::eYesNo16>(&m_ptr->m_Produces_Magic_Weapons));
//    appendChild("m_Upkeep_yield", new QMoMTreeItem<int16_t>(&m_ptr->m_Upkeep_yield));
//    appendChild("m_Food_and_pop_bonus", new QMoMTreeItem<uint16_t>(&m_ptr->m_Food_and_pop_bonus));
//    appendChild("m_Zero_24", new QMoMTreeItem<uint16_t>(&m_ptr->m_Zero_24));
//    appendChild("m_Unk_26", new QMoMTreeItem<uint16_t>(&m_ptr->m_Unk_26));
//    appendChild("m_Mana_produced", new QMoMTreeItem<uint16_t>(&m_ptr->m_Mana_produced));
//    appendChild("m_Unk_2A", new QMoMTreeItem<uint16_t>(&m_ptr->m_Unk_2A));
//    appendChild("m_Building_cost", new QMoMTreeItem<uint16_t>(&m_ptr->m_Building_cost));
//    appendChild("m_Zero_2E", new QMoMTreeItem<uint16_t>(&m_ptr->m_Zero_2E));
//    appendChild("m_Unk_30", new QMoMTreeItem<uint16_t>(&m_ptr->m_Unk_30));
//    appendChild("m_Unk_32", new QMoMTreeItem<uint16_t>(&m_ptr->m_Unk_32));
//}

//class QMoMTreeItemSubtree_Buildings : public QMoMTreeItemBase
//{
//public:
//    explicit QMoMTreeItemSubtree_Buildings();

//public slots:
//	void slot_gameUpdated();

//signals:

//private:
//	void update_Buildings();
//};


//QMoMTreeItemSubtree_Buildings::QMoMTreeItemSubtree_Buildings() :
//    QMoMTreeItemBase("Buildings")
//{
//}

//void QMoMTreeItemSubtree_Buildings::slot_gameUpdated()
//{
//	QMoMTreeItemBase* parentItem = this->parent();

//	int toprow = row();

//    int nrBuildings = 0;
//    if ((0 != game()) && (0 != game()->getDataSegment()))
//    {
//        const MoM::EXE_Reloc& addr = game()->getDataSegment()->m_WizardsExe_Pointers.addr_Building_Data;
//        parentItem->child(toprow, 1)->setData(toQStr(addr), Qt::EditRole);
//    }
//    else
//    {
//        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
//    }
//    if (0 == game())
//    {
//        parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
//    }
//    else
//    {
//        nrBuildings = MoM::eBuilding_MAX;
//        parentItem->child(toprow, 2)->setData(QString("NrBuildings = %0").arg(nrBuildings), Qt::EditRole);
//    }


//    update_Buildings();
////    int row = 0;
////    if (nrBuildings > rowCount())
////    {
////        beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrBuildings - 1);
////        update_Buildings(ptree, game, row);
////        endInsertRows();
////    }
////    else
////    {
////        update_Buildings(ptree, game, row);
////    }
////    removeUnusedRows(toprow, ptree, row);

////    toprow++;
//}

//void QMoMTreeItemSubtree_Buildings::update_Buildings()
//{
//	int row = 0;
//    for (MoM::eBuilding building = (MoM::eBuilding)0; (0 != game()) && (0 != game()->getBuilding_Data()) && (building < MoM::eBuilding_MAX); MoM::inc(building))
//    {
//        MoM::Building_Data* buildingData = game()->getBuilding_Data(building);
//        if (0 == buildingData)
//            break;
//        if (building >= rowCount())
//        {
//			// Row consists of 3 cells.
//			// The first cell contains the subtree and a title+icon
//			QIcon icon = MoM::QMoMResources::instance().getIcon(building);
//			QMoMTreeItemBase* psubtree = new QMoMTreeItemSubtree_Building_Data(buildingData, toQStr(building), icon);
//            setChild(row, 0, psubtree);

//            child(row, 1)->setData(QString(), Qt::EditRole);
//            child(row, 2)->setData(QString("Building[%0]").arg((int)building), Qt::EditRole);
//        }
//        ++row;
//    }
//}

//void UnitModel::update_Buildings(QMoMTreeItemBase* ptree, const QMoMGamePtr& game, int& row)
//{
//    for (MoM::eBuilding building = (MoM::eBuilding)0; (0 != game) && (0 != game->getBuilding_Data()) && (building < MoM::eBuilding_MAX); MoM::inc(building))
//    {
//        MoM::Building_Data* buildingData = game->getBuilding_Data(building);
//        if (0 == buildingData)
//            break;
//        if (building >= ptree->rowCount())
//        {
//			QMoMTreeItemBase* psubtree = new QMoMTreeItemSubtree_Building_Data(buildingData, toQStr(building));
//            ptree->setChild(row, 0, psubtree);

//            ptree->child(row, 0)->setData(toQStr(building), Qt::UserRole);
////            ptree->child(row, 1)->setData(QString("(%0, %1, %2) %3")
////                                                    .arg(lair->m_XPos).arg(lair->m_YPos).arg(lair->m_Plane)
////                                                    .arg(prettyQStr(lair->m_Status)), Qt::UserRole);
//            ptree->child(row, 2)->setData(QString("Building[%0]").arg((int)building), Qt::EditRole);
//        }
//        ++row;
//    }
//}

void update_Cities(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (int cityNr = 0; (0 != game) && (cityNr < game->getNrCities()) && (cityNr < (int)MoM::gMAX_CITIES); ++cityNr)
    {
        MoM::City* city = game->getCity(cityNr);
        if (0 == city)
            break;
        if (cityNr >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(city, ""));
        }
        ptree->child(cityNr, 0)->setData(QString("%0 \"%1\"").arg(toQStr(city->m_Race)).arg(toQStr(city->m_City_Name)),
                Qt::UserRole);
        ptree->child(cityNr, 1)->setData(toQStr(city->m_Owner), Qt::UserRole);
        ptree->child(cityNr, 2)->setData(QString("City[%0]").arg(cityNr), Qt::EditRole);

        ++row;
    }
}

void update_Events(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;

    MoM::Events_Status* eventStatus = game->getEvents_Status();
    if (0 == eventStatus)
        return;

    {
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(eventStatus, "Events Status"));
        }

        ++row;
    }
}

void update_Game_Data(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(game->getGameData_SaveGame(), ""));
    }
    ptree->child(row, 0)->setData(QString("SAVE:Game_Data"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString("SAVEn.GAM only"), Qt::EditRole);
    ++row;

    MoMDataSegment* dataSegment = game->getDataSegment();
    MoMMagicDataSegment* magicDataSegment = game->getMagicDataSegment();

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(game->getGameData_WizardsExe(), ""));

        if ((0 != dataSegment) && game->getGameData_WizardsExe())
        {
            ptree->child(row, 0)->appendChild(QString("Copyright/Version"), new QMoMTreeItemModel<char[41]>(dataSegment->m_Copyright_and_Version));
            ptree->child(row, 0)->appendChild(QString(" RNG_Next_Turn_Seed"), new QMoMTreeItemModel<uint32_t>((uint32_t*)&dataSegment->m_Next_Turn_seed_storage_lo));
            ptree->child(row, 0)->appendChild(QString(" RNG_Seed"), new QMoMTreeItemModel<uint32_t>((uint32_t*)&dataSegment->m_RNG_seed_lo));
            ptree->child(row, 0)->appendChild(QString(" DEBUG Off"), new QMoMTreeItemModel<uint16_t>(&dataSegment->m_DEBUG_Off));
            ptree->child(row, 0)->appendChild(QString(" BIOS Clock snapshot"), new QMoMTreeItemModel<uint32_t>(&dataSegment->m_BIOS_clock_snapshot));
            ptree->child(row, 0)->appendChild(QString("Game_State"), new QMoMTreeItemModel<MoM::eGameState>(&dataSegment->m_Game_flow));
            ptree->child(row, 0)->appendChild(QString("Kyrub_ds:9294"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_kyrub_dseg_9294));
            ptree->child(row, 0)->appendChild(QString("Kyrub_ds:9296"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_kyrub_dseg_9296));
            ptree->child(row, 0)->appendChild(QString("m_Selected_Item"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_Selected_Item));
            ptree->child(row, 0)->appendChild(QString("m_Location_Item"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_Location_Item));
            ptree->child(row, 0)->appendChild(QString("XPos Item"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_XPos_Item));
            ptree->child(row, 0)->appendChild(QString("YPos Item"), new QMoMTreeItemModel<int16_t>(&dataSegment->m_YPos_Item));
            ptree->child(row, 0)->appendChild(QString("Plane Item"), new QMoMTreeItemModel<MoM::ePlane16>(&dataSegment->m_Plane_Item));
            ptree->child(row, 0)->appendChild(QString("Nr_city_queue"), new QMoMTreeItemModel<int8_t>(&dataSegment->m_nr_city_queue));
        }

        if ((0 != magicDataSegment) && game->getGameData_WizardsExe())
        {
            ptree->child(row, 0)->appendChild(QString("Copyright1/Version"), new QMoMTreeItemModel<char[41]>(magicDataSegment->m_Copyright1_and_Version));
            ptree->child(row, 0)->appendChild(QString("Copyright2/Version"), new QMoMTreeItemModel<char[41]>(magicDataSegment->m_Copyright2_and_Version));
            ptree->child(row, 0)->appendChild(QString(" RNG_Seed"), new QMoMTreeItemModel<uint32_t>((uint32_t*)&magicDataSegment->m_RNG_seed_lo));
            ptree->child(row, 0)->appendChild(QString(" DEBUG Off"), new QMoMTreeItemModel<uint16_t>(&magicDataSegment->m_DEBUG_Off));
            ptree->child(row, 0)->appendChild(QString(" BIOS Clock snapshot"), new QMoMTreeItemModel<uint32_t>(&magicDataSegment->m_BIOS_clock_snapshot));
        }
    }
    ptree->child(row, 0)->setData(QString("MEM:Game Data"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString("Memory only"), Qt::EditRole);
    ++row;

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(game->getGameSettings(), ""));
    }
    ptree->child(row, 0)->setData(QString("Game_Settings (MAGIC.SET)"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString("Memory only"), Qt::EditRole);
    ++row;

    if (row >= ptree->rowCount())
    {
        QMoMTreeItemModelBase* psubtree = new QMoMTreeItemModelBase("Difficulty Table");
        ptree->appendTree(psubtree, "");
    }
    ptree->child(row, 0)->setData(QString("Difficulty Table"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString(), Qt::EditRole);
    for (int subrow = 0; (0 != dataSegment) && (subrow < (int)ARRAYSIZE(dataSegment->m_DifficultyTable)); ++subrow)
    {
        QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
        if (subrow >= psubtree->rowCount())
        {
            MoM::eDifficulty difficulty = (MoM::eDifficulty)subrow;
            psubtree->appendTree(constructTreeItem(&dataSegment->m_DifficultyTable[subrow], toQStr(difficulty)), "");
        }
    }
    ++row;

    if (row >= ptree->rowCount())
    {
        QMoMTreeItemModelBase* psubtree = new QMoMTreeItemModelBase("Tax");
        ptree->appendTree(psubtree, "");
    }
    ptree->child(row, 0)->setData(QString("Tax unrest table"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString("Unrest"), Qt::EditRole);
    ptree->child(row, 2)->setData(QString(), Qt::EditRole);
    for (int subrow = 0; (0 != dataSegment) && (subrow < ARRAYSIZE(dataSegment->m_Tax_Unrest_Table)); ++subrow)
    {
        QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
        if (subrow >= psubtree->rowCount())
        {
            MoM::eTax_Rate taxRate = (MoM::eTax_Rate)subrow;
            psubtree->appendChild(prettyQStr(taxRate), new QMoMTreeItemModel<uint16_t> ((uint16_t*)&dataSegment->m_Tax_Unrest_Table[taxRate]));
        }
    }
    ++row;

    if (row >= ptree->rowCount())
    {
        QMoMTreeItemModelBase* psubtree = new QMoMTreeItemModelBase("City Queue");
        ptree->appendTree(psubtree, "");
    }
    ptree->child(row, 0)->setData(QString("City Completion Queue"), Qt::EditRole);
    ptree->child(row, 1)->setData(QString("City Nr (-1=skip)"), Qt::EditRole);
    if (0 == dataSegment)
    {
        ptree->child(row, 2)->setData(QString("Completed"), Qt::EditRole);
    }
    else
    {
        ptree->child(row, 2)->setData(QString("Completed (%0 entries)").arg(dataSegment->m_nr_city_queue), Qt::EditRole);
    }
    for (int subrow = 0; (0 != dataSegment) && (subrow < ARRAYSIZE(dataSegment->m_arr20_city_queue)); ++subrow)
    {
        QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
        if (subrow >= psubtree->rowCount())
        {
            psubtree->appendChild(QString("[%0]").arg(subrow), new QMoMTreeItemModel<int8_t> (&dataSegment->m_arr20_city_queue[subrow].m_CityNr));
            psubtree->setChild(subrow, 2, new QMoMTreeItemModel<MoM::eProducing>(&dataSegment->m_arr20_city_queue[subrow].m_Producing));
        }
    }
    ++row;
}

void update_Heroes_Hired(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row, MoM::ePlayer wizardNr)
{
    if (0 == game)
        return;

    MoM::Wizard* wizard = game->getWizard(wizardNr);
    if (0 == wizard)
        return;

    for (int heroSlotNr = 0; heroSlotNr < (int)MoM::gMAX_HIRED_HEROES; ++heroSlotNr)
    {
        MoM::Hired_Hero& hiredHero = wizard->m_Heroes_hired_by_wizard[heroSlotNr];
        int unitNr = hiredHero.m_Unit_Nr;
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(&hiredHero, "Hired_Hero (Wizard)"));
        }

        MoM::Unit* unit = game->getUnit(unitNr);
        if (0 == unit || unitNr < 0 || unitNr >= (int)MoM::gMAX_UNITS)
        {
            ptree->child(row, 0)->setData(QString(), Qt::UserRole);
        }
        else
        {
            ptree->child(row, 0)->setData(toQStr(unit->m_Unit_Type), Qt::UserRole);
        }
        ptree->child(row, 1)->setData(QString(hiredHero.m_Hero_name), Qt::UserRole);
        ptree->child(row, 2)->setData(QString("HeroSlot[%0]").arg(heroSlotNr), Qt::EditRole);

        ++row;
    }
}

void update_Hero_Stats(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row, MoM::ePlayer wizardNr)
{
    if (0 == game)
        return;

    for (int heroTypeNr = 0; heroTypeNr < (int)MoM::gMAX_HERO_TYPES; ++heroTypeNr)
    {
        MoM::eUnit_Type unitType = (MoM::eUnit_Type)heroTypeNr;
        MoM::Hero_stats* heroStats = game->getHeroStats(wizardNr, unitType);
        if (0 == heroStats)
            break;
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(heroStats, "Hero_Stats (Wizard)"));
        }

        // Update top-level unit fields
        ptree->child(row, 0)->setData(toQStr(unitType), Qt::UserRole);
        ptree->child(row, 1)->setData(QString(), Qt::UserRole);
        ptree->child(row, 2)->setData(QString("HeroType[%0]").arg(heroTypeNr), Qt::EditRole);

        ++row;
    }
}

void UnitModel::update_Items_in_Game(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (int i = 0; (0 != game) && (i < (int)MoM::gMAX_ITEMS_VALID); ++i)
    {
		MoM::Item* item = game->getItem(i);
        if (0 == item)
        {
            break;
        }
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(item, ""));

            ptree->child(row, 2)->setData(QString("Item[%0]").arg(i), Qt::UserRole);
        }
        ptree->child(row, 0)->setData(toQStr(item->m_Item_Name), Qt::EditRole);
        if ((MoM::toUInt(item->m_Icon) < MoM::eItem_Icon_MAX) && ((item->m_Cost > 0) || (0 != item->m_Icon)))
        {
            ptree->child(row, 0)->setLazyIcon(item->m_Icon);
            ptree->child(row, 1)->setData(toQStr(item->m_Item_Type), Qt::UserRole);
        }
        else
        {
            ptree->child(row, 0)->setLazyIcon(QIcon());
            ptree->child(row, 1)->setData(QString(), Qt::UserRole);
        }

        ++row;
    }
}

void UnitModel::update_Lairs(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (int lairNr = 0; (0 != game) && (lairNr < (int)MoM::gMAX_NODES_LAIRS_TOWERS); ++lairNr)
    {
        MoM::Tower_Node_Lair* lair = game->getLair(lairNr);
        if (0 == lair)
            break;
        if (lairNr >= ptree->rowCount())
        {
            QMoMTreeItemModelBase* subtree = constructTreeItem(lair, "");
            ptree->setChild(row, 0, subtree);

            MoM::Node_Attr* nodeAttr = MoM::MoMController(game.data()).findNodeAttrAtLocation(MoM::MoMLocation(*lair));
            if (0 != nodeAttr)
            {
                subtree->setChild(subtree->rowCount(), 0, constructTreeItem(nodeAttr, "Node attr"));
            }

            ptree->child(lairNr, 0)->setData(prettyQStr(lair->m_Type), Qt::EditRole);
            ptree->child(lairNr, 1)->setData(QString("(%0, %1, %2) %3")
                                                    .arg(lair->m_XPos).arg(lair->m_YPos).arg((int)lair->m_Plane)
                                                    .arg(prettyQStr(lair->m_Status)), Qt::UserRole);
            ptree->child(lairNr, 2)->setData(QString("Lair[%0]").arg(lairNr), Qt::EditRole);

            if (MoM::toUInt(lair->m_Type) < MoM::eTower_Node_Lair_Type_MAX)
            {
                ptree->child(row, 0)->setLazyIcon(lair->m_Type);
            }
        }
        ++row;
    }
}

void update_Races(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;

    MoM::MoMDataSegment* dataSegment = game->getDataSegment();
    if (0 == dataSegment)
        return;

    for (int raceNr = 0; raceNr < (int)ARRAYSIZE(dataSegment->m_Race_Data); ++raceNr)
    {
        if (row >= ptree->rowCount())
        {
			MoM::eRace race = (MoM::eRace)raceNr;
            ptree->setChild(row, 0, constructTreeItem(&dataSegment->m_Race_Data[raceNr], ""));

            // Additional custom fields:
            // Name which is pointed to by PtrName
            const char* ptrName = game->getNameByOffset(dataSegment->m_Race_Data[raceNr].m_PtrName);
            if (0 != ptrName)
            {
                ptree->child(row, 0)->appendChild(QString("*PtrName"), new QMoMTreeItemModel<const char*>(ptrName));
            }

            ptree->child(row, 0)->setData(prettyQStr((MoM::eRace)row), Qt::EditRole);
            ptree->child(row, 0)->setLazyIcon(race);
            QString toolTip = QString(MoM::QMoMResources::instance().getHelpText(race).c_str());
            ptree->child(row, 0)->setToolTip(toolTip);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString("Race[%0]").arg(raceNr), Qt::EditRole);
        }

        ++row;
    }
}

void UnitModel::update_Spell_Data(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;

    for (int spellNr = 0; spellNr < (int)MoM::eSpell_MAX; ++spellNr)
    {
        MoM::eSpell spell = (MoM::eSpell)spellNr;
        MoM::Spell_Data* spellData = game->getSpellData(spell);
        if (0 == spellData)
            break;

        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(spellData, ""));
        }

        ptree->child(spellNr, 0)->setData(spellData->m_SpellName, Qt::EditRole);
        ptree->child(spellNr, 0)->setLazyIcon(spell);
        QString toolTip = MoM::QMoMResources::instance().getHelpText(spell).c_str();
        ptree->child(row, 0)->setToolTip(toolTip);

        if ((spellNr <= MoM::SPELL_Magic_Spirit) && (spellNr - 1) % 10 == 0)
        {
            const char* rarityStr[4] = { "Common", "Uncommon", "Rare", "Very Rare" };
            int rarity = ((spellNr + 40 - 1) % 40 / 10);
            QString qRarity = rarityStr[rarity];
            MoM::eRealm_Type color = (MoM::eRealm_Type)((spellNr - 1) / 40);
            ptree->child(spellNr, 1)->setData(QString("%0, %1").arg(toQStr(color)).arg(qRarity), Qt::UserRole);
        }
        else
        {
            ptree->child(spellNr, 1)->setData(QString(), Qt::UserRole);
        }
        ptree->child(spellNr, 2)->setData(QString("Spell[%0]").arg(spellNr), Qt::EditRole);
        ++row;
    }
}

void update_Spells_Cast_in_Battle(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;
    MoM::Spells_Cast_in_Battle* pSpells_Cast_in_Battle = game->getSpells_Cast_in_Battle();

    if (row >= ptree->rowCount())
    {
        ptree->setChild(row, 0, constructTreeItem(pSpells_Cast_in_Battle, ""));
    }
    ptree->child(row, 0)->setData(toQStr("Spells_Cast_in_Battle"), Qt::UserRole);
    ptree->child(row, 1)->setData(QString(), Qt::EditRole);
    ptree->child(row, 2)->setData(QString(), Qt::EditRole);

    ++row;
}

void update_Units_in_Game(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (int unitNr = 0; (0 != game) && (unitNr < game->getNrUnits()) && (unitNr < (int)MoM::gMAX_UNITS); ++unitNr)
    {
        MoM::Unit* unit = game->getUnit(unitNr);
        if (0 == unit)
            break;
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(unit, ""));

            // Additional custom fields

            // Weapon_Type, which is an enum stored as a bitfield
            MoM::unionUnit_Weapon_Mutation mask;
            mask.bits = 0;
            mask.s.Weapon_Type = 0x03;
            ptree->child(row, 0)->appendChild(QString("Weapon_Type"), new QMoMTreeItemModel<MoM::eWeaponType>((MoM::eWeaponType*)&unit->m_Weapon_Mutation.bits, mask.bits));
/*
            // Unit Type Data, if available
            if ((unsigned)unit.m_Unit_Type < MoM::eUnit_Type_MAX)
            {
                MoM::Unit_Type_Data* pUnitData = game->getUnit_Type_Data(unit.m_Unit_Type);
                if (0 != pUnitData)
                {
                    constructTreeItem(ptree->child(row, 0), pUnitData, "Unit_Type_Data (Global)");
                }
            }

            // Hired Hero, if applicable
            if ((unsigned)unit.m_Hero_Slot_Number < MoM::gMAX_HIRED_HEROES)
            {
                MoM::Hired_Hero& hiredHero = game->getWizard(unit.m_Owner).m_Heroes_hired_by_wizard[unit.m_Hero_Slot_Number];

                constructTreeItem(ptree->child(row, 0), &hiredHero, "Hired_Hero (Wizard)");
            }

            // Hero Stats, if applicable
            if ((unsigned)unit.m_Unit_Type < MoM::gMAX_HERO_TYPES)
            {
                MoM::Hero_stats& heroStats = game->getHero_stats(unit.m_Owner, unit.m_Unit_Type);

                constructTreeItem(ptree->child(row, 0), &heroStats, "Hero_Stats (Wizard)");
            }
*/
        }

        // Update top-level unit fields
        ptree->child(row, 0)->setData(toQStr(unit->m_Unit_Type), Qt::UserRole);
        ptree->child(row, 1)->setData(toQStr(unit->m_Owner), Qt::UserRole);
        ptree->child(row, 2)->setData(QString("Unit[%0]").arg(unitNr), Qt::EditRole);

        ++row;
    }
}

void UnitModel::update_Unit_Types(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    if (0 == game)
        return;

    MoM::MoMDataSegment* dataSegment = game->getDataSegment();
    if (0 == dataSegment)
        return;

    for (int unitTypeNr = 0; unitTypeNr < MoM::eUnit_Type_MAX; ++unitTypeNr)
    {
        MoM::eUnit_Type unitType = (MoM::eUnit_Type)unitTypeNr;
        MoM::Unit_Type_Data* pUnitData = game->getUnitTypeData(unitType);
        if (0 == pUnitData)
        {
            break;
        }
        if (row >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(pUnitData, ""));

            // Update top-level unit fields (only need to do this once, since they won't change)
            ptree->child(row, 0)->setData(prettyQStr(unitType), Qt::EditRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString("UnitType[%0]").arg(unitTypeNr), Qt::EditRole);

            // Additional custom fields:
            // Name which is pointed to by PtrName
            const char* ptrName = game->getNameByOffset(pUnitData->m_PtrName);
            if (0 != ptrName)
            {
                ptree->child(row, 0)->appendChild(QString("*PtrName"), new QMoMTreeItemModel<const char*>(ptrName));
            }

            ptree->child(row, 0)->setLazyIcon(unitType);

        }

        ++row;
    }
}

void update_Wizards(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row)
{
    for (int wizardNr = 0; (0 != game) && (wizardNr < game->getNrWizards()) && (wizardNr < (int)MoM::gMAX_WIZARD_RECORDS); ++wizardNr)
    {
        MoM::Wizard* wizard = game->getWizard(wizardNr);
        if (0 == wizard)
            break;
        if (wizardNr >= ptree->rowCount())
        {
            ptree->setChild(row, 0, constructTreeItem(wizard, ""));
            QString toolTip = MoM::QMoMResources::instance().getHelpText(wizard->m_Portrait).c_str();
            ptree->child(row, 0)->setToolTip(toolTip);

            QMoMTreeItemModelBase* subtree = ptree->child(row, 0);
            MoM::Fortress* fortresses = game->getFortresses();
            if (0 != fortresses)
            {
                MoM::Fortress* fortress = &fortresses[wizardNr];
                int subrow = subtree->rowCount();
                subtree->setChild(subrow, 0, constructTreeItem(fortress, "Fortress"));

                MoM::MoMController momController(game.data());
                MoM::City* city = momController.findCityAtLocation(MoM::MoMLocation(*fortress));
                if (0 != city)
                {
                    int cityNr = (int)(city - game->getCity(0));
                    subtree->child(subrow,0)->appendChild("Fortress city", new QMoMTreeItemModel<char[14]>(city->m_City_Name),
                                                          QString("city[%0]").arg(cityNr));
                }
            }
        }
        ptree->child(wizardNr, 0)->setData(toQStr(wizard->m_Portrait), Qt::UserRole);
        ptree->child(wizardNr, 1)->setData(QString("\"%0\"").arg(toQStr(wizard->m_Name)), Qt::UserRole);
        ptree->child(wizardNr, 2)->setData(QString("wizard[%0]").arg(wizardNr), Qt::EditRole);

        ++row;
    }
}

void UnitModel::threadUpdateModelData()
{
    QMoMTreeItemPtr parentItem = m_rootItem;

    QMoMGamePtr game = QMoMTreeItemModelBase::game();

	int toprow = 0;

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrGameData = 0;
        if (0 != game)
        {
            nrGameData = 4; // TODO: Proper nr rows
        }
        parentItem->child(toprow, 0)->setData(tr("Game Data"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            parentItem->child(toprow, 2)->setData(tr("MoM Version: %0").arg(toQStr(game->getMoMVersion())), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrGameData > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrGameData - 1);
            update_Game_Data(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Game_Data(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

//        QMoMTreeItemSubtree_Buildings* ptree = dynamic_cast<QMoMTreeItemSubtree_Buildings*>(parentItem->child(toprow, 0));
//        if (0 == ptree)
//		{
//            ptree = new QMoMTreeItemSubtree_Buildings;
//            parentItem->setChild(toprow, 0, ptree);
//		}
//        else
//        {
//            ptree->slot_gameUpdated();
//        }
        int nrBuildings = 0;
        parentItem->child(toprow, 0)->setData(tr("Buildings"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrBuildings = MoM::eBuilding_array_MAX;
            parentItem->child(toprow, 2)->setData(tr("NrBuildings = %0").arg(nrBuildings), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrBuildings > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrBuildings - 1);
            update_Buildings(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Buildings(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

	{
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrRaces = 0;
        parentItem->child(toprow, 0)->setData(tr("Races"), Qt::EditRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if ((0 == game) || (0 == game->getDataSegment()))
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrRaces = ARRAYSIZE(game->getDataSegment()->m_Race_Data);
            parentItem->child(toprow, 2)->setData(QString("NrRaces = %1").arg(nrRaces), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrRaces > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrRaces - 1);
            update_Races(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Races(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

	{
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrWizards = 0;
        parentItem->child(toprow, 0)->setData(tr("Wizards"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrWizards = game->getNrWizards();
            parentItem->child(toprow, 2)->setData(tr("NrWizards = %0").arg(nrWizards), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if ((nrWizards > ptree->rowCount()))
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrWizards - 1);
            update_Wizards(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Wizards(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrLairs = 0;
        parentItem->child(toprow, 0)->setData(tr("Lairs"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrLairs = MoM::gMAX_NODES_LAIRS_TOWERS;
            parentItem->child(toprow, 2)->setData(tr("NrLairs = %0").arg(nrLairs), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrLairs > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrLairs - 1);
            update_Lairs(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Lairs(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrCities = 0;
        parentItem->child(toprow, 0)->setData(tr("Cities"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrCities = game->getNrCities();
            parentItem->child(toprow, 2)->setData(tr("NrCities = %0").arg(nrCities), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrCities > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrCities - 1);
            update_Cities(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Cities(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

//        int nrCities = 0;
        parentItem->child(toprow, 0)->setData(tr("Events"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
//        if (nrCities > ptree->rowCount())
//        {
//            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrCities - 1);
//            update_Cities(ptree, game, row);
//            endInsertRows();
//        }
//        else
        {
            update_Events(ptree, game, row);
        }
//        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrUnitSections = 0;
        parentItem->child(toprow, 0)->setData(tr("Units"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 == game)
        {
            parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);
        }
        else
        {
            nrUnitSections = 4;     // TODO: Proper number
            parentItem->child(toprow, 2)->setData(tr("NrUnits in Game = %0").arg(game->getNrUnits()), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);

        int oldRowCount = ptree->rowCount();
        if (nrUnitSections > oldRowCount)
        {
            beginInsertRows(createIndex(toprow, 0, ptree), oldRowCount, nrUnitSections - 1);
        }

        {
            if (row >= ptree->rowCount())
            {
                QMoMTreeItemModelBase* psubtree = new QMoMTreeItemModelBase("XP Level Table");
                ptree->appendTree(psubtree, "");
            }

            MoMDataSegment* dataSegment = 0;
            if (0 != game)
            {
                dataSegment = game->getDataSegment();
            }

            ptree->child(row, 0)->setData(QString("XP Table (Global)"), Qt::EditRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString(), Qt::EditRole);

            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            int subrow = 0;
            for (subrow = 0; (0 != dataSegment) && (subrow < (int)ARRAYSIZE(dataSegment->m_XP_Level_Table)); ++subrow)
            {
                if (subrow >= psubtree->rowCount())
                {
                    psubtree->appendChild(QString("Level %0").arg(subrow + 1), new QMoMTreeItemModel<uint16_t> (&dataSegment->m_XP_Level_Table[subrow]));
                }
            }
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }
            ptree->child(row, 0)->setData(tr("Unit Types (Global)"), Qt::UserRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            if (0 == game)
            {
                ptree->child(row, 2)->setData(QString(), Qt::EditRole);
            }
            else
            {
                ptree->child(row, 2)->setData(tr("NrUnit Types = %0").arg((int)MoM::eUnit_Type_MAX), Qt::EditRole);
            }

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            update_Unit_Types(psubtree, game, subrow);
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        MOM_FOREACH(ePlayer, playerNr, gMAX_VALID_WIZARDS)
        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }
            ptree->child(row, 0)->setData(tr("Hero Stats (%0)").arg(prettyQStr(playerNr)), Qt::EditRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            if (0 == game)
            {
                ptree->child(row, 2)->setData(QString(), Qt::EditRole);
            }
            else
            {
                ptree->child(row, 2)->setData(tr("NrHero Types = %0").arg(MoM::gMAX_HERO_TYPES), Qt::EditRole);
            }

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            update_Hero_Stats(psubtree, game, subrow, playerNr);
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }
            ptree->child(row, 0)->setData(tr("Heroes Hired (YOU)"), Qt::UserRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            if (0 == game)
            {
                ptree->child(row, 2)->setData(QString(), Qt::EditRole);
            }
            else
            {
                ptree->child(row, 2)->setData(tr("NrHero Slots = %0").arg(MoM::gMAX_HIRED_HEROES), Qt::EditRole);
            }

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            update_Heroes_Hired(psubtree, game, subrow, MoM::PLAYER_YOU);
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }
            ptree->child(row, 0)->setData(tr("Units in Game"), Qt::UserRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            if (0 == game)
            {
                ptree->child(row, 2)->setData(QString(), Qt::EditRole);
            }
            else
            {
                ptree->child(row, 2)->setData(tr("NrUnits in Game = %0").arg(game->getNrUnits()), Qt::EditRole);
            }

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            update_Units_in_Game(psubtree, game, subrow);
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        if (nrUnitSections > oldRowCount)
        {
            endInsertRows();
        }

        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrItems = 0;
        parentItem->child(toprow, 0)->setData(tr("Items"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        if (0 != game)
        {
            nrItems = game->getNrItemsValid();
            parentItem->child(toprow, 2)->setData(tr("NrItems = %0").arg(nrItems), Qt::EditRole);
        }

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        if (nrItems > ptree->rowCount())
        {
            beginInsertRows(createIndex(toprow, 0, ptree), ptree->rowCount(), nrItems - 1);
            update_Items_in_Game(ptree, game, row);
            endInsertRows();
        }
        else
        {
            update_Items_in_Game(ptree, game, row);
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrSpellSections = 3;    // TODO: Proper number
        parentItem->child(toprow, 0)->setData(tr("Spells"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);

        int oldRowCount = ptree->rowCount();
        if (nrSpellSections > oldRowCount)
        {
            beginInsertRows(createIndex(toprow, 0, ptree), oldRowCount, nrSpellSections - 1);
        }

        {
            if (row >= ptree->rowCount())
            {
                QMoMTreeItemModelBase* psubtree = new QMoMTreeItemModelBase("Spell Data");
                ptree->appendTree(psubtree, "");
            }

            ptree->child(row, 0)->setData(QString("Spell Data"), Qt::EditRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString("Memory only"), Qt::EditRole);

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            update_Spell_Data(psubtree, game, subrow);
            removeUnusedRows(row, psubtree, subrow);

            ++row;
        }

        {
            if (row >= ptree->rowCount())
            {
                ptree->setChild(row, 0, new QMoMTreeItemModelBase(""));
            }
            if ((0 != game) && (0 != game->getDataSegment()))
            {
                if (0 == ptree->child(row, 0)->rowCount())
                {
                    ptree->setChild(row, 0, MoM::constructTreeItem(&game->getDataSegment()->m_Upkeep_Enchantments, ""));
                }
            }
            else
            {
                if (ptree->child(row, 0)->rowCount() > 0)
                {
                    removeUnusedRows(row, ptree->child(row, 0), 0);
                }
            }

            ptree->child(row, 0)->setData(QString("Upkeep Enchantments"), Qt::EditRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString(), Qt::EditRole);

            ++row;
        }

        {
            if (row >= ptree->rowCount())
            {
                ptree->setChild(row, 0, new QMoMTreeItemModelBase("EXE:Spell Saves"));
            }

            uint8_t* ovl112 = 0;
            MoM::MoMExeWizards* wizardsExe = 0;
            if (0 != game)
            {
                ovl112 = game->getWizardsOverlay(112);
                wizardsExe = game->getWizardsExe();
            }

            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            int subrow = 0;
            if ((0 != ovl112) && (0 != wizardsExe) && (0 == psubtree->rowCount()))
            {
                for (subrow = 0; subrow < (int)wizardsExe->getNrSpellSaves(); ++subrow)
                {
                    psubtree->appendChild("Spell", new QMoMTreeItemModel<MoM::eSpell>((MoM::eSpell*)&ovl112[ wizardsExe->getSpellSave(subrow).spellOffset ]));
                    if (0 == wizardsExe->getSpellSave(subrow).saveOffset)
                    {
                        psubtree->child(subrow, 2)->setData(QString("0 (unchangeable)"), Qt::EditRole);
                    }
                    else
                    {
                        psubtree->setChild(subrow, 2, new QMoMTreeItemModel<int16_t>((int16_t*)&ovl112[ wizardsExe->getSpellSave(subrow).saveOffset ]));
                    }
                }
            }
            ptree->child(row, 0)->setData(QString("Spell Saves"), Qt::EditRole);
            ptree->child(row, 1)->setData(QString("Spells can be swapped!"), Qt::EditRole);
            ptree->child(row, 2)->setData(QString("MODIFIES Wizards.exe"), Qt::EditRole);

            if (0 == ovl112)
            {
                removeUnusedRows(row, psubtree, subrow);
            }

            ++row;
        }

        if (nrSpellSections > oldRowCount)
        {
            endInsertRows();
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrBattleSections = 0;
        if ((0 != game) && (0 != game->getDataSegment()))
        {
            nrBattleSections = 3+5;       // TODO: Proper number
        }
        parentItem->child(toprow, 0)->setData(tr("MEM:Battle"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString("Memory only"), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);

        int oldRowCount = ptree->rowCount();
        if (nrBattleSections > oldRowCount)
        {
            beginInsertRows(createIndex(toprow, 0, ptree), oldRowCount, nrBattleSections - 1);
        }

        if ((0 != game) && (0 != game->getDataSegment()))
        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }

            int nrBattle_Units = 0;
            int nrEntries = 0;

            ptree->child(row, 0)->setData(tr("Battle_Units"), Qt::UserRole);
            const MoM::EXE_Reloc& addr = game->getDataSegment()->m_addr_Battle_Unit;
            ptree->child(row, 1)->setData(toQStr(addr), Qt::EditRole);
            if (0 != game->getNumber_of_Battle_Units())
            {
                nrBattle_Units = *game->getNumber_of_Battle_Units();
                nrEntries = nrBattle_Units + 1;
                ptree->child(row, 2)->setData(tr("NrBattle_Units = %0").arg(nrBattle_Units), Qt::EditRole);
            }
            else
            {
                ptree->child(row, 2)->setData(QString(), Qt::EditRole);
            }

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            if (nrEntries > psubtree->rowCount())
            {
                beginInsertRows(createIndex(row, 0, psubtree), psubtree->rowCount(), nrEntries - 1);
                update_Battle_Units(psubtree, game, subrow);
                endInsertRows();
            }
            else
            {
                update_Battle_Units(psubtree, game, subrow);
            }

            removeUnusedRows(row, psubtree, subrow);

            row++;
        }

        update_Battlefield(ptree, game, row);

        update_Spells_Cast_in_Battle(ptree, game, row);

        if ((0 != game) && (0 != game->getDataSegment()))
        {
            if (ptree->rowCount() == row)
            {
                ptree->appendChild("m_Combat_turn", new QMoMTreeItemModel<int16_t>(&game->getDataSegment()->m_Combat_turn));
                ptree->appendChild("m_External_battle_condition", new QMoMTreeItemModel<MoM::eBattleCondition>(&game->getDataSegment()->m_External_battle_condition));
                ptree->appendChild("m_BattleUnit_on_move", new QMoMTreeItemModel<int16_t>(&game->getDataSegment()->m_BattleUnit_on_move));
                ptree->appendChild("m_Defending_wizard", new QMoMTreeItemModel<int16_t>(&game->getDataSegment()->m_Defending_wizard));
                ptree->appendChild("m_Attacking_wizard", new QMoMTreeItemModel<int16_t>(&game->getDataSegment()->m_Attacking_wizard));
            }
            row += 5;
        }

        if (nrBattleSections > oldRowCount)
        {
            endInsertRows();
        }
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        parentItem->child(toprow, 0)->setData(tr("Current Unit View"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
        update_Battle_Unit_View(ptree, game, row);
        removeUnusedRows(toprow, ptree, row);

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrMiscellaneous = 0;
        parentItem->child(toprow, 0)->setData(tr("EXE:Miscellaneous"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString("MODIFIES Wizards.exe"), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);

        uint8_t* ovl083 = 0;
        uint8_t* ovl117 = 0;
        uint8_t* ovl120 = 0;
        uint8_t* ovl122 = 0;
        uint8_t* ovl136 = 0;
        uint8_t* ovl140 = 0;
        uint8_t* ovl142 = 0;
        uint8_t* ovl164 = 0;
        if (0 != game)
        {
            ovl083 = game->getWizardsOverlay(83);
            ovl117 = game->getWizardsOverlay(117);
            ovl120 = game->getWizardsOverlay(120);
            ovl122 = game->getWizardsOverlay(122);
            ovl136 = game->getWizardsOverlay(136);
            ovl140 = game->getWizardsOverlay(140);
            ovl142 = game->getWizardsOverlay(142);
            ovl164 = game->getWizardsOverlay(164);
        }
        if ((0 != ovl083) && (0 != ovl117) && (0 != ovl120) && (0 != ovl122) && (0 != ovl136) && (0 != ovl140) && (0 != ovl142) && (0 != ovl164))
        {
            nrMiscellaneous = 39;        // TODO: Proper number
        }

        int oldRowCount = ptree->rowCount();
        if (nrMiscellaneous > oldRowCount)
        {
            beginInsertRows(createIndex(toprow, 0, ptree), oldRowCount, nrMiscellaneous - 1);
        }

        if ((0 != ovl083) && (0 != ovl117) && (0 != ovl120) && (0 != ovl122) && (0 != ovl136) && (0 != ovl140) && (0 != ovl142) && (0 != ovl164))
        {
            if (0 == ptree->rowCount())
            {
                if (game->getMoMVersion() >= std::string("1.40j"))
                {
                    ptree->appendChild("City Wall Defense", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl122[ 0x0FEB ]));
                    ptree->child(row, 2)->setData("Default +3 shields", Qt::EditRole);
                    row++;
                    ptree->appendChild("Broken Wall Defense", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl122[ 0x0FF1 ]));
                    ptree->child(row, 2)->setData("Default +1 shields", Qt::EditRole);
                    row++;
                }
                else
                {
                    ptree->appendChild("City Wall Defense", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl122[ 0x0FEC ]));
                    ptree->child(row, 2)->setData("Default +3 shields", Qt::EditRole);
                    row++;
                    ptree->appendChild("Broken Wall Defense", new QMoMTreeItemModelBase("1 (unchangeable)"));
                    ptree->child(row, 2)->setData("Default +1 shields", Qt::EditRole);
                    row++;
                }
                ptree->appendChild("Large Shield", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl122[ 0x16F1 ]));
                ptree->child(row, 2)->setData("Default +2 shields", Qt::EditRole);
                row++;

                ptree->appendChild("Noble", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl120[ 0x0AC7 ]));
                ptree->child(row, 2)->setData("Default 10 gold/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Noble", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl140[ 0x068F ]));
                ptree->child(row, 2)->setData("Second occurrence of Noble", Qt::EditRole);
                row++;

                if (game->getMoMVersion() < std::string("5.0"))
                {
                    ptree->appendChild("Turns before AI aggressive", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl164[ 0x12BB ]));
                    ptree->child(row, 2)->setData("Default 100 turns", Qt::EditRole);
                    row++;
                }

                ptree->appendChild("Max spells of colored unit", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl117[ 0x0179 ]));
                ptree->child(row, 2)->setData("Default 40 spells", Qt::EditRole);
                row++;

                ptree->appendChild("Max city size", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl140[ 0x1660 ]));
                ptree->child(row, 2)->setData("Default 25 pop", Qt::EditRole);
                row++;

                ptree->appendChild("Wild game: food bonus", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl142[ 0x04E8 ]));
                ptree->child(row, 2)->setData("Default 2 food/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Granary: food bonus", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl142[ 0x073C ]));
                ptree->child(row, 2)->setData("Default 2 food/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Farmer's market: food bonus", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl142[ 0x0767 ]));
                ptree->child(row, 2)->setData("Default 3 food/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Forester's guild: food bonus", new QMoMTreeItemModel<int8_t>((int8_t*)&ovl142[ 0x06DE ]));
                ptree->child(row, 2)->setData("Default 2 food/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Animist's guild: food/farmer", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl142[ 0x0698 ]));
                ptree->child(row, 2)->setData("Default 3 food/turn", Qt::EditRole);
                row++;

                ptree->appendChild("Race with implicit Animist's guild", new QMoMTreeItemModel<MoM::eRace>((MoM::eRace*)&ovl142[ 0x065A ]));
                ptree->child(row, 2)->setData("Default Halfling (6)", Qt::EditRole);
                row++;

                ptree->appendChild("Max spell pump mana (power of 2)", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl136[ 0x097E ]));
                ptree->child(row, 2)->setData("Default 2 (max pump=2^2*cost)", Qt::EditRole);
                row++;


                if (game->getMoMVersion() < std::string("5.0"))
                {
                    ptree->appendChild("Sage master rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x173F ]));
                    ptree->child(row, 2)->setData("Default 25%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Conjurer rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1766 ]));
                    ptree->child(row, 2)->setData("Default 25%", Qt::EditRole);
                    row++;
                    ptree->appendChild("Runemaster rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1893 ]));
                    ptree->child(row, 2)->setData("Default 25%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Chaos master rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x178B ]));
                    ptree->child(row, 2)->setData("Default 15%", Qt::EditRole);
                    row++;
                    ptree->appendChild("Min chaos books for rp", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1799 ]));
                    ptree->child(row, 2)->setData("Default 7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Added to chaos books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x17AA ]));
                    ptree->child(row, 2)->setData("Default -7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Multiplier to chaos books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x17AD ]));
                    ptree->child(row, 2)->setData("Default 10%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Nature master rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x17C8 ]));
                    ptree->child(row, 2)->setData("Default 15%", Qt::EditRole);
                    row++;
                    ptree->appendChild("Min nature books for rp", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x17D6 ]));
                    ptree->child(row, 2)->setData("Default 7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Added to nature books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x17E7 ]));
                    ptree->child(row, 2)->setData("Default -7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Multiplier to nature books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x17EA ]));
                    ptree->child(row, 2)->setData("Default 10%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Sorcery master rp bonus", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1805 ]));
                    ptree->child(row, 2)->setData("Default 15%", Qt::EditRole);
                    row++;
                    ptree->appendChild("Min sorcery books for rp", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1813 ]));
                    ptree->child(row, 2)->setData("Default 7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Added to sorcery books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x1824 ]));
                    ptree->child(row, 2)->setData("Default -7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Multiplier to sorcery books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x1827 ]));
                    ptree->child(row, 2)->setData("Default 10%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Min life books for rp", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x183C ]));
                    ptree->child(row, 2)->setData("Default 7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Added to life books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x184D ]));
                    ptree->child(row, 2)->setData("Default -7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Multiplier to life books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x1850 ]));
                    ptree->child(row, 2)->setData("Default 10%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Min death books for rp", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl120[ 0x1865 ]));
                    ptree->child(row, 2)->setData("Default 7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Added to death books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x1876 ]));
                    ptree->child(row, 2)->setData("Default -7", Qt::EditRole);
                    row++;
                    ptree->appendChild("Multiplier to death books for rp", new QMoMTreeItemModel<int16_t>((int16_t*)&ovl120[ 0x1879 ]));
                    ptree->child(row, 2)->setData("Default 10%", Qt::EditRole);
                    row++;

                    ptree->appendChild("Max books condition", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl083[ 0x14c6 ]));
                    ptree->child(row, 2)->setData("Default 12, allowing 13 books", Qt::EditRole);
                    row++;
                    ptree->appendChild("Max books condition", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl083[ 0x14e5 ]));
                    ptree->child(row, 2)->setData("Always set to same as the one before", Qt::EditRole);
                    row++;
                }
                ptree->appendChild("Max skills condition", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl083[ 0x14c0 ]));
                ptree->child(row, 2)->setData("Default 5, allowing 6 skills", Qt::EditRole);
                row++;
                ptree->appendChild("Max skills condition", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl083[ 0x14da ]));
                ptree->child(row, 2)->setData("Always set to same as the one before", Qt::EditRole);
                row++;
                ptree->appendChild("Chance book (vs skill)", new QMoMTreeItemModel<uint8_t>((uint8_t*)&ovl083[ 0x14f0 ]));
                ptree->child(row, 2)->setData("Default 75%", Qt::EditRole);
                row++;
           }
        }

        if (nrMiscellaneous > oldRowCount)
        {
            endInsertRows();
        }

        if (0 == nrMiscellaneous)
        {
            removeUnusedRows(toprow, ptree, row);
        }

        toprow++;
    }

    {
        if (toprow == parentItem->rowCount())
        {
            parentItem->appendEmptyRow();
        }

        int nrCustomizerSections = 0;
        if ((0 != game) && (0 != game->getMagicDataSegment()))
        {
            nrCustomizerSections = 2; // TODO: Proper number
        }
        parentItem->child(toprow, 0)->setData(tr("Customizer"), Qt::UserRole);
        parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
        parentItem->child(toprow, 2)->setData(QString("Setup only"), Qt::EditRole);

        int row = 0;
        QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);

        int oldRowCount = ptree->rowCount();
        if (nrCustomizerSections > oldRowCount)
        {
            beginInsertRows(createIndex(toprow, 0, ptree), oldRowCount, nrCustomizerSections - 1);
        }

        if ((0 != game) && (0 != game->getMagicDataSegment()))
        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }

            ptree->child(row, 0)->setData(tr("MEM:Customizer"), Qt::UserRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString(), Qt::EditRole);

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);
            MoMMagicDataSegment* pMagicDataSegment = game->getMagicDataSegment();

            if (0 == psubtree->rowCount())
            {
                psubtree->appendChild("Want input GUESS", new QMoMTreeItemModel<int16_t>(&pMagicDataSegment->m_Want_input_GUESS));
                subrow++;

                for (int i = 0; i < 10; ++i)
				{
                    psubtree->appendChild(QString("Nr spells %0 books").arg(i+2), new QMoMTreeItemModel<int16_t>(&pMagicDataSegment->m_Nr_spell_choices[i]));
					subrow++;
				}				
				
                psubtree->appendChild("Sorcery_Picks_Divider", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Sorcery_Picks_Divider));
                subrow++;
                psubtree->appendChild("Nature_Picks_Divider", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Nature_Picks_Divider));
                subrow++;
                psubtree->appendChild("Chaos_Picks_Divider", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Chaos_Picks_Divider));
                subrow++;
                psubtree->appendChild("Death_Picks_Divider", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Death_Picks_Divider));
                subrow++;
                psubtree->appendChild("Life_Picks_Divider", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Life_Picks_Divider));
                subrow++;

                psubtree->appendChild("Total_Picks_Left", new QMoMTreeItemModel<uint16_t>(&pMagicDataSegment->m_Total_Picks_Left));
                subrow++;

//                psubtree->appendChild("Select_Wizard_Shown", new TreeItem<MoM::ePortrait>(&pMagicDataSegment->m_Select_Wizard_Shown));
//                subrow++;

                for (MoM::ePortrait portrait = (MoM::ePortrait)0; portrait < MoM::ePortrait_MAX; MoM::inc(portrait))
                {
                    psubtree->appendTree(constructTreeItem(&pMagicDataSegment->m_Wizard_Types[portrait], toQStr(portrait)), "");
                    psubtree->child(subrow, 1)->setData(QString(), Qt::EditRole);
                    psubtree->child(subrow, 2)->setData(QString("WizardType[%0]").arg((int)portrait), Qt::EditRole);
                    subrow++;
                }

                for (MoM::eUnit_Type heroType = (MoM::eUnit_Type)0; MoM::toUInt(heroType) < MoM::gMAX_HERO_TYPES; MoM::inc(heroType))
                {
                    psubtree->appendTree(constructTreeItem(&pMagicDataSegment->m_Hero_Stats_Initializers[heroType], toQStr(heroType)), "");
                    psubtree->child(subrow, 1)->setData(QString(), Qt::EditRole);
                    psubtree->child(subrow, 2)->setData(QString("HeroType[%0]").arg((int)heroType), Qt::EditRole);
                    subrow++;
                }
            }

//            removeUnusedRows(row, psubtree, subrow);

            row++;
        }

        uint8_t* magicExeContents = 0;
        uint8_t* ovl51 = 0;
        if (0 != game)
        {
            magicExeContents = game->getMagicExeContents();
            ovl51 = game->getMagicOverlay(51);
        }
        if ((0 != magicExeContents) && (0 != ovl51))
        {
            if (row == ptree->rowCount())
            {
                ptree->appendEmptyRow();
            }

            ptree->child(row, 0)->setData(tr("EXE:Customizer"), Qt::UserRole);
            ptree->child(row, 1)->setData(QString(), Qt::EditRole);
            ptree->child(row, 2)->setData(QString("MODIFIES Magic.exe"), Qt::EditRole);

            int subrow = 0;
            QMoMTreeItemModelBase* psubtree = ptree->child(row, 0);

            if (0 == psubtree->rowCount())
            {
                psubtree->appendChild("Total picks", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x3394B]));
                psubtree->child(subrow, 2)->setData("(default 11)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Landmass_total_tiles Small", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1E9A + 3]));
                psubtree->child(subrow, 2)->setData("(default 360)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Landmass_total_tiles Medium", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1EA1 + 3]));
                psubtree->child(subrow, 2)->setData("(default 480)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Landmass_total_tiles Large", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1EA8 + 3]));
                psubtree->child(subrow, 2)->setData("Game may freeze if too high 1200 < n <= 1720  (default 720)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Landmass_min continent_tiles Small", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1F61 + 1]));
                psubtree->child(subrow, 2)->setData("10 + dice(n)  (default 5: 6-15)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Landmass_min continent_tiles Medium", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1F73 + 1]));
                psubtree->child(subrow, 2)->setData("10 + dice(n)  (default 10: 11-20)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Landmass_min continent_tiles Large", new QMoMTreeItemModel<uint16_t>((uint16_t*)&ovl51[0x1F82 + 1]));
                psubtree->child(subrow, 2)->setData("10 + dice(n)  (default 20: 21-30)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Nr of Towers", new QMoMTreeItemModel<uint8_t>(&ovl51[0x08C1 + 2]));
                psubtree->child(subrow, 2)->setData("Max 6 towers  (default 6)", Qt::EditRole);
                subrow++;
//                psubtree->appendChild("Offset Myrror Towers (don't change)", new TreeItem<uint8_t>(&ovl51[0x2EE1 + 2]));
//                subrow++;
                psubtree->appendChild("Nr of Towers + 6", new QMoMTreeItemModel<uint8_t>(&ovl51[0x2F31 + 2]));
                psubtree->child(subrow, 2)->setData("Always set to towers + 6", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Nr Normal lairs", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x3933E + 3]));
                psubtree->child(subrow, 2)->setData("Max 25  (default 25)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Nr Weak lairs", new QMoMTreeItemModel<uint8_t>(&magicExeContents[0x396A2 + 2]));
                psubtree->child(subrow, 2)->setData("Max 32  (default 32)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Normal lair monster cash Arcanus", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39574 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) * 50 + 50  (default 29: 100-1500)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Normal lair monster cash Myrror", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39588 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) * 100 + 100  (default 24: 200-2500)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Weak lair monster cash Arcanus", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x3966B + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) * 10  (default 10: 10-100)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Weak lair monster cash Myrror", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39670 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) * 10  (default 20: 20-200)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Nr monsters in lair (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39987 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n)  See OSG p.416-418  (default n = 4)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Nr secondary monsters in lair (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39B6E + 1]));
                psubtree->child(subrow, 2)->setData("dice(n - Nr primary monsters)  (default n = 10)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Lower primary monsters (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x39A93 + 1]));
                psubtree->child(subrow, 2)->setData("1 in dice(n) throws primary back  (default n = 2)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Chance same race on continents (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x3DD82 + 1]));
                psubtree->child(subrow, 2)->setData("1 in dice(n) makes the race random  (default n = 4)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Distance to connect cities", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x3EAF0 + 1]));
                psubtree->child(subrow, 2)->setData("Diagonal tiles count as 1.5  (default 11 squares)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Turns before random events start", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x47C32 + 3]));
                psubtree->child(subrow, 2)->setData("(default 50 turns)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Random picks of opponents on Normal (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x467f2 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n)  (default n = 3)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Random picks of opponents on Hard (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x46801 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) + d3  (default n = 3)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Random picks of opponents on Impossible (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x46811 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) + d5  (default n = 5)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Random picks of opponents on Impossible+1 (dice)", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x46821 + 1]));
                psubtree->child(subrow, 2)->setData("dice(n) + d8  (default n = 8)", Qt::EditRole);
                subrow++;

                psubtree->appendChild("Total nr of AI picks on Normal and below", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x469ed + 3]));
                psubtree->child(subrow, 2)->setData("(default 11 picks)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Total nr of AI picks on Hard", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x469f9 + 3]));
                psubtree->child(subrow, 2)->setData("(default 13 picks)", Qt::EditRole);
                subrow++;
                psubtree->appendChild("Total nr of AI picks on Impossible", new QMoMTreeItemModel<uint16_t>((uint16_t*)&magicExeContents[0x46a05 + 3]));
                psubtree->child(subrow, 2)->setData("(default 15 picks)", Qt::EditRole);
                subrow++;
            }

//            removeUnusedRows(row, psubtree, subrow);

            row++;
        }

        if (nrCustomizerSections > oldRowCount)
        {
            endInsertRows();
        }
        if (0 == nrCustomizerSections)
        {
            removeUnusedRows(toprow, ptree, row);
        }

        toprow++;
    }

    //{
    //    if (toprow == parentItem->rowCount())
    //    {
    //        parentItem->appendEmptyRow();
    //    }

    //    parentItem->child(toprow, 0)->setData(tr("Custom extensions"), Qt::UserRole);
    //    parentItem->child(toprow, 1)->setData(QString(), Qt::EditRole);
    //    parentItem->child(toprow, 2)->setData(QString(), Qt::EditRole);

    //    int row = 0;
    //    QMoMTreeItemModelBase* ptree = parentItem->child(toprow, 0);
    //    update_Custom_extensions(ptree, game, row);
    //    removeUnusedRows(toprow, ptree, row);

    //    toprow++;
    //}

    emit dataChanged(index(0, 0), index(parentItem->rowCount() - 1, m_columnCount - 1));
}

void UnitModel::traverseTree(QMoMTreeItemModelBase* node, int& nrToResolve)
{
    bool succesfulResolution = false;
    for (int row = 0; (nrToResolve > 0) && (row < node->rowCount()); ++row)
    {
        for (int col = 0; (nrToResolve > 0) && (col < this->columnCount()); ++col)
        {
            succesfulResolution = node->child(row, col)->resolveIcon();
            if (succesfulResolution)
            {
                nrToResolve--;
                // TODO: signal update to GUI
//                emit dataChanged(createIndex(index(row, col, node), index(row, col, node));
            }
        }

        traverseTree(node->child(row, 0), nrToResolve);
    }
}

//QMutex gUnitModelMutex;

class ThreadUpdateModelData : public QThread
{
public:
	explicit ThreadUpdateModelData(UnitModel* unitModel) : QThread(), m_unitModel(unitModel)
	{
	}

	void run()
	{
//		QMutexLocker lock(&gUnitModelMutex);
        assert(0 != m_unitModel);
		m_unitModel->threadUpdateModelData();
	}

private:
	UnitModel* m_unitModel;
};

ThreadUpdateModelData* g_pThreadUpdateModelData = 0;

void UnitModel::abortUpdate()
{
 //   if(0 != g_pThreadUpdateModelData)
    //{
    //    m_abortUpdate = true;
    //    g_pThreadUpdateModelData->wait();
    //    delete g_pThreadUpdateModelData;
    //    g_pThreadUpdateModelData = 0;
    //}
}

void UnitModel::startUpdate()
{
    //if (0 != g_pThreadUpdateModelData)
    //{
    //    abortUpdate();
    //}
    //m_abortUpdate = false;
    //assert(0 == g_pThreadUpdateModelData);
    //g_pThreadUpdateModelData = new ThreadUpdateModelData(this);
    //g_pThreadUpdateModelData->start();
    threadUpdateModelData();
//    waitForUpdate();	// TODO: Remove wait
}

void UnitModel::waitForUpdate()
{
 //   if (0 != g_pThreadUpdateModelData)
    //{
    //	g_pThreadUpdateModelData->wait();
    //	delete g_pThreadUpdateModelData;
    //	g_pThreadUpdateModelData = 0;
    //}
}
