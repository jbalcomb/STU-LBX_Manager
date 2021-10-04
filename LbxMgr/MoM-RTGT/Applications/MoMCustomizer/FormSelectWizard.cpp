#include "FormSelectWizard.h"
#include "ui_FormSelectWizard.h"

#include <QDebug>

#include <MoMGameBase.h>
#include <QMoMResources.h>
using namespace MoM;

FormSelectWizard::FormSelectWizard(QWidget *parent, QMoMGamePtr& game) :
    QWidget(parent),
    ui(new Ui::FormSelectWizard),
    m_game(game)
{
    ui->setupUi(this);

    QFont font = MoM::QMoMResources::g_Font;
    font.setBold(true);
    font.setPixelSize(20);
    this->setFont(font);
    ui->pushButton_Custom->setFont(font);
    ui->pushButton_Custom->setText("Custom");
    ui->pushButton_Wizard_0->setText("Merlin");
}

FormSelectWizard::~FormSelectWizard()
{
    delete ui;
}

void FormSelectWizard::on_pushButton_Custom_clicked()
{
}

void FormSelectWizard::on_pushButton_Wizard_0_clicked()
{
    int portraitNr = 0;

    // Trigger MoM to OK the screen
    qDebug() << "Trigger Portrait 0 in Game Options";
    MoMMagicDataSegment* dseg = m_game->getMagicDataSegment();
    uint16_t new_wizard_id = 0;
    bool ok = m_game->commitData(&dseg->m_Button_IDs[portraitNr], &new_wizard_id, 2);
    qDebug() << "Trigger Portrait in Game Options -> " << ok;

    emit signal_accepted();
}
