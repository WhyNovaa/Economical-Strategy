#include "player.h"
#include <qDebug>
int Player::next_ID = 1;
Player::Player() : ID(next_ID++), priority(false), money(10000), raw(2), product(2),
    def_facts(QVector<DefFactory>(2)), auto_facts(QVector<AutoFactory>(2)), status("in"), finish_status(false) {}
Player::Player(const bool& _priority, const int& _money, const int& _raw, const int& _product,
               const QVector<DefFactory>& _def_facts, const QVector<AutoFactory>& _auto_facts, const QString& _status):
    ID(next_ID++), priority(_priority), money(_money), raw(_raw), product(_product), def_facts(_def_facts),
    auto_facts(_auto_facts), status(_status), finish_status(false) {}

void Player::setPriority(const bool& _priority) { priority = _priority; }
void Player::setMoney(const int& _money) { money = _money; }
void Player::setRaw(const int& _raw) { raw = _raw; }
void Player::setProduct(const int& _product) { product = _product; }
void Player::setDefFacts(const QVector<DefFactory>& _def_facts) { def_facts = _def_facts; }
void Player::setAutoFacts(const QVector<AutoFactory>& _auto_facts) { auto_facts = _auto_facts; }
void Player::setStatus(const QString& _status) { status = _status; }
void Player::setFinishStatus(const bool& _finish_status) { finish_status = _finish_status; }

int Player::getID() const { return ID; }
bool Player::getPriority() const { return priority; }
int Player::getMoney() const { return money; }
int Player::getRaw() const { return raw; }
int Player::getProduct() const { return product; }
QVector<DefFactory> Player::getDefFacts() const { return def_facts; }
QVector<AutoFactory> Player::getAutoFacts() const { return auto_facts; }
QString Player::getStatus() const { return status; }
bool Player::getFinishStatus() const { return finish_status; }


void Player::setDefaultSettings() {
    priority = false;
    money = 10000;
    raw = 2;
    product = 2;
    def_facts = QVector<DefFactory>(2);
    auto_facts = QVector<AutoFactory>(2);
}

bool Player::checkIfInGame() {
    return (money >= 0);
}

// 20 монет за 1 сырья
// 40 за готовое сырье
void Player::payPerRound() {
    money -= 20 * raw + 40 * product;
    if(money < 0) {
        status = "out";
    }
}
int Player::putRawInFabrics(int amount) {
    int free_space_for_raw = def_facts.size() * 2 + auto_facts.size() * 4;
    for(const auto& fab : def_facts) {
        free_space_for_raw -= fab.getAmount();
    }
    for(const auto& fab : auto_facts) {
        free_space_for_raw -= fab.getAmount();
    }
    if(free_space_for_raw < amount) {
        return -1;
    }
    int raw_money = 0;
    int amount_copy = amount;
    for(auto& fab : auto_facts) {
        for(int i = 0; i < 4; i++)
        {
            if(amount_copy > 0 && fab.getAmount() < 4) {
                raw_money += fab.getCost();
                amount_copy--;
            }
        }
    }
    for(auto& fab : def_facts) {
        for(int i = 0; i < 2; i++)
        {
            if(amount_copy > 0 && fab.getAmount() < 2) {
                raw_money += fab.getCost();
                amount_copy--;
            }
        }
    }
    if(money < raw_money) {
        return -2;
    }

    raw -= amount;
    money -= raw_money;
    for(auto& fab : auto_facts) {
        for(int i = 0; i < 4; i++)
        {
            if(amount > 0 && fab.getAmount() < 4) {
                fab.addRaw(1);
                amount--;
            }
        }
    }
    for(auto& fab : def_facts) {
        for(int i = 0; i < 2; i++)
        {
            if(amount > 0 && fab.getAmount() < 2) {
                fab.addRaw(1);
                amount--;
            }
        }
    }
    return 1;
}

bool Player::upgradeFacts(const int& amount) {

    int counter = 0;
    if(amount <= def_facts.size())
    {
        bool flag = true;
        while(flag) {
            flag = false;
            for(int i = 0; i < def_facts.size(); i++) {
                if(def_facts[i].getAmount() == 0 && counter < amount) {
                    counter++;
                    def_facts.erase(def_facts.begin() + i);
                    flag = true;
                    break;
                }
            }
            if (flag){
                upgrade_facts.push_back(qMakePair(1, 0));
                money -= 3000 / 2;
            }
        }
        if(counter < amount) {
            int temp = amount - counter;
            for(auto& fab : def_facts) {
                if(counter < amount) {
                    counter++;
                    raw += fab.getAmount();
                    money += fab.getAmount() * fab.getCost();
                    fab.setAmount(0);
                }
            }
            def_facts.erase(def_facts.begin(), def_facts.begin() + temp);
            upgrade_facts.push_back(qMakePair(temp, 0));
            money -= temp * 3000 / 2;
        }
        // ПЕРЕСМОТРИТЕ НАВСЯКИЙ
        //money -= amount * 3000 / 2;
        return true;
    }
    else
    {
        return false;
    }
}
void Player::updateUpgrade() {
    for(auto& i : upgrade_facts) {
        i.second++;
    }

    bool flag = true;
    while(flag) {
        flag = false;
        for(int i = 0; i < upgrade_facts.size(); i++) {
            if (upgrade_facts[i].second == 8){
                // Предупреждение о необходимости 1500
                box = new QMessageBox;
                QString mess = "Для апгрейда фабрики " + QString::number(i+1) + " игроку " + QString::number(ID) +  " необходимо 1500 в противном случае деньги пропадут впустую";
                box->setInformativeText(mess);
                box->show();
            }

            if(upgrade_facts[i].second == 9) {
                for(int j = 0; j < upgrade_facts[i].first; ++j) {
                    if (getMoney() >= 1500){
                        money -= 1500;
                        auto_facts.push_back(AutoFactory());
                    }
                }
                upgrade_facts.erase(upgrade_facts.begin() + i);
                flag = true;
                break;
            }
        }
    }
}

void Player::updateProduct() {
    for(auto& fab : def_facts) {
        product += fab.produce();
    }
    for(auto& fab : auto_facts) {
        product += fab.produce();
    }
}
void Player::roundUpdate() {
    finish_status = false;
    payPerRound();
    updateUpgrade();
    updateProduct();
}

bool Player::deleteFabrics(const int& amount) {
    if(def_facts.size() + auto_facts.size() < amount) {
        return false;
    }
    srand(time(NULL));
    QVector<Factory*> all;
    for(auto& i : def_facts) {
        all.push_back(&i);
    }
    for(auto& i : auto_facts) {
        all.push_back(&i);
    }
    for(int i = 0; i < amount; i++) {
        all.erase(all.begin() + rand() % all.size());
    }
    QVector<DefFactory> new_def;
    QVector<AutoFactory> new_auto;
    for (auto& i : all) {
        if (typeid(*i) == typeid(DefFactory)) {
            new_def.push_back(*dynamic_cast<DefFactory*>(i));
        }
        else {
            new_auto.push_back(*dynamic_cast<AutoFactory*>(i));
        }
    }
    setDefFacts(new_def);
    setAutoFacts(new_auto);
    return true;
}
