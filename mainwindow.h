#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
//#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_submit_clicked();
    void onActionLog_OutClicked();



    void on_tabWidget_currentChanged(int index);

    void on_comboBox_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_pushButton_2_clicked();

    void on_buy_clicked();

    void on_quantity_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    void getDataandSet(QString side);
//    login* loginwindow;

};

#endif // MAINWINDOW_H
