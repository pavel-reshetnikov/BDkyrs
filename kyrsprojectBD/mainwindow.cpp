#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSqlError"
#include "QSqlRecord"
#include "QString"
#include "QDate"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("kyrsproject");
    db.setUserName("postgres");
    db.setPort(5432);
    db.setPassword("12345");
    if (!db.open()){
        qDebug()<<db.lastError().text();
    }
    else{
        qDebug()<<"Succefull";
    }
    model = new QSqlTableModel(this,db);
    model->setTable("competitions");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
    model->setHeaderData(1, Qt::Horizontal, "Название соревнования");
    model->setHeaderData(2, Qt::Horizontal, "Дата");
    model->setHeaderData(3, Qt::Horizontal, "Место");
    ui->tableView->setModel(model);
    qmodel = new QSqlQueryModel;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked() // поиск
{

    QString str = ui->lineEdit->text();
    QDate d = QDate::fromString(str, "YYYY-MM-dd");
    qDebug()<<bool(str.toInt()); // проверка состоит ли строка только из цифр
    QString sq;
    switch(ui->comboBox->currentIndex()){
    case 0:
        if( bool(str.toInt()) ){//если цифра

             sq = "SELECT * FROM search_c('%" + str +"%'," + str + ", NULL)";

                 }
        else {
             sq = "SELECT * FROM search_c('%" + str +"%',NULL , NULL)";
        }
                qmodel->setQuery(sq);
                qmodel->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
                qmodel->setHeaderData(1, Qt::Horizontal, "Название соревнования");
                qmodel->setHeaderData(2, Qt::Horizontal, "Дата");
                qmodel->setHeaderData(3, Qt::Horizontal, "Место");
                ui->tableView->setModel(qmodel);
            break;


    case 1:
    if( bool(str.toInt()) ){

         sq = "SELECT * FROM search_s('%" + str +"%'," + str + ")";

             }
    else{
         sq = "SELECT * FROM search_s('%" + str +"%', NULL)";
    }
            qmodel->setQuery(sq);
            qmodel->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
            qmodel->setHeaderData(1, Qt::Horizontal, "ID соревнования");
            qmodel->setHeaderData(2, Qt::Horizontal, "Имя");
            qmodel->setHeaderData(3, Qt::Horizontal, "Фамилия");
            qmodel->setHeaderData(4, Qt::Horizontal, "Возраст");
            ui->tableView->setModel(qmodel);
        break;
    case 2:
        if( bool(str.toInt()) ){

             sq = "SELECT * FROM search_r('%" + str +"%'," + str + ")";

                 }
        else{
             sq = "SELECT * FROM search_r('%" + str +"%', NULL)";
        }
                qmodel->setQuery(sq);
                qmodel->setHeaderData(0, Qt::Horizontal, "ID");
                qmodel->setHeaderData(1, Qt::Horizontal, "ID спортсмена");
                qmodel->setHeaderData(2, Qt::Horizontal, "Норматив");
                qmodel->setHeaderData(3, Qt::Horizontal, "Балл");
                ui->tableView->setModel(qmodel);
            break;
}


}

void MainWindow::on_pushButton_2_clicked()
{

   model->removeRow(row);
   model->select();
   ui->tableView->setModel(model);

}


void MainWindow::on_comboBox_activated(int index)
{
    //qDebug()<<ui->comboBox->currentIndex();
    switch(ui->comboBox->currentIndex()){
    case 0:
        model->setTable("competitions");
            model->select();
            model->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
            model->setHeaderData(1, Qt::Horizontal, "Название соревнования");
            model->setHeaderData(2, Qt::Horizontal, "Дата");
            model->setHeaderData(3, Qt::Horizontal, "Место");
            ui->tableView->setModel(model);
            tf=0;

        break;
    case 1:
        model->setTable("sportsman");
            model->select();
            model->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
            model->setHeaderData(1, Qt::Horizontal, "ID соревнования");
            model->setHeaderData(2, Qt::Horizontal, "Имя");
            model->setHeaderData(3, Qt::Horizontal, "Фамилия");
            model->setHeaderData(4, Qt::Horizontal, "Возраст");
            ui->tableView->setModel(model);
            tf=1;

        break;     
    case 2:
        model->setTable("result");
            model->select();
            model->setHeaderData(0, Qt::Horizontal, "ID");
            model->setHeaderData(1, Qt::Horizontal, "ID спортсмена");
            model->setHeaderData(2, Qt::Horizontal, "Норматив");
            model->setHeaderData(3, Qt::Horizontal, "Балл");
            ui->tableView->setModel(model);
            tf=2;

        break;
    default:
        qDebug()<<"Error combobox";
        break;
    }
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}

//qmodel = new QSqlQueryModel;
void MainWindow::on_pushButton_3_clicked()
{

switch(tf){
case 0:
{
    QModelIndex index = model->index(row,0);
    QString sqq = index.data().toString();
    qDebug()<<"ind1="<<index.data().toString();
    QString sq = "SELECT * FROM spor_f(" + sqq +")";
    qmodel->setQuery(sq);

    qmodel->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
    qmodel->setHeaderData(1, Qt::Horizontal, "Имя");
    qmodel->setHeaderData(2, Qt::Horizontal, "Фамилия");
    qmodel->setHeaderData(3, Qt::Horizontal, "Возраст");

    ui->tableView->setModel(qmodel);
    tf++;
    break;
}
case 1:
{  
    qmodel2 = new QSqlQueryModel;
    qmodel = model;
    QModelIndex index = qmodel->index(row,0);
    QString sqq = index.data().toString();
    qDebug()<<"ind2="<<index.data().toString();
    QString sq = "SELECT * FROM result_f(" + sqq +")";
    qmodel2->setQuery(sq);

    qmodel2->setHeaderData(0, Qt::Horizontal, "Норматив");
    qmodel2->setHeaderData(1, Qt::Horizontal, "Балл");
    ui->tableView->setModel(qmodel2);
    tf++;
    break;
}
default:
    break;
}
}

void MainWindow::on_action_triggered()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_action_2_triggered()
{
    model->removeRow(row);
    model->select();
    ui->tableView->setModel(model);
}
