#ifndef FACTORY_H
#define FACTORY_H

class Factory
{
protected:
    int cost;
    int amount;
    int max_per_round;
public:
    Factory(const int& _cost, const int& _amount, const int& _max_per_round) : cost(_cost), amount(_amount), max_per_round(_max_per_round) {};

    int produce() {
        if(amount > 0) {
            amount = 0;
            return max_per_round;
        } else {
            return 0;
        }
    }

    void addRaw(const int& raw) {
        amount += raw;
    }

    virtual int getCost() const = 0;
    virtual int getAmount() const = 0;
    virtual void setAmount(const int& _amount) = 0;
    virtual ~Factory() {};
};


class DefFactory: public Factory
{
public:
    DefFactory() : Factory(50, 0, 2) {};

    int getCost() const override { return cost; }
    int getAmount() const override { return amount; }
    void setAmount(const int& _amount) override { amount = _amount; }

    virtual ~DefFactory() {};
};


class AutoFactory: public Factory
{
public:
    AutoFactory() : Factory(20, 0, 4) {};

    int getCost() const override { return cost; }
    int getAmount() const override { return amount; }
    void setAmount(const int& _amount) override { amount = _amount; }

    virtual ~AutoFactory() {};
};

#endif // FACTORY_H
