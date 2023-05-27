#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "userdata.h"
#include "login.h"
#include <QFileDialog>
#include <QHBoxLayout>
#include <QDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QTableWidgetItem>
#include<iostream>
#include <QUrlQuery>




QString flower ="Rose";


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->error_label->hide();
    setWindowTitle("Flower Stock Exchange");
    QIcon icon(":/res/logo.png");
    setWindowIcon(icon);

   ui->price_edit->setPlaceholderText("Price");
   ui->client_id->setPlaceholderText("Client Id");

    networkManager = new QNetworkAccessManager(this);


    connect(ui->actionLog_Out, &QAction::triggered, this, &MainWindow::onActionLog_OutClicked);

    ui->tableWidget->setColumnCount(9);
    ui->table_prev->setColumnCount(9);// Set the column count
    QStringList headerLabels = { "Order ID", "Client ID", "Instrument", "Side", "Status", "Quantity", "Price", "Transaction Time","Reason" };
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->table_prev->setHorizontalHeaderLabels(headerLabels);

    ui->tableWidget->horizontalHeader()->resizeSection(0, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(1, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(4, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(5, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(6, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(7, 200);
    ui->tableWidget->horizontalHeader()->resizeSection(8, 100);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    ui->table_prev->horizontalHeader()->resizeSection(0, 80);
    ui->table_prev->horizontalHeader()->resizeSection(1, 100);
    ui->table_prev->horizontalHeader()->resizeSection(2, 90);
    ui->table_prev->horizontalHeader()->resizeSection(3, 80);
    ui->table_prev->horizontalHeader()->resizeSection(4, 70);
    ui->table_prev->horizontalHeader()->resizeSection(5, 70);
    ui->table_prev->horizontalHeader()->resizeSection(6, 100);
    ui->table_prev->horizontalHeader()->resizeSection(7, 200);
    ui->table_prev->horizontalHeader()->resizeSection(8, 100);
    ui->table_prev->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    qDebug()<<userId()<<token();





}

MainWindow::~MainWindow()
{
    delete ui;
}
QString OpenfilePath ;

void MainWindow::on_pushButton_clicked()
{
    OpenfilePath = QFileDialog::getOpenFileName(this, "Open Csv File", "", "CSV Files (*.csv)");
    if (!OpenfilePath.isEmpty()) {
        ui->plainTextEdit->setPlainText(OpenfilePath);
        ui->pushButton->setText("Change File");
    }
}



void MainWindow::on_submit_clicked()
{

        //api code lines
        QString url = "http://20.198.103.92:8080/api/order/new";
        QUrl apiUrl(url);
        QNetworkRequest request(apiUrl);

        request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        request.setRawHeader("ID", QByteArray::number(userId()));
        request.setRawHeader("Token", token().toUtf8());




        ui->tableWidget->clearContents();
        ui->tableWidget->setRowCount(0);


    QFile file(OpenfilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << file.errorString();
        return; // Failed to open the file, exit the function
    }

    QTextStream in(&file);
    QString headerLine = in.readLine(); // Read the header line and ignore it

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList data = line.split(',');

        // Create the payload JSON object
        QUrlQuery postData;
        postData.addQueryItem("Client_ID", data.value(0));
        postData.addQueryItem("Instrument", data.value(1));
        postData.addQueryItem("Side", data.value(2));
        postData.addQueryItem("Quantity", data.value(3));
        postData.addQueryItem("Price", data.value(4));

        QByteArray payloadData = postData.toString(QUrl::FullyEncoded).toUtf8();
//        qDebug() << "input: " << payloadData;



        QNetworkReply* reply = networkManager->post(request, payloadData);


        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
//                 qDebug() << "responce: " << response;

                // Process the response and append the data to the table
                QString cleanedResponse = response;
                cleanedResponse.remove("[");
                cleanedResponse.remove("]");
                QStringList lines = cleanedResponse.split("\",\"");

                // Append the response data to the table
                int rowCount = ui->tableWidget->rowCount();
                ui->tableWidget->setRowCount(rowCount + lines.size());

                for (int i = 0; i < lines.size(); ++i) {
                    QString line = lines.at(i);
                    line.remove("\"");
                    QStringList responseData = line.split(',');

                    for (int j = 0; j < responseData.size(); ++j) {
                        QTableWidgetItem* item = new QTableWidgetItem(responseData.at(j));
                        item->setTextAlignment(Qt::AlignCenter);
                        ui->tableWidget->setItem(rowCount + i, j, item);
                    }
                }
            } else {
                qDebug() << "Error: " << reply->errorString();
            }

            reply->deleteLater();
        });
    }

    file.close();
}






void MainWindow::onActionLog_OutClicked()
{
    StoreData("0",0);
    this->hide();
    login * loginwindow=new login();
    loginwindow->show();

    qDebug() << "logout";
}


void MainWindow::on_tabWidget_currentChanged(int index)
{

   if(index==1){
       ui->table_prev->clearContents();
       ui->table_prev->setRowCount(0);

       QString url = "http://lseg.sinethd.com:8080/api/order/get";
       QUrl apiUrl(url);
       QNetworkRequest request(apiUrl);

       request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
       request.setRawHeader("ID", QByteArray::number(userId()));
       request.setRawHeader("Token", token().toUtf8());

       QNetworkReply* reply = networkManager->get(request);

       // Connect the finished signal to a lambda function or a slot
       connect(reply, &QNetworkReply::finished, this, [=]() {
           if (reply->error() == QNetworkReply::NoError) {
               // Read the response data
               QByteArray responseData = reply->readAll();

               QString cleanedResponse = responseData;
               cleanedResponse.remove("[");
               cleanedResponse.remove("]");
               QStringList lines = cleanedResponse.split("\",\"");

               // Append the response data to the table
               int rowCount = ui->table_prev->rowCount();
               ui->table_prev->setRowCount(rowCount + lines.size());

               for (int i = 0; i < lines.size(); ++i) {
                   QString line = lines.at(i);
                   line.remove("\"");
                   QStringList responseData = line.split(',');

                   for (int j = 0; j < responseData.size(); ++j) {
                       QTableWidgetItem* item = new QTableWidgetItem(responseData.at(j));
                       item->setTextAlignment(Qt::AlignCenter);
                       ui->table_prev->setItem(rowCount + i, j, item);
                   }
               }
           }
       });


   }

}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{

}


void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    flower=arg1;
}


void MainWindow::on_pushButton_2_clicked()
{
 getDataandSet("2");
}


void MainWindow::on_buy_clicked()
{
getDataandSet("1");
}


void MainWindow::getDataandSet(QString side)
{

    QString url = "http://20.198.103.92:8080/api/order/new";
    QUrl apiUrl(url);
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("ID", QByteArray::number(userId()));
    request.setRawHeader("Token", token().toUtf8());

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    QString cId=ui->client_id->text();
    QString quantity= ui->quantity->text();
    QString price = ui->price_edit->text();



    QUrlQuery postData;
    postData.addQueryItem("Client_ID", cId);
    postData.addQueryItem("Instrument", flower);
    postData.addQueryItem("Side", side);
    postData.addQueryItem("Quantity", quantity);
    postData.addQueryItem("Price", price);

    QByteArray payloadData = postData.toString(QUrl::FullyEncoded).toUtf8();
        qDebug() << "input: " << payloadData;



    QNetworkReply* reply = networkManager->post(request, payloadData);


    connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
//                 qDebug() << "responce: " << response;

            // Process the response and append the data to the table
            QString cleanedResponse = response;
            cleanedResponse.remove("[");
            cleanedResponse.remove("]");
            QStringList lines = cleanedResponse.split("\",\"");

            // Append the response data to the table
            int rowCount = ui->tableWidget->rowCount();
            ui->tableWidget->setRowCount(rowCount + lines.size());

            for (int i = 0; i < lines.size(); ++i) {
                QString line = lines.at(i);
                line.remove("\"");
                QStringList responseData = line.split(',');

                for (int j = 0; j < responseData.size(); ++j) {
                    QTableWidgetItem* item = new QTableWidgetItem(responseData.at(j));
                    item->setTextAlignment(Qt::AlignCenter);
                    ui->tableWidget->setItem(rowCount + i, j, item);
                }
            }
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
    });

}

void MainWindow::on_quantity_valueChanged(int arg1)
{
    ui->error_label->hide();
    if(arg1%10!=0){
        ui->error_label->show();
    }
}

