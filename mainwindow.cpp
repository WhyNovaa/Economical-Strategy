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
int session_key = rg->bounded(100, 1000);
QVector <int> money_backup;    //откат денюжек
QVector <int> raw_backup;
QVector <int> product_backup;
QVector <int> def_backup;
QVector <int> auto_backup;

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
    start_grid->addWidget(start_button, 1, 0, 1, 2);

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
void MainWindow::updateBankPlayers() {
    b1->setAllPlayers(players);
}
void MainWindow:: updatePlayers() {
    for(int i =0; i < players_interface.size(); i ++) {
        players_interface[i]->setPlayer(players[i]);
        players_interface[i]->updateData();
        money_backup[i]=players[i].getMoney()*session_key;
        raw_backup[i]=players[i].getRaw()*session_key;
        product_backup[i]=players[i].getProduct()*session_key;
        def_backup[i]=players[i].getDefFacts().size()*session_key;
        auto_backup[i]=players[i].getAutoFacts().size()*session_key;
    }
    checkGameOver();
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::slot_pass(QString password)
{
    if(password == "end"){
        pm->close();

        createGameMenu();

        for(int i=0; i<players.size(); i++){
            money_backup.push_back(players[i].getMoney()*session_key);
            raw_backup.push_back(players[i].getRaw()*session_key);
            product_backup.push_back(players[i].getProduct()*session_key);
            def_backup.push_back(players[i].getDefFacts().size()*session_key);
            auto_backup.push_back(players[i].getAutoFacts().size()*session_key);
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

    players_interface[current_ind]->hide();
    qDebug() << "right";
    check = "right";
    qDebug()<<current_ind;

    if((current_ind+1 <= players.size()-1 && players[current_ind+1].getStatus()=="in") || (current_ind+1>players.size()-1 && players[0].getStatus()=="in")){

        pch = new pass_check(this);
        pch -> show();
        pch->activateWindow();

        connect(this, &MainWindow::signal_index, pch, &pass_check::slot_index);
        connect(pch, &pass_check::signal_pass_check, this, &MainWindow::slot_pass_check);
        if (current_ind+1<players.size()){
            emit signal_index(passwords[current_ind + 1], current_ind + 1);
        }else{
            emit signal_index(passwords[0], 0);
        }
    }
    else{
        players_interface[current_ind]->close();
        current_ind++;
        if(current_ind < 0) {
            current_ind = players.size() - 1;
        }
        if(current_ind > players.size() - 1) {
            current_ind = 0;
        }
        players_interface[current_ind]->show();
    }

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key) || (raw_backup[i]!=players[i].getRaw()*session_key) || (product_backup[i]!=players[i].getProduct()*session_key) || (def_backup[i]!=players[i].getDefFacts().size()*session_key) || (auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
        }
    }
    checkGameOver();
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);

    qDebug() << "left";
    check = "left";
    qDebug()<<current_ind;

    players_interface[current_ind]->hide();
    if((current_ind-1>=0 && players[current_ind-1].getStatus()=="in") || (current_ind-1<0 && players[players.size()-1].getStatus()=="in")){

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
    }
    else{
        players_interface[current_ind]->close();
        current_ind--;
        if(current_ind < 0) {
            current_ind = players.size() - 1;
        }
        if(current_ind > players.size() - 1) {
            current_ind = 0;
        }
        players_interface[current_ind]->show();
    }

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key) || (raw_backup[i]!=players[i].getRaw()*session_key) || (product_backup[i]!=players[i].getProduct()*session_key) || (def_backup[i]!=players[i].getDefFacts().size()*session_key) || (auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
        }
    }
    checkGameOver();
}

void MainWindow::upgradeFactSlot() {
    fabric_dialog *rec1 = new fabric_dialog(this);
    rec1->show();
    if(rec1->exec() == QDialog::Accepted) {
        int amount = rec1->getAmount();
        if (amount == 0) {
            QMessageBox::information(this, "Улучшение фабрик", "Ошибка ввода данных, повторите попытку");
        } else if(amount > players[current_ind].getDefFacts().size()) {
            QMessageBox::information(this, "Улучшение фабрик", "Недостаточно фабрик, повторите попытку");
        }
        else if(amount * 3000 > players[current_ind].getMoney()) {
            QMessageBox::information(this, "Улучшение фабрик", "Недостаточно средств, повторите попытку");
        }
        else {
            players[current_ind].upgradeFacts(amount);
            QMessageBox::information(this, "Улучшение фабрик", "Операция выполнена успешно");
            this->updateBankPlayers();
            this->updatePlayers();
        }
        delete rec1;
    }
}
void MainWindow::produceSlot() {
    product_dialog *rec1 = new product_dialog(this);
    rec1->show();
    if(rec1->exec() == QDialog::Accepted) {
        int amount = rec1->getAmount();
        if (amount == 0) {
            QMessageBox::information(this, "Переработка сырья", "Ошибка ввода данных, повторите попытку");
        }
        else if(amount > players[current_ind].getRaw()) {
            QMessageBox::information(this, "Переработка сырья", "Недостаточно сырья, повторите попытку");
        }
        else if(!players[current_ind].putRawInFabrics(amount)) {
            QMessageBox::information(this, "Переработка сырья", "Недостаточно места на фабриках, повторите попытку");
        }
        else {
            QMessageBox::information(this, "Переработка сырья", "Операция выполнена успешно");
            this->updateBankPlayers();
            this->updatePlayers();
        }
        delete rec1;
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
    connect(upgr_fact, SIGNAL(clicked()), w, SLOT( upgradeFactSlot()));

    make_bid = new QPushButton;
    make_bid->setText("Сделать ставку");
    connect(make_bid, SIGNAL(clicked()), w, SLOT(auctionSlot()));

    produce = new QPushButton;
    produce->setText("Произвести");
    connect(produce, SIGNAL(clicked()), w, SLOT(produceSlot()));

    make_credit = new QPushButton;
    make_credit->setText("Кредит");
    connect(make_credit, SIGNAL(clicked()), w, SLOT(creditSlot()));

    insurance = new QPushButton;
    insurance->setText("Взять страховку");
    connect(insurance, SIGNAL(clicked()), w, SLOT(insuranceSlot()));

    give_up = new QPushButton;
    give_up->setText("Сдаться");
    connect(give_up, SIGNAL(clicked()), w, SLOT(giveUpSlot()));

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


    give_up->setStyleSheet("QPushButton{border: 2px solid #cfa055; border-radius:10px;padding : 0 8px; background-color: #d2a970; } QPushButton:hover {border: 2px solid #f44336; background-color: #f44336;} QPushButton:pressed { border: 2px solid #760d05; background-color: #9a0b00}");


    lay->addWidget(give_up, 0, 0, 1, 2);
    lay->addWidget(name, 1, 0, 1, 2);
    lay->addWidget(money, 2, 0);
    lay->addWidget(raw, 3, 0);
    lay->addWidget(product, 4, 0);
    lay->addWidget(def_facts, 5, 0);
    lay->addWidget(auto_facts, 6, 0);

    lay->addWidget(make_bid, 2, 1);
    lay->addWidget(make_credit, 3, 1);
    lay->addWidget(insurance, 4, 1);
    lay->addWidget(produce, 5, 1);
    lay->addWidget(upgr_fact, 6, 1);

    lay->addWidget(left_but, 7, 0);
    lay->addWidget(right_but, 7, 1);

    wid->setLayout(lay);

    QPixmap bkgnd("/Economical-Strategy/resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    wid->setPalette(palette);

    wid->setWindowIcon(QIcon("/Economical-Strategy/resources/logo.png"));

    wid->setWindowState(Qt::WindowFullScreen);

}

void PlayerInterface:: setPlayer(const Player& g1) {
    current_player = g1;
}

void PlayerInterface::updateData() {
    money->setText("Деньги: " + QString::number(current_player.getMoney()));
    raw->setText("Сырье: " + QString::number(current_player.getRaw()));
    product->setText("Готового сырья: " + QString::number(current_player.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(current_player.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(current_player.getAutoFacts().size()));
}

void PlayerInterface::show() {
    updateData();
    wid->show();
}

void PlayerInterface::close() {
    wid->close();
}

void PlayerInterface::hide(){
    raw->setText("Сырье:");
    product->setText("Готового сырья: ");
    def_facts->setText("Обычных фабрик: ");
    auto_facts->setText("Автоматических фабрик: ");
    money->setText("Деньги: ");
    qDebug()<<"hide";
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
    bet_dialog *bet = new bet_dialog( b1->getCurRawCount(),b1->getCurProdCount(), b1->getCurRawPrice(), b1->getCurProdPrice(), this);
    bet->show();
    if(bet->exec() == QDialog::Accepted) { //взятие кредита
        if (bet->getRawPrice() == -1 || bet->getProdPrice() == -1){
            QMessageBox::information(this, "Аукцион в банке", "Ошибка ввода данных, повторите попытку");
            delete bet;
        }
        else{
            int g = b1->add_offer(bet->getRawPrice(), bet->getProdPrice(), players[current_ind]);
            if (g == 1) {
                QMessageBox::information(this, "Аукцион в банке", "Ваше предложение было принято");
            }
            if (g == -1) {
                QMessageBox::information(this, "Аукцион в банке", "Ваше предложение было отклонено из-за недостаточных средств или кол-ва продукции на складе");
            }
            if(g == 0){
                QMessageBox::information(this, "Аукцион в банке", "Вы уже сделали предложение");
            }
            delete bet;
        }
    }

}
void MainWindow:: creditSlot() {
    credit_dialog *rec1 = new credit_dialog(this);
    rec1->show();
    if(rec1->exec() == QDialog::Accepted) { //взятие кредита
        if (rec1->getCredit() == -1) {
            QMessageBox::information(this, "Взятие кредита", "Ошибка ввода данных, повторите попытку");
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
        if(rec1->getLowCredit() != -4){
            if (rec1->getLowCredit() == -1) {
                QMessageBox::information(this, "Погашение кредита", "Ошибка ввода данных, повторите попытку");

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

            }
            delete rec1;
        }
        else{}
    }//взятие кредита, 0, если не смог купить, 1,если смог купить, -1 если уже взял кредит
}

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
        int b = b1->buyInsurance(players[current_ind], 300); // пока цена страховки триста
        if (b == 1) {
            QMessageBox::information(this, "Взятие страховки", "Вы взяли страховку");
        }
        if(b == 0){
            QMessageBox::information(this, "Взятие страховки", "Недостаточно средств для взятия страховки");
        }
        if (b == -1) {
            QMessageBox::information(this, "Взятие страховки", "Вы уже взяли страховку");
        }
    }
    players=b1->getAllPlayers();
    this->updatePlayers();
}




//---------------------------------GameOver-----------------------------------------

void MainWindow::giveUpSlot() {

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Сдаться", "Вы уверены, что хотите сдаться?", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::No) {

    }
    else {
        players[current_ind].setMoney(0);
        players[current_ind].setPriority(0);
        players[current_ind].setProduct(0);
        players[current_ind].setRaw(0);
        players[current_ind].setAutoFacts(QVector<AutoFactory>(0));
        players[current_ind].setDefFacts(QVector<DefFactory>(0));
        players[current_ind].setStatus("out");
        b1->setAllPlayers(players);
        updatePlayers();
    }

}



void MainWindow::checkGameOver()
{

    for (const auto &it: players){
        if (it.getMoney() >= 30000){
            QMessageBox::warning(this, "Игра окончена", "Игрок " + QString::number(it.getID()) + " победил");
            QApplication::quit();
        }
    }

    int alive_players = 0;
    for (const auto &it: players){
        if (it.getStatus() != "out")
            ++alive_players;
    }
    if (alive_players <= 1){
        // Т.е. играть некому
        for (const auto &it1: players){
            if (it1.getStatus() != "out")
            {
                // Ищем победителя
                QMessageBox::warning(this, "Игра окончена", "Игрок " + QString::number(it1.getID()) + " победил");
                QApplication::quit();
            }
        }
        QMessageBox::warning(this, "Игра окончена", "Победила дружба");
        QApplication::quit();
    }
}
