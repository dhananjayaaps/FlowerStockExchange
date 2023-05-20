#include "mainwindow.h"
#include "ui_mainwindow.h"
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


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);

    ui->tableWidget->setColumnCount(8); // Set the column count
    QStringList headerLabels = { "Order ID", "Client ID", "Instrument", "Side", "Status", "Quantity", "Price", "Transaction Time" };
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);

    ui->tableWidget->horizontalHeader()->resizeSection(0, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(1, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(4, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(5, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(6, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(7, 200);


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);

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
    }
}


void MainWindow::on_submit_clicked()
{

        //api code lines
        QString url = "http://20.198.103.92:8080/api/order/new";
        QUrl apiUrl(url);
        QNetworkRequest request(apiUrl);

        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(84));

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
        QJsonObject payload;
        payload["Client_ID"] = data.value(0);
        payload["Instrument"] = data.value(1);
        payload["Side"] = data.value(2);
        payload["Quantity"] = data.value(3);
        payload["Price"] = data.value(4);

        // Convert the payload to JSON data
        QJsonDocument payloadDoc(payload);
        QByteArray payloadData = payloadDoc.toJson(QJsonDocument::Compact);

        // Make the API request
        QNetworkReply* reply = networkManager->post(request, payloadData);

        // Connect the finished signal to handle the response
        connect(reply, &QNetworkReply::finished, [=]() {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();

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





//void MainWindow::on_submit_clicked()
//{
////

//    //api code lines
//    QString url = "http://20.198.103.92:8123/api/order/new";
//    QUrl apiUrl(url);
//    QNetworkRequest request(apiUrl);

//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//    request.setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(84));

//    ui->tableWidget->clearContents();
//    ui->tableWidget->setRowCount(0);



//    QFile file(OpenfilePath);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        QTextStream in(&file);
//        QString headerLine = in.readLine();

//        while (!in.atEnd()) {
//            QString line = in.readLine();
//            QStringList data = line.split(',');

//            QJsonObject payload;
//                payload["Client_ID"] = "aa18";
//                payload["Instrument"] = "Rose";
//                payload["Side"] = "2";
//                payload["Quantity"] = "60";
//                payload["Price"] = "75";
//                QJsonDocument payloadDoc(payload);
//                QByteArray payloadData = payloadDoc.toJson();

////                requesting
//                QNetworkReply* reply = networkManager->post(request, payloadData);


//                connect(reply, &QNetworkReply::finished, [=]() {
//                        if (reply->error() == QNetworkReply::NoError) {
//                            QByteArray response = reply->readAll();

//                            QString cleanedResponse = response;
//                            std::cout<<"cleanedResponse";
//                            cleanedResponse.remove("[");
//                            cleanedResponse.remove("]");
//                            QStringList lines = cleanedResponse.split("\",\"");
//                            ui->tableWidget->setRowCount(lines.size());


//                            for (int i = 0; i < lines.size(); ++i) {
//                                QString line = lines.at(i);
//                                line.remove("\"");
//                                QStringList data = line.split(',');

//                                for (int j = 0; j < data.size(); ++j) {
//                                    QTableWidgetItem* item = new QTableWidgetItem(data.at(j));
//                                    item->setTextAlignment(Qt::AlignCenter);
//                                    ui->tableWidget->setItem(i, j, item);
//                                }
//                            }




//                        } else {
//                            qDebug() << "Error: " << reply->errorString();
//                        }

//                        reply->deleteLater();
//                    });





//        }

//        file.close();
//    } else {
//        qDebug() << "Failed to open file:" << file.errorString();
//    }






//}


//while start point
//    QJsonObject payload;
//    payload["Client_ID"] = "aa18";
//    payload["Instrument"] = "Rose";
//    payload["Side"] = "2";
//    payload["Quantity"] = "60";
//    payload["Price"] = "75";
//    QJsonDocument payloadDoc(payload);
//    QByteArray payloadData = payloadDoc.toJson();

    //requesting
//    QNetworkReply* reply = networkManager->post(request, payloadData);


//    connect(reply, &QNetworkReply::finished, [=]() {
//            if (reply->error() == QNetworkReply::NoError) {
//                QByteArray response = reply->readAll();

//                QString cleanedResponse = response;
//                cleanedResponse.remove("[");
//                cleanedResponse.remove("]");
//                QStringList lines = cleanedResponse.split("\",\"");
//                ui->tableWidget->setRowCount(lines.size());


//                for (int i = 0; i < lines.size(); ++i) {
//                    QString line = lines.at(i);
//                    line.remove("\"");
//                    QStringList data = line.split(',');

//                    for (int j = 0; j < data.size(); ++j) {
//                        QTableWidgetItem* item = new QTableWidgetItem(data.at(j));
//                        item->setTextAlignment(Qt::AlignCenter);
//                        ui->tableWidget->setItem(i, j, item);
//                    }
//                }




//            } else {
//                qDebug() << "Error: " << reply->errorString();
//            }

//            reply->deleteLater();
//        });

//}
