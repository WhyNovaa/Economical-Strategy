#include "mainwindow.h"
#include <QApplication>
#include "factory.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    return a.exec(); //second buy
}
