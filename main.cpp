#include "mainwindow.h"
#include <QApplication>
#include "factory.h"
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.css");
    file.open(QFile::ReadOnly);
    a.setStyleSheet((file.readAll()));
    MainWindow w;

    return a.exec(); //second buy
}
