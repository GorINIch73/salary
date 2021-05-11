#ifndef FORMPAYSLIP_H
#define FORMPAYSLIP_H

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

class QMenu;
class ChargesEditForm;


namespace Ui {
class FormPayslip;
}

class FormPayslip : public QWidget
{
    Q_OBJECT

public:
    explicit FormPayslip(QSqlDatabase db,QWidget *parent = nullptr);
    ~FormPayslip();

signals:

    void signalFromQuery(QString sq); // сигнал для запроса

private slots:
    void slotSelectionChange(const QItemSelection &current, const QItemSelection &previous);

    void on_pushButton_close_clicked();
//    void on_pushButton_flt_clr_clicked();
//    void on_pushButton_clr_flt_dec_clicked();

    //void on_lineEdit_flt_all_textChanged(const QString &arg1);
//    void on_lineEdit_flt_dec_textChanged(const QString &arg1);

    void on_pushButton_first_clicked();
    void on_pushButton_prev_clicked();
    void on_pushButton_next_clicked();
    void on_pushButton_last_clicked();
    void on_pushButton_refr_clicked();
    void on_pushButton_add_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_lst_clicked();

    void onPayslipSelected(const QModelIndex &index);
    //void onChargesSelected(const QModelIndex &index);
    void onChenged();

    void on_lineEdit_flt_all_textChanged(const QString &arg1);

    void on_pushButton_refresh_clicked();

    void on_pushButton_add_ch_clicked();

private:
    Ui::FormPayslip *ui;

    bool payslip_chenged;

    QSqlDatabase base;
//    QSqlRelationalTableModel *payslip; //ведомость для редактирования
//    QSqlTableModel *model_payslip; //ведомость для редактирования
    QSqlQueryModel *model_payslip_list; // ведомость
    QSqlQueryModel *model_charges_list; //начисления

    QSqlQueryComboModel  *employee_model;
    QSqlQueryComboModel *position_model;
    QSqlQueryComboModel *branch_model;

    //QDataWidgetMapper *mapper;
//    QSqlRelationalDelegate *delegate;
//    QCompleter *completer;

    //выпадающее меню редактирования начислений
    QMenu *m_charges_menu;
    ChargesEditForm  *m_charges_popup_form;

    void SetupTable();
    void Tune();
    void setPayslip(); // записываем значения в базу
    void getPayslip(int id); // яитаем значения их базы
    void clearPayslip(); // очистка формы
    void getCharges(int id); // заполнение списка начислений


};

#endif // FORMPAYSLIP_H
