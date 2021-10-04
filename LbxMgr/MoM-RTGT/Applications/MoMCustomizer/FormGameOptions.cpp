#include "FormGameOptions.h"
#include "ui_FormGameOptions.h"

#include <QDebug>

#include <MoMGameBase.h>
using namespace MoM;

FormGameOptions::FormGameOptions(QWidget *parent, QMoMGamePtr& game) :
    QWidget(parent),
    ui(new Ui::FormGameOptions),
    m_game(game)
{
    ui->setupUi(this);
}

FormGameOptions::~FormGameOptions()
{
    delete ui;
}

void FormGameOptions::on_pushButton_OK_clicked()
{
    // Trigger MoM to OK the screen
    qDebug() << "Trigger OK in Game Options";
    MoMMagicDataSegment* dseg = m_game->getMagicDataSegment();
    uint16_t new_OK_id = 0;
    bool ok = m_game->commitData(&dseg->m_Button_OK_ID, &new_OK_id, 2);
    qDebug() << "Trigger OK in Game Options -> " << ok;

    // TODO: Problem if MoM does not accept OK.
    //       In that case it becomes stuck in an endless loop on a message.

    emit signal_accepted();
}

void FormGameOptions::on_pushButton_Cancel_clicked()
{
    emit signal_rejected();
}
