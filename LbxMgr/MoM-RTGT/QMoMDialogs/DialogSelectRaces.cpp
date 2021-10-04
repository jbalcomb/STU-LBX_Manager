#include "DialogSelectRaces.h"
#include "ui_DialogSelectRaces.h"

#include <QMessageBox>

#include "MoMController.h"
#include "MoMGenerated.h"
#include "MoMUtility.h"
#include "QMoMResources.h"
#include "QMoMUtility.h"

using namespace MoM;

DialogSelectRaces::DialogSelectRaces(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogSelectRaces)
{
    ui->setupUi(this);
    postInitialize();
}

DialogSelectRaces::~DialogSelectRaces()
{
    preFinalize();
    delete ui;
}

void DialogSelectRaces::on_pushButton_ReplaceRace_clicked()
{
    eRace  fromRace = (eRace)ui->comboBox_ReplaceFromRace->currentIndex();
    eRace  toRace   = (eRace)ui->comboBox_ReplaceByRace->currentIndex();
    ePlane plane    = (ePlane)ui->comboBox_Plane->currentIndex();

    MoMController momController(m_game.data());
    if (!momController.replaceRace(fromRace, toRace, plane))
    {
        (void)QMessageBox::warning(this,
                                   tr("Select Races"),
                                   tr("Failed to replace race: %1").arg(momController.errorString().c_str()));
    }
    else
    {
        (void)QMessageBox::information(this,
                                       tr("Select Races"),
                                       tr("Replaced race %0 by %1 on %2").arg(prettyQStr(fromRace)).arg(prettyQStr(toRace)).arg(prettyQStr(plane)));
    }
}

void DialogSelectRaces::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;
    ui->comboBox_ReplaceFromRace->clear();
    MOM_FOREACH(eRace, race, gMAX_RACES)
    {
        ui->comboBox_ReplaceFromRace->addItem(*QMoMResources::instance().getIcon(race, 2), prettyQStr(race));
    }
    ui->comboBox_ReplaceByRace->clear();
    MOM_FOREACH(eRace, race, gMAX_RACES)
    {
        ui->comboBox_ReplaceByRace->addItem(*QMoMResources::instance().getIcon(race, 2), prettyQStr(race));
    }
    ui->comboBox_Plane->clear();
    MOM_FOREACH(ePlane, plane, ePlane_MAX)
    {
        ui->comboBox_Plane->addItem(prettyQStr(plane));
    }
}

void DialogSelectRaces::slot_gameUpdated()
{
}

