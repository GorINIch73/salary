#ifndef FORMPAYSLIP_H
#define FORMPAYSLIP_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QDataWidgetMapper>
#include <QItemSelection>

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

private:
    Ui::FormPayslip *ui;

    QSqlDatabase base;
    QSqlTableModel *modelArticles;
    QSqlQueryModel *modelBank_decryption;
    QDataWidgetMapper *mapper;

    void SetupTable();
    void Tune();
};

#endif // FORMPAYSLIP_H
