#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Economical Strategy");

    createStart();
    close();
}

void MainWindow::createStart() {

    start_button = new QPushButton;
    start_spinBoxButton = new QSpinBox;
    start_widget = new QWidget;
    start_grid = new QGridLayout;
    start_label = new QLabel;

    start_button->setText("Играть");
    start_label->setText("Введите количество игроков");

    start_grid->addWidget(start_label, 0, 0);
    start_grid->addWidget(start_spinBoxButton, 0, 1);
    start_grid->addWidget(start_button, 1, 0);

    start_widget->setLayout(start_grid);
    start_widget->setFixedSize(300, 150);
    start_widget->show();


}


MainWindow::~MainWindow()
{
    delete ui;
}
