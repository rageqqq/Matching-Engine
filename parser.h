#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <unordered_map>
#include <random>

#include "order.h"
#include "stock_orders.h"

using namespace std;

class parser {
private:
    unordered_map<string,StockOrders> m;
    random_device rd;
    mt19937 gen;
    vector<string> stocknames;

public:
    parser();

    void initializeStockNames();

    vector<string> generateValues();

    string generatePacket(vector<string> values);

    string generateEmptyPacket();

    void print();

    vector<order> addOrder(order o);

    void decryptData(string s);

    order decryptPacket(string s);

    vector<string> encryptData(order o);

    vector<string> getStockNames();

    mt19937 getGenerator();

};

#endif //PARSER_H