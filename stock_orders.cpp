#include "order.h"
#include "stock_orders.h"

#include <string>
#include <map>
#include <iostream>
#include <vector>

using namespace std;


StockOrders::StockOrders() {
    buys.clear();
    sells.clear();
}

vector<order> StockOrders::processOrder(order o) {
    cout << "Order added: ";
    o.printOrder();
    if(o.getIsBuy()) {
        return processBuy(o);
    } else {
        return processSell(o);
    }
}

vector<order> StockOrders::processBuy(order o) {
    vector<order> output;
    float buyPrice = o.getPrice();
    int buyId = o.getID();
    int buyQuantity = o.getRemainingQuantity();
    while(buyQuantity > 0) {
        if(sells.empty()) { //no sell orders, so we directly insert the buy order
            buys.insert({{-buyPrice,buyId},o});
            return output;
        }

        //get details of earliest sell order with lowest price
        order &sell = sells.begin()->second;
        float sellPrice = sell.getPrice();
        int sellQuantity = sell.getRemainingQuantity();

        //lowest price sell order is higher than buy order price, unable to match order
        if(sellPrice > buyPrice) { 
            buys.insert({{-buyPrice,buyId},o});
            return output;
        }

        //sell order able to fill whole buy order
        if(sellQuantity > buyQuantity) { 
            sell.reduceQuantity(buyQuantity);
            buyQuantity = 0;
            output.push_back(sell);
        } else{ //need to check 1 or more sell orders
            sell.reduceQuantity(buyQuantity);
            buyQuantity -= sellQuantity;
            output.push_back(sell);
            sells.erase(sells.begin());
        }
    }
    return output;
}

vector<order> StockOrders::processSell(order o) {
    vector<order> output;
    float sellPrice = o.getPrice();
    int sellId = o.getID();
    int sellQuantity = o.getRemainingQuantity();
    while(sellQuantity > 0) {
        if(buys.empty()) { //no buy orders, so we directly insert sell order
            sells.insert({{sellPrice,sellId},o});
            return output;
        }

        //get details of earliest buy order with highest price
        order &buy = buys.begin()->second;
        float buyPrice = buy.getPrice();
        int buyQuantity = buy.getRemainingQuantity();

        //price of sell order is higher than highest buy order price, unable to match order
        if(buyPrice < sellPrice) {
            sells.insert({{sellPrice,sellId},o});
            return output;
        }

        //need to check 1 buy order only
        if(buyQuantity > sellQuantity) { 
            buy.reduceQuantity(sellQuantity); //will be 0
            sellQuantity = 0;
            output.push_back(buy);
        } else { //need to check 1 or more buy orders
            buy.reduceQuantity(sellQuantity);
            sellQuantity -= buyQuantity;
            output.push_back(buy);
            buys.erase(buys.begin());
        }
    }
    return output;
}

void StockOrders::printBuys() {
    if(buys.empty()) {
        cout << "Buy Orders empty" << '\n';
    } else {
        cout << "Buy Orders: " << '\n';
        for(auto it = buys.begin(); it != buys.end(); it++) {
            it->second.printOrder();
        }
    }
    return;
}

void StockOrders::printSells() {
    if(sells.empty()) {
        cout << "Sell Orders empty" << '\n';
    } else {
        cout << "Sell Orders: " << '\n';
        for(auto it = sells.begin(); it != sells.end(); it++) {
            it->second.printOrder();
        }
    }
    return;
}

void StockOrders::print() {
    cout << "_________________________________________\n";
    printBuys();
    printSells();
    cout << "_________________________________________\n";
    return;
}
