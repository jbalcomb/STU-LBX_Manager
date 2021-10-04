// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

// Qt
#include <QImageWriter>
#include <QRegExp>
#include <QSettings>
#include <QTimer>
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Standard C++ Library
#include <sstream>

// Library
#include "MoMController.h"
#include "MoMGameCustom.h"
#include "MoMGenerated.h"
#include "MoMLbxBase.h"
#include "MoMGameMemory.h"
#include "MoMManageCities.h"
#include "MoMProcess.h"
#include "MoMGameSave.h"
#include "MoMUtility.h"
#include "QMoMResources.h"
#include "QMoMSettings.h"

// Local
#include "DialogAddUnit.h"
#include "DialogCalculatorAddress.h"
#include "DialogLbxEditor.h"
#include "DialogMap.h"
#include "DialogTables.h"
#include "DialogTools.h"


MainWindow* MainWindow::m_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timerReread(new QTimer()),
    m_timerUpdateIcons(new QTimer()),
    m_UnitModel(this),
    m_filedialogLoadGame(this),
    m_filedialogSaveGame(this),
    m_game(),
    m_manageCities()
{
    ui->setupUi(this);

	m_instance = this;

    QStringList searchPaths(":/images");
    searchPaths.append(":/units");
    searchPaths.append(":/abilities");
    searchPaths.append(":/spells");
    searchPaths.append(":/race");
    searchPaths.append(":/wizards");
    QDir::setSearchPaths(QString("images"), searchPaths);

    QStringList filters;
    filters << "MoM files (*.insecticide* *.gam wizards*.exe magic*.exe builddat.lbx itemdata.lbx itempow.lbx spelldat.lbx terrstat.lbx"
               " *.INSECTICIDE* *.GAM WIZARDS*.EXE MAGIC*.EXE BUILDDAT.LBX ITEMDATA.LBX ITEMPOW.LBX SPELLDAT.LBX TERRSTAT.LBX)";
    filters << "SAVE *.GAM files (*.gam *.GAM)";
    filters << "EXE files (*.exe *.EXE)";
    filters << "LBX files (*.lbx *.LBX)";
    filters << "Memory dumps (*.insecticide* *.INSECTICIDE*)";
    filters << "All files (*)";

    m_filedialogLoadGame.setObjectName("filedialogLoadGame");
    m_filedialogLoadGame.setWindowTitle(tr("Open MoM file"));
    m_filedialogLoadGame.setNameFilters(filters);
    m_filedialogLoadGame.setAcceptMode(QFileDialog::AcceptOpen);
    m_filedialogLoadGame.setFileMode(QFileDialog::ExistingFile);
    m_filedialogLoadGame.setViewMode(QFileDialog::Detail);

    m_filedialogSaveGame.setObjectName("filedialogSaveGame");
    m_filedialogSaveGame.setWindowTitle(tr("Save MoM file"));
    m_filedialogSaveGame.setNameFilters(filters);
    m_filedialogSaveGame.setAcceptMode(QFileDialog::AcceptSave);
    m_filedialogSaveGame.setFileMode(QFileDialog::AnyFile);
    m_filedialogSaveGame.setViewMode(QFileDialog::Detail);

    // CONFIG

    setFont(MoM::QMoMResources::g_Font);
    ui->treeView_MoM->setFont(MoM::QMoMResources::g_Font);

#ifdef _WIN32
    m_filedialogLoadGame.setDirectory("C:/GAMES/");
    m_filedialogSaveGame.setDirectory("C:/GAMES/");
#else // Linux
//    m_filedialogLoadGame.setDirectory("/media/C_DRIVE/GAMES/MAGIC-work/");
//    m_filedialogSaveGame.setDirectory("/media/C_DRIVE/GAMES/MAGIC-work/");
    QFont fontTreeView(MoM::QMoMResources::g_Font);
    fontTreeView.setPointSize(fontTreeView.pointSize() - 1);
    ui->treeView_MoM->setFont(fontTreeView);
#endif

    // Connect the item model UnitModel to the treeview
    // TODO: Move to new method in UnitModel
    ui->treeView_MoM->setModel(&m_UnitModel);

    // Read settings
    QMoMSettings::readSettingsWindow(this);

    QObject::connect(ui->treeView_MoM, SIGNAL(clicked(const QModelIndex &)), &m_UnitModel, SLOT(slot_selectionChanged(const QModelIndex &)));

    // Make internal connections to handle events centrally
    QObject::connect(this, SIGNAL(signal_gameChanged(QMoMGamePtr)), this, SLOT(slot_gameChanged(QMoMGamePtr)));
	QObject::connect(this, SIGNAL(signal_gameUpdated()), this, SLOT(slot_gameUpdated()));
    // Connect a timer to trigger refresh updates
    QObject::connect(m_timerReread.data(), SIGNAL(timeout()), this, SLOT(slot_timerReread()));
    QObject::connect(m_timerUpdateIcons.data(), SIGNAL(timeout()), this, SLOT(slot_timerUpdateIcons()));

    // Connect the item model UnitModel to signals from this class
	if (ui->checkBox_UpdateTree->isChecked())
	{
        QObject::connect(this, SIGNAL(signal_gameChanged(QMoMGamePtr)), &m_UnitModel, SLOT(slot_gameChanged(QMoMGamePtr)));
        QObject::connect(this, SIGNAL(signal_gameUpdated()), &m_UnitModel, SLOT(slot_gameUpdated()));
	}

    // Signal to start with an empty game
    emit signal_gameChanged(m_game);

	// Give the tree view a kick to display its contents
    // TODO: This should not be necessary
    m_UnitModel.slot_gameChanged(m_game);
    ui->treeView_MoM->update();

    // Start the timers
    m_timerReread->start(5000);
    m_timerUpdateIcons->start(100);
}

MainWindow::~MainWindow()
{
    QMoMSettings::writeSettingsWindow(this);

    delete ui;
}

QTreeWidgetItem* MainWindow::addTreeFeature(QTreeWidgetItem* parent, 
                                const QString& feature, const QIcon& featureIcon, 
                                const QString& value, const QIcon& valueIcon, 
                                const QString& comment)
{
    QStringList qFeature(feature);
    qFeature.append(value);
    qFeature.append(comment);
    QTreeWidgetItem* qtreeFeature = new QTreeWidgetItem(parent, qFeature);
    qtreeFeature->setIcon(0, featureIcon);
    qtreeFeature->setIcon(1, valueIcon);
    return qtreeFeature;
}

void MainWindow::applyBuildQueues()
{
    if (m_game.isNull())
    {
        (void)QMessageBox::warning(this,
            tr("Apply Building Queues"),
            tr("There is no game to operate on"));
        return;
    }

    bool ok = refreshMemory();
    if (ok)
    {
//        MoM::MoMController momController(m_game.data());
//        bool changed = momController.applyBuildingQueue(MoM::PLAYER_YOU);
        bool changed = m_manageCities->apply();
        if (changed)
        {
            statusBar()->showMessage(tr("Building Queues applied involving a change"));
            emit signal_gameUpdated();
        }
    }

}

bool MainWindow::refreshMemory()
{
    bool ok = true;
    if (!m_game.isNull())
    {
        ok = m_game->readData();
        if (!ok)
        {
            (void)QMessageBox::warning(this, 
                tr("Refresh Memory"),
                tr("Failed to reread the process memory"));
        }
    }
    return ok;
}

void MainWindow::update()
{
    std::string title = "MoM Real-Time Game Tweaker";
    if (!m_game.isNull())
    {
        title = m_game->getSources();
    }

    title += " " VERSION;

    setWindowTitle(title.c_str());

    ui->pushButton_Reread->setEnabled(
            (0 != dynamic_cast<MoM::MoMGameMemory*>(m_game.data()))
            || (0 != dynamic_cast<MoM::MoMGameCustom*>(m_game.data()))
            );
    ui->pushButton_Save->setEnabled(0 != dynamic_cast<MoM::MoMGameSave*>(m_game.data()));
    ui->checkBox_UseIcons->setEnabled(ui->checkBox_UpdateTree->isChecked());
}

void MainWindow::on_pushButton_Connect_clicked()
{
    std::unique_ptr<MoM::MoMProcess> momProcess( new MoM::MoMProcess );

    bool ok = momProcess->findProcessAndData();
	QMoMGamePtr newGame;
    QMoMGameMemoryPtr memGame( new MoM::MoMGameMemory );
    QMoMGameCustomPtr customGame( new MoM::MoMGameCustom );
    if (ok && memGame->openGame(momProcess))
    {
		newGame = memGame.dynamicCast<MoM::MoMGameBase>();
    }
    else if (ok && customGame->openGame(momProcess))
    {
        newGame = customGame.dynamicCast<MoM::MoMGameBase>();
    }
    else
    {
        ok = false;
    }
    if (!ok)
    {
        statusBar()->showMessage(tr("Game connection failed"));
        (void)QMessageBox::warning(this,
            tr("Connect to MoM"),
            tr("Could not find MoM window"));
    }
    else
    {
        statusBar()->showMessage(tr("Game connected"));
    }

	if (ok)
	{
        emit signal_gameChanged(newGame);
	}
}

void MainWindow::on_pushButton_LbxEditor_clicked()
{
    DialogLbxEditor* dialog = new DialogLbxEditor(MainWindow::getInstance());
    dialog->show();
}

void MainWindow::on_pushButton_Load_clicked()
{
    if (!m_filedialogLoadGame.exec())
        return;

    QString fileName = m_filedialogLoadGame.selectedFiles().first();

    MoM::MoMLbxBase lbxFile;
    QMoMGamePtr newGame;

    statusBar()->showMessage(tr("Loading game..."));

    QMoMGameSavePtr saveGame( new MoM::MoMGameSave );
    QMoMGameMemoryPtr memoryGame = m_game.dynamicCast<MoM::MoMGameMemory>();
    bool ok = saveGame->load(fileName.toUtf8());
    if (ok)
    {
        newGame = saveGame.dynamicCast<MoM::MoMGameBase>();
    }
    else if (memoryGame)
    {
        ok = memoryGame->load(fileName.toUtf8());
        if (ok)
        {
            newGame = memoryGame.dynamicCast<MoM::MoMGameBase>();
        }
    }
    else
    {
    }

    if (!ok)
    {
        statusBar()->showMessage(tr("Failed to load game"));
        (void)QMessageBox::warning(this,
            tr("Load MoM game"),
            tr("Could not load MoM game '%1'").arg(fileName));
    }
    else
    {
        statusBar()->showMessage(tr("Game loaded"));
    }

	emit signal_gameChanged(newGame);
}

void MainWindow::on_pushButton_Save_clicked()
{
    bool ok = true;
    QMoMGameSavePtr saveGame = m_game.dynamicCast<MoM::MoMGameSave>();
    QMoMGameMemoryPtr memoryGame = m_game.dynamicCast<MoM::MoMGameMemory>();
    if (saveGame && m_game->isOpen())
    {
        if (!m_filedialogSaveGame.exec())
        {
            return;
        }

        QString fileName = m_filedialogSaveGame.selectedFiles().first();
        ok = saveGame->save(fileName.toUtf8());
        if (!ok)
        {
            (void)QMessageBox::warning(this,
                tr("Save MoM file"),
                tr("Failed to save MoM game '%1'").arg(fileName));
        }
    }
    else if (memoryGame && m_game->isOpen())
    {
        if (!m_filedialogSaveGame.exec())
        {
            return;
        }

        QString fileName = m_filedialogSaveGame.selectedFiles().first();
        ok = memoryGame->save(fileName.toUtf8());
        if (!ok)
        {
            (void)QMessageBox::warning(this,
                tr("Save MoM memory"),
                tr("Failed to save MoM game '%1'").arg(fileName));
        }
    }
    else
    {
        (void)QMessageBox::warning(this, 
            tr("Save MoM game"),
            tr("There is nothing to save"));
        ok = false;
    }


    if (!ok)
    {
        statusBar()->showMessage(tr("Failed to save game"));
    }
    else
    {
        statusBar()->showMessage(tr("Game saved"));
    }
}

void MainWindow::on_pushButton_Reread_clicked()
{
    if (m_game.isNull())
        return;

    bool ok = m_game->readData();
    if (!ok)
    {
        statusBar()->showMessage(tr("Reread failed"));
    }
    else
    {
        statusBar()->showMessage(tr("Reread completed"));
		emit signal_gameUpdated();
    }
}

void MainWindow::slot_timerReread()
{
    if (m_game.isNull() || (0 != dynamic_cast<MoM::MoMGameSave*>(m_game.data())))
        return;

    bool ok = m_game->readData();
    if (!ok)
    {
        statusBar()->showMessage(tr("Reread failed"), 200);
    }
    else
    {
        statusBar()->showMessage(tr("Reread completed"), 200);
    }
	if (ok && ui->checkBox_ApplyBuildQueues->isChecked())
	{
		applyBuildQueues();
	}
	if (ok)
	{
		emit signal_gameUpdated();
    }
}

void MainWindow::slot_timerUpdateIcons()
{
    if (ui->checkBox_UpdateTree->isChecked() && ui->checkBox_UseIcons->isChecked())
    {
        m_UnitModel.updateFirstUnresolvedIcon();
    }
}

void MainWindow::on_pushButton_AddUnit_clicked()
{
    MoM::DialogAddUnit* dialog = new MoM::DialogAddUnit(this);
    dialog->show();

    // Connect the item model UnitModel to the dialog
    QObject::connect(&m_UnitModel, SIGNAL(signal_unitChanged(QMoMUnitPtr)), dialog, SLOT(slot_unitChanged(QMoMUnitPtr)));
}

void MainWindow::on_pushButton_Calculator_clicked()
{
    DialogCalculatorAddress* dialog = new DialogCalculatorAddress(MainWindow::getInstance());
    dialog->show();

    QObject::connect(&m_UnitModel, SIGNAL(signal_addressChanged(const void*)), dialog, SLOT(slot_addressChanged(const void*)));
}

void MainWindow::on_pushButton_ShowTables_clicked()
{
    DialogTables* dialog = new DialogTables(this);
    dialog->show();
}

void MainWindow::on_pushButton_Map_clicked()
{
    MoM::DialogMap* dialog = new MoM::DialogMap(this);
    dialog->show();
}

void MainWindow::on_pushButton_Tools_clicked()
{
    DialogTools* dialog = new DialogTools(this);
    dialog->show();
}

void MainWindow::on_checkBox_UpdateTree_clicked()
{
	if (ui->checkBox_UpdateTree->isChecked())
    {
	    // Connect the item model UnitModel to signals from this class
		QObject::connect(this, SIGNAL(signal_gameChanged(QMoMGamePtr)), &m_UnitModel, SLOT(slot_gameChanged(QMoMGamePtr)));
		QObject::connect(this, SIGNAL(signal_gameUpdated()), &m_UnitModel, SLOT(slot_gameUpdated()));
		statusBar()->showMessage(tr("Updating Treeview..."));
	    m_UnitModel.slot_gameChanged(m_game);
        statusBar()->showMessage(tr("Treeview updated"));
	}
	else
	{
	    // Disconnect the item model UnitModel to signals from this class
		QObject::disconnect(this, SIGNAL(signal_gameChanged(QMoMGamePtr)), &m_UnitModel, SLOT(slot_gameChanged(QMoMGamePtr)));
		QObject::disconnect(this, SIGNAL(signal_gameUpdated()), &m_UnitModel, SLOT(slot_gameUpdated()));
        statusBar()->showMessage(tr("Treeview is not updated anymore"));
	}
    update();
}

void MainWindow::slot_gameChanged(const QMoMGamePtr& game)
{
    m_game = game;
    m_manageCities = QSharedPointer<MoM::MoMManageCities>(new MoM::MoMManageCities(m_game.data()));

	// TODO: Resources should be loaded centrally...
	// Load resources
    MoM::QMoMResources::instance().setGame(m_game);

	update();
}

void MainWindow::slot_gameUpdated()
{
	update();
}
