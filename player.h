#ifndef PLAYER_H
#define PLAYER_H

#include "factory.h"

#include <QVector>

class Player
{
private:
    bool in_game;

    bool priority;
    int money;
    int raw;
    int product;
    QVector<DefFactory> def_facts;
    QVector<AutoFactory> auto_facts;

    // 20 монет за 1 сырья
    // 40 за готовое сырье
public:
    Player();
    Player(const bool& _in_game, const bool& _priority, const int& _money, const int& _raw, const int& _product,
           const QVector<DefFactory>& _def_fact, const QVector<AutoFactory>& _auto_fact);

    void setInGame(const bool& _in_game);
    void setPriority(const bool& _priority);
    void setMoney(const int& _money);
    void setRaw(const int& raw);
    void setProduct(const int& product);
    void setDefFacts(const QVector<DefFactory>& _def_facts);
    void setAutoFacts(const QVector<AutoFactory>& _auto_facts);

    bool getInGame() const;
    bool getPriority() const;
    int getMoney() const;
    int getRaw() const;
    int getProduct() const;
    QVector<DefFactory> getDefFacts() const;
    QVector<AutoFactory> getAutoFacts() const;

    void setDefaultSettings();
    void makeBet(const int& bet);
    bool checkIfInGame();
};

#endif // PLAYER_H
