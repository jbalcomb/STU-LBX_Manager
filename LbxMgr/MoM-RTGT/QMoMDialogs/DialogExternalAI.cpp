#include "DialogExternalAI.h"
#include "ui_DialogExternalAI.h"

#include "MoMTemplate.h"

DialogExternalAI::DialogExternalAI(QWidget *parent) :
    QMoMDialogBase(parent),
    ui(new Ui::DialogExternalAI),
    hookManager(0)
{
    ui->setupUi(this);
    postInitialize();
}

DialogExternalAI::~DialogExternalAI()
{
    preFinalize();

    delete hookManager;

    delete ui;
}

void DialogExternalAI::on_pushButton_InsertHook_clicked()
{
    if (hookManager->insertHook())
    {
        ui->label_Status->setText("Hook inserted");
    }
    else
    {
        ui->label_Status->setText("Insert failed");
    }
}

void DialogExternalAI::on_pushButton_RaiseHook_clicked()
{
    if (hookManager->raiseHook())
    {
        ui->label_Status->setText("Hook raised");
    }
    else
    {
        ui->label_Status->setText("Raise failed");
    }
}

void DialogExternalAI::on_pushButton_WaitForHook_clicked()
{
    std::string errorString = "Bait ready";

    bool ok = hookManager->waitForBait(0.5);
    if (!ok)
    {
        errorString = "No bait";
    }

    MoM::MoMHookData data;
    if (ok)
    {
        ok = hookManager->readBaitData(data);
        if (!ok)
        {
            errorString = hookManager->errorString();
        }
    }

    if (ok)
    {
        ui->lineEdit_BattleUnitNr->setText(QString("%0").arg(data.battleUnitNr));
        ui->lineEdit_TargetBattleUnitNr->setText(QString("%0").arg(data.targetID));
        ui->lineEdit_Tactic->setText(QString("%0").arg(data.tactic));
        ui->lineEdit_TargetXpos->setText(QString("%0").arg(data.targetXpos));
        ui->lineEdit_TargetYpos->setText(QString("%0").arg(data.targetYpos));
        ui->lineEdit_Parm1->setText(QString("%0").arg(data.parm1));
        ui->lineEdit_Parm2->setText(QString("%0").arg(data.parm2));
    }

    ui->label_Status->setText(errorString.c_str());
}

void DialogExternalAI::on_pushButton_RetractHook_clicked()
{
    if (hookManager->retractHook())
    {
        ui->label_Status->setText("Hook retracted");
    }
    else
    {
        ui->label_Status->setText("Retract failed");
    }
}

void DialogExternalAI::on_pushButton_ReleaseHook_clicked()
{
    bool ok = true;
    std::string errorString = "Bait released";

    MoM::MoMHookData data;

    data.targetID = ui->lineEdit_TargetBattleUnitNr->text().toInt();
    data.tactic = ui->lineEdit_Tactic->text().toInt();
    data.targetXpos = ui->lineEdit_TargetXpos->text().toInt();
    data.targetYpos = ui->lineEdit_TargetYpos->text().toInt();
    data.parm1 = ui->lineEdit_Parm1->text().toInt();
    data.parm2 = ui->lineEdit_Parm1->text().toInt();

    ok = hookManager->writeBaitData(data);
    if (!ok)
    {
        errorString = hookManager->errorString();
    }

    if (ok)
    {
        ok = m_game->readData();
    }

    if (ok)
    {
        ok = hookManager->releaseBait();
        if (!ok)
        {
            errorString = "Release failed";
        }
    }

    ui->label_Status->setText(errorString.c_str());

    if (ok)
    {
        on_pushButton_WaitForHook_clicked();
    }
}

void DialogExternalAI::slot_gameChanged(const QMoMGamePtr &game)
{
    m_game = game;

    delete hookManager;
    hookManager = new MoM::MoMHookManager(m_game.data());

    slot_gameUpdated();
}

void DialogExternalAI::slot_gameUpdated()
{
}
