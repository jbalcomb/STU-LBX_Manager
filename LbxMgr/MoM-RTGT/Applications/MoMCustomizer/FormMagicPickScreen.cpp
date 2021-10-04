#include "FormMagicPickScreen.h"
#include "ui_FormMagicPickScreen.h"

FormMagicPickScreen::FormMagicPickScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormMagicPickScreen)
{
    ui->setupUi(this);
}

FormMagicPickScreen::~FormMagicPickScreen()
{
    delete ui;
}

void FormMagicPickScreen::on_pushButton_OK_clicked()
{
    emit signal_accepted();
}
