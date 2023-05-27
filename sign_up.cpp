#include "sign_up.h"
#include "ui_sign_up.h"
#include "login.h"
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


Sign_up::Sign_up(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sign_up)
{
    ui->setupUi(this);
    setWindowTitle("Sign Up");
    QIcon icon(":/res/logo.png");
    setWindowIcon(icon);
    networkManager = new QNetworkAccessManager(this);
    ui->error_label_signup->hide();


}

Sign_up::~Sign_up()
{
    delete ui;
}

void Sign_up::on_btn_signup_clicked()
{
    QString first = ui->firstname->toPlainText();
    QString last = ui->lastname->toPlainText();
    QString user = ui->username->toPlainText();
    QString email = ui->email->toPlainText();
    QString phone  = ui->phonenum->toPlainText();
    QString password = ui->password->toPlainText();

    QString url = "http://20.198.103.92:8085/api/users/new";
    QUrl apiUrl = QUrl::fromUserInput(url);
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("username", user);
    params.addQueryItem("phoneNumber", phone);
    params.addQueryItem("firstName", first);
    params.addQueryItem("lastName", last);
    params.addQueryItem("email", email);
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

             login* destinationPage = new login();
             this->hide();
             destinationPage->show();


         }else{



             if (responseObject.contains("error")) {
                 QString error = responseObject["error"].toString();
//               qDebug() << "Error: " << error;
                 ui->error_label_signup->setText(error);
                 ui->error_label_signup->show();
             }

         }

        reply->deleteLater();
    });

}


void Sign_up::on_btnSignIn_clicked()
{

    login* destinationPage = new login();
    this->hide();
    destinationPage->show();
}

