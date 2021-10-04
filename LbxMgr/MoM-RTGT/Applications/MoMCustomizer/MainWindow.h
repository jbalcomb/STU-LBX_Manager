// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

/// \file

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QMessageBox>
#include <QTreeWidget>

#include "MoMGameCustom.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    bool refreshMemory();
    void update();

private:
    Ui::MainWindow *ui;

    QTimer* m_timer;
    std::unique_ptr<MoM::MoMGameCustom> m_game;
    bool m_updating;

private slots:
    void slot_WizardType_editingFinished();
    void on_pushButton_SaveWizardTypes_clicked();
    void on_comboBox_Wizard_currentIndexChanged(int index);
    void on_pushButton_RandomizeHeroAbilities_clicked();
    void on_comboBox_NrMonsters_currentIndexChanged(int index);
    void on_comboBox_WeakLairMonsterStrength_currentIndexChanged(int index);
    void on_comboBox_NrWeakLairs_currentIndexChanged(int index);
    void on_comboBox_NormalLairMonsterStrength_currentIndexChanged(int index);
    void on_comboBox_NrNormalLairs_currentIndexChanged(int index);
    void on_comboBox_NrNodes_currentIndexChanged(int index);
    void on_comboBox_NrTowers_currentIndexChanged(int index);
    void on_comboBox_InitialRoads_currentIndexChanged(int index);
    void on_comboBox_LandSize_currentIndexChanged(int index);
    void on_pushButton_Connect_clicked();
    void on_pushButton_Reread_clicked();

    void onTimer();
    void on_pushButton_Wizard_clicked();
};

#endif // MAINWINDOW_H
