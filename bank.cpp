#include "bank.h"
#include "QDebug"
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

bool Bank::checkGameOver()
{
    return all.size() == 1;
}
int Bank:: getCurRawPrice(){
    return cur_raw_price;
}
int Bank:: getCurProdPrice(){
    return cur_prod_price;
}
int Bank:: getCurRawCount(){
    return cur_raw_count;
}
int Bank:: getCurProdCount(){
    return cur_prod_count;
}
void Bank:: setCurRawPrice(int a) {
    cur_raw_price = a;
}
void Bank:: setCurProdPrice(int a) {
    cur_prod_price = a;
}
void Bank::setRawCount(int a){
    cur_raw_count = a;
}
void Bank::setProdCount(int a){
    cur_prod_count = a;
}

void Bank:: setAllPlayers(QVector <Player>all1) {
    all = all1;
}

QVector <Player> Bank:: getAllPlayers(){
    return all;
}
// получение qvector -- списка застраховавшихся в этом месяце
//геттеры и сеттеры


int Bank::auction(QVector<Player> players, QVector<std::pair<int, int>> offers) {
    QVector<double> coefficents;
    for (auto& i : players) {
        int auto_in_money = 0;
        int def_in_money = 0;
        int cash = i.getMoney();
        int raw_in_money = i.getRaw();
        int product_in_money = i.getProduct();
        double kef = 0;

        for (auto& j : i.getAutoFacts()) {
            auto_in_money++;
        }

        for (auto& j : i.getDefFacts()) {
            def_in_money++;
        }

        kef += auto_in_money * 5000 + def_in_money * 1000 + cash + raw_in_money * 50 + product_in_money * 100;
        kef /= 10000;
        coefficents.push_back(kef);
    }


    QVector<int> margin;
    for (size_t i = 0; i < offers.size(); ++i) {
        margin.push_back((abs(offers[i].first - cur_raw_price)) + abs(offers[i].second - cur_prod_price));  // маржа игрока
    }

    for (int i =0; i < margin.size();i++) {
        margin[i] /=coefficents[i];
    }

    int max = -10000, max_ind = -1;

    for (int i =0; i < margin.size();i++) {
        if (margin[i] > max) {
            max = margin[i];
            max_ind = i;
        }
    }
    return max_ind;
    // и что должна  возвращать функция?
}//нужно прописать момент с балансом цен через просмотр баланса цен либо через понижение цены на условно 20%

void Bank::pricing() {
    int average = 0;
    int amount_of_raw = 0;
    int amount_of_product = 0;
    const int max_of_amount_of_raw = 6;
    const int max_of_amount_of_product = 4;
    srand(time(NULL));

    for (auto& i : all) {
        average += i.getMoney();
    }

    int high_border = average / all.size() / 100;   // это будет максимальная граница цены за сырьё И минимальная граница цены за продукт
    int low_border = high_border / 2; // минимальная граница цены за сырьё
    int high_border_of_product = 2 * high_border; // максимальная граница цены за продукт

    int price_of_raw = rand() % low_border + high_border;  // итоговая цена за сырьё
    int price_of_product = rand() % high_border + high_border_of_product;  // итоговая цена за продукт
    cur_raw_price = price_of_raw;
    cur_prod_price = price_of_product;

    for (auto& i : all) {
        amount_of_raw += i.getRaw();
    }

    if (amount_of_raw / all.size() < max_of_amount_of_raw) {
        amount_of_raw = max_of_amount_of_raw - amount_of_raw / all.size(); // количество сырья, которое предлагает банк
    }
    else {
        amount_of_raw = 1;
    }
    cur_raw_count = amount_of_raw;
    for (auto& i : all) {
        amount_of_product += i.getProduct();
    }

    if (amount_of_product / all.size() < max_of_amount_of_product) {
        amount_of_product = amount_of_product / all.size(); // количество продукта, которое хочет забрать банк
    }
    else {
        amount_of_product = 4;
    }
    cur_prod_count = amount_of_product;
}

int Bank:: credit(Player& player, int money) {
    for (const auto &it: credit_defaulters) {
        if (it.pl.getID() == player.getID()) {
            return -1;
        }
    }
    if (player.getMoney() < money) {
         return 0;
    }
    else{
        for(auto &x: all) {
            if(x.getID() == player.getID()) {
                qDebug() << "found";
                x.setMoney(player.getMoney() - money);
            }
        }
        cred p1(player, money);
        credit_defaulters.push_back(p1);
        return 1;
    }
}//взятие кредита, 0, если не смог купить, 1,если смог купить, -1 если уже взял кредит

QVector<Player> Bank:: checkCredits() {
    QVector <Player>v1;
    for ( auto &it: credit_defaulters) {
        it.duration--;
        if (it.duration == 0) {
            v1.push_back(it.pl);
        }
    }
    return v1;
} //проверка списка должников, возвращает Qvector просрочивших оплату кредита

int Bank::payCredit(Player& player, int money){
    if (player.getMoney() < money) {
        return -1;
    }   //функция возвращает в mainwindow -1, если игрок не может оплатить кредит, -2 если он не брал его
    // 0, если погасил кредит, и положительное число если оплатил и не погасил, оно равно оставшейся сумме кредита
    else{
        bool found = 0;
        for (auto &it: credit_defaulters) {
            if (it.pl.getID() == player.getID()) {
                found = 1; // 1, если игрок есть в списке взявших кредит и 0 если его там нет
            }
        }
        if (!found) {
            return -2;
        }
        for(auto &x: all) {
            if(x.getID() == player.getID()) {
                qDebug() << "found";
                x.setMoney(player.getMoney() - money);
            }
        }
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
                    for(auto &x: all) {
                        if(x.getID() == player.getID()) {
                            qDebug() << "found";
                            x.setMoney(player.getMoney() - it.balance);
                        }
                    }
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
        for(auto &x: all) {
            if(x.getID() == player.getID()) {
                qDebug() << "found";
                x.setMoney(player.getMoney() - money);
                insured_players.push_back(x);
                return 1;
            }
        }
    }
} //покупка страховки, 0, если не смог купить, 1,если смог купить


void Bank:: resetInsurance(){
    insured_players.clear();
} // сброс списка застраховавшихся в начале месяца или после ивента
