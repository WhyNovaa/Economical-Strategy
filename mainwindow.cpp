#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"password_menu.h"
#include<QString>
#include<QVector>
#include<QRandomGenerator>
#include<QDebug>
#include<QMessageBox>

int MainWindow::current_ind = 0;

QVector <QString> passwords; // тут будут храниться пароли игроков
QRandomGenerator *rg = QRandomGenerator::global();
int session_key = rg->bounded(100000, 1000000);
QVector <int> money_backup; //откат денюжек

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

    QFont font = start_button->font();
    font.setPointSize(13);
    font.setBold(true);
    start_button->setFont(font);

    start_label->setText("Введите количество игроков");
    start_label->setFont(font);

    start_spinBoxButton->setFont(font);

    start_button->setStyleSheet("QPushButton { background-color: #5ac47f; color: black; border: 2px solid gray } QPushButton:hover {background-color: #398453} QPushButton:pressed {background-color: #1c4f2e}");

    start_grid->addWidget(start_label, 0, 0);
    start_grid->addWidget(start_spinBoxButton, 0, 1);
    start_grid->addWidget(start_button, 1, 0);

    start_widget->setLayout(start_grid);
    start_widget->setFixedSize(400, 200);


    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    start_widget->setPalette(palette);

    start_widget->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

    start_widget->show();
}

void MainWindow::createGameMenu() {
    for(auto& pl : players) {
        players_interface.push_back(new PlayerInterface(pl, this));
    }
    b1 = new Bank(players);
}

void MainWindow::clearStartMenu() {
    delete start_button;
    delete start_spinBoxButton;
    delete start_grid;
    delete start_label;
    delete start_widget;
}

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

void MainWindow:: updatePlayers() {
    for(int i =0; i < players_interface.size(); i ++) {
        players_interface[i]->setPlayer(players[i]);
        players_interface[i]->updateData();
        money_backup[i]=players[i].getMoney()*session_key;
    }

}
void PlayerInterface:: setPlayer(const Player& g1) {
    current_player = g1;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void PlayerInterface::updateData() {
    money->setText("Деньги: " + QString::number(current_player.getMoney()));
    raw->setText("Сырье: " + QString::number(current_player.getRaw()));
    product->setText("Готового сырья: " + QString::number(current_player.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(current_player.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(current_player.getAutoFacts().size()));
}


void MainWindow::slot_pass(QString password)
{
    if(password == "end"){
        pm->close();

        createGameMenu();

        for(int i=0; i<players.size(); i++){
            money_backup.push_back(players[i].getMoney()*session_key);
        }

        players_interface[0]->show();
    }else{
        passwords.push_back(password);
    }
}

int flag = -1;
QString check;
void MainWindow::slot_pass_check(int answ)
{
    players_interface[current_ind]->close();
    if(check == "right"){
        current_ind++;
    }
    if(check == "left"){
        current_ind--;
    }
    if(current_ind < 0) {
        current_ind = players.size() - 1;
    }
    if(current_ind > players.size() - 1) {
        current_ind = 0;
    }
    flag = answ;
    if(flag == 1){
        pch->close();
        players_interface[current_ind]->show();
        flag = -1;
    }
}

void MainWindow::rightButtonClicked() {
    //здесь реализация перехода к другому интерфейсу вправо

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);


    qDebug() << "right";
    check = "right";

    pch = new pass_check(this);
    pch -> show();

    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    if(current_ind+1> players.size() - 1){
        emit signal_index(passwords[0], 0);
    }else{
        emit signal_index(passwords[current_ind+1], current_ind+1);
    }

    for(int i=0; i<players.size(); i++){
        if(money_backup[i] != players[i].getMoney()*session_key){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
        }
    }
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);


    qDebug() << "left";
    check = "left";

    pch = new pass_check(this);
    pch -> show();
    pch->activateWindow();

    connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
    connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
    if (current_ind-1<0){
        emit signal_index(passwords[players.size() - 1], players.size() - 1);
    }else{
        emit signal_index(passwords[current_ind - 1], current_ind - 1);
    }

    for(int i=0; i<players.size(); i++){
        if(money_backup[i]!=players[i].getMoney()*session_key){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
        }
    }
}
//-------------------------PlayerInterface-------------------------

PlayerInterface::PlayerInterface(const Player& pl, const QMainWindow* w) {
    current_player = pl;
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
    connect(make_bid, SIGNAL(clicked()), w, SLOT(auctionSlot()));

    produce = new QPushButton;
    produce->setText("Произвести");

    make_credit = new QPushButton;
    make_credit->setText("Кредит");
    connect(make_credit, SIGNAL(clicked()), w, SLOT(creditSlot()));

    insurance = new QPushButton;
    insurance->setText("Взять страховку");
    connect(insurance, SIGNAL(clicked()), w, SLOT(insuranceSlot()));

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
    updateData();
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

//<----------------------------------Bank---------------------------------------------->
void MainWindow:: auctionSlot() {

}
void MainWindow:: creditSlot() {
    credit_dialog *rec1 = new credit_dialog(this);
    rec1->show();
    if(rec1->exec() == QDialog::Accepted) { //взятие кредита
        if (rec1->getCredit() == -1) {
             QMessageBox::information(this, "Взятие кредита", "Ошибка ввода дданых повторите попытку");
            delete rec1;
        }
    else{
        int b = b1->credit(players[current_ind],rec1->getCredit());
        if (b == 1) {
             QMessageBox::information(this, "Взятие кредита", "Вы взяли кредит");
        }
        if(b == 0) {
              QMessageBox::information(this, "Взятие кредита", "Ваш баланс меньше суммы кредита, банк отказывется кредитовать вас");
        }
        if(b == -1) {
              QMessageBox::information(this, "Взятие кредита", "Вы уже взяли кредит, для начала погасите его");
        }
        players=b1->getAllPlayers();
        this->updatePlayers();
        }
        delete rec1;
    }
    else { //погашение кредита
        if (rec1->getLowCredit() == -1) {
            QMessageBox::information(this, "Погашение кредита", "Ошибка ввода дданых повторите попытку");
            delete rec1;
        }
        else{
            int b2 = b1->payCredit(players[current_ind],rec1->getLowCredit());
            if (b2 >0) {
                QMessageBox::information(this, "Погашение кредита", "Вы погасили часть кредита, оставшаяся сумма: " + QString::number(b2));
            }
            if(b2 == 0) {
                QMessageBox::information(this, "Погашение кредита", "Вы погасили кредит!");
            }
            if(b2 == -2) {
                QMessageBox::information(this, "Погашение кредита", "Вы не брали кредит");
            }
            if(b2 == -1) {
                QMessageBox::information(this, "Погашение кредита", "Не хватает средств для погашения кредита");
            }
            players=b1->getAllPlayers();
            this->updatePlayers();
             delete rec1;
        }
    }
}//взятие кредита, 0, если не смог купить, 1,если смог купить, -1 если уже взял кредит

void MainWindow:: insuranceSlot() {
    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);

    this->setPalette(palette);
    this->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

    this->setFixedSize(400, 200);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Взятие страховки", "Стоимость страховки на следующий ход -- 300. Взятие страховки уберегает вас от негативных эффектов событий. Хотите оформить страховку?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No) {

    }
    else {
        bool b = b1->buyInsurance(players[current_ind], 300); // пока цена страховки триста
        if (b) {
            QMessageBox::StandardButton reply = QMessageBox::information(this, "Взятие страховки", "Вы взяли страховку");
        }
        else{
             QMessageBox::StandardButton reply = QMessageBox::information(this, "Взятие страховки", "Недостаточно средств для взятия страховки");
        }
    }
    players=b1->getAllPlayers();
    this->updatePlayers();
}
