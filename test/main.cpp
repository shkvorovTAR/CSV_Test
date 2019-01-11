#include <QCoreApplication>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include<string>
#include<iostream>
#include <QTextStream>

double compareLux(double lux,double SuperLux){
    if(lux>SuperLux){
        return SuperLux;
    } else {
        return lux;
    }
}

int main(int argc, char *argv[])
{
    qDebug()<<"Enter file name:";
    qDebug()<<"Input with a .csv after file name!";
    QTextStream input(stdin);
    QString qsFileName=input.readLine();
    QFile file(qsFileName);
    QString message;
    if (!file.exists()) {
        message = "Can't find file %1 to read\n";
        message = message.arg(qsFileName);
        qDebug() << message;
        return -1;
    }
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        message = "Can't open the file %1 for reading\n";
        message = message.arg(qsFileName);
        qDebug() << message;
        return -1;
    }
    double SuperLux=1000;
    double lux=0;
    int days=0;
    QString dayWithSuperLux;
    QString day="";
    QStringList qsl;
    while (!file.atEnd()) {
        QString qsLine = file.readLine();
        qsl=qsLine.split(',');
        if(qsl.size()==3){
            double tempLux = qsl.at(0).toDouble();
            QString qsTemp(QString::number(tempLux,'f',6));
            qsl[0]=qsTemp;
            qint64 unixTimeMilSec = static_cast<qint64>(qsl.at(1).toLongLong());
            qint64 unixTimeSec = (unixTimeMilSec/ static_cast<qint64>(1000));
            QDateTime dt = QDateTime::fromMSecsSinceEpoch(unixTimeMilSec);
            qsl[1]=dt.date().toString();
            if(day==qsl.at(1)||day==""){
                days++;
                lux+=qsl.at(0).toDouble();
            } else if(day!=qsl.at(1)){
                lux=lux/days;
                qDebug().noquote() << day<< "- average lux: "<< lux;
                SuperLux=compareLux(lux, SuperLux);
                if(SuperLux==lux) dayWithSuperLux=day;
                lux=qsl.at(0).toDouble();
                days=1;
            }

        }
        day=qsl.at(1);
    }
    lux=lux/days;
    qDebug().noquote() << day<< "- average lux: "<< lux;
    SuperLux=compareLux(lux,SuperLux);
    if(SuperLux==lux) dayWithSuperLux=day;
    qDebug().noquote() <<"Day with small lux: "<<dayWithSuperLux<<", " <<SuperLux<< " lux";
    file.close();

}
