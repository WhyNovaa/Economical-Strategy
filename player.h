#ifndef PLAYER_H
#define PLAYER_H

#include "factory.h"

#include <QVector>

class Player
{
private:
    int ID;
    bool priority;
    int money;
    int raw;
    int product;
    QVector<DefFactory> def_facts;
    QVector<AutoFactory> auto_facts;
    QVector<QPair<int, int>> upgrade_facts;
    QString status;
    // 20 монет за 1 сырья
    // 40 за готовое сырье
public:
    static int next_ID;

    Player();
    Player(const bool& _priority, const int& _money, const int& _raw, const int& _product,
           const QVector<DefFactory>& _def_fact, const QVector<AutoFactory>& _auto_fact, const QString& _status);

    void setPriority(const bool& _priority);
    void setMoney(const int& _money);
    void setRaw(const int& raw);
    void setProduct(const int& product);
    void setDefFacts(const QVector<DefFactory>& _def_facts);
    void setAutoFacts(const QVector<AutoFactory>& _auto_facts);
    void setStatus(const QString& status);

    int getID() const;
    bool getPriority() const;
    int getMoney() const;
    int getRaw() const;
    int getProduct() const;
    QVector<DefFactory> getDefFacts() const;
    QVector<AutoFactory> getAutoFacts() const;
    QString getStatus() const;

    bool upgradeFacts(const int& amount); //возвращает true - все норм, false - не смогло улучшить (сюда можно закинуть только кратное 2 количество)
    void updateUpgrade();

    void payPerRound();
    void updateProduct(); //достаю все готовое сырье
    bool putRawInFabrics(const int& amount); //возвращает true - все норм, false - не смогло засунуть сырье на переработку
    void setDefaultSettings();
    void makeBet(const int& bet);
    bool checkIfInGame();



    void roundUpdate(); //обновляет все за ход(вызывать в начале раунда)
};

#endif // PLAYER_H
