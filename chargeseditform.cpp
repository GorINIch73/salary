#include "chargeseditform.h"
#include "ui_chargeseditform.h"

ChargesEditForm::ChargesEditForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChargesEditForm)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Popup);
    layout()->setContentsMargins(0,0,0,0);


}

ChargesEditForm::~ChargesEditForm()
{
    delete ui;
}
