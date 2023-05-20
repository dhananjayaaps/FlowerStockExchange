#ifndef SPLASHSCREEN_H
#define SPLASHSCREEN_H

#include <QWidget>

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
};

#endif // SPLASHSCREEN_H
