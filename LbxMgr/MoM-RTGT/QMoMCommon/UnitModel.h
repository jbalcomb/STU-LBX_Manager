// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef UNITMODEL_H
#define UNITMODEL_H

#include <QAbstractItemModel>
#include <QSharedPointer>

#include "MoMGameBase.h"
#include "MoMUnit.h"
#include "QMoMSharedPointers.h"
#include "QMoMTreeItemModel.h"

class UnitModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit UnitModel(QObject *parent = 0);
	virtual ~UnitModel();

    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

//    void setGame(const QMoMGamePtr& game);
//    void update();
    void updateFirstUnresolvedIcon()
    {
        int nrToResolve = 500;
        traverseTree(m_rootItem.data(), nrToResolve);
    }

public slots:
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();
    void slot_selectionChanged(const QModelIndex &index);

signals:
    void signal_unitChanged(const QMoMUnitPtr& unit);
    void signal_addressChanged(const void *momPointer);

private:
	void abortUpdate();
	void startUpdate();
    void waitForUpdate();
	void threadUpdateModelData();
    void traverseTree(QMoMTreeItemModelBase* node, int& nrToResolve);

//    void setupModelData(const QMoMGamePtr& game);

    template<class T>
    void checkUnitChanged(QMoMTreeItemModelBase* itemBase);

    void removeUnusedRows(int toprow, QMoMTreeItemModelBase* ptree, int firstUnusedRow);

    void update_Buildings(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row);
    void update_Items_in_Game(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row);
    void update_Lairs(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row);
    void update_Spell_Data(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row);
    void update_Unit_Types(QMoMTreeItemModelBase* ptree, const QMoMGamePtr& game, int& row);


    static const int m_columnCount = 3;

    QMoMTreeItemPtr m_rootItem;
	bool m_abortUpdate;

	friend class ThreadUpdateModelData;
};

#endif // UNITMODEL_H
