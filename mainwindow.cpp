#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mainrun.h"

#include <QFileDialog>
#include <QHBoxLayout>
#include <QDialog>
#include <QDebug>
#include <QTableWidgetItem>
#include<iostream>


QString OpenfilePath ;
QString tim;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Flower Stock Exchange");
    QIcon icon(":/new/prefix1/logo.png");
    setWindowIcon(icon);


    ui->tableWidget->setColumnCount(9);

    QStringList headerLabels = { "Order ID", "Client ID", "Instrument", "Side", "Status", "Quantity", "Price", "Transaction Time","Reason" };
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);


    ui->tableWidget->horizontalHeader()->resizeSection(0, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(1, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(2, 90);
    ui->tableWidget->horizontalHeader()->resizeSection(3, 80);
    ui->tableWidget->horizontalHeader()->resizeSection(4, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(5, 70);
    ui->tableWidget->horizontalHeader()->resizeSection(6, 100);
    ui->tableWidget->horizontalHeader()->resizeSection(7, 170);
    ui->tableWidget->horizontalHeader()->resizeSection(8, 130);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);


}

MainWindow::~MainWindow()
{
    delete ui;
}


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
    if(!OpenfilePath.isEmpty()){
       int a = mainfunction(OpenfilePath.toStdString());
       if(a){
           QFile file("execution_rep.csv");
               if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                   // Error handling

               }

                QTextStream in(&file);
                int row = 0;
                QString line = in.readLine();
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList data = line.split(",");
                    ui->tableWidget->setRowCount(1+row);
                    // Add data to the table widget
                    int column = 0;
                    for (const QString& value : data) {
                       QTableWidgetItem* item = new QTableWidgetItem(value.trimmed());
                       item->setTextAlignment(Qt::AlignCenter);
                       ui->tableWidget->setItem(row, column, item);
                       column++;
                    }
                    row++;

                }

                file.close();
//                std::cout << "Time difference: " <<Time_dif;
                ui->label->setText("Time : "+tim +" milliseconds");
       }


    }
}



void Time_dif(int timedff){
    tim = QString::number(timedff);
}
