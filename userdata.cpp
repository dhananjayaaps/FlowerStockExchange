#include "userdata.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString Token,userid;

void GetData(){
    QFile configFile("config_file.ini");

        if (configFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&configFile);
            while (!in.atEnd())
            {
                QString line = in.readLine();

                if (line == "[Token]")
                {
                    line = in.readLine();
                    Token = line;
                }
                else if (line == "[UserID]")
                {
                    line = in.readLine();
                    userid = line;
                }
            }

            configFile.close();

//            qDebug() << "Token:" << Token;
//            qDebug() << "User ID:" << userid;
        }
        else
        {
            qDebug() << "Failed to open the configuration file for reading.";
        }
}

int userId(){
    GetData();
    return userid.toInt();
}
QString token(){
    GetData();
    return Token;
}

bool StoreData(QString token,int userid){

    QFile configFile("config_file.ini");

    if (configFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&configFile);

        out << "[Token]\n" << token << "\n\n";
        out << "[UserID]\n" << userid << "\n\n";

        configFile.close();

        return true;
    }
    else
    {
        return false;
    }

}
