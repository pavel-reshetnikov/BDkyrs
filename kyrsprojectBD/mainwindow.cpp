#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QSqlError"
#include "QSqlRecord"
#include "QString"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QFile>
#include <QDir>


void MainWindow::readJson() //Считывание .json файла
{
    QString val;
    QFile file;
    file.setFileName("bddan.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8(), &error);
    qDebug() << "Error: " << error.errorString() << error.offset << error.error;

    if (doc.isObject())
    {
        QJsonObject json = doc.object();
        QJsonValue parameters = json.value("parameters");

        if (parameters.isObject())
        {

            QJsonObject obj = parameters.toObject();
            HostName = obj["HostName"].toString();
            DatabaseName = obj["DatabaseName"].toString();
            Port = obj["Port"].toInt();
            Password = obj["Password"].toString();
            UserName = obj["UserName"].toString();

        }

    }

}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readJson();
    qDebug() << QSqlDatabase::drivers();
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(HostName);
    db.setDatabaseName(DatabaseName);
    db.setUserName(UserName);
    db.setPort(Port);
    db.setPassword(Password);
    if (!db.open()){
        qDebug()<<db.lastError().text();
        qDebug()<< Password;
        qDebug()<< Port;
        qDebug()<< DatabaseName;
        qDebug()<<HostName;
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
    ui->lineEdit->setPlaceholderText(QString("Поиск..."));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{

   model->removeRow(row);
   model->select();
   ui->tableView->setModel(model);

}


void MainWindow::on_comboBox_activated(int index)
{
   // qDebug()<<ui->comboBox->currentIndex();
    switch(ui->comboBox->currentIndex()){
    case 0:
        model->setTable("competitions");
            model->select();
            model->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
            model->setHeaderData(1, Qt::Horizontal, "Название соревнования");
            model->setHeaderData(2, Qt::Horizontal, "Дата");
            model->setHeaderData(3, Qt::Horizontal, "Место");
            ui->tableView->setModel(model);
            ui->lineEdit->setEnabled(true);
            ui->lineEdit->setPlaceholderText(QString("Поиск..."));
            del = true;

        break;
    case 1:
        model->setTable("sportsman");
            model->select();
            model->setHeaderData(0, Qt::Horizontal, "ID"); // Изменяет название столбца
            model->setHeaderData(1, Qt::Horizontal, "Имя");
            model->setHeaderData(2, Qt::Horizontal, "Фамилия");
            model->setHeaderData(3, Qt::Horizontal, "Дата рождения");
            ui->tableView->setModel(model);
            ui->lineEdit->setEnabled(true);
            ui->lineEdit->setPlaceholderText(QString("Поиск..."));
            del = true;

        break;     
    case 2: // SELECT * FROM qu_result()
        qmodel->setQuery("SELECT * FROM qu_result()");
        qmodel->setHeaderData(0, Qt::Horizontal, "ID");
        qmodel->setHeaderData(1, Qt::Horizontal, "ID Спор./Сорев.");
        qmodel->setHeaderData(2, Qt::Horizontal, "Норматив");
        qmodel->setHeaderData(3, Qt::Horizontal, "Балл");
        qmodel->setHeaderData(4, Qt::Horizontal, "Имя");
        qmodel->setHeaderData(5, Qt::Horizontal, "Фамилия");
        qmodel->setHeaderData(6, Qt::Horizontal, "Название Соревнования");

        ui->tableView->setModel(qmodel);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText(QString("Поиск..."));
        del = true;
            break;
    case 3: //qu_sporcomp()
        qmodel->setQuery("SELECT * FROM qu_sporcomp()");
        qmodel->setHeaderData(0, Qt::Horizontal, "ID");
        qmodel->setHeaderData(1, Qt::Horizontal, "ID Спортсмена");
        qmodel->setHeaderData(2, Qt::Horizontal, "ID Соревнования");
        qmodel->setHeaderData(3, Qt::Horizontal, "Имя");
        qmodel->setHeaderData(4, Qt::Horizontal, "Фамилия");
        qmodel->setHeaderData(5, Qt::Horizontal, "Название Соревнования");

        ui->tableView->setModel(qmodel);
        ui->lineEdit->setEnabled(true);
        ui->lineEdit->setPlaceholderText(QString("Поиск..."));
del = true;
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


void MainWindow::on_action_triggered()
{
    switch(ui->comboBox->currentIndex()){
    case 2:
        model->setTable("result");
        model->select();
            ui->tableView->setModel(model);
    model->insertRow(model->rowCount());
    del = true;
        break;
    case 3:
        model->setTable("sporcomp");
        model->select();
            ui->tableView->setModel(model);
    model->insertRow(model->rowCount());
    del = true;
        break;
    default:
        model->insertRow(model->rowCount());
        del = true;
        break;

    }




   // model->insertRow(model->rowCount());
}


void MainWindow::on_action_2_triggered()
{
    switch(ui->comboBox->currentIndex()){

    case 2:
        model->setTable("result");
        model->setHeaderData(0, Qt::Horizontal, "ID");
        model->setHeaderData(1, Qt::Horizontal, "ID Спор./сор.");
        model->setHeaderData(2, Qt::Horizontal, "Норматив");
        model->setHeaderData(3, Qt::Horizontal, "Балл");
        model->select();
            ui->tableView->setModel(model);
            if(del){
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setPlaceholderText(QString("Выберете строку и нажмите 'Удалить' ещё раз."));
    del = false;
            }
            else{
                del = true;
                model->removeRow(row);
                model->setTable("result");
                model->setHeaderData(0, Qt::Horizontal, "ID");
                model->setHeaderData(1, Qt::Horizontal, "ID Спор./сор.");
                model->setHeaderData(2, Qt::Horizontal, "Норматив");
                model->setHeaderData(3, Qt::Horizontal, "Балл");
                model->select();
                ui->lineEdit->setEnabled(true);
                ui->lineEdit->setPlaceholderText(QString("Поиск..."));
            }


        break;

    case 3:
        model->setTable("sporcomp");
        model->select();
            ui->tableView->setModel(model);
            if(del){
    ui->lineEdit->setEnabled(false);
    ui->lineEdit->setPlaceholderText(QString("Выберете строку и нажмите 'Удалить' ещё раз."));
    del = false;
            }
            else{
                del = true;
                model->removeRow(row);
                model->setTable("sporcomp");
                model->select();
                ui->lineEdit->setEnabled(true);
                ui->lineEdit->setPlaceholderText(QString("Поиск..."));
            }

        break;

    default:
         model->removeRow(row);
         model->select();
         ui->tableView->setModel(model);

        break;

    }



   // model->removeRow(row);
    //model->select();
   // ui->tableView->setModel(model);
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    QString str = ui->lineEdit->text();
   // qDebug()<<bool(str.toInt()); // проверка состоит ли строка только из цифр
    QString sq;
    del = true;
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
                del = true;
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
            qmodel->setHeaderData(1, Qt::Horizontal, "Имя");
            qmodel->setHeaderData(2, Qt::Horizontal, "Фамилия");
            qmodel->setHeaderData(3, Qt::Horizontal, "Дата рождения");
            ui->tableView->setModel(qmodel);
            del = true;
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
                qmodel->setHeaderData(1, Qt::Horizontal, "ID Спо/сор");
                qmodel->setHeaderData(2, Qt::Horizontal, "Норматив");
                qmodel->setHeaderData(3, Qt::Horizontal, "Балл");
                ui->tableView->setModel(qmodel);
                del = true;
            break;
    case 3: //SELECT * FROM search_sporcomp('Михаил',NULL)
        if( bool(str.toInt()) ){

             sq = "SELECT * FROM search_sporcomp('%" + str +"%'," + str + ")";

                 }
        else{
             sq = "SELECT * FROM search_sporcomp('%" + str +"%', NULL)";
        }
                qmodel->setQuery(sq);
                qmodel->setHeaderData(0, Qt::Horizontal, "ID");
                qmodel->setHeaderData(1, Qt::Horizontal, "ID Спортсмена");
                qmodel->setHeaderData(2, Qt::Horizontal, "ID Соревнования");
                qmodel->setHeaderData(3, Qt::Horizontal, "Имя");
                qmodel->setHeaderData(4, Qt::Horizontal, "Фамилия");
                qmodel->setHeaderData(5, Qt::Horizontal, "Название Соревнования");
                ui->tableView->setModel(qmodel);
                del = true;
        break;

}
}

