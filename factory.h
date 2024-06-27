#ifndef FACTORY_H
#define FACTORY_H

class Factory
{
protected:
    int cost;
    int amount;
    int max_per_round;
public:
    virtual ~Factory() = 0;
};

class DefFactory: public Factory
{
private:
    int speed = 1;
    int cost = 50;
    int max_per_round = 2;
public:
    DefFactory();

    int produce(int r) {
        if (r <= max_per_round){
            return (r);
        }
        else {
            return max_per_round;
        }
    }

    int getCost() {
        return cost;
    }
};

class AutoFactory: public Factory
{
private:
    int speed = 4;
    int cost = 20;
    int max_per_round = 4;
public:
    AutoFactory();

    int produce(int r) {
        if (r <= max_per_round){
            return (r);
        }
        else {
            return max_per_round;
        }
    }

    int getCost() {
        return cost;
    }
};

#endif // FACTORY_H
