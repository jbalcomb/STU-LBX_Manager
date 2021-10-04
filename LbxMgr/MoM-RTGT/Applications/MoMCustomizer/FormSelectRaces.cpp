#include "FormSelectRaces.h"
#include "ui_FormSelectRaces.h"

FormSelectRaces::FormSelectRaces(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSelectRaces)
{
    ui->setupUi(this);
}

FormSelectRaces::~FormSelectRaces()
{
    delete ui;
}
