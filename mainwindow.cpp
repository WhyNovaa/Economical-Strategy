#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"password_menu.h"
#include<QString>
#include<QVector>

int MainWindow::current_ind = 0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Economical Strategy");

    createStartMenu();

}

void MainWindow::createStartMenu() {

    start_button = new QPushButton;
    start_spinBoxButton = new QSpinBox;
    start_widget = new QWidget;
    start_grid = new QGridLayout;
    start_label = new QLabel;

    connect(start_button, SIGNAL( clicked() ), this, SLOT( startButtonClicked() ));

    start_button->setText("Играть");
    start_label->setText("Введите количество игроков");

    start_button->setStyleSheet("QPushButton { background-color: blue; color: white; }");

    start_grid->addWidget(start_label, 0, 0);
    start_grid->addWidget(start_spinBoxButton, 0, 1);
    start_grid->addWidget(start_button, 1, 0);

    start_widget->setLayout(start_grid);
    start_widget->setFixedSize(300, 150);
    start_widget->show();

}

void MainWindow::createGameMenu() {
    for(const auto& pl : players) {
        players_interface.push_back(new PlayerInterface(pl, this));
    }
}

void MainWindow::clearStartMenu() {
    delete start_button;
    delete start_spinBoxButton;
    delete start_grid;
    delete start_label;
    delete start_widget;
}

QVector <QString> passwords; // тут будут храниться пароли игроков

void MainWindow::startButtonClicked() {
    if(start_spinBoxButton->text().toInt() >= 2 && start_spinBoxButton->text().toInt() <= 6)
    {
        players = QVector<Player>(start_spinBoxButton->text().toInt());
        players[0].setPriority(true);

        start_widget->close();
        clearStartMenu();

        //сначала пароль
        pm = new password_menu(this);
        pm->show();

        connect(this, &MainWindow::signal_pass, pm, &password_menu::slot);
        connect(pm, &password_menu::signal_pass_back, this, &MainWindow::slot_pass);
        emit signal_pass(players.size());

        /*createGameMenu();
        players_interface[0]->show();*/ //создадим после ввода паролей
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update() {

}


void MainWindow::slot_pass(QString password)
{
    if(password == "end"){
        pm->close();

        createGameMenu();
        players_interface[0]->show();
    }else{
        passwords.push_back(password);
    }
}

int flag = -1;
void MainWindow::slot_pass_check(int answ)
{
    flag = answ;
    if(flag == 1){
        pch->close();
        players_interface[current_ind]->show();
        update();
        flag = -1;
    }
}

void MainWindow::rightButtonClicked() {
    //здесь реализация перехода к другому интерфейсу вправо
    qDebug() << "right";
    players_interface[current_ind++]->close();
    if(current_ind > players.size() - 1) {
        current_ind = 0;
    }

    pch = new pass_check(this);
    pch -> show();
    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    emit signal_index(passwords[current_ind]);
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево
    qDebug() << "left";
    players_interface[current_ind--]->close();
    if(current_ind < 0) {
        current_ind = players.size() - 1;
    }

    pch = new pass_check(this);
    pch -> show();
    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    emit signal_index(passwords[current_ind]);
}
//-------------------------PlayerInterface-------------------------

PlayerInterface::PlayerInterface(const Player& pl, const QMainWindow* w) {
    wid = new QWidget;
    lay = new QGridLayout;

    left_but = new QPushButton;
    right_but = new QPushButton;

    name = new QLabel;
    money = new QLabel;
    raw = new QLabel;
    product = new QLabel;
    def_facts = new QLabel;
    auto_facts = new QLabel;

    left_but->setText("<-");
    right_but->setText("->");

    connect(left_but, SIGNAL( clicked() ), w, SLOT( leftButtonClicked() ));
    connect(right_but, SIGNAL( clicked() ), w, SLOT( rightButtonClicked() ));

    name->setText("Игрок " + QString::number(pl.getID()));
    money->setText("Деньги: " + QString::number(pl.getMoney()));
    raw->setText("Сырье: " + QString::number(pl.getRaw()));
    product->setText("Готового сырья: " + QString::number(pl.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(pl.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(pl.getAutoFacts().size()));

    lay->addWidget(name, 0, 0);
    lay->addWidget(money, 1, 0);
    lay->addWidget(raw, 2, 0);
    lay->addWidget(product, 3, 0);
    lay->addWidget(def_facts, 4, 0);
    lay->addWidget(auto_facts, 5, 0);

    lay->addWidget(left_but, 6, 0);
    lay->addWidget(right_but, 6, 5);
    wid->setLayout(lay);
}

void PlayerInterface::show() {
    wid->show();
}

void PlayerInterface::close() {
    wid->close();
}

PlayerInterface::~PlayerInterface() {
    delete wid;
    delete lay;
    delete money;
    delete raw;
    delete product;
    delete def_facts;
    delete auto_facts;
}
