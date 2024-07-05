#ifndef BANK_H
#define BANK_H
#include <string>
#include <QVector>
#include <algorithm>
#include "player.h"

struct cred{
    Player pl;
    int balance;
    int duration;
    cred(Player p1, int Money): pl(p1), balance(Money){ duration = 1;} // кредит выдаётся на 3 месяца
};
struct offer{
    int raw;
    int prod;
    int ID;
    offer(int r, int p, int id) {
        raw = r; prod = p; ID = id;
    }
};

class Bank {
private:

    QVector<cred> credit_defaulters; // Игроки, просрочившие кредит
    QVector<Player> insured_players; // Игроки со страховкой
    QVector<Player> all;
    QVector<offer> cur_offers;
    int cur_raw_price;
    int cur_prod_price;
    int cur_raw_count;
    int cur_prod_count; // текущее предложение банка, поля инициализируются после выполнения функции pricing

public:

    Bank(QVector<Player> p) : all(p) {this->pricing();}

    void pricing();

    int auction(QVector<Player> players);

    void resetInsurance(); // сброс списка застраховавшихся

    int payCredit(Player& player, int money);

    int credit(Player& player, int money); //выдача кредита

    int buyInsurance(Player& player, int money); //покупка страховки

    QVector<Player> checkCredits(); //проверка списка должников

    //геттеры и сеттеры
    int  getCurRawPrice();
    int  getCurProdPrice();
    int  getCurRawCount();
    int getCurProdCount();
    void setCurRawPrice(int a);
    void setCurProdPrice(int a) ;
    void setRawCount(int a);
    void setProdCount(int a);

    int randomEvent();

    void setAllPlayers(QVector <Player>all);

    QVector <Player> getAllPlayers();

    void setCurrentOffers(QVector<offer> p);
    QVector<offer> getCurrentOffers(); //чисто формальные офферы

    int add_offer(int raw, int prod, Player pl);

    void setCreditDefaulters(const QVector<cred> defaulterList);

    void setInsuredPlayers(const QVector<Player> insuredList);

    QVector<cred> getCreditDefaulters();

    QVector<Player> getInsuredPlayers();

    QVector<Player> getInsurance(); // получение списка застраховавшихся в этом месяце

    bool checkGameOver();

};

#endif // BANK_H
