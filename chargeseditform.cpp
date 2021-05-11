#include "chargeseditform.h"
#include "ui_chargeseditform.h"

#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

#include <QSqlError>
#include <QModelIndexList>
#include <QCompleter>

#include "mousewheelwidgetadjustmentguard.h"

ChargesEditForm::ChargesEditForm(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChargesEditForm)
{
    ui->setupUi(this);

    setWindowFlag(Qt::Popup);
//    setAttribute(Qt::WA_DeleteOnClose);
    layout()->setContentsMargins(0,0,0,0);

//    connect(this,SIGNAL(QWidget::closeEvent()),this,SLOT(beforeClose()));
//    connect(this,SIGNAL(QWidget::hideEvent()),this,SLOT(beforeClose()));

    base=db;


    // поля редактирования
      // тип начисления
      type_of_charge_id_model = new QSqlQueryComboModel( "charge", "type_of_charges", base, this );
      ui->comboBox_type_of_charge_id->setModel(type_of_charge_id_model);
      ui->comboBox_type_of_charge_id->setFocusPolicy(Qt::StrongFocus);
      ui->comboBox_type_of_charge_id->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui->comboBox_type_of_charge_id)); //блокируем прокрутку колесом мыши
      //     настраиваем комплитер
      QCompleter *completer_type_of_charge_id = new QCompleter(this);
      completer_type_of_charge_id->setCaseSensitivity(Qt::CaseInsensitive);
      completer_type_of_charge_id->setFilterMode(Qt::MatchContains);
      completer_type_of_charge_id->setModel(type_of_charge_id_model);
      ui->comboBox_type_of_charge_id->setCompleter(completer_type_of_charge_id);

      // Приказ
      order_id_model = new QSqlQueryComboModel( "order_number || '-' || date_of_order", "orders", base, this );
      ui->comboBox_order_id->setModel(order_id_model);
      ui->comboBox_order_id->setFocusPolicy(Qt::StrongFocus);
      ui->comboBox_order_id->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui->comboBox_order_id)); //блокируем прокрутку колесом мыши
      //     настраиваем комплитер
      QCompleter *completer_order_id = new QCompleter(this);
      completer_order_id->setCaseSensitivity(Qt::CaseInsensitive);
      completer_order_id->setFilterMode(Qt::MatchContains);
      completer_order_id->setModel(order_id_model);
      ui->comboBox_order_id->setCompleter(completer_order_id);


      // сигнал создания запроса во вкладках
      connect(this, SIGNAL(on_pushButton_refresh_clicked()),parent, SLOT(on_pushButton_refresh_clicked()));
}

ChargesEditForm::~ChargesEditForm()
{
    //qDebug() << "уничтожено!";
    delete ui;
}

void ChargesEditForm::getData(int id)
{
    // загрузка данных в форму
    QSqlQuery query(base);

    QString req = QString("SELECT id, type_of_charge_id, sum, order_id, verified_with_order, note FROM charges WHERE id= %1").arg(id);

    if(!query.exec(req)) {
        qDebug() << req;
        qDebug() << "ERROR get: " << query.lastError().text();
        return;
    }

    if(query.first()) {
        ui->lineEdit_id->setText(query.value(0).toString());
        ui->comboBox_type_of_charge_id->setCurrentIndex(ui->comboBox_type_of_charge_id->findData(query.value(1).toString()));
        //        ui->comboBox_type_of_charge_id->setCurrentText(query.value(1).toString());
        ui->doubleSpinBox->setValue(query.value(2).toDouble());
//        ui->comboBox_order_id->setCurrentText(query.value(3).toString());
        ui->comboBox_order_id->setCurrentIndex(ui->comboBox_order_id->findData(query.value(3).toString()));
        ui->checkBox_verified->setChecked(query.value(4).toBool());
        ui->lineEdit_note->setText(query.value(5).toString());


        // расчетный процент
    }

}

void ChargesEditForm::setData()
{
    // запись данных в базу
    // если id не пустой
    if (!ui->lineEdit_id->text().isEmpty()) {
        // формируем строку запроса
        QString squery = "";
        // type_of_charge_id, sum, order_id, verified_with_order, note
        squery.append(QString("type_of_charge_id = '%1',").arg(ui->comboBox_type_of_charge_id->currentData().toString()));
        squery.append(QString("sum = '%1',").arg(ui->doubleSpinBox->value()));
        squery.append(QString("order_id = '%1',").arg(ui->comboBox_order_id->currentData().toString()));
        squery.append(QString("verified_with_order = '%1',").arg(ui->checkBox_verified->isChecked()));
        squery.append(QString("note = '%1',").arg(ui->lineEdit_note->text()));

        squery.chop(1);
        squery = "UPDATE charges SET " + squery;

        squery.append(QString(" WHERE id = %1 ").arg(ui->lineEdit_id->text()));

        qDebug() << squery;

        QSqlQuery query(base);
        if(!query.exec(squery)) {
            qDebug() << "ERROR Query update payslip " << query.lastError().text();
            qDebug() << squery;
        }
        // очистить форму
        ui->lineEdit_id->setText("");
//        ui->comboBox_type_of_charge_id->setCurrentIndex(-1);
        ui->comboBox_type_of_charge_id->setCurrentText("");
        ui->doubleSpinBox->setValue(0);
        ui->comboBox_order_id->setCurrentText("");
        ui->checkBox_verified->setChecked(0);
        ui->lineEdit_note->setText("");
        // обновить список
        // послать сигнал на обновление списка зачислений
        // on_pushButton_refresh_clicked();

        emit on_pushButton_refresh_clicked();

    }


}



void ChargesEditForm::hideEvent(QHideEvent *event)
{
    qDebug() << "погашено";
    //сохранить изменения
    setData();
    QWidget::hideEvent(event);

}
