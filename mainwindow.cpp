#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    players_interface[0]->setAsMainWindow();
}

void MainWindow::clearStartMenu() {
    delete start_button;
    delete start_spinBoxButton;
    delete start_grid;
    delete start_label;
    delete start_widget;
}

void MainWindow::startButtonClicked() {
    if(start_spinBoxButton->text().toInt() >= 2)
    {
        players = QVector<Player>(start_spinBoxButton->text().toInt());
        players[0].setPriority(true);

        start_widget->close();
        clearStartMenu();

        createGameMenu();
        players_interface[0]->show();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update() {

}

void MainWindow::rightButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево
    qDebug() << "left";
    players_interface[current_ind++]->close();
    if(current_ind > players.size() - 1) {
        current_ind = 0;
    }
    players_interface[current_ind]->show();
    update();
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу вправо
    qDebug() << "right";
    players_interface[current_ind--]->close();
    if(current_ind < 0) {
        current_ind = players.size() - 1;
    }
    players_interface[current_ind]->show();
    update();
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

void PlayerInterface::setAsMainWindow() {
    //wid->setLayout(lay);
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

