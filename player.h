#ifndef PLAYER_H
#define PLAYER_H

#include "factory.h"

#include <QVector>

class Player
{
private:
    bool priority;
    int money;
    int raw;
    int product;
    QVector<DefFactory*> defFacts;
    QVector<AutoFactory*> autoFacts;

    // 20 монет за 1 сырья
    // 40 за готовое сырье
public:
    Player();

    void makeBet();
};

#endif // PLAYER_H
