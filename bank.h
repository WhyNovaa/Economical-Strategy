#ifndef BANK_H
#define BANK_H
#include <string>
#include <vector>
#include <algorithm>
#include "player.h"

class Bank {
private:
    std::vector<Player*> credit_defaulters; // Игроки, просрочившие кредит
    std::vector<Player*> insured_players; // Игроки со страховкой
    std::vector<Player*> all;

public:

    void setCreditDefaulters(const std::vector<Player>& defaulterList) ;

    std::vector<Player*> getCreditDefaulters();

    void setInsuredPlayers(const std::vector<Player>& insuredList) ;

    std::vector<Player*> getInsuredPlayers();

    //геттеры и сеттеры

    Bank(std::vector<Player>& p) : players(p) {}

    void auction(std::vector<Player>& players, std::vector<std::pair<int, int>> offers);

    bool credit(Player& player, int money); //выдача кредита

    std::vector<Player*> checkCredits(); //проверка списка должников

    bool buyInsurance(Player& player); //покупка страховки должника

    std::vector<Player*> getInsurance(); // получение списка застраховавшихся в этом месяце

    void resetInsurance(); // сброс списка застраховавшихся
};

#endif // BANK_H
