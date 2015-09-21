#include "dialogcustom.h"
#include "ui_dialogcustom.h"

DialogCustom::DialogCustom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustom)
{
    ui->setupUi(this);
}

DialogCustom::~DialogCustom()
{
    delete ui;
}
