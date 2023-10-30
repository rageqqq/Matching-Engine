#ifndef ORDER_H
#define ORDER_H

#include <string>
using namespace std;

class order {
private:
    static int currID;
    float price;
    int quantity;
    int remainingQuantity;
    bool isBuy;
    const int id;
    string name;

public:
    order(float a, int b, bool c, string d);

    void printOrder();

    int getID();

    float getPrice();

    int getOriginalQuantity();

    int getRemainingQuantity();

    bool getIsBuy();

    string getName();

    void reduceQuantity(int a);
    
};

#endif //ORDER_H