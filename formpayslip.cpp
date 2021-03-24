#include "formpayslip.h"
#include "ui_formpayslip.h"


#include <QDebug>
#include <QSqlQuery>
#include <QMessageBox>

#include <QSqlError>
#include <QModelIndexList>

#include "mousewheelwidgetadjustmentguard.h"

#include "chargeseditform.h"
#include <QMenu>
#include<QVBoxLayout>



FormPayslip::FormPayslip(QSqlDatabase db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPayslip)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_DeleteOnClose);

    base=db;
    //payslip_chenged=false;


    // поля редактирования
      // ФИО
      employee_model = new QSqlQueryComboModel( "full_name", "employees", base, this );
      ui->comboBox_employee->setModel(employee_model);
      ui->comboBox_employee->setFocusPolicy(Qt::StrongFocus);
      ui->comboBox_employee->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui->comboBox_employee)); //блокируем прокрутку колесом мыши
      //     настраиваем комплитер
      QCompleter *completer_employee = new QCompleter(this);
      completer_employee->setCaseSensitivity(Qt::CaseInsensitive);
      completer_employee->setFilterMode(Qt::MatchContains);
      completer_employee->setModel(employee_model);
      ui->comboBox_employee->setCompleter(completer_employee);

      // Должность
      position_model = new QSqlQueryComboModel( "id || '-' || position", "positions", base, this );
      ui->comboBox_position->setModel(position_model);
      ui->comboBox_position->setFocusPolicy(Qt::StrongFocus);
      ui->comboBox_position->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui->comboBox_position)); //блокируем прокрутку колесом мыши
      //     настраиваем комплитер
      QCompleter *completer_position = new QCompleter(this);
      completer_position->setCaseSensitivity(Qt::CaseInsensitive);
      completer_position->setFilterMode(Qt::MatchContains);
      completer_position->setModel(position_model);
      ui->comboBox_position->setCompleter(completer_position);

      // Отделение
      branch_model = new QSqlQueryComboModel( "branch", "branches", base, this );
      ui->comboBox_branch->setModel(branch_model);
      ui->comboBox_branch->setFocusPolicy(Qt::StrongFocus);
      ui->comboBox_branch->installEventFilter(new MouseWheelWidgetAdjustmentGuard(ui->comboBox_branch)); //блокируем прокрутку колесом мыши
      //     настраиваем комплитер
      QCompleter *completer_branch = new QCompleter(this);
      completer_branch->setCaseSensitivity(Qt::CaseInsensitive);
      completer_branch->setFilterMode(Qt::MatchContains);
      completer_branch->setModel(branch_model);
      ui->comboBox_branch->setCompleter(completer_branch);




      // список ведомостей
      model_payslip_list = new QSqlQueryModel(this);
      model_payslip_list->setQuery("SELECT id, month, employee_id, position_id, branch_id, number_of_bets, salary, clock_rate,hours_worked, timesheet_found, work_schedule_found, employment_contract_found, employment_history, diploma_found, certificate_found, note FROM payslip ORDER BY employee_id",base);
//      model_payslip_list->setQuery("SELECT id, month, employees.full_name, positions.position, branches.branch, number_of_bets, salary, clock_rate,hours_worked, timesheet_found, work_schedule_found, employment_contract_found, employment_history, diploma_found, certificate_found, note FROM payslip LEFT JOIN employees on payslip.employee_id=employees.id LEFT JOIN positions on payslip.position_id=positions.id LEFT JOIN branches on payslip.branch_id=branches.id ORDER BY employees.full_name",base);
      ui->tableView_payslip->setModel(model_payslip_list);
      ui->tableView_payslip->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
      ui->tableView_payslip->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
      ui->tableView_payslip->horizontalHeader()->setStretchLastSection(true);
      ui->tableView_payslip->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // по содержимому


      // список начислений
      model_charges_list = new QSqlQueryModel(this);
      model_charges_list->setQuery("SELECT id, type_of_charge_id, sum, order_id, payslip_id, verified_with_order, note FROM charges ORDER BY type_of_charge_id",base);

      ui->tableView_charges->setModel(model_charges_list);
      ui->tableView_charges->setSelectionBehavior(QAbstractItemView::SelectRows); // Разрешаем выделение строк
      ui->tableView_charges->setSelectionMode(QAbstractItemView::SingleSelection); // Устанавливаем режим выделения лишь одно строки в таблице
      ui->tableView_charges->horizontalHeader()->setStretchLastSection(true);
      ui->tableView_charges->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive); // по содержимому


      // настройка выпадающего меню начислений
      ui->tableView_charges->setContextMenuPolicy(Qt::CustomContextMenu);
      m_charges_menu = new QMenu(this);
      m_charges_popup_form = new ChargesEditForm(this);
//      m_charges_popup_form->set_ID(id); //настройка на ID
      QVBoxLayout *charges_layout = new QVBoxLayout;
      charges_layout->addWidget(m_charges_popup_form);
      m_charges_menu->setLayout(charges_layout);

      connect(ui->tableView_charges, &QTableView::customContextMenuRequested, [&](const QPoint &p)
      {
//          m_charges_menu->popup(mapToGlobal(p));
//          m_charges_menu->popup(mapTo(ui->tableView_charges, p));
//          m_charges_menu->popup(ui->tableView_charges->pos());
//            m_charges_menu->popup(mapToGlobal(p));
//          m_charges_menu->popup(p);
          m_charges_menu->popup(QCursor::pos());
          qDebug() << "открылось";
      });

      //создание обьектов таблиц


//    payslip = new QSqlRelationalTableModel(this,base);
//    model_payslip = new QSqlTableModel(this,base);
//    model_payslip_list = new QSqlQueryModel(this);
//    model_charges_list = new QSqlQueryModel(this);

//    delegate = new QSqlRelationalDelegate(this);
//    mapper = new QDataWidgetMapper(this);
//    completer = new QCompleter(this);


    //Настраиваем модели
    SetupTable();

    //modelQuestionnaire->setFilter("questionnaire.id='250'");

//    modelBank->select();
//    modelBank_decryption->select();
//    modelArticles->select();
//    modelCounterparties->select();

//    ui->comboBox_flt_counterparties->setCurrentIndex(-1); //убираем значения по умолчанию в комбобоксах после селекта
//    ui->comboBox_articles->setCurrentIndex(-1); //убираем значения по умолчанию в комбобоксах после селекта
//    ui->comboBox_contracts->setCurrentIndex(-1); //убираем значения по умолчанию в комбобоксах после селекта



    // сигнал изменения строки выделения в tableVew
//    connect(ui->tableView_payslip->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
//                 SLOT(slotSelectionChange(const QItemSelection &, const QItemSelection &)));

    connect(ui->tableView_payslip->selectionModel(),&QItemSelectionModel::currentChanged,
            this, &FormPayslip::onPayslipSelected);

    // сигнал создания запроса во вкладках
//    connect(this, SIGNAL(signalFromQuery(QString)),parent, SLOT(slot_goQuery(QString)));


    // коннектим сигналы изменений полей к слоту
//    connect(ui->comboBox_month, &QComboBox::currentTextChanged, this, &FormPayslip::onChenged);

    ui->tableView_payslip->selectRow(0);

//    getPayslip(1);

}

FormPayslip::~FormPayslip()
{
    delete ui;
}

void FormPayslip::slotSelectionChange(const QItemSelection &current, const QItemSelection &previous)
{
    // изменение строки в ведомостях

//    current.indexes()
   // getPayslip(model_payslip_list->data(model_payslip_list->index(ui->tableView_payslip->currentIndex().row(), 0)).toInt());

}

void FormPayslip::onPayslipSelected(const QModelIndex &index)
{

    setPayslip(); // сохранить старые данные

    //получить ID ведомости
    int id = model_payslip_list->data(model_payslip_list->index(index.row(), 0)).toInt();
    // заполнить форму данных ведомости
    getPayslip(id);
    // заполнить список начислений
    getCharges(id);

//    qDebug() << model_payslip_list->data(model_payslip_list->index(index.row(), 0)).toInt();


}

//void FormPayslip::onChargesSelected(const QModelIndex &index)
//{
//    // выделение начисления в списке
//    // если были старые записи сохранить их
//    // определить ID
//    int id = model_charges_list->data(model_charges_list->index(index.row(), 0)).toInt();
//    // загрузить новые


//}

void FormPayslip::onChenged()
{
    // Установить флаг редактирования
    payslip_chenged=true;
    qDebug() << "payslip_chenged=true";

    // не вышло определять ручное редактирование - отказался от данного метода - хотя решение надо продумать

}

void FormPayslip::SetupTable()
{


}

void FormPayslip::Tune()
{

}

void FormPayslip::setPayslip()
{
    // если id не пустой
    if (!ui->lineEdit_id->text().isEmpty()) {

        // выборка для сравнения
        QSqlQuery query_in(base);
        QString req_in = QString("SELECT id, month, employee_id, position_id, branch_id, number_of_bets, salary, clock_rate,hours_worked, timesheet_found, work_schedule_found, employment_contract_found, employment_history, diploma_found, certificate_found, note FROM payslip WHERE id= %1").arg(ui->lineEdit_id->text());
        if(!query_in.exec(req_in)) {
            qDebug() << req_in;
            qDebug() << "ERROR get: " << query_in.lastError().text();
            return;
        }
        if(query_in.first()) {  // если найдено

                // формируем строку запроса
                QString squery = "";
                // если поменялось
                if(query_in.value(1)!=ui->comboBox_month->currentIndex()) squery.append(QString("month = '%1',").arg(ui->comboBox_month->currentIndex()));
                if(query_in.value(2)!=ui->comboBox_employee->currentData().toString()) squery.append(QString("employee_id = '%1',").arg(ui->comboBox_employee->currentData().toString()));
                if(query_in.value(3)!=ui->comboBox_position->currentData().toString()) squery.append(QString("position_id = '%1',").arg(ui->comboBox_position->currentData().toString()));
                if(query_in.value(4)!=ui->comboBox_branch->currentData().toString()) squery.append(QString("branch_id = '%1',").arg(ui->comboBox_branch->currentData().toString()));

                if(query_in.value(5)!=ui->doubleSpinBox_number_of_bets->value()) squery.append(QString("number_of_bets = '%1',").arg(ui->doubleSpinBox_number_of_bets->value()));
                if(query_in.value(6)!=ui->doubleSpinBox_salary->value()) squery.append(QString("salary = '%1',").arg(ui->doubleSpinBox_salary->value()));
                if(query_in.value(7)!=ui->doubleSpinBox_clock_rate->value()) squery.append(QString("clock_rate = '%1',").arg(ui->doubleSpinBox_clock_rate->value()));
                if(query_in.value(8)!=ui->doubleSpinBox_hours_worked->value()) squery.append(QString("hours_worked = '%1',").arg(ui->doubleSpinBox_hours_worked->value()));

                if(query_in.value(9)!=ui->checkBox_timesheet_found->isChecked()) squery.append(QString("timesheet_found = '%1',").arg(ui->checkBox_timesheet_found->isChecked()));
                if(query_in.value(10)!=ui->checkBox_work_schedule_found->isChecked()) squery.append(QString("work_schedule_found = '%1',").arg(ui->checkBox_work_schedule_found->isChecked()));
                if(query_in.value(11)!=ui->checkBox_employment_contract_found->isChecked()) squery.append(QString("employment_contract_found = '%1',").arg(ui->checkBox_employment_contract_found->isChecked()));
                if(query_in.value(12)!=ui->checkBox_employment_history->isChecked()) squery.append(QString("employment_history = '%1',").arg(ui->checkBox_employment_history->isChecked()));
                if(query_in.value(13)!=ui->checkBox_diploma_found->isChecked()) squery.append(QString("diploma_found = '%1',").arg(ui->checkBox_diploma_found->isChecked()));
                if(query_in.value(14)!=ui->checkBox_certificate_found->isChecked()) squery.append(QString("certificate_found = '%1',").arg(ui->checkBox_certificate_found->isChecked()));

                if(query_in.value(15)!=ui->plainTextEdit_note->toPlainText()) squery.append(QString("note = '%1', ").arg(ui->plainTextEdit_note->toPlainText()));

                // если есть изменения
                if (!squery.isEmpty()) {
                    squery.chop(1);
                    squery = "UPDATE payslip SET " + squery;

                    squery.append(QString(" WHERE id = %1 ").arg(ui->lineEdit_id->text()));


                    qDebug() << squery;

                    QSqlQuery query(base);

                    if(!query.exec(squery)) {
                        qDebug() << "ERROR Query update payslip " << query.lastError().text();
                        qDebug() << squery;
                    }
                    payslip_chenged=false;
                    // обновить список
                    int row = ui->tableView_payslip->currentIndex().row();
//                    qDebug() << model_payslip_list->query().exec();

                    // перезапрашиваем
                    model_payslip_list->setQuery(model_payslip_list->query().lastQuery(),base);
//                    model_payslip_list->setQuery(model_payslip_list->query());
                    // догружаем полностью
                    while (model_payslip_list->canFetchMore())
                         model_payslip_list->fetchMore();

                    ui->tableView_payslip->selectRow(row);
                }
        }
        else
            qDebug() << "Не найден id для изменений!";


    }

}

void FormPayslip::getPayslip(int id)
{
    QSqlQuery query(base);

    QString req = QString("SELECT id, month, employee_id, position_id, branch_id, number_of_bets, salary, clock_rate,hours_worked, timesheet_found, work_schedule_found, employment_contract_found, employment_history, diploma_found, certificate_found, note FROM payslip WHERE id= %1").arg(id);

    if(!query.exec(req)) {
        qDebug() << req;
        qDebug() << "ERROR get: " << query.lastError().text();
        return;
    }

    if(query.first()) {

        ui->lineEdit_id->setText(query.value(0).toString());
        ui->comboBox_month->setCurrentIndex(query.value(1).toInt());
        ui->comboBox_employee->setCurrentIndex(ui->comboBox_employee->findData(query.value(2).toString()));
        ui->comboBox_position->setCurrentIndex(ui->comboBox_position->findData(query.value(3).toString()));
        ui->comboBox_branch->setCurrentIndex(ui->comboBox_branch->findData(query.value(4).toString()));

        ui->doubleSpinBox_number_of_bets->setValue(query.value(5).toDouble());
        ui->doubleSpinBox_salary->setValue(query.value(6).toDouble());
        ui->doubleSpinBox_clock_rate->setValue(query.value(7).toDouble());
        ui->doubleSpinBox_hours_worked->setValue(query.value(8).toDouble());

        ui->checkBox_timesheet_found->setChecked(query.value(9).toBool());
        ui->checkBox_work_schedule_found->setChecked(query.value(10).toBool());
        ui->checkBox_employment_contract_found->setChecked(query.value(11).toBool());
        ui->checkBox_employment_history->setChecked(query.value(12).toBool());
        ui->checkBox_diploma_found->setChecked(query.value(13).toBool());
        ui->checkBox_certificate_found->setChecked(query.value(14).toBool());

        ui->plainTextEdit_note->setPlainText(query.value(15).toString());


        // расчетный оклад
        //ui->doubleSpinBox_calc_salarysetValue(1000);
    }



}

void FormPayslip::getCharges(int id)
{
    //заполнение списка текущих начислений
    //QSqlQuery query(base);

    QString req = QString("SELECT id, type_of_charge_id, sum, order_id, verified_with_order, note FROM charges WHERE payslip_id= %1 ORDER BY type_of_charge_id").arg(id);
//    QString req = QString("SELECT id, type_of_charge_id, sum, order_id, verified_with_order, note FROM charges ORDER BY type_of_charge_id");
    qDebug() << "list: " << req;
    model_charges_list->setQuery(req,base);
    model_charges_list->query();


}

void FormPayslip::on_pushButton_close_clicked()
{
    close();
}

void FormPayslip::on_pushButton_first_clicked()
{
    // прыгаем на первую
    ui->tableView_payslip->selectRow(0);

}

void FormPayslip::on_pushButton_prev_clicked()
{
    // прыгаем на предыдущую запись
    ui->tableView_payslip->selectRow(ui->tableView_payslip->currentIndex().row()-1);
}

void FormPayslip::on_pushButton_next_clicked()
{
    // прыгаем на следующую запись
    ui->tableView_payslip->selectRow(ui->tableView_payslip->currentIndex().row()+1);

}

void FormPayslip::on_pushButton_last_clicked()
{
    // последняя запись
    ui->tableView_payslip->selectRow(model_payslip_list->rowCount()-1);

}

void FormPayslip::on_pushButton_refr_clicked()
{

    //обновляем список ведомостей
    model_payslip_list->setQuery(model_payslip_list->query().lastQuery(),base);

    //обновляем комбобоксы
    employee_model->setQuery(employee_model->query().lastQuery(),base);;
    position_model->setQuery(position_model->query().lastQuery(),base);;
    branch_model->setQuery(branch_model->query().lastQuery(),base);;

    //докачиваем все
    while (model_payslip_list->canFetchMore())
         model_payslip_list->fetchMore();

    //прыгаем на старую запись
    ui->tableView_payslip->selectRow(model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, ui->lineEdit_id->text(), 1).first().row());

}

void FormPayslip::on_pushButton_add_clicked()
{


    // добавляем запись
    QSqlQuery query(base);
    QString req = QString("INSERT INTO payslip (month) VALUES ('0') ");
    if(!query.exec(req)) {
        qDebug() << req;
        qDebug() << "ERROR add: " << query.lastError().text();
        return;
    }
    if(!query.exec("SELECT LAST_INSERT_ROWID();")) {
        qDebug() << req;
        qDebug() << "ERROR get id: " << query.lastError().text();
        return;
    }
    query.first();
    int id=query.value(0).toInt();
    qDebug() << id;


//    setPayslip(); // сохранить старые данные
//    getPayslip(id);    // взять новые

    // найти как переключиться на добавленную строку !!!!!!!!!!!!!!!!!!!!!!!!!!!
    //просто перейти на последнюю не всегда правильно
    // при большой базе модель вернет номер не последней записи, а последнего загруженного значения (хотя для модели запроса возможно это и не так)
    model_payslip_list->setQuery(model_payslip_list->query().lastQuery(),base);
    // догружаем полностью
    while (model_payslip_list->canFetchMore())
         model_payslip_list->fetchMore();

//    model_payslip_list->setQuery(model_payslip_list->query());
//    ui->tableView_payslip->
    //ui->tableView_payslip->repaint();
//    QModelIndexList mdl = model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, id, 1);

//    qDebug() <<   model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, id, 1);
   qDebug() <<   model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, id, 1);

   qDebug() <<   model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, id, 1).first().row();
    ui->tableView_payslip->selectRow(model_payslip_list->match(model_payslip_list->index(0, 0), Qt::DisplayRole, id, 1).first().row());
//    ui->tableView_payslip->selectRow(model_payslip_list->rowCount()-1);

    // устанавливаем курсор на редактирование
    ui->comboBox_month->setFocus();
//    QCoreApplication::postEvent(this, new QStatusTipEvent(QString("Добавление...")));
}

void FormPayslip::on_pushButton_del_clicked()
{

}

void FormPayslip::on_pushButton_lst_clicked()
{

}





void FormPayslip::on_lineEdit_flt_all_textChanged(const QString &arg1)
{

}
