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

#include "MoMGameBase.h"
#include "MoMManageCities.h"
#include "UnitModel.h"

namespace Ui {
    class MainWindow;
}

namespace MoM {
    class MoMUnit;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void applyBuildQueues();
    bool refreshMemory();
    void update();

	static MainWindow* getInstance()
	{
		return m_instance;
	}

    QMoMGamePtr getGame()
    {
        return m_game;
    }

    QSharedPointer<MoM::MoMManageCities> getManageCities()
    {
        return m_manageCities;
    }

private:
    QTreeWidgetItem* addTreeFeature(QTreeWidgetItem* parent, 
        const QString& feature, const QIcon& featureIcon, 
        const QString& value, const QIcon& valueIcon, 
        const QString& comment = "");

private:
    Ui::MainWindow *ui;

	static MainWindow* m_instance;

    QSharedPointer<QTimer> m_timerReread;
    QSharedPointer<QTimer> m_timerUpdateIcons;

    UnitModel m_UnitModel;

    QFileDialog m_filedialogLoadGame;
    QFileDialog m_filedialogSaveGame;

    QMoMGamePtr m_game;
    QSharedPointer<MoM::MoMManageCities> m_manageCities;

public slots:
    void on_checkBox_UpdateTree_clicked();
    void on_pushButton_AddUnit_clicked();
    void on_pushButton_Calculator_clicked();
    void on_pushButton_Connect_clicked();
    void on_pushButton_LbxEditor_clicked();
    void on_pushButton_Load_clicked();
    void on_pushButton_Map_clicked();
    void on_pushButton_Reread_clicked();
    void on_pushButton_Save_clicked();
    void on_pushButton_ShowTables_clicked();
    void on_pushButton_Tools_clicked();

    void slot_gameChanged(const QMoMGamePtr& game);
	void slot_gameUpdated();
    void slot_timerReread();
    void slot_timerUpdateIcons();

signals:
    void signal_gameChanged(const QMoMGamePtr& game);
	void signal_gameUpdated();

    friend class Test_MoMTweaker_MainWindow;
};

#endif // MAINWINDOW_H
