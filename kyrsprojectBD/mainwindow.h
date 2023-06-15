#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QJsonObject>

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

    void on_pushButton_2_clicked();

    void on_comboBox_activated(int index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_action_triggered();

    void on_action_2_triggered();

    void on_lineEdit_textEdited(const QString &arg1);

    void readJson();

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;
    QSqlTableModel *model;
    QSqlQueryModel *qmodel;
    QSqlQueryModel *qmodel2;
    int row;
    int tf = 0;
    bool del = true;
    QString HostName = "";
    QString DatabaseName = "";
    QString UserName = "";
    QString Password = "0";
    int Port = 0;

};
#endif // MAINWINDOW_H
