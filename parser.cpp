#include "parser.h"
#include "stock_orders.h"
#include "order.h"

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <vector>
#include <random>

using namespace std;

#define NAMES_SIZE 100 //number of stock names generated
#define QUANTITY_MAX 100000 //maximum quantity for each order
#define PRICE_MAX 10000 //max price for each order * 100 for precision purposes
#define DEFAULT_BUFLEN 50 //standard size of each packet sent between client and server

parser::parser() {
    m.clear();
    random_device rd;
    gen = mt19937(rd());
    vector<string> stocknames;
    initializeStockNames();
}

//generate random stock names for generator
void parser::initializeStockNames() { 
    uniform_int_distribution<> dis(0,25);
    unordered_set<string> s;
    while(stocknames.size() < NAMES_SIZE) {
        string temp;
        for(int j=0; j<3; j++) {
            temp += 'A' + dis(gen);
        }
        if(s.find(temp) == s.end()) {
            s.insert(temp);
            stocknames.push_back(temp);
        }
    }
}


//generate random values for testing
//output format: price, quantity, isBuy, name
vector<string> parser::generateValues() { 
    uniform_int_distribution<> disQuantity(1,QUANTITY_MAX); // for quantity generation
    uniform_int_distribution<> disPrice(1,PRICE_MAX); // for price generation
    uniform_int_distribution<> disName(1,NAMES_SIZE); // for name selection
    uniform_int_distribution<> disBuy(0,1); // for buy/sell order type selection
    vector<string> output;
    output.push_back(to_string(disPrice(gen)));
    output.push_back(to_string(disQuantity(gen)));
    output.push_back(to_string(disBuy(gen)));
    output.push_back(stocknames[disName(gen)-1]);
    return output;
}

//generate packet for client to send to server
//output format example: "price|quantity|isBuy|name|"
string parser::generatePacket(vector<string> vec) {
    string output;
    for(auto str: vec) {
        output += str + "|";
    }
    int padding = DEFAULT_BUFLEN - output.size();
    for(int i=0; i<padding; i++) {
        output += '0';
    }
    return output;
}

//generate null packet as a response to client for no matches
//output: "NULL0000000000000000000000000"
string parser::generateEmptyPacket() { 
    string output = "NULL";
    int padding = DEFAULT_BUFLEN - output.size();
    for(int i=0; i<padding; i++) {
        output += '0';
    }
    return output;
}

vector<order> parser::addOrder(order o) {
    string name = o.getName();
    vector<order> output;
    if(m.find(name) == m.end()) { //unable to find stockorders with name in order
        StockOrders s;
        output = s.processOrder(o);
        m[name] = s;
    } else {
        StockOrders &s = m.find(name)->second;
        output = s.processOrder(o);
    }
    return output;
}

void parser::print() {
    for(auto& it: m) {
        cout << "Stock Orders for " << it.first << ": \n";
        StockOrders s = it.second;
        s.print();
    }
    return;
}

//print data received from server if order matched
void parser::decryptData(string s) { 
    if(s.substr(0,4) == "NULL") { //null means no orders matched
        return;
    }
    cout << "Order Matched: ";
    vector<string> vec;
    string temp;
    for(auto i: s) {
        if(i == '|') {
            vec.push_back(temp);
            temp.clear();
        } else {
            temp += i;
        }
    }
    cout << "ID: " << vec[0] << " | Name: " << vec[4] << " | Price: " << vec[1] << " | Original quantity: " << vec[2] << " | isBuy: " << vec[3] << '\n';
    return;
}

//decrypt packet from client into order to be processed
order parser::decryptPacket(string s) {
    vector<string> vec;
    string temp;
    for(auto i: s) {
        if(i == '|') {
            vec.push_back(temp);
            temp.clear();
        } else {
            temp += i;
        }
    }
    float a;
    int b;
    bool c;
    string d;
    int counter = 0;
    for(string str: vec) {
        switch(counter) {
            case 0:
                a = (float)stoi(str)/100;
            case 1:
                b = stoi(str);
            case 2:
                c = (str == "1");
            case 3:
                d = str;
                break;
        }
        counter++;
    }
    order o(a, b, c, d);
    return o;
}

//convert data to send back to client
vector<string> parser::encryptData(order o) { 
    vector<string> output;  //ID | price | quantity | isBuy | name
    output.push_back(to_string(o.getID()));
    output.push_back(to_string(static_cast<int>(o.getPrice()*100)));
    output.push_back(to_string(o.getOriginalQuantity()));
    string s = (o.getIsBuy()) ? "1" : "0";
    output.push_back(s);
    output.push_back(o.getName());
    return output;
}

vector<string> parser::getStockNames(){ return stocknames;}

mt19937 parser::getGenerator() { return gen;}


