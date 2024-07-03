#include "player.h"
int Player::next_ID = 1;
Player::Player() : ID(next_ID++), priority(false), money(10000), raw(2), product(2),
    def_facts(QVector<DefFactory>(2)), auto_facts(QVector<AutoFactory>(2)), status("in") {}
Player::Player(const bool& _priority, const int& _money, const int& _raw, const int& _product,
               const QVector<DefFactory>& _def_facts, const QVector<AutoFactory>& _auto_facts, const QString& _status):
    ID(next_ID++), priority(_priority), money(_money), raw(_raw), product(_product), def_facts(_def_facts),
    auto_facts(_auto_facts), status(_status) {}

void Player::setPriority(const bool& _priority) { priority = _priority; }
void Player::setMoney(const int& _money) { money = _money; }
void Player::setRaw(const int& _raw) { raw = _raw; }
void Player::setProduct(const int& _product) { product = _product; }
void Player::setDefFacts(const QVector<DefFactory>& _def_facts) { def_facts = _def_facts; }
void Player::setAutoFacts(const QVector<AutoFactory>& _auto_facts) { auto_facts = _auto_facts; }
void Player::setStatus(const QString& _status) { status = _status; }

int Player::getID() const { return ID; }
bool Player::getPriority() const { return priority; }
int Player::getMoney() const { return money; }
int Player::getRaw() const { return raw; }
int Player::getProduct() const { return product; }
QVector<DefFactory> Player::getDefFacts() const { return def_facts; }
QVector<AutoFactory> Player::getAutoFacts() const { return auto_facts; }
QString Player::getStatus() const { return status; }


void Player::setDefaultSettings() {
    priority = false;
    money = 10000;
    raw = 2;
    product = 2;
    def_facts = QVector<DefFactory>(2);
    auto_facts = QVector<AutoFactory>(2);
}

void Player::makeBet(const int& bet) {  }

bool Player::checkIfInGame() {
    return (money >= 0);
}

// 20 монет за 1 сырья
// 40 за готовое сырье
void Player::payPerRound() {
    money -= 20 * raw + 40 * product;
}
bool Player::putRawInFabrics(const int& amount) {
    if(amount % 2 != 0) {
        return false;
    }
    int free_space_for_raw = def_facts.size() + auto_facts.size();
    for(const auto& fab : def_facts) {
        free_space_for_raw -= fab.getAmount();
    }
    for(const auto& fab : auto_facts) {
        free_space_for_raw -= fab.getAmount();
    }
    if(free_space_for_raw <= 0) {
        return false;
    }
    for(auto& fab : def_facts) {
        if(free_space_for_raw > 0 && fab.getAmount() == 0) {
            fab.addRaw(2);
            free_space_for_raw -= 2;
        }
    }
    for(auto& fab : auto_facts) {
        if(free_space_for_raw > 0 && fab.getAmount() == 0) {
            fab.addRaw(4);
            free_space_for_raw -= 4;
        }
    }
    return true;
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
        }
        if(counter < amount) {
            int temp = amount - counter;
            for(auto& fab : def_facts) {
                if(counter < amount) {
                    counter++;
                    raw += fab.getAmount();
                    fab.setAmount(0);
                }
            }
            def_facts.erase(def_facts.begin(), def_facts.begin() + temp);
        }
        return true;
    }
    else
    {
        return false;
    }
}
void Player::updateUpgrade() {
    bool flag = true;
    while(flag) {
        flag = false;
        for(int i = 0; i < upgrade_facts.size(); i++) {
            if(upgrade_facts[i].second == 9) {
                for(int i = 0; i < upgrade_facts[i].first; i++) {
                    auto_facts.push_back(AutoFactory());
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
    for(auto& i : upgrade_facts) {
        i.second++;
    }
    payPerRound();
    updateUpgrade();
    updateProduct();
}
//sad
