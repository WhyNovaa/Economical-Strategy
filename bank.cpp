#include "bank.h"

void Bank:: setCreditDefaulters(const QVector<cred> defaulterList) {
    credit_defaulters = defaulterList;
}

QVector<cred> Bank:: getCreditDefaulters() {
    return credit_defaulters;
}

void Bank:: setInsuredPlayers(const QVector<Player> insuredList) {
    insured_players = insuredList;
}

QVector<Player>Bank:: getInsuredPlayers() {
    return insured_players;
}
// получение qvector -- списка застраховавшихся в этом месяце
//геттеры и сеттеры


void Bank::auction(QVector<Player> players, QVector<std::pair<int, int>> offers) {

}//нужно прописать момент с балансом цен через просмотр баланса цен либо через понижение цены на условно 20%

bool Bank:: credit(Player& player, int money) {
    for (const auto &it: credit_defaulters) {
        if (it.pl.getID() == player.getID()) {
            return -1;
        }
    }
    if (player.getMoney() < money) {
         return 0;
    }
    else{
        player.setMoney(player.getMoney() - money);
        cred p1(player, money);
        credit_defaulters.push_back(p1);
        return 1;
    }
}//взятие кредита, 0, если не смог купить, 1,если смог купить, -1 если уже взял кредит

QVector<Player> Bank:: checkCredits() {
    QVector <Player>v1;
    for (const auto &it: credit_defaulters) {
        if (it.duration == 0) {
            v1.push_back(it.pl);
        }
    }
    return v1;
} //проверка списка должников, возвращает Qvector просрочивших оплату кредита

int Bank::payCredit(Player& player, int money){
    if (player.getMoney() < money) {
        return -1;
    }   //функция возвращает в mainwindow -1, если игрок не может оплатить кредит
    // 0, если погасил кредит, и положительное число если оплатил и не погасил, оно равно оставшейся сумме кредита
    else{
        player.setMoney(player.getMoney() - money);
        for (auto &it: credit_defaulters) {
            if (it.pl.getID() == player.getID()) {
                it.balance -= money;
                if (it.balance > 0){
                    return it.balance;
                }
                if(it.balance == 0) {
                    return 0;
                }
                if(it.balance < 0) {
                    player.setMoney(player.getMoney() - it.balance);
                    return 0;
                }
            }
        }
    }
}

bool Bank:: buyInsurance(Player& player, int money) {
    if (player.getMoney() < money) {
        return 0;
    }
    else{
        player.setMoney(player.getMoney() - money);
        insured_players.push_back(player);
        return 1;
    }
} //покупка страховки, 0, если не смог купить, 1,если смог купить


void Bank:: resetInsurance(){
    insured_players.clear();
} // сброс списка застраховавшихся в начале месяца или после ивента
