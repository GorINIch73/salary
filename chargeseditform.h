#ifndef CHARGESEDITFORM_H
#define CHARGESEDITFORM_H

#include <QWidget>

namespace Ui {
class ChargesEditForm;
}

class ChargesEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChargesEditForm(QWidget *parent = nullptr);
    ~ChargesEditForm();

private:
    Ui::ChargesEditForm *ui;
};

#endif // CHARGESEDITFORM_H
