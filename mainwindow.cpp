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
    this->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));


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

    start_button->setStyleSheet("QPushButton { background-color: #5ac47f; color: black; border: 2px solid gray }");

    start_grid->addWidget(start_label, 0, 0);
    start_grid->addWidget(start_spinBoxButton, 0, 1);
    start_grid->addWidget(start_button, 1, 0);

    start_widget->setLayout(start_grid);
    start_widget->setFixedSize(300, 150);


    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    start_widget->setPalette(palette);

    start_widget->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

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

        QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Window, bkgnd);
        pm->setPalette(palette);

        pm->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

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

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);


    qDebug() << "right";
    current_ind++;
    if(current_ind > players.size() - 1) {
        current_ind = 0;
    }

    pch = new pass_check(this);
    pch -> show();

    players_interface[current_ind]->close();

    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    emit signal_index(passwords[current_ind], current_ind);
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);


    qDebug() << "left";
    current_ind--;
    if(current_ind < 0) {
        current_ind = players.size() - 1;
    }

    pch = new pass_check(this);
    pch -> show();
    pch->activateWindow();

    players_interface[current_ind]->close();

    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    emit signal_index(passwords[current_ind], current_ind);
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
    name->setAlignment(Qt::AlignCenter);


    QFont font = name->font();
    font.setPointSize(32);
    font.setBold(true);
    name->setFont(font);


    money->setText("Деньги: " + QString::number(pl.getMoney()));

    font = money->font();
    font.setPointSize(24);
    font.setBold(true);

    raw->setText("Сырье: " + QString::number(pl.getRaw()));
    product->setText("Готового сырья: " + QString::number(pl.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(pl.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(pl.getAutoFacts().size()));

    money->setAlignment(Qt::AlignCenter);
    raw->setAlignment(Qt::AlignCenter);
    product->setAlignment(Qt::AlignCenter);
    def_facts->setAlignment(Qt::AlignCenter);
    auto_facts->setAlignment(Qt::AlignCenter);


    money->setFont(font);
    raw->setFont(font);
    product->setFont(font);
    def_facts->setFont(font);
    auto_facts->setFont(font);

    upgr_fact = new QPushButton;
    upgr_fact->setText("Улучшить фабрику");

    make_bid = new QPushButton;
    make_bid->setText("Сделать ставку");

    produce = new QPushButton;
    produce->setText("Произвести");

    make_credit = new QPushButton;
    make_credit->setText("Кредит");

    insurance = new QPushButton;
    insurance->setText("Взять страховку");

    upgr_fact->setFont(font);
    make_bid->setFont(font);
    produce->setFont(font);
    make_credit->setFont(font);
    insurance->setFont(font);

    upgr_fact->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    make_bid->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    produce->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    make_credit->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    insurance->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);


    lay->addWidget(name, 0, 0, 1, 2);
    lay->addWidget(money, 1, 0);
    lay->addWidget(raw, 2, 0);
    lay->addWidget(product, 3, 0);
    lay->addWidget(def_facts, 4, 0);
    lay->addWidget(auto_facts, 5, 0);

    lay->addWidget(make_bid, 1, 1);
    lay->addWidget(make_credit, 2, 1);
    lay->addWidget(insurance, 3, 1);
    lay->addWidget(produce, 4, 1);
    lay->addWidget(upgr_fact, 5, 1);

    lay->addWidget(left_but, 6, 0);
    lay->addWidget(right_but, 6, 1);

    wid->setLayout(lay);

    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    wid->setPalette(palette);

    wid->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

    wid->setWindowState(Qt::WindowFullScreen);
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

void PlayerInterface::setLeftBtnEn(bool b) {
    left_but->setEnabled(b);
}

void PlayerInterface::setRightBtnEn(bool b) {
    right_but->setEnabled(b);
}
