#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include <QDebug>
#include <QSettings>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QtPrintSupport/QPrinter>
#include <QPrintDialog>
#include <QTextStream>
#include <QTextDocument>

#include <QPdfWriter>
#include <QPainter>
#include <QDesktopServices>
#include <QCommandLineParser>
#include <QSettings>

#include "formquery.h"
#include "formpayslip.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    database = QSqlDatabase::addDatabase("QSQLITE","main");

    //сбрасываем тригеры меню
    on_actionCloseBase_triggered();

    // если есть параметры в командной строке то открываем
    if (QCoreApplication::arguments().count() > 1) {
        databaseName=QCoreApplication::arguments().at(1);
        on_actionOpenBase_triggered();
    }

    SetHistory();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpenPayslip_triggered()
{
    FormPayslip  *pay = new FormPayslip(database,this);
    ui->tabWidgetMain->insertTab(0,pay,tr("Ведомость"));
    ui->tabWidgetMain->setCurrentIndex(0);
}

void MainWindow::on_actionAbout_triggered()
{
    //об авторах
    QMessageBox::information(this,"Info","Программа ведения финансового аудита ЗП.\n\nGorINIch`2020 ver0.01\nggorinich@gmail.com");

}

void MainWindow::slot_goQuery(QString sq)
{
    // вызов окна запроса
    // придумать как вызвать из других окон и передать параметры!
    FormQuery *query = new FormQuery(database, sq, this);
    ui->tabWidgetMain->insertTab(0,query,tr("Запрос"));
    ui->tabWidgetMain->setCurrentIndex(0);

}

void MainWindow::slot_OpenBase(QString fb)
{
    qDebug() << "Open history";

    databaseName = fb;
    on_actionOpenBase_triggered();
}

bool MainWindow::OpenBase()
{

    //проверяем на наличие файл базы
    if(!QFile(databaseName).exists()){
        qDebug() << "Файла базы нет!";
    }

    // открываем базу
    database.setDatabaseName(databaseName);
    if(!database.open()){
      qDebug() << "Ошибка открытия базы!";
      this->setWindowTitle("Error!");
      return false;
    }

    //читаем настнойки из базы
    QSqlQuery query(database);
    if (!query.exec("SELECT organization, date_begin, date_end FROM options"))
            qDebug() << "Ошибка чтения настроек: " << query.lastError().text();
     query.first();

    // титульный окна имя базы
    this->setWindowTitle("Аудит ЗП: '" + query.value(0).toString() + "' База: "+ databaseName);

     // добавляем имя базы в историю
     QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
 //     settings.setValue(SETTINGS_BASE_FILE1, "");
     QString file1=settings.value(SETTINGS_BASE_FILE1, "").toString();
     QString file2=settings.value(SETTINGS_BASE_FILE2, "").toString();
     QString file3=settings.value(SETTINGS_BASE_FILE3, "").toString();

     if(file1!=databaseName) {
         // сохраняем
         settings.setValue(SETTINGS_BASE_FILE3, file2);
         settings.setValue(SETTINGS_BASE_FILE2, file1);
         settings.setValue(SETTINGS_BASE_FILE1, databaseName);
         // сбвигаем меню
         ui->actionFile01->setText(databaseName);
         ui->actionFile02->setText(file1);
         ui->actionFile03->setText(file2);
     }


     return true;

}

void MainWindow::SetHistory()
{
    // чтот динамически не вышло -  пока сделаю по простому
    // gпрочитать и добавить историю открытий баз

    // считать из настроек имя базы

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, ORGANIZATION_NAME, APPLICATION_NAME);
    QString file1=settings.value(SETTINGS_BASE_FILE1, "").toString();
    QString file2=settings.value(SETTINGS_BASE_FILE2, "").toString();
    QString file3=settings.value(SETTINGS_BASE_FILE3, "").toString();


    if(!file1.isEmpty())
        ui->actionFile01->setText(file1);
    if(!file2.isEmpty())
        ui->actionFile02->setText(file2);
    if(!file3.isEmpty())
        ui->actionFile03->setText(file3);
}


void MainWindow::on_actionCloseBase_triggered()
{
    // закрыть все вкладки
    ui->tabWidgetMain->clear();

    //закрыть базу
    databaseName="";
    database.close();
    this->setWindowTitle("Аудит: no base");
    ui->actionCloseBase->setEnabled(false);
    ui->actionOpenBase->setEnabled(true);
//    ui->actionSaveAs->setEnabled(false);

//    ui->actionBank->setEnabled(false);
//    ui->actionArticles->setEnabled(false);
//    ui->actionCounterparties->setEnabled(false);
//    ui->actionContracts->setEnabled(false);
//    ui->actionOptions->setEnabled(false);
//    ui->actionImportPP->setEnabled(false);
//    ui->actionRep_bank->setEnabled(false);
//    ui->actionRepContracts->setEnabled(false);
//    ui->actionRepContracsShort->setEnabled(false);
//    ui->actionRepContractsIsNote->setEnabled(false);
}

void MainWindow::on_actionOpenBase_triggered()
{
    // отктыьб базу данных
    // выбор файла базы данных
    if (databaseName.isEmpty())
     databaseName = QFileDialog::getOpenFileName(this,tr("Open base"),"./",QString("Data base Files (*%1);; All file (*)").arg(FILE_EXT));

    // если файл присутствует
    //проверяем на наличие файл базы
    if(!QFile(databaseName).exists()){
        qDebug() << "Файла базы нет!";
        QMessageBox::information(this,"Error","Выбранная база не существует!");
        return;
    }

     if (!databaseName.isEmpty()) {
        // закрывкем старую
            database.close();
            // сохранить в настройказ имя базы
    //        QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
            //пишем настройки
    //        settings.setValue(SETTINGS_BASE_NAME, filename);
    //        settings.sync();
            // открыть меню
            ui->actionOpenBase->setEnabled(false);
            ui->actionCloseBase->setEnabled(true);

//            ui->actionSaveAs->setEnabled(true);

//            ui->actionBank->setEnabled(true);
//            ui->actionArticles->setEnabled(true);
//            ui->actionCounterparties->setEnabled(true);
//            ui->actionContracts->setEnabled(true);
//            ui->actionOptions->setEnabled(true);
//            ui->actionImportPP->setEnabled(true);
//            ui->actionRep_bank->setEnabled(true);
//            ui->actionRepContracts->setEnabled(true);
//            ui->actionRepContracsShort->setEnabled(true);
//            ui->actionRepContractsIsNote->setEnabled(true);

            // открываем новую базу
            OpenBase();
     }
}

void MainWindow::on_actionSaveAs_triggered()
{
    // дублирование базы
    if (!databaseName.isEmpty()) {
        // запросить новое имя
        QString  newBase = QFileDialog::getSaveFileName(this,tr("Сохранить как"),databaseName,QString("Data base Files (*%1);; All (*)").arg(FILE_EXT));
        // если дано новое имя
        if (databaseName != newBase) {
            database.close();
            qDebug() << "copy " << databaseName << " to " << newBase;
            //если нет расширения добавляем
            if (newBase.indexOf(FILE_EXT)==-1)
                newBase.append(FILE_EXT);

            // возможно надо проверить не открыта ли база кем то еще
            if (!QFile::copy(databaseName,newBase)) {
                    QMessageBox::critical(this,"ERROR","База не скопирована!");
                    return;
            }
            databaseName = newBase;
        }
        on_actionOpenBase_triggered();
    }
}

void MainWindow::on_actionExit_triggered()
{
     QApplication::closeAllWindows();
}

void MainWindow::on_actionNewBase_triggered()
{
    // выбор файла базы данных
    QString newBase =  QFileDialog::getSaveFileName(this,tr("Create new base"),"./",QString("Data base Fules (*%1);;All (*)").arg(FILE_EXT));

     if (!newBase.isEmpty()) {
        // создаем

         //если нет расширения добавляем
         if (newBase.indexOf(FILE_EXT)==-1)
             newBase.append(FILE_EXT);

         //проверяем на наличие файл базы
         if(QFile(newBase).exists()){
             qDebug() << "Файла базы есть!";
             QMessageBox::information(this,"Error","Выбранная база уже существует. Выберете другое имя!");
             return;
         }
//-----------------------------------------------------------------------------------------------------------------------------
   return; // заглушка
//-----------------------------------------------------------------------------------------------------------------------------

     // открываем базу
         QSqlDatabase dbm = QSqlDatabase::addDatabase("QSQLITE","new");
         dbm.setDatabaseName(newBase);
         if(!dbm.open()){
           qDebug() << "Ошибка открытия базы!";
           QMessageBox::critical(this,"Error",dbm.lastError().text());
           return;
         }

         QSqlQuery a_query = QSqlQuery(dbm);

         // запрос на создание таблицы ПП
         QString str = "CREATE TABLE bank ("
             "id                    INTEGER         PRIMARY KEY AUTOINCREMENT"
                                                   " UNIQUE,"
             "payment_number        VARCHAR,"
             "payment_date          DATE            DEFAULT [2000-01-01],"
             "counterparty_id       INTEGER         REFERENCES counterparties (id),"
             "decryption_of_payment TEXT,"
             "amount_of_payment     DECIMAL (20, 2) DEFAULT (0),"
             "this_receipt          BOOLEAN         DEFAULT (false),"
             "article               VARCHAR,"
             "note                  VARCHAR"
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица ПП: " << a_query.lastError().text();


         // запрос на создание таблицы Расшифровок
         str = "CREATE TABLE bank_decryption ("
             "id                   INTEGER         PRIMARY KEY AUTOINCREMENT"
                                                  " UNIQUE,"
             "bank_id              INTEGER         REFERENCES bank (id),"
             "sum                  DECIMAL (20, 2),"
             "article_id           INTEGER         REFERENCES articles (id),"
             "contract_id          INTEGER         REFERENCES contracts (id),"
             "expense_confirmation BOOLEAN         DEFAULT (false) "
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица Расшифровок: " << a_query.lastError().text();

         // запрос на создание таблицы Статей
         str = "CREATE TABLE articles ("
             "id      INTEGER PRIMARY KEY AUTOINCREMENT"
                             " UNIQUE,"
             "article VARCHAR,"
             "code    VARCHAR,"
             "subcode VARCHAR"
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица Статей: " << a_query.lastError().text();

         // запрос на создание таблицы Контрагентов
         str = "CREATE TABLE counterparties ("
             "id           INTEGER PRIMARY KEY AUTOINCREMENT"
                                  " UNIQUE,"
             "counterparty TEXT,"
             "note         VARCHAR"
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица Контрагентов: " << a_query.lastError().text();

         // запрос на создание таблицы Контрактов
         str = "CREATE TABLE contracts ("
             "id              INTEGER         PRIMARY KEY AUTOINCREMENT"
                                             " UNIQUE,"
             "contract_number VARCHAR,"
             "contract_date   DATE            DEFAULT [2000-01-01],"
             "due_date        DATE            DEFAULT [2000-01-01],"
             "counterparty_id INTEGER         REFERENCES counterparties (id),"
             "price           DECIMAL (20, 2) DEFAULT (0),"
             "state_contract  BOOLEAN         DEFAULT (false),"
             "completed       BOOLEAN         DEFAULT (false),"
             "found           BOOLEAN         DEFAULT (false),"
             "note            VARCHAR"
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица Контрактов: " << a_query.lastError().text();

         // запрос на создание таблицы Настроек
         str = "CREATE TABLE options ("
             "organization       VARCHAR,"
             "date_begin         DATE,"
             "date_end           DATE,"
             "rep_contract_found BOOLEAN DEFAULT (false) "
         ");";
         if (!a_query.exec(str))
             qDebug() << "таблица Настроек: " << a_query.lastError().text();
         // вставить строку
         str = "INSERT INTO options (organization) values ('');";
         if (!a_query.exec(str))
             qDebug() << "таблица настроек строка: " << a_query.lastError().text();


         //
         QMessageBox::information(this,"Info","Операция завершена.");
         dbm.close();

         // откроем созданную базу
        databaseName=newBase;
        on_actionOpenBase_triggered();
     }
}

void MainWindow::on_actionFile01_triggered()
{
    //файл истории 01
    if(!ui->actionFile01->text().isEmpty()) {
        databaseName = ui->actionFile01->text();
        on_actionOpenBase_triggered();
    }
}

void MainWindow::on_actionFile02_triggered()
{
    //файл истории 02
    if(!ui->actionFile02->text().isEmpty()) {
        databaseName = ui->actionFile02->text();
        on_actionOpenBase_triggered();
    }
}

void MainWindow::on_actionFile03_triggered()
{
    //файл истории 03
    if(!ui->actionFile03->text().isEmpty()) {
        databaseName = ui->actionFile03->text();
        on_actionOpenBase_triggered();
    }
}


