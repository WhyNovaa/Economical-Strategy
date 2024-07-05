#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"password_menu.h"
#include "month_end_dialog.h"
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
    font.setPointSize(12);
    font.setBold(true);
    start_button->setFont(font);

    start_label->setText("Введите количество игроков");
    start_label->setFont(font);

    start_spinBoxButton->setFont(font);
    start_spinBoxButton->setMinimum(2);
    start_spinBoxButton->setMaximum(6);

    start_button->setStyleSheet("QPushButton { background-color: #5ac47f; color: black; border: 2px solid gray } QPushButton:hover {background-color: #398453} QPushButton:pressed {background-color: #1c4f2e}");

    start_grid->addWidget(start_label, 0, 0);
    start_grid->addWidget(start_spinBoxButton, 0, 1);
    start_grid->addWidget(start_button, 1, 0, 1, 2);

    start_widget->setLayout(start_grid);
    start_widget->setFixedSize(400, 200);


    QPixmap bkgnd(":resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    start_widget->setPalette(palette);

    start_widget->setWindowIcon(QIcon(":resources/logo.png"));

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

        QPixmap bkgnd(":resources/bg.jpg");
        bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
        QPalette palette;
        palette.setBrush(QPalette::Window, bkgnd);
        pm->setPalette(palette);

        pm->setWindowIcon(QIcon(":resources/logo.png"));

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
        if (players[current_ind].getFinishStatus() == true){
            players_interface[current_ind]->hide_out();
        }
        else{
            players_interface[current_ind]->anti_hide();
        }
        flag = -1;
    }
}

void MainWindow::onDialogClosed(QString word)
{
    if(word == "closed"){
        players_interface[current_ind]->anti_hide();
    }
}

void MainWindow::rightButtonClicked() {
    //здесь реализация перехода к другому интерфейсу вправо

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);
    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    players_interface[current_ind]->hide();
    qDebug() << "right";
    check = "right";

    if((current_ind+1 <= players.size()-1 && players[current_ind+1].getStatus()=="in") || (current_ind+1>players.size()-1 && players[0].getStatus()=="in")){
        pch = new pass_check(this);
        pch -> show();
        pch->activateWindow();
        connect(pch, &pass_check::dialogClosed, this, &MainWindow::onDialogClosed);

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
        //players_interface[current_ind]->anti_hide();
        players_interface[current_ind]->show();
        players_interface[current_ind]->hide_out();
    }


    checkGameOver();
}

void MainWindow::leftButtonClicked() {
    //здесь реализация перехода к другому интерфейсу влево

    // PlayerInterface temp();
    // PlayerInterface::setLeftBtnEn(false);
    // PlayerInterface::setRightBtnEn(false);

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    qDebug() << "left";
    check = "left";

    players_interface[current_ind]->hide();
    if((current_ind-1>=0 && players[current_ind-1].getStatus()=="in") || (current_ind-1<0 && players[players.size()-1].getStatus()=="in")){

    pch = new pass_check(this);
    pch -> show();
    pch->activateWindow();
    connect(pch, &pass_check::dialogClosed, this, &MainWindow::onDialogClosed);

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
       // players_interface[current_ind]->anti_hide();
        players_interface[current_ind]->hide_out();
        players_interface[current_ind]->show();
    }

    checkGameOver();
}

void MainWindow::upgradeFactSlot() {

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }


    fabric_dialog *rec1 = new fabric_dialog(this);
    rec1->show();
    if(rec1->exec() == QDialog::Accepted) {
        int amount = rec1->getAmount();
        if (amount == 0) {
            QMessageBox::information(this, "Улучшение фабрик", "Ошибка ввода данных, повторите попытку");
        } else if(amount > players[current_ind].getDefFacts().size()) {
            QMessageBox::information(this, "Улучшение фабрик", "Недостаточно фабрик, повторите попытку");
        }
        else if(amount * 3000 / 2 > players[current_ind].getMoney()) {
            QMessageBox::information(this, "Улучшение фабрик", "Недостаточно средств, повторите попытку");
        }
        else {
            if (players[current_ind].getDefFacts().size() > 0){
                players[current_ind].upgradeFacts(amount);

                QMessageBox::information(this, "Улучшение фабрик", "Операция выполнена успешно");

                this->updateBankPlayers();
                this->updatePlayers();
            }else
            QMessageBox::information(this, "Улучшение фабрик", "Нечего улучшать");
        }
        delete rec1;
    }
}


void MainWindow::produceSlot() {

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }


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
        int res = players[current_ind].putRawInFabrics(amount);
        if(res == -1) {
            QMessageBox::information(this, "Переработка сырья", "Недостаточно места на фабриках, повторите попытку");
        }
        else if(res == -2) {
            QMessageBox::information(this, "Переработка сырья", "Недостаточно денег, повторите попытку");
        }
        else {
            QMessageBox::information(this, "Переработка сырья", "Операция выполнена успешно");
            this->updateBankPlayers();
            this->updatePlayers();
        }
        delete rec1;
    }
}

void MainWindow::checkNextMonth() {
    bool not_fin = false;
    for (const auto& i: players){
        if (!(i.getStatus() == "out" or i.getFinishStatus() == true)) {
            not_fin = true;
        }
    }

    if (!not_fin) {
        month++;
        QString s = "";
        QVector <Player>v1= b1->checkCredits();
        if (!v1.empty()) {
            for(auto &x: v1) {
                s += QString::number(x.getID()) + " ";
            }
        }
        int winner = b1->auction(players);
        int rand_code = b1->randomEvent();
        month_end_dialog *m1 = new month_end_dialog(this);
        if (winner != 0) {
            m1->setAuction(" игрок " + QString::number(winner));
        }
        else {
            m1->setAuction(" никто не поучаствовал в аукционе");
        }
        m1->setCredit("игроки " + s);
        m1->setRandomEvent("-");
        if(m1->exec() == QDialog::Accepted) {
            delete m1;
            players = b1->getAllPlayers();
            for (auto& i: players){
                i.roundUpdate();
            }
            this->updateBankPlayers();
            b1->resetInsurance();
            b1->pricing(); // у нас передаётся приоритет по кругу?
            // если игрок не смог выплатить аренду, то его кикает из игры?
            //если игрок в конце хода получил статус out, его интерфйес отключается на следущем ходе? что за это отвечает?
        }
    }
}

int MainWindow::month = 1;


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
    current_month = new QLabel;

    left_but->setText("<-");
    right_but->setText("->");
    current_month->setText("Месяц: " + QString::number(MainWindow::month));

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
    current_month->setAlignment(Qt::AlignCenter);


    money->setFont(font);
    raw->setFont(font);
    product->setFont(font);
    def_facts->setFont(font);
    auto_facts->setFont(font);

    current_month->setFont(font);

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

    info_butt  = new QPushButton;
    info_butt->setText("Информация");
    connect(info_butt, SIGNAL(clicked()), w, SLOT(createTableSlot()));

    finish_turn  = new QPushButton;
    finish_turn->setText("Закончить ход");
    connect(finish_turn, SIGNAL(clicked()), w, SLOT(finishTurnSlot()));

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

    font = finish_turn->font();
    font.setPointSize(14);
    font.setBold(true);

    finish_turn->setFont(font);
    give_up->setFont(font);
    left_but->setFont(font);
    right_but->setFont(font);
    info_butt->setFont(font);

    give_up->setStyleSheet("QPushButton{border: 2px solid #cfa055; border-radius:10px;padding : 0 8px; background-color: #d2a970; } QPushButton:hover {border: 2px solid #f44336; background-color: #f44336;} QPushButton:pressed { border: 2px solid #760d05; background-color: #9a0b00}");
    finish_turn->setStyleSheet("QPushButton{border: 2px solid #cfa055; border-radius:10px;padding : 0 8px; background-color: #d2a970; } QPushButton:hover {border: 2px solid #f44336; background-color: #f44336;} QPushButton:pressed { border: 2px solid #760d05; background-color: #9a0b00}");

    lay->addWidget(name, 0, 0, 1, 3);
    lay->addWidget(current_month, 0, 3, 1, 1);

    lay->addWidget(money, 1, 0, 1, 2);
    lay->addWidget(make_bid, 1, 2, 1, 2);

    lay->addWidget(raw, 2, 0, 1, 2);
    lay->addWidget(make_credit, 2, 2, 1, 2);

    lay->addWidget(product, 3, 0, 1, 2);
    lay->addWidget(insurance, 3, 2, 1, 2);

    lay->addWidget(def_facts, 4, 0, 1, 2);
    lay->addWidget(produce, 4, 2, 1, 2);

    lay->addWidget(auto_facts, 5, 0, 1, 2);
    lay->addWidget(upgr_fact, 5, 2, 1, 2);

    lay->addWidget(info_butt, 6, 0, 1, 4);

    lay->addWidget(left_but, 7, 0, 1, 1);
    lay->addWidget(finish_turn, 7, 1, 1, 1);
    lay->addWidget(give_up, 7, 2, 1, 1);
    lay->addWidget(right_but, 7, 3, 1, 1);


    wid->setLayout(lay);

    QPixmap bkgnd(":resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    wid->setPalette(palette);

    wid->setWindowIcon(QIcon(":resources/logo.png"));

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
    current_month->setText("Месяц " + QString::number(MainWindow::month));
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
    right_but->setEnabled(false);
    left_but->setEnabled(false);
    upgr_fact->setEnabled(false);
    make_bid->setEnabled(false);
    produce->setEnabled(false);
    make_credit->setEnabled(false);
    insurance->setEnabled(false);
    info_butt->setEnabled(false);
    give_up->setEnabled(false);
    finish_turn->setEnabled(false);
}

void PlayerInterface::hide_out(){
    money->setText("Деньги: " + QString::number(current_player.getMoney()));
    raw->setText("Сырье: " + QString::number(current_player.getRaw()));
    product->setText("Готового сырья: " + QString::number(current_player.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(current_player.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(current_player.getAutoFacts().size()));
    right_but->setEnabled(true);
    left_but->setEnabled(true);
    upgr_fact->setEnabled(false);
    make_bid->setEnabled(false);
    produce->setEnabled(false);
    make_credit->setEnabled(false);
    insurance->setEnabled(false);
    info_butt->setEnabled(false);
    give_up->setEnabled(false);
    finish_turn->setEnabled(false);
}

void PlayerInterface::anti_hide(){
    money->setText("Деньги: " + QString::number(current_player.getMoney()));
    raw->setText("Сырье: " + QString::number(current_player.getRaw()));
    product->setText("Готового сырья: " + QString::number(current_player.getProduct()));
    def_facts->setText("Обычных фабрик: " + QString::number(current_player.getDefFacts().size()));
    auto_facts->setText("Автоматических фабрик: " + QString::number(current_player.getAutoFacts().size()));
    right_but->setEnabled(true);
    left_but->setEnabled(true);
    upgr_fact->setEnabled(true);
    make_bid->setEnabled(true);
    produce->setEnabled(true);
    make_credit->setEnabled(true);
    insurance->setEnabled(true);
    info_butt->setEnabled(true);
    give_up->setEnabled(true);
    finish_turn->setEnabled(true);
}

PlayerInterface::~PlayerInterface() {
    delete wid;
    delete lay;
    delete money;
    delete raw;
    delete product;
    delete def_facts;
    delete auto_facts;
    delete make_bid;
    delete produce;
    delete upgr_fact;
    delete make_credit;
    delete insurance;
    delete give_up;
    delete info_butt;

}

void PlayerInterface::setLeftBtnEn(bool b) {
    left_but->setEnabled(b);
}

void PlayerInterface::setRightBtnEn(bool b) {
    right_but->setEnabled(b);
}

void MainWindow::createTableSlot(){ // Берет инфу из players; любое его обновление сулит обновление таблицы

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }


    static QTableWidget tableWidget(players.size(), 5);
     tableWidget.setFixedSize(645, 300);
     tableWidget.setWindowTitle("Таблица с информацией");

     QStringList headers;
     headers << "Деньги" << "Сырье" << "Продукт" << "ОФ" << "АФ";
     tableWidget.setHorizontalHeaderLabels(headers);

     for (const auto &it: players){
         tableWidget.setItem(it.getID()-1, 0, new QTableWidgetItem(QString::number(it.getMoney())));
         tableWidget.setItem(it.getID()-1, 1,new QTableWidgetItem(QString::number(it.getRaw())));
         tableWidget.setItem(it.getID()-1, 2, new QTableWidgetItem(QString::number(it.getProduct())));
         tableWidget.setItem(it.getID()-1, 3, new QTableWidgetItem(QString::number(it.getDefFacts().size())));
         tableWidget.setItem(it.getID()-1, 4, new QTableWidgetItem(QString::number(it.getAutoFacts().size())));
     }

     tableWidget.show();
     tableWidget.setWindowFlag(Qt::WindowStaysOnTopHint);

     tableWidget.setWindowIcon(QIcon(":resources/logo.png"));
}

//<----------------------------------Bank---------------------------------------------->
void MainWindow:: auctionSlot() {

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    this->updateBankPlayers();
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
            if (g == -2) {
                QMessageBox::information(this, "Аукцион в банке", "Вы не можете принять участие в аукционе из-за недостаточного кол-ва продукции");
            }
            players=b1->getAllPlayers();
            this->updatePlayers();
            delete bet;
        }
    }

}
void MainWindow:: creditSlot() {

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    this->updateBankPlayers();
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

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    QPixmap bkgnd(":resources/bg.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);

    this->setPalette(palette);
    this->setWindowIcon(QIcon(":resources/logo.png"));

    this->setFixedSize(400, 200);
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Взятие страховки", "Стоимость страховки на следующий ход -- 300. Взятие страховки уберегает вас от негативных эффектов событий. Хотите оформить страховку?", QMessageBox::Yes | QMessageBox::No);
    this->updateBankPlayers();
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

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

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
        players_interface[current_ind]->hide_out();
        checkNextMonth();
        updatePlayers();
    }
}

void MainWindow::finishTurnSlot() {

    for(int i=0; i<players.size(); i++){
        if((money_backup[i]!=players[i].getMoney()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((raw_backup[i]!=players[i].getRaw()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((product_backup[i]!=players[i].getProduct()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((def_backup[i]!=players[i].getDefFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }
    for(int i=0; i<players.size(); i++){
        if((auto_backup[i]!=players[i].getAutoFacts().size()*session_key)){
            QMessageBox::warning(this, "warning", "Игроки пльзуются читами");
            players[i].setStatus("out");
            players_interface[current_ind]->hide_out();
            updatePlayers();
        }
    }

    players[current_ind].setFinishStatus(true);
    players_interface[current_ind]->hide_out();
    checkNextMonth();
    updatePlayers();
}



void MainWindow::checkGameOver()
{

    for (const auto &it: players){
        if (it.getMoney() >= 30000 && it.getStatus()=="in"){
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
