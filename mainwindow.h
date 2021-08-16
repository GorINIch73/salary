#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

#define ORGANIZATION_NAME "gorinich-co"
#define ORGANIZATION_DOMAIN ""
#define APPLICATION_NAME "salary"
#define SETTINGS_BASE_FILE1 "File1"
#define SETTINGS_BASE_FILE2 "File2"
#define SETTINGS_BASE_FILE3 "File3"

#define FILE_EXT ".dbsl"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void slot_goQuery(QString sq); // запуск запроса
    void slot_OpenBase(QString fb); // открвть базу по имени


    void on_actionCloseBase_triggered();

    void on_actionOpenBase_triggered();

    void on_actionSaveAs_triggered();

    void on_actionExit_triggered();

    void on_actionNewBase_triggered();

    void on_actionFile01_triggered();

    void on_actionFile02_triggered();

    void on_actionFile03_triggered();

    void on_actionOpenPayslip_triggered();

    void on_actionAbout_triggered();

    void on_actionEmployees_triggered();

private:
    Ui::MainWindow *ui;

    QString databaseName;
    QSqlDatabase database;

    bool OpenBase();
    void SetHistory();

};
#endif // MAINWINDOW_H
