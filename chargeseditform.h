#ifndef CHARGESEDITFORM_H
#define CHARGESEDITFORM_H

#include <QWidget>
#include <QSqlDatabase>
//#include <QSqlQueryModel>

#include "qsqlquerycombomodel.h"

namespace Ui {
class ChargesEditForm;
}

class ChargesEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChargesEditForm(QSqlDatabase db, QWidget *parent = nullptr);
    ~ChargesEditForm();

    void getData(int id);

signals:

    void on_pushButton_refresh_clicked(); // сигнал для обновления начислений

private:
    Ui::ChargesEditForm *ui;

    QSqlDatabase base;
    QSqlQueryComboModel *type_of_charge_id_model;
    QSqlQueryComboModel *order_id_model;

    void setData();


    // QWidget interface
public slots:

    // QWidget interface
protected:
    void hideEvent(QHideEvent *event);
};

#endif // CHARGESEDITFORM_H
