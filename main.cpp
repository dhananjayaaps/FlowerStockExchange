#include "mainwindow.h"
#include "splashscreen.h"

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    splashScreen w;
//    MainWindow w;
    w.show();
//    bool b = postfunction();
    return a.exec();
}
