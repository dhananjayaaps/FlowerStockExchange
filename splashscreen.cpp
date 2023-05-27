#include "splashscreen.h"
#include "ui_splashscreen.h"
#include "login.h"
#include "mainwindow.h"
#include <QLabel>
#include <QMovie>
#include <QStackedLayout>
#include <QTimer>
#include <iostream>
#include "userdata.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QUrlQuery>
#include <QFile>
#include <QTextStream>
#include <QObject>

QMovie* movie;

splashScreen::splashScreen(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::splashScreen)
{
    ui->setupUi(this);
    setWindowTitle("Get Started");
    QIcon icon(":/res/logo.png");
    setWindowIcon(icon);
    networkManager = new QNetworkAccessManager(this);
    mainWindow = new MainWindow();

    ui->pushButton->setVisible(false);

    QFrame* container = new QFrame(this);
    container->setStyleSheet("background-color: transparent;"); // Set the container background to transparent

    QStackedLayout* layout = new QStackedLayout(container);
    layout->setContentsMargins(0, 0, 0, 0); // Set margins to 0

    movie = new QMovie(":/res/back_1.gif");
    movie->setScaledSize(this->size());

    QLabel* label = new QLabel(container);
    label->setMovie(movie);
    movie->start();

    layout->addWidget(label);

    setLayout(layout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    connect(movie, &QMovie::frameChanged, this, &splashScreen::checkAnimationEnd);

    // Send the QMovie widget to the back
    label->lower();

    QTimer::singleShot(6000, [this]() {
        ui->pushButton->setVisible(true);
    });
}

void splashScreen::checkAnimationEnd()
{
    QTimer::singleShot(9500, []() {
        if (movie) {
            movie->stop();
        }
    });
}

splashScreen::~splashScreen()
{
    delete ui;
}


void splashScreen::on_pushButton_clicked()
{
validate();

}

void splashScreen::validate(){
    QString url = "http://20.198.103.92:8085/api/users/token";
    QUrl apiUrl = QUrl::fromUserInput(url);
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("userId", QString::number(userId()));
    params.addQueryItem("token", token());

    QByteArray postData = params.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkReply* reply = networkManager->post(request, postData);


    connect(reply, &QNetworkReply::finished, [=]() {

         int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
//         QByteArray responseData = reply->readAll();
//         QJsonDocument responseDoc = QJsonDocument::fromJson(responseData);
//         QJsonObject responseObject = responseDoc.object();
         if (statusCode==200){
             this->hide();
             mainWindow->show();

         }else{

             login* destinationPage = new login();
             this->hide();
             destinationPage->show();
         }

        reply->deleteLater();
    });
}

