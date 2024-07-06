#include "bank.h"
#include "QDebug"
void Bank:: setCreditDefaulters(const QVector<cred> defaulterList) {
    credit_defaulters = defaulterList;
}
bool comp(cred c1, cred c2) {
    return c1.balance > c2.balance;
}
bool id_comp_offers(offer c1, offer c2) {
    return c1.ID < c2.ID;
}
bool id_comp_players(Player p1, Player p2) {
    return p1.getID()< p2.getID();
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


void Bank::setCurrentOffers(QVector<offer> p) {
    cur_offers = p;
}
QVector<offer> Bank::getCurrentOffers() {
    return cur_offers;
}

int Bank:: add_offer(int raw, int prod, Player pl) {
    if (pl.getProduct()< cur_prod_count) {
        return -2;
    }
    if ( pl.getMoney()< raw) {
        return -1;
    }
    else{
        if (cur_offers.empty())
        {
            offer o(raw, prod, pl.getID());
            cur_offers.push_back(o);
            for(auto &x: all) {
                if(x.getID() == pl.getID()) {
                    qDebug() << "found";
                    x.setMoney(pl.getMoney() - raw);
                    x.setProduct(pl.getProduct() - cur_prod_count);
                } // снисаем с игрока деньги и продукцию, если его предложение будет принято, то ему накинут денег за продукцию
            }
            return 1;
        }
        for (auto &x: cur_offers) {
            if (x.ID == pl.getID()){
                return 0;
            }
        }
        offer o(raw, prod, pl.getID());
        cur_offers.push_back(o);
        for(auto &x: all) {
            if(x.getID() == pl.getID()) {
                qDebug() << "found";
                x.setMoney(pl.getMoney() - raw);
                x.setProduct(pl.getProduct() - cur_prod_count);
            }
        }
        return 1; // 1 если предложения не было в офферах и 0 если было, -1 если не может сделать предложения из-за
        //недостаточного кол-ва продукции или денег // -2 если впринципе не может поучавствовать
    }
}

int Bank::auction(QVector<Player> players) {
    players = all; //костыль
    int a;
    bool b1 = 1;
    offer o(1,-1,-1);
    for(int i =0; i < players.size(); i++) {
        a = players[i].getID();
        b1 = 1;
        for (int j = 0 ; j < cur_offers.size();j++) {
            if (cur_offers[j].ID == a) {
                if (players[i].getStatus() == "out") //?
                {
                    qDebug() << "works";
                    cur_offers[j].prod = 0; cur_offers[j].raw = 0;
                } //обнуляем предложение выбывшего игрока
                b1 = 0;
                break;
            }
        }
        if (b1) {
            o.ID = a; o.prod = 0; o.raw =0;
            cur_offers.push_back(o);
        }
    } // добавляем в вектор предложений предложения игроков которые не смогли или не захотели участвовать или обанкротились
    // в них предложения равны 0;

    qDebug()<< (cur_offers.size()== players.size());
    QVector<double> coefficents;
    for (auto& i : players) {
        if (i.getStatus() != "out") {
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
        else {
            coefficents.push_back(1); // если игрок банкрот, его кэф равен 1
        }
    }

    std::sort(cur_offers.begin(), cur_offers.end(), id_comp_offers); //синхронизируем вектор предложений с ветором игроков
    QVector<int> margin;
    for(int i = 0; i < cur_offers.size();i++) {
        qDebug() << i + 1 << " " << cur_offers[i].prod << " " << cur_offers[i].raw << "/";
    }
    for (int i = 0; i < cur_offers.size(); i++) {
        if (cur_offers[i].prod && cur_offers[i].raw) {
        margin.push_back((cur_offers[i].raw - cur_raw_price ) + (cur_prod_price - cur_offers[i].prod));  // маржа игрока
        }
        if (!cur_offers[i].prod && !cur_offers[i].raw) {
            margin.push_back(0);
        }
    }

    for (int i =0; i < margin.size();i++) {
        margin[i] /=coefficents[i];
    }

    int max = -10000, max_ind = -1;

    QVector <Player> top_offers;
    QVector <int> top_offers_ids;
    for (int i =0; i < margin.size();i++) {
        if (margin[i] > max) {
            max = margin[i];
            max_ind = i + 1; // ищем максимально выгодное предложение
        }
    }
    for (int i = 0 ; i < margin.size(); i++) {
        if (margin[i] == max) {
            top_offers.push_back(players[i]);
        }
    }               // ищем айди игроков с таким же предолжением
    for (int i = 0; i < top_offers.size(); i ++) {
        if (top_offers[i].getPriority() == 1) {
            max_ind = top_offers[i].getID();
        }
    }       // нашли победителя и записали его id
    if (max == 0) {
        return 0;
    }
    for (int i =0; i < players.size(); i ++) {
        if(players[i].getID() != max_ind) {
            players[i].setMoney(players[i].getMoney() + cur_offers[i].raw);
            players[i].setProduct(players[i].getProduct() + cur_prod_count);
            // возвращаем деньги ипродукцию, которые банк взял под залог
        }
        else {
            players[i].setMoney(players[i].getMoney() + cur_offers[i].prod); // начисляем деньши за продажу прода победителю
            players[i].setRaw(players[i].getRaw() + cur_raw_count);
        }
    }
    all = players;
    qDebug() << max_ind << " " << max ;
    for(int i = 0; i < margin.size();i++) {
        qDebug() << i + 1 << " " << margin[i] << "/";
    }
    cur_offers.clear();
    return max_ind;
            //.!!! осталось синхронизировать полученый вектор  с вектором в мейнвиндоу
    // top_offers_ids.push_back( cur_offers[max_ind].ID);
    // for (int i =0; i < margin.size();i++) {
    //     if (margin[i] == max) {
    //         top_offers_ids.push_back(cur_offers[i].ID);
    //     }
    // } // ищем айди игроков с таким же предолжением

    // for (int i =0 ; i < top_offers_ids.size(); i ++) {
    //     for(int j = 0; j < players.size(); j++) {
    //         if (top_offers_ids[i] == top_offers[j].getID()) {
    //             top_offers.push_back(top_offers[j]);
    //         }
    //     }
    // }
    // Player winner;
    // for (int i = 0; i < top_offers.size();i++) {
    //     if (top_offers[i].getPriority() == 1) {
    //         winner = top_offers[i];
    //     } // ищем победителя по приоритету
    // }
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
                x.setMoney(player.getMoney() + money);
            }
        }
        cred p1(player, money * 1.1);
        credit_defaulters.push_back(p1);
        return 1;
    }
}//взятие кредита, 0, если не смог купить, 1,если смог купить, -1 если уже взял кредит

QVector <Player> Bank:: checkCredits() {
    QVector <Player> v;
    for ( auto &it: credit_defaulters) {
        it.duration--;
        if (it.duration == 0) {
            for(int i = 0; i < all.size();i++){
                if(all[i].getID() == it.pl.getID()) {
                    all[i].setStatus("out");
                    v.push_back(all[i]);
                }
            }
        }
    }
    return v;
} //Ставим статус out всем игрокам просрочившим кредит нужно прописать

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
                    std::sort(credit_defaulters.begin(), credit_defaulters.end(), comp); // запись кредита с нулевым значением будет в конце
                    credit_defaulters.pop_back();
                    return 0;
                }
                if(it.balance < 0) {
                    for(auto &x: all) {
                        if(x.getID() == player.getID()) {
                            qDebug() << "found";
                            x.setMoney(x.getMoney() - it.balance);
                        }
                    }// вернули избыточные деньги
                    std::sort(credit_defaulters.begin(), credit_defaulters.end(), comp); // запись кредита с нулевым значением будет в конце
                    credit_defaulters.pop_back();
                    return 0;
                }
            }
        }
    }
}

int Bank:: buyInsurance(Player& player, int money) {
    if (player.getMoney() < money) {
        return 0;
    }
    else{
        for (auto &x: insured_players) {
            if(x.getID() == player.getID()) {
                qDebug() << "found";
                return -1;
            }
        }
        for(auto &x: all) {
            if(x.getID() == player.getID()) {
                qDebug() << "found";
                x.setMoney(player.getMoney() - money);
                insured_players.push_back(x);
                return 1;
            }
        }
    }
} //покупка страховки, 0, если не смог купить, 1,если смог купить, -1 если уже взял


void Bank:: resetInsurance(){
    insured_players.clear();
} // сброс списка застраховавшихся в начале месяца или после ивента


int Bank::randomEvent() {
    int random = rand() % 11;

    switch (random) {

    case 1: {
        setProdCount(getCurProdCount() / 2);
        setRawCount(getCurRawCount() / 2);
        setCurProdPrice(getCurProdPrice() / 2);
        setCurRawPrice(getCurRawPrice() * 2);

        for (auto& i : all) {
            bool has_an_insurance = 0;

            for (auto& k : insured_players) {

                if (i.getID() == k.getID()) {
                    has_an_insurance = 1;
                }
            }
            int duty = 500;
            if (i.getStatus() != "out" || !has_an_insurance) {

                if (i.getMoney() >= 500) {
                    i.setMoney(i.getMoney() - 500);
                }

                else if (i.getProduct() != 0 || i.getRaw() != 0) {
                    duty -= i.getMoney();
                    i.setMoney(0);

                    while (i.getRaw() != 0 && duty > 0) {
                        i.setRaw(i.getRaw() - 1);
                        duty -= 50;
                    }

                    if (duty > 0) {

                        while (i.getProduct() != 0 && duty > 0) {
                            i.setProduct(i.getProduct() - 1);
                            duty -= 100;
                        }
                    }
                    // отсюда делать скриншот для отчета
                    else {
                        int res_2 = 0;
                        res_2 += abs(duty);
                        i.setMoney(res_2);
                    }
                }
            }
        }
        return 1;
    }

    case 2: {
        int random_for_fabric = rand() % all.size() + 1;
        for (auto& i : all) {
            bool has_an_insurance = 0;

            for (auto& k : insured_players) {

                if (i.getID() == k.getID()) {
                    has_an_insurance = 1;
                }
            }

            if (i.getID() == random_for_fabric && !has_an_insurance) {

                if (i.getStatus() != "out") {
                    if (!i.getAutoFacts().empty() || !i.getDefFacts().empty()) {
                        i.deleteFabrics(rand() % (i.getAutoFacts().size() + i.getDefFacts().size()));
                    }
                    else {
                        i.setMoney(i.getMoney() - 500);
                    }
                }
            }
        }
        return 2;
    }

    case 3: {
        int random_for_birthday = rand() % all.size() + 1;
        for (auto& i : all) {
            bool has_an_insurance = 0;

            for (auto& k : insured_players) {

                if (i.getID() == k.getID()) {
                    has_an_insurance = 1;
                }
            }
            if (i.getStatus() != "out") {

                if (i.getID() == random_for_birthday) {
                    i.setMoney(i.getMoney() + 100 * (all.size() - 1));    // прибавляются 100 баксов от каждого игрока

                    if (has_an_insurance) {
                        i.setMoney(i.getMoney() - 100 * (insured_players.size() - 1));

                    if (i.getID() == random_for_birthday) {
                        i.setMoney(i.getMoney() + 100 * (real_all_size - 1));    // прибавляются 100 баксов от каждого игрока
                        if (has_an_insurance) {
                            i.setMoney(i.getMoney() - 100 * (insured_players.size() - 1));
                            if (i.getMoney() < 0) { //?????????Вопрос по логике выхода из игры????????????????
                                i.setStatus("out");
                            }
                        }

                        else {
                            i.setMoney(i.getMoney() - 100 * insured_players.size());
                            if (i.getMoney() < 0){ //?????????Вопрос по логике выхода из игры????????????????
                                i.setStatus("out");
                            }
                        }

                    }

                    else {
                        i.setMoney(i.getMoney() - 100 * insured_players.size());
                    }
                }
                else {
                    if (!has_an_insurance) {
                        i.setMoney(i.getMoney() - 100);
                    }
                }
            }
        }
        return 3;
    }
    case 4: {     // реализация наследства (оч сложно)


        return 4;
    }
    default: {
        return 0;
    }
    }
    return 0;
}

