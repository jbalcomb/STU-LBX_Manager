// ---------------------------------------------------------------------------
// Copyright:   2011 Klaas van Aarsen
// License:     GPL
// Author:      I like Serena (aka Klaas van Aarsen)
// Created:     2010-05-01
// ---------------------------------------------------------------------------

// Third party
#include <QRegExp>
#include <qtimer.h>
#include "MainWindow.h"
#include "ui_MainWindow.h"

// Standard C++ Library
#include <stdlib.h>     // rand(), srand()
#include <sstream>
#include <time.h>       // time()

// Library
#include <MoMGenerated.h>
#include <MoMUtility.h>
#include <QMoMTreeItemModel.h>

// Local
#include "DialogWizard.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_timer(),
    m_updating(false)
{
    ui->setupUi(this);
    srand(time(NULL));

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
    m_timer->start(2000);

    QStringList searchPaths(":/images");
    searchPaths.append(":/units");
    searchPaths.append(":/abilities");
    searchPaths.append(":/spells");
    searchPaths.append(":/race");
    searchPaths.append(":/wizards");
    QDir::setSearchPaths(QString("images"), searchPaths);

    // CONFIG
#ifdef _WIN32
    setFont(QFont("Monotype Corsiva", 11));
#else // Linux
    // Note: attribute italic=true is required for URW Chancery L
    setFont(QFont("URW Chancery L", 12, -1, true));
#endif

    ui->comboBox_WizardSkill->clear();
    ui->comboBox_WizardSkill->addItem("None");
    for (MoM::eSkill skill = (MoM::eSkill)0; skill < MoM::eSkill_MAX; MoM::inc(skill))
    {
        ui->comboBox_WizardSkill->addItem(prettyQStr(skill));
    }

    update();
}

MainWindow::~MainWindow()
{
    delete m_timer;

    delete ui;

    m_game.reset();
}

void MainWindow::update()
{
    ui->pushButton_Reread->setEnabled(0 != m_game.get());
    ui->tab_Landmass->setEnabled(0 != m_game.get());
    ui->tab_Lairs->setEnabled(0 != m_game.get());
    ui->tab_Picks->setEnabled(0 != m_game.get());
    ui->tab_Spells->setEnabled(0 != m_game.get());
    ui->tab_Heroes->setEnabled(0 != m_game.get());

    if (0 == m_game.get())
        return;

    m_updating = true;

    this->setWindowTitle(m_game->getSources().c_str());

    MoM::MoMMagicDataSegment* pDataSegment = m_game->getMagicDataSegment();
    uint8_t* magicExeContents = m_game->getMagicExeContents();
    uint8_t* ovl51 = m_game->getMagicOverlay(51);

    if ((0 != pDataSegment) && (0 != ovl51) && (ui->comboBox_LandSize->currentIndex() + 1 != ui->comboBox_LandSize->count()))
    {
        MoM::eLand_Size l_Land_Size = pDataSegment->m_Game_Settings.m_Land_Size;
        uint16_t l_Total_tiles_Large = *(uint16_t*)&ovl51[0x1EA8 + 3];

        int index;
        if (l_Land_Size < MoM::LANDSIZE_Large)
        {
            index = (int)l_Land_Size;
        }
        else if (l_Total_tiles_Large <= 720)
        {
            index = 2;
        }
        else if (l_Total_tiles_Large <= 960)
        {
            index = 3;
        }
        else
        {
            index = 4;
        }

        ui->comboBox_LandSize->setCurrentIndex(index);
    }

    if ((0 != magicExeContents) && (ui->comboBox_InitialRoads->currentIndex() + 1 != ui->comboBox_InitialRoads->count()))
    {
        uint16_t l_Road_length = *(uint16_t*)&magicExeContents[0x3EAF0 + 1];

        int index;
        if (0 == l_Road_length)
        {
            index = 0;
        }
        else if (l_Road_length < 11)
        {
            index = 1;
        }
        else if (l_Road_length == 11)
        {
            index = 2;
        }
        else
        {
            index = 3;
        }

        ui->comboBox_InitialRoads->setCurrentIndex(index);
    }

    if ((0 != magicExeContents) && (ui->comboBox_NormalLairMonsterStrength->currentIndex() + 1 != ui->comboBox_NormalLairMonsterStrength->count()))
    {
        uint16_t l_Arcanus_Strength = *(uint16_t*)&magicExeContents[0x39574 + 1];

        int index;
        if (l_Arcanus_Strength < 29)
        {
            index = 0;
        }
        else if (l_Arcanus_Strength == 29)
        {
            index = 1;
        }
        else
        {
            index = 2;
        }

        ui->comboBox_NormalLairMonsterStrength->setCurrentIndex(index);
    }

    if ((0 != magicExeContents) && (ui->comboBox_WeakLairMonsterStrength->currentIndex() + 1 != ui->comboBox_WeakLairMonsterStrength->count()))
    {
        uint16_t l_Arcanus_Strength = *(uint16_t*)&magicExeContents[0x3966B + 1];

        int index;
        if (l_Arcanus_Strength <= 10)
        {
            index = 0;
        }
        else if (l_Arcanus_Strength <= 150)
        {
            index = 1;
        }
        else
        {
            index = 2;
        }

        ui->comboBox_WeakLairMonsterStrength->setCurrentIndex(index);
    }

    if ((0 != magicExeContents) && (ui->comboBox_NrMonsters->currentIndex() + 1 != ui->comboBox_NrMonsters->count()))
    {
        uint16_t l_Nr_monsters_in_lair = *(uint16_t*)&magicExeContents[0x39987 + 1];

        int index;
        if (l_Nr_monsters_in_lair < 4)
        {
            index = 0;
        }
        else if (l_Nr_monsters_in_lair == 4)
        {
            index = 1;
        }
        else
        {
            index = 2;
        }

        ui->comboBox_NrMonsters->setCurrentIndex(index);
    }

    if (0 != pDataSegment)
    {
        /*
        int curIndex = ui->comboBox_Wizard->currentIndex();
        ui->comboBox_Wizard->clear();
        for (int wizardNr = 1; wizardNr < m_game->getNrWizards(); ++wizardNr)
        {
            MoM::Wizard* wizard = m_game->getWizard(wizardNr);
            if (0 == wizard)
            {
                break;
            }
            MoM::ePortrait portrait = m_game->getWizard(wizardNr)->m_Portrait;
            ui->comboBox_Wizard->addItem(prettyQStr(portrait));
        }
        ui->comboBox_Wizard->setCurrentIndex(curIndex);
        */

        MoM::ePortrait portrait = (MoM::ePortrait)ui->comboBox_Wizard->currentIndex();
        MoM::Wizard_Type_Data* wizardType = &pDataSegment->m_Wizard_Types[portrait];

        ui->lineEdit_Name->setText(wizardType->m_Wizard_Name);
        ui->lineEdit_Life->setText(QString("%0").arg(wizardType->m_Life_books));
        ui->lineEdit_Death->setText(QString("%0").arg(wizardType->m_Death_books));
        ui->lineEdit_Nature->setText(QString("%0").arg(wizardType->m_Nature_books));
        ui->lineEdit_Chaos->setText(QString("%0").arg(wizardType->m_Chaos_books));
        ui->lineEdit_Sorcery->setText(QString("%0").arg(wizardType->m_Sorcery_books));
        if (MoM::SKILL16_None == wizardType->m_Skill)
        {
            ui->comboBox_WizardSkill->setCurrentIndex(0);
        }
        else
        {
            ui->comboBox_WizardSkill->setCurrentIndex(1 + wizardType->m_Skill);
        }
    }

    m_updating = false;
}

void MainWindow::on_pushButton_Connect_clicked()
{
    std::unique_ptr<MoM::MoMProcess> momProcess( new MoM::MoMProcess );
    std::unique_ptr<MoM::MoMGameCustom> customGame( new MoM::MoMGameCustom );

    if (!momProcess->findProcessAndData())
    {
        statusBar()->showMessage(tr("Game connection failed"));
        (void)QMessageBox::warning(this,
            tr("Connect to MoM"),
            tr("Could not find MoM"));
    }
    else if (!customGame->openGame(momProcess))
    {
        statusBar()->showMessage(tr("Game connection failed"));
        (void)QMessageBox::warning(this,
            tr("Connect to MoM Setup"),
            tr("MoM is not currently in its setup"));
    }
    else
    {
        statusBar()->showMessage(tr("Connected to MoM Setup"));
        m_game = std::move(customGame);
    }

    update();
}

void MainWindow::on_pushButton_Reread_clicked()
{
    if (0 == m_game.get())
        return;
    bool ok = m_game->readData();
    if (!ok)
    {
        statusBar()->showMessage(tr("Reread failed"));
        m_game.reset();
    }
    else
    {
        statusBar()->showMessage(tr("Reread completed"));
    }

    update();
}

void MainWindow::onTimer()
{
    if (0 == m_game.get())
        return;

    bool ok = m_game->readData();
    if (!ok)
    {
        statusBar()->showMessage(tr("Reread failed"), 500);
        m_game.reset();
    }
    else
    {
//        statusBar()->showMessage(tr("Reread completed"), 500);
    }
    update();
}

void MainWindow::on_comboBox_LandSize_currentIndexChanged(int index)
{
    if (0 == m_game.get())
        return;
    MoM::MoMMagicDataSegment* pDataSegment = m_game->getMagicDataSegment();
    uint8_t* ovl51 = m_game->getMagicOverlay(51);
    if ((0 == pDataSegment) || (0 == ovl51))
        return;

    MoM::eLand_Size l_Land_Size = (MoM::eLand_Size)0;
    uint16_t        l_Total_tiles_Large = 720;
    if (index + 1 == ui->comboBox_LandSize->count())
    {
        index = rand() % (ui->comboBox_LandSize->count() - 1);
    }

    switch (index)
    {
    case 0: l_Land_Size = MoM::LANDSIZE_Small; break;
    case 1: l_Land_Size = MoM::LANDSIZE_Medium; break;
    case 2: l_Land_Size = MoM::LANDSIZE_Large; l_Total_tiles_Large = 720; break;
    case 3: l_Land_Size = MoM::LANDSIZE_Large; l_Total_tiles_Large = 960; break;
    case 4: l_Land_Size = MoM::LANDSIZE_Large; l_Total_tiles_Large = 1200; break;
    default: break;
    }

    if (!m_game->commitData(&pDataSegment->m_Game_Settings.m_Land_Size, &l_Land_Size, sizeof(l_Land_Size)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
    if (!m_game->commitData((uint16_t*)&ovl51[0x1EA8 + 3], &l_Total_tiles_Large, sizeof(l_Total_tiles_Large)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_comboBox_InitialRoads_currentIndexChanged(int index)
{
    if (0 == m_game.get())
        return;
    uint8_t* magicExeContents = m_game->getMagicExeContents();
    if (0 == magicExeContents)
        return;

    uint16_t l_Road_length = 11;
    if (index + 1 == ui->comboBox_InitialRoads->count())
    {
        index = rand() % (ui->comboBox_InitialRoads->count() - 1);
    }
    switch (index)
    {
    case 0: l_Road_length = 0; break;
    case 1: l_Road_length = 6; break;
    case 2: l_Road_length = 11; break;
    case 3: l_Road_length = 21; break;
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x3EAF0 + 1], &l_Road_length, sizeof(l_Road_length)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_comboBox_NrTowers_currentIndexChanged(int)
{

}

void MainWindow::on_comboBox_NrNodes_currentIndexChanged(int)
{

}

void MainWindow::on_comboBox_NrNormalLairs_currentIndexChanged(int)
{

}

void MainWindow::on_comboBox_NormalLairMonsterStrength_currentIndexChanged(int index)
{
    if (0 == m_game.get())
        return;
    uint8_t* magicExeContents = m_game->getMagicExeContents();
    if (0 == magicExeContents)
        return;

    uint16_t l_Arcanus_Strength = 29;
    uint16_t l_Myrror_Strength = 24;
    if (index + 1 == ui->comboBox_NormalLairMonsterStrength->count())
    {
        index = rand() % (ui->comboBox_NormalLairMonsterStrength->count() - 1);
    }
    switch (index)
    {
    case 0: l_Arcanus_Strength = 14; l_Myrror_Strength = 12; break;
    case 1: l_Arcanus_Strength = 29; l_Myrror_Strength = 24; break;
    case 2: l_Arcanus_Strength = 44; l_Myrror_Strength = 36; break;
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x39574 + 1], &l_Arcanus_Strength, sizeof(l_Arcanus_Strength)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x39588 + 1], &l_Myrror_Strength, sizeof(l_Myrror_Strength)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_comboBox_NrWeakLairs_currentIndexChanged(int)
{

}

void MainWindow::on_comboBox_WeakLairMonsterStrength_currentIndexChanged(int index)
{
    if (0 == m_game.get())
        return;
    uint8_t* magicExeContents = m_game->getMagicExeContents();
    if (0 == magicExeContents)
        return;

    uint16_t l_Arcanus_Strength = 10;
    uint16_t l_Myrror_Strength = 20;
    if (index + 1 == ui->comboBox_WeakLairMonsterStrength->count())
    {
        index = rand() % (ui->comboBox_WeakLairMonsterStrength->count() - 1);
    }
    switch (index)
    {
    case 0: l_Arcanus_Strength = 10; l_Myrror_Strength = 20; break;
    case 1: l_Arcanus_Strength = 150; l_Myrror_Strength = 250; break;
    case 2: l_Arcanus_Strength = 300; l_Myrror_Strength = 500; break;
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x3966B + 1], &l_Arcanus_Strength, sizeof(l_Arcanus_Strength)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x39670 + 1], &l_Myrror_Strength, sizeof(l_Myrror_Strength)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_comboBox_NrMonsters_currentIndexChanged(int index)
{
    if (0 == m_game.get())
        return;
    uint8_t* magicExeContents = m_game->getMagicExeContents();
    if (0 == magicExeContents)
        return;

    uint16_t l_Nr_monsters_in_lair = 4;
    if (index + 1 == ui->comboBox_NrMonsters->count())
    {
        index = rand() % (ui->comboBox_NrMonsters->count() - 1);
    }
    switch (index)
    {
    case 0: l_Nr_monsters_in_lair = 2; break;
    case 1: l_Nr_monsters_in_lair = 4; break;
    case 2: l_Nr_monsters_in_lair = 6; break;
    }
    if (!m_game->commitData((uint16_t*)&magicExeContents[0x39987 + 1], &l_Nr_monsters_in_lair, sizeof(l_Nr_monsters_in_lair)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }

//    psubtree->appendChild("Nr monsters in lair (dice)", new TreeItem<uint16_t>((uint16_t*)&magicExeContents[0x39987 + 1]));
//    psubtree->child(subrow, 2)->setData("dice(n)  See OSG p.416-418  (default n = 4)", Qt::EditRole);
//    subrow++;
//    psubtree->appendChild("Nr secondary monsters in lair (dice)", new TreeItem<uint16_t>((uint16_t*)&magicExeContents[0x39B6E + 1]));
//    psubtree->child(subrow, 2)->setData("dice(n - Nr primary monsters)  (default n = 10)", Qt::EditRole);
//    subrow++;
//    psubtree->appendChild("Lower primary monsters (dice)", new TreeItem<uint16_t>((uint16_t*)&magicExeContents[0x39A93 + 1]));
//    psubtree->child(subrow, 2)->setData("1 in dice(n) throws primary back  (default n = 2)", Qt::EditRole);
//    subrow++;
}

void MainWindow::on_pushButton_RandomizeHeroAbilities_clicked()
{
    if (0 == m_game.get())
        return;
    MoM::MoMMagicDataSegment* pDataSegment = m_game->getMagicDataSegment();
    if (0 == pDataSegment)
        return;

    for (size_t heroNr = 0; heroNr < MoM::gMAX_HERO_TYPES; ++heroNr)
    {
        MoM::Hero_Stats_Initializer* heroInit = &pDataSegment->m_Hero_Stats_Initializers[heroNr];
        MoM::Hero_Ability* heroAbilities = &heroInit->m_Hero_Abilities.s;

        if (heroAbilities->Leadership)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Leadership_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Legendary)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Legendary_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Blademaster)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Blademaster_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Armsmaster)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Armsmaster_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Constitution)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Constitution_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Might)
        {
            heroInit->m_Nr_Random_picks++;
        }

        if (heroAbilities->Might_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Arcane_Power)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Arcane_Power_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Sage)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Sage_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }

        if (heroAbilities->Prayermaster)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Prayermaster_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Agility_X)
        {
            heroInit->m_Nr_Random_picks += 2;
        }
        if (heroAbilities->Lucky)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Charmed)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Noble)
        {
            heroInit->m_Nr_Random_picks++;
        }
        if (heroAbilities->Agility)
        {
            heroInit->m_Nr_Random_picks++;
        }

        heroInit->m_Hero_Abilities.bits = 0;
    }

    if (!m_game->commitData(pDataSegment->m_Hero_Stats_Initializers, pDataSegment->m_Hero_Stats_Initializers, sizeof(pDataSegment->m_Hero_Stats_Initializers)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_comboBox_Wizard_currentIndexChanged(int)
{
    update();
}

void MainWindow::on_pushButton_SaveWizardTypes_clicked()
{
    if (0 == m_game.get())
        return;
    MoM::MoMMagicDataSegment* pMemDataSegment = m_game->getMagicDataSegment();
    if (0 == pMemDataSegment)
        return;
    MoM::MoMMagicDataSegment* pExeDataSegment = m_game->getMagicExeDataSegment();
    if (0 == pExeDataSegment)
        return;

    if (!m_game->commitData(pExeDataSegment, pMemDataSegment, sizeof(pExeDataSegment->m_Wizard_Types)))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::slot_WizardType_editingFinished()
{
    if (m_updating)
        return;

    if (0 == m_game.get())
        return;
    MoM::MoMMagicDataSegment* pDataSegment = m_game->getMagicDataSegment();
    if (0 == pDataSegment)
        return;

    MoM::ePortrait portrait = (MoM::ePortrait)ui->comboBox_Wizard->currentIndex();
    MoM::Wizard_Type_Data* wizardType = &pDataSegment->m_Wizard_Types[portrait];

    memset(wizardType->m_Wizard_Name, '\0', sizeof(wizardType->m_Wizard_Name));
    strncpy(wizardType->m_Wizard_Name, ui->lineEdit_Name->text().toStdString().data(), sizeof(wizardType->m_Wizard_Name) - 1);
    wizardType->m_Life_books = ui->lineEdit_Life->text().toShort();
    wizardType->m_Death_books = ui->lineEdit_Death->text().toShort();
    wizardType->m_Nature_books = ui->lineEdit_Nature->text().toShort();
    wizardType->m_Chaos_books = ui->lineEdit_Chaos->text().toShort();
    wizardType->m_Sorcery_books = ui->lineEdit_Sorcery->text().toShort();
    if (0 == ui->comboBox_WizardSkill->currentIndex())
    {
        wizardType->m_Skill = MoM::SKILL16_None;
    }
    else if (ui->comboBox_WizardSkill->currentIndex() < MoM::eSkill_MAX + 1)
    {
        wizardType->m_Skill = (MoM::eSkill16)(ui->comboBox_WizardSkill->currentIndex() - 1);
    }
    else
    {
    }

    if (!m_game->commitData(&pDataSegment->m_Wizard_Types[portrait], &pDataSegment->m_Wizard_Types[portrait], sizeof(pDataSegment->m_Wizard_Types[portrait])))
    {
        statusBar()->showMessage(tr("Failed to commit"), 1000);
    }
}

void MainWindow::on_pushButton_Wizard_clicked()
{
    DialogWizard* dialog = new DialogWizard(this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->show();
}
