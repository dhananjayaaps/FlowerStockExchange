
#include "ui_login.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sign_up.h"
#include "ui_sign_up.h"
#include "login.h"
#include "userdata.h"
#include <QLabel>
#include <QMovie>
#include <QStackedLayout>
#include <QTimer>
#include <iostream>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>
#include <QFile>
#include <QTextStream>




QMovie* movie1;


login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    setWindowTitle("Sign In");
    QIcon icon(":/res/logo.png");
    setWindowIcon(icon);
    networkManager = new QNetworkAccessManager(this);
    ui->error_label->hide();


    mainWindow = new MainWindow();
    int time=8000;

        QTimer::singleShot(time, [this]() {
            ui->background->setStyleSheet("background-image: url(:/res/login_ui1.png);");

        });
        time+=8000;
        QTimer::singleShot(time, [this]() {
            ui->background->setStyleSheet("background-image: url(:/res/login_ui2.png);");

        });
        time+=8000;
        QTimer::singleShot(time, [this]() {
            ui->background->setStyleSheet("background-image: url(:/res/login_ui.png);");

        });

}

login::~login()
{
    delete ui;
}



void login::on_btnSignup_clicked()
{
    Sign_up* destinationPage = new Sign_up();
    this->hide();
    destinationPage->show();


}


void login::on_btnSisgnIn_clicked()
{
    QString email = ui->email_edit->toPlainText();
    QString password = ui->password_edit->toPlainText();
    QString url = "http://20.198.103.92:8085/api/users/login";
    QUrl apiUrl = QUrl::fromUserInput(url);
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("username", email);
    params.addQueryItem("password", password);

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    // Make the API request
    QNetworkReply* reply = networkManager->post(request, postData);

    // Connect the finished signal to handle the response
    connect(reply, &QNetworkReply::finished, [=]() {

         int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
         QByteArray responseData = reply->readAll();
         QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
         QJsonObject responseObject = responseDoc.object();
         if (statusCode==200){

             QString token;
             int userId;

             if (responseObject.contains("token")) {
                 token = responseObject["token"].toString();
                 userId=responseObject["userId"].toInt();

                 if(StoreData(token,userId)){
                    this->close();
                    mainWindow->show();
                 }else{
                     ui->error_label->setText("Try again!");
                     ui->error_label->show();
                 }



             }


         }else{



             if (responseObject.contains("error")) {
                 QString error = responseObject["error"].toString();
//               qDebug() << "Error: " << error;
                 ui->error_label->setText(error);
                 ui->error_label->show();
             }

         }







//        if (reply->error() == QNetworkReply::NoError) {
//            qDebug() << "No Error  :)";
//            QByteArray responseData = reply->readAll();
//            QString responseString = QString::fromUtf8(responseData);
//            qDebug() << responseString;
//            // Process the response as needed
//        } else {
//            qDebug() << "Error: " << reply->errorString();
//        }

        reply->deleteLater();
    });






}

