#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"
#include<password_menu.h>
#include"pass_check.h"

#include <qDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>
#include <QLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE


class PlayerInterface : public QMainWindow {
    Q_OBJECT
public:


    QWidget* wid;
    QGridLayout* lay;

    QPushButton* left_but;
    QPushButton* right_but;

    QLabel* name;
    QLabel* money;
    QLabel* raw;
    QLabel* product;
    QLabel* def_facts;
    QLabel* auto_facts;

    void show();
    void close();
    PlayerInterface(const Player& pl, const QMainWindow* w);

    void setAsMainWindow();
    ~PlayerInterface();
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    static int current_ind;

    void createStartMenu();
    void clearStartMenu();

    void createGameMenu();

private slots:
    void startButtonClicked();
    void leftButtonClicked();
    void rightButtonClicked();
public slots:
    void slot_pass(QString password);
    void slot_pass_check(int);
signals:
    void signal_pass(int);
    void signal_index(QString);
private:
    Ui::MainWindow *ui;

    QVector<Player> players;
    QVector<PlayerInterface*> players_interface;

    QPushButton* start_button;

    QWidget* start_widget;
    QSpinBox* start_spinBoxButton;
    QGridLayout* start_grid;
    QLabel* start_label;

    password_menu *pm;
    pass_check *pch;
};



#endif // MAINWINDOW_H
