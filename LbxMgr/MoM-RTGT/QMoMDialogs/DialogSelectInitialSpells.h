// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef DIALOGSELECTINITIALSPELLS_H
#define DIALOGSELECTINITIALSPELLS_H

#include <QAbstractButton>
#include <QDialog>
#include <QListWidgetItem>

#include "QMoMDialogBase.h"

namespace Ui {
    class DialogSelectInitialSpells;
}

class DialogSelectInitialSpells : public QMoMDialogBase
{
    Q_OBJECT

public:
    explicit DialogSelectInitialSpells(QWidget *parent = 0);
    ~DialogSelectInitialSpells();

public slots:
    void slot_gameChanged(const QMoMGamePtr& game);
    void slot_gameUpdated();

private slots:
    void on_listWidget_VeryRare_clicked(QModelIndex index);
    void on_listWidget_Rare_clicked(QModelIndex index);
    void on_listWidget_Uncommon_clicked(QModelIndex index);
    void on_listWidget_Common_clicked(QModelIndex index);
    void on_buttonBox_clicked(QAbstractButton* button);
    void on_pushButton_Sorcery_clicked();
    void on_pushButton_Nature_clicked();
    void on_pushButton_Chaos_clicked();
    void on_pushButton_Death_clicked();
    void on_pushButton_Life_clicked();
    void on_listWidget_VeryRare_itemClicked(QListWidgetItem* item);
    void on_listWidget_Rare_itemClicked(QListWidgetItem* item);
    void on_listWidget_Uncommon_itemClicked(QListWidgetItem* item);
    void on_listWidget_Common_itemClicked(QListWidgetItem* item);

private:
    bool apply();
    void update();

    Ui::DialogSelectInitialSpells *ui;

    int m_Realm_Type;
    class InitialSpells* m_InitialSpells;
};

#endif // DIALOGSELECTINITIALSPELLS_H
