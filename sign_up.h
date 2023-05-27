#ifndef SIGN_UP_H
#define SIGN_UP_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class Sign_up;
}

class Sign_up : public QWidget
{
    Q_OBJECT

public:
    explicit Sign_up(QWidget *parent = nullptr);
    ~Sign_up();

private slots:
    void on_btn_signup_clicked();

    void on_btnSignIn_clicked();

private:
    Ui::Sign_up *ui;
    QNetworkAccessManager *networkManager;
};

#endif // SIGN_UP_H
