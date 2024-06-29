#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

#include <qDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGridLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void createStartMenu();
    void clearStartMenu();

private slots:
    void startButtonClicked();

private:
    Ui::MainWindow *ui;

    QVector<Player> players;


    QPushButton* start_button;
    QWidget* start_widget;
    QSpinBox* start_spinBoxButton;
    QGridLayout* start_grid;
    QLabel* start_label;



};
#endif // MAINWINDOW_H
