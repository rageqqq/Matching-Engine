
#ifndef STOCK_ORDERS_H
#define STOCK_ORDERS_H

#include <string>
#include <map>
#include <vector>

#include "order.h"

using namespace std;

class StockOrders {
private:
    map<pair<float,int>,order> buys;
    map<pair<float,int>,order> sells;

public:
    StockOrders();

    vector<order> processOrder(order o);

    vector<order> processBuy(order o);

    vector<order> processSell(order o);

    void printSells();

    void printBuys();

    void print();
};

#endif //STOCK_ORDERS_H