#include "formemployees.h"
#include "ui_formemployees.h"


#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

#include <QSqlError>

FormEmployees::FormEmployees(QSqlDatabase db,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormEmployees)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    base=db;

    //создание обьектов таблиц
    modelEmployees = new QSqlRelationalTableModel(this,base);
    mapper = new QDataWidgetMapper(this);

    //Настраиваем модели
    SetupTable();

    modelEmployees->select();

    // сигнал изменения строки выделения в tableVew
    connect(ui->tableView->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &)));

    // сигнал создания запроса во вкладках
//    connect(this, SIGNAL(signalFromQuery(QString)),parent, SLOT(slot_goQuery(QString)));

    ui->tableView->selectRow(0);
}

FormEmployees::~FormEmployees()
{
    delete ui;

}

void FormEmployees::slotSelectionChange(const QItemSelection &current, const QItemSelection &previous)
{
    // при изменение строки в таблвьюве устанавливаем маппер на соответствующую запись
    //mapper->submit();
     mapper->setCurrentIndex(ui->tableView->currentIndex().row());
}

void FormEmployees::SetupTable()
{

    //Таблица работников
    modelEmployees->setTable("employees");
    modelEmployees->setSort(modelEmployees->fieldIndex("full_name"),Qt::AscendingOrder);

    // названия колонок
    modelEmployees->setHeaderData(modelEmployees->fieldIndex("full_name"),Qt::Horizontal,"ФИО");

    ui->tableView->setModel(modelEmployees);
    ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);  //запрет редактирования
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents); // по содержимому

    // настриаваем маписн на поля редактирования вопроса
    mapper->setModel(modelEmployees);

    mapper->addMapping(ui->lineEdit_id, modelEmployees->fieldIndex("id"));
    mapper->addMapping(ui->lineEdit_fio, modelEmployees->fieldIndex("full_name"));
    mapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);


}

void FormEmployees::on_pushButton_first_clicked()
{
    // перая запись
    ui->tableView->selectRow(0);
}


void FormEmployees::on_pushButton_prev_clicked()
{
    // прыгаем на предыдущую запись
    ui->tableView->selectRow(ui->tableView->currentIndex().row()-1);
}


void FormEmployees::on_pushButton_next_clicked()
{
    // прыгаем на следующую запись
    ui->tableView->selectRow(ui->tableView->currentIndex().row()+1);
}


void FormEmployees::on_pushButton_last_clicked()
{
    // последняя запись
    ui->tableView->selectRow(modelEmployees->rowCount()-1);
}


void FormEmployees::on_pushButton_refr_clicked()
{
    //восстановление курсора
    int row = ui->tableView->currentIndex().row();
    //обновить по простому
    modelEmployees->select();
    // восстанавливаем строку
    ui->tableView->selectRow(row);
}


void FormEmployees::on_pushButton_add_clicked()
{
    // добавление
    modelEmployees->submit(); // субмитим

    int row= ui->tableView->currentIndex().row()+1; // выбираем следующую

    // вставляем
    modelEmployees->insertRow(row);
    modelEmployees->setData(modelEmployees->index(row,modelEmployees->fieldIndex("full_name")),""); // добавляем пустой для возможности сабмита

    modelEmployees->submit(); // субмитим
    // устанавливаем курсор на строку редактирования
    ui->tableView->selectRow(row);
    // устанавливаем курсор на редактирование имени
    ui->lineEdit_fio->setFocus();
}


void FormEmployees::on_pushButton_del_clicked()
{
    // удаление
    // подтверждение
    if(QMessageBox::Yes != QMessageBox::question(this, tr("Внимание!"),
                                                 tr("Уверены в удалении?")))  return;
    // если есть связи не удаляем!
    QSqlQuery query(base);

    QString tt = QString("SELECT id FROM payslip WHERE employee_id = \%1").arg(modelEmployees->data(modelEmployees->index(ui->tableView->currentIndex().row(), 0)).toString());
    query.exec(tt);
    if (query.first()) {
        qDebug() << "Имеются связанные записи. Удаление невозможно!";
        QMessageBox::critical(this,"Error","Имеются связанные записи. Удаление невозможно!");
        return;
    }

    modelEmployees->removeRow(ui->tableView->currentIndex().row());
    // прыгаем на предыдущую запись
    ui->tableView->selectRow(ui->tableView->currentIndex().row()-1);
}


void FormEmployees::on_pushButton_close_clicked()
{
    // на всякий случай
    mapper->submit();
    modelEmployees->submit();

      close();
}

