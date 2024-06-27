#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "player.h"

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

    void createStart();
    void clearStart();
private slots:
    void startClicked();
private:
    Ui::MainWindow *ui;

    QPushButton* start_button;
    QWidget* start_widget;
    QSpinBox* start_spinBoxButton;
    QGridLayout* start_grid;
    QLabel* start_label;



};
#endif // MAINWINDOW_H
