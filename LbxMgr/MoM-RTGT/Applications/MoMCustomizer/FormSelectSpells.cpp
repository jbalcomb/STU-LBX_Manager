#include "FormSelectSpells.h"
#include "ui_FormSelectSpells.h"

FormSelectSpells::FormSelectSpells(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSelectSpells)
{
    ui->setupUi(this);
}

FormSelectSpells::~FormSelectSpells()
{
    delete ui;
}

void FormSelectSpells::on_pushButton_OK_clicked()
{
    emit signal_accepted();
}
