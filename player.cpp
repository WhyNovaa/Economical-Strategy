#include "player.h"

Player::Player() : priority(false), money(10000), raw(2), product(2),
    def_facts(QVector<DefFactory>(2)), auto_facts(QVector<AutoFactory>(2)) {}
Player::Player(const bool& _priority, const int& _money, const int& _raw,
               const int& _product, const QVector<DefFactory>& _def_facts, const QVector<AutoFactory>& _auto_facts) : priority(_priority), money(_money),
    raw(_raw), product(_product), def_facts(_def_facts), auto_facts(_auto_facts) {}


void Player::setPriority(const bool& _priority) { priority = _priority; }
void Player::setMoney(const int& _money) { money = _money; }
void Player::setRaw(const int& _raw) { raw = _raw; }
void Player::setProduct(const int& _product) { product = _product; }
void Player::setDefFacts(const QVector<DefFactory>& _def_facts) { def_facts = _def_facts; }
void Player::setAutoFacts(const QVector<AutoFactory>& _auto_facts) { auto_facts = _auto_facts; }


bool Player::getPriority() const { return priority; }
int Player::getMoney() const { return money; }
int Player::getRaw() const { return raw; }
int Player::getProduct() const { return product; }
QVector<DefFactory> Player::getDefFacts() const { return def_facts; }
QVector<AutoFactory> Player::getAutoFacts() const { return auto_facts; }


void Player::setDefaultSettings() {
    priority = false;
    money = 10000;
    raw = 2;
    product = 2;
    def_facts = QVector<DefFactory>(2);
    auto_facts = QVector<AutoFactory>(2);
}

void Player::makeBet(const int& bet) {  }
