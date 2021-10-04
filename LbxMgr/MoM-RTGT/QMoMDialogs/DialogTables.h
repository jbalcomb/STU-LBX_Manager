// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef DIALOGTABLES_H
#define DIALOGTABLES_H

#include <QDialog>
#include <QModelIndex>

#include "QMoMDialogBase.h"

namespace Ui {
    class DialogTables;
}

class DialogTables : public QMoMDialogBase
{
    Q_OBJECT

public:
    explicit DialogTables(QWidget *parent = 0);
    ~DialogTables();

protected:
    virtual void keyPressEvent(QKeyEvent* event);

private:
    QColor getRealmColor(MoM::eRealm_Type realm) const;

    void buildTable(const QStringList& labels, int rows, const char* slotFunction);
    void initializeTable(const QStringList& labels, int rows);
    void finalizeTable();

    void update_BuildingData();
    void update_Cities();
    void update_HeroData();
    void update_ItemData();
    void update_ItemPowers();
    void update_ItemsInGame();
    void update_LevelBonus();
    void update_RaceData();
    void update_SpellData();
    void update_UnitTypes();
    void update_UnitsInGame();
    void update_UnrestTable();
//    void update_Wizards();

private slots:
    void on_comboBox_Table_currentIndexChanged(QString );
    void on_tableWidget_clicked(QModelIndex index);
    void on_tableWidget_customContextMenuRequested(const QPoint &pos);

    void slot_addRow_to_BuildingData(int row);
    void slot_addRow_to_Cities(int row);
    void slot_addRow_to_HeroData(int row);
    void slot_addRow_to_ItemData(int row);
    void slot_addRow_to_ItemPowers(int row);
    void slot_addRow_to_ItemsInGame(int row);
    void slot_addRow_to_LevelBonus(int row);
    void slot_addRow_to_RaceData(int row);
    void slot_addRow_to_SpellData(int row);
    void slot_addRow_to_UnitTypes(int row);
    void slot_addRow_to_UnitsInGame(int row);
    void slot_addRow_to_UnrestTable(int row);
//    void slot_addRow_to_Wizards(int row);

    void slot_Copy();
    void slot_ItemAction();
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();

signals:
    void signal_addRow(int row);

private:
    Ui::DialogTables *ui;
};

#endif // DIALOGTABLES_H
