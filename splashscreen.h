#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class splashScreen;
}

class splashScreen : public QWidget
{
    Q_OBJECT

public:
    explicit splashScreen(QWidget *parent = nullptr);
    ~splashScreen();

private:
    Ui::splashScreen *ui;
    MainWindow* mainWindow;
    QNetworkAccessManager *networkManager;
    void validate();


private slots:
    void checkAnimationEnd();

    void on_pushButton_clicked();
};
void buttonClicked();

#endif // SPLASHSCREEN_H
