#ifndef FORMEMPLOYEES_H
#define FORMEMPLOYEES_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QItemSelection>
#include <QSqlRelationalDelegate>
#include <QCompleter>


#include "qsqlquerycombomodel.h"

namespace Ui {
class FormEmployees;
}

class FormEmployees : public QWidget
{
    Q_OBJECT

public:
    explicit FormEmployees(QSqlDatabase db,QWidget *parent = nullptr);
    ~FormEmployees();

private slots:
    void slotSelectionChange(const QItemSelection &current, const QItemSelection &previous);

    void on_pushButton_first_clicked();

    void on_pushButton_prev_clicked();

    void on_pushButton_next_clicked();

    void on_pushButton_last_clicked();

    void on_pushButton_refr_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_close_clicked();

private:
    Ui::FormEmployees *ui;

    QSqlDatabase base;

    QSqlRelationalTableModel *modelEmployees;
//    QSqlQueryModel *modelBank_decryption;
//    QSqlQueryModel *modelCounterparties;
    QDataWidgetMapper *mapper;
//    QSqlRelationalDelegate *delegate;
//    QCompleter *completer_counterparties;

    void SetupTable();
};

#endif // FORMEMPLOYEES_H
