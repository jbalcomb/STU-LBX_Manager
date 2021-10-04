#include "DialogWizard.h"
#include "ui_DialogWizard.h"

#include <QDebug>
#include <QKeyEvent>

#include <MoMGameCustom.h>
#include <QMoMResources.h>
#include "FormGameOptions.h"
#include "FormSelectWizard.h"
#include "FormWizardsName.h"
#include "FormMagicPickScreen.h"
#include "FormSelectSpells.h"
#include "FormSelectRaces.h"
#include "FormSelectBanner.h"

using namespace MoM;

//
// CHANGES to MAGIC.EXE
// ====================
//
// Start as "MC_MAGIC.EXE JENNY".
// The parameter JENNY skips the intro video.
//
// Change the mainscreen menu handling to jump right to NEW GAME.
// seg001:09EC  74 03 (jz :09F1) ->  EB 36 (jmp :0A24)
//
// TODO: When executing this I found:
// TODO: - MoMProcess doesn't allow changing static code yet, since it is not loaded (yet)
// TODO: - The addresses were off by 9 bytes, which indicates a mistake in the address calculations in MoMMagicExe.
//         As a result I had to change seg001:09EC + 9 instead (from MoMMagicExe).
// TODO: - The executable changed by MoMGameCustom is always MAGIC.EXE rather than the
//         executable it connected to (MC_MAGIC.EXE).
//

DialogWizard::DialogWizard(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWizard),
    m_form()
{
    ui->setupUi(this);

    // Start MoM process
    qDebug() << "Start MoM process";
    std::unique_ptr<MoMProcess> momProcess(new MoMProcess);
    bool ok = momProcess->createProcess(
                "C:\\Program Files (x86)\\DOSBox-0.74\\DOSBox.exe",
                "\"C:\\Program Files (x86)\\DOSBox-0.74\\DOSBox.exe\""
                    " -conf C:\\games\\MAGIC-work\\MC_dosbox.conf"      // Execute dosbox.conf with "MAGIC.EXE JENNY"
                    " -noconsole",                                      // No console (in Windows)
                "C:\\games\\MAGIC-work");
    qDebug() << "createProcess() -> " << ok;
    if (ok)
    {
        MoMGameCustom* gameCustom = new MoM::MoMGameCustom;
        m_game = QMoMGameCustomPtr(gameCustom);
        ok = gameCustom->openGame(momProcess);
        qDebug() << "Open MoMGameCustom -> " << ok;
    }

    // Select new game
    if (ok)
    {
        // Trigger MoM to select a new game
        qDebug() << "Trigger New game in main screen";
        MoMMagicDataSegment* dseg = m_game->getMagicDataSegment();
        uint16_t new_button_ID = 0;
        ok = m_game->commitData(&dseg->m_Button_New_game_ID, &new_button_ID, 2);
        qDebug() << "Trigger New game in main screen -> " << ok;
    }

//    uint8_t* seg001 = 0;
//    if (ok)
//    {
//        // m_game->changeMem("seg001:09EC", "\x74\x03", "\xEB\x38", 2);
//        // TODO: Directly in memory instead of on file?
//        seg001 = m_game->getMagicExe()->getSegment(001);
//        qDebug("seg001=%p", seg001);
//        ok = (0 != seg001);
//    }
//    uint8_t* ptr = 0;
//    if (ok)
//    {
//        ptr = seg001 + 0x09EC + 9; // ??? 9 bytes mismatch with IDA
//        qDebug("Orig %02X %02X", ptr[0], ptr[1]);
//        //ok = (ptr[0] == 0x74 && ptr[1] == 0x03);
//    }
//    if (ok)
//    {
//        ok = m_game->commitData(ptr, "\xEB\x36", 2);
//        qDebug("Commit() -> %d. New %02X %02X", ok, ptr[0], ptr[1]);
//    }

    // Display first form
    slot_selectGameOptions();
}

DialogWizard::~DialogWizard()
{
    delete ui;
}

void DialogWizard::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        // Previous form
        qDebug() << "Key_Escape";
        emit signal_rejected();
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        // Next form
        qDebug() << "Key_Enter";
        emit signal_accepted();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

void DialogWizard::slot_selectGameOptions()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormGameOptions* form = new FormGameOptions(this, m_game);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_rejected()));
    QObject::connect(form, SIGNAL(signal_rejected()), this, SLOT(slot_rejected()));
    QObject::connect(this, SIGNAL(signal_accepted()), form, SLOT(on_pushButton_OK_clicked()));
    QObject::connect(form, SIGNAL(signal_accepted()), this, SLOT(slot_selectWizard()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectWizard()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormSelectWizard* form = new FormSelectWizard(this, m_game);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectGameOptions()));
    QObject::connect(form, SIGNAL(signal_accepted()), this, SLOT(slot_selectName()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectName()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormWizardsName* form = new FormWizardsName(this);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectWizard()));
    QObject::connect(this, SIGNAL(signal_accepted()), this, SLOT(slot_selectMagicPicks()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectMagicPicks()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormMagicPickScreen* form = new FormMagicPickScreen(this);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectName()));
    QObject::connect(form, SIGNAL(signal_accepted()), this, SLOT(slot_selectSpells()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectSpells()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormSelectSpells* form = new FormSelectSpells(this);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectMagicPicks()));
    QObject::connect(form, SIGNAL(signal_accepted()), this, SLOT(slot_selectRaces()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectRaces()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormSelectRaces* form = new FormSelectRaces(this);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectSpells()));
    QObject::connect(this, SIGNAL(signal_accepted()), this, SLOT(slot_selectBanner()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_selectBanner()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    FormSelectBanner* form = new FormSelectBanner(this);
    QObject::connect(this, SIGNAL(signal_rejected()), this, SLOT(slot_selectRaces()));
    QObject::connect(this, SIGNAL(signal_accepted()), this, SLOT(slot_accepted()));
    form->show();
    m_form = form;
}

void DialogWizard::slot_accepted()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    qDebug() << "accepted";
    qDebug() << "Execute world generation ... (TODO)";
    qDebug() << "Start WIZARDS.EXE ... (TODO)";
    close();
}

void DialogWizard::slot_rejected()
{
    delete m_form;
    disconnect(SIGNAL(signal_rejected()));
    disconnect(SIGNAL(signal_accepted()));

    qDebug() << "rejected";
    close();
}
