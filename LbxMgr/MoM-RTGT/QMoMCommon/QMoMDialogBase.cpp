#include "QMoMDialogBase.h"

#include "MainWindow.h"
#include "QMoMResources.h"
#include "QMoMSettings.h"

using namespace MoM;

QMoMDialogBase::QMoMDialogBase(QWidget *parent) :
    QDialog(parent),
    m_game(),
    m_font()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setWindowFlags(Qt::Window);

    setFont(MoM::QMoMResources::g_Font);
    m_font = MoM::QMoMResources::g_Font;
}

QMoMDialogBase::~QMoMDialogBase()
{
}

void QMoMDialogBase::postInitialize()
{
    QMoMSettings::readSettingsWindow(this);

    QObject::connect(MainWindow::getInstance(), SIGNAL(signal_gameChanged(QMoMGamePtr)), this, SLOT(slot_gameChanged(QMoMGamePtr)));
    QObject::connect(MainWindow::getInstance(), SIGNAL(signal_gameUpdated()), this, SLOT(slot_gameUpdated()));
    slot_gameChanged(MainWindow::getInstance()->getGame());
}

void QMoMDialogBase::preFinalize()
{
    QMoMSettings::writeSettingsWindow(this);
}
