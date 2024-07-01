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
    cred(Player p1, int Money): pl(p1), balance(Money){ duration = 12;}
};

class Bank {
private:

    QVector<cred> credit_defaulters; // Игроки, просрочившие кредит
    QVector<Player> insured_players; // Игроки со страховкой
    QVector<Player> all;

public:

    Bank(QVector<Player> p) : all(p) {}

    void auction(QVector<Player> players, QVector<std::pair<int, int>> offers);

    void resetInsurance(); // сброс списка застраховавшихся

    int payCredit(Player player, int money);

    bool credit(Player player, int money); //выдача кредита

    bool buyInsurance(Player player); //покупка страховки должника

    QVector<Player> checkCredits(); //проверка списка должников

    //геттеры и сеттеры

    void setCreditDefaulters(const QVector<cred> defaulterList);

    void setInsuredPlayers(const QVector<Player> insuredList);

    QVector<cred> getCreditDefaulters();

    QVector<Player> getInsuredPlayers();

    QVector<Player> getInsurance(); // получение списка застраховавшихся в этом месяце

};

#endif // BANK_H
