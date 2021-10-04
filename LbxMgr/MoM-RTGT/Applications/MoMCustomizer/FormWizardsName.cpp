#include "FormWizardsName.h"
#include "ui_FormWizardsName.h"

FormWizardsName::FormWizardsName(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormWizardsName)
{
    ui->setupUi(this);
}

FormWizardsName::~FormWizardsName()
{
    delete ui;
}
