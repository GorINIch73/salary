#include "formpayslip.h"
#include "ui_formpayslip.h"


#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

#include <QSqlError>

FormPayslip::FormPayslip(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPayslip)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    base=db;

    //создание обьектов таблиц
    modelArticles = new QSqlTableModel(this,base);
    modelBank_decryption = new QSqlQueryModel(this);
    mapper = new QDataWidgetMapper(this);

    //Настраиваем модели
    SetupTable();

//    modelArticles->select();

    // сигнал изменения строки выделения в tableVew
//    connect(ui->tableView_articles->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &)));

    // сигнал создания запроса во вкладках
//    connect(this, SIGNAL(signalFromQuery(QString)),parent, SLOT(slot_goQuery(QString)));
    //ui->tableView_articles->selectRow(0);

}

FormPayslip::~FormPayslip()
{
    delete ui;
}

void FormPayslip::slotSelectionChange(const QItemSelection &current, const QItemSelection &previous)
{

}

void FormPayslip::SetupTable()
{

}

void FormPayslip::Tune()
{

}

void FormPayslip::on_pushButton_close_clicked()
{

}

void FormPayslip::on_pushButton_first_clicked()
{

}

void FormPayslip::on_pushButton_prev_clicked()
{

}

void FormPayslip::on_pushButton_next_clicked()
{

}

void FormPayslip::on_pushButton_last_clicked()
{

}

void FormPayslip::on_pushButton_refr_clicked()
{

}

void FormPayslip::on_pushButton_add_clicked()
{

}

void FormPayslip::on_pushButton_del_clicked()
{

}

void FormPayslip::on_pushButton_lst_clicked()
{

}


