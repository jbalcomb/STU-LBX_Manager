#include "FormSelectBanner.h"
#include "ui_FormSelectBanner.h"

FormSelectBanner::FormSelectBanner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormSelectBanner)
{
    ui->setupUi(this);
}

FormSelectBanner::~FormSelectBanner()
{
    delete ui;
}
