#include "splashscreen.h"
#include "ui_splashscreen.h"

splashScreen::splashScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::splashScreen)
{
    ui->setupUi(this);
}

splashScreen::~splashScreen()
{
    delete ui;
}
