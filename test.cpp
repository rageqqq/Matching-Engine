#include <iostream>
#include <string>
#include <iomanip>
#include <vector>

#include "order.cpp"
#include "stock_orders.cpp"
#include "parser.cpp"

using namespace std;

void testOrders(float price, int quantity) {
    try {
        order o1(1.01,1,true,"aaa");
        o1.printOrder();
        order o2(price, quantity,true,"aaa");
        o2.printOrder();
    }
    catch (const invalid_argument ia) {
        cout << ia.what() << '\n';
    }
    
}

void testPartialFill(bool buy, int partialCount, int count) {
    vector<order> vec;
    vector<order> matches;
    StockOrders s;

    string str = (buy) ? "buy" : "sell";

    cout << "_________________________________________\n";
    cout << "Testing partial fill of " + str + " orders\n";
    
    for(int i=0; i<partialCount; i++) {
        order o(1.01,1,!buy,"aaa");
        vec.push_back(o);
    }
    order o = order(1.01, count, buy, "aaa");
    vec.push_back(o);
    for(order o : vec) {
        matches = s.processOrder(o);
        if(!matches.empty()) {
            for(order match: matches) {
                cout << "Order matched: ";
                match.printOrder();
            }
        }
    }
    s.print();
    return;
}

void testMatchingPriority(bool buy) {
    vector<order> vec;
    vector<order> matches;
    StockOrders s;
    string str = (buy) ? "buy" : "sell";

    cout << "_________________________________________\n";
    cout << "Testing matching priority of " + str + " orders\n";
    for(int i=0; i<5; i++) {
        order o(1.01,1,!buy,"aaa");
        vec.push_back(o);
    }
    order o(1.01, 1, buy, "aaa");
    vec.push_back(o);
    for(order o : vec) {
        matches = s.processOrder(o);
        if(!matches.empty()) {
            for(order match: matches) {
                cout << "Order matched: ";
                match.printOrder();
            }
            cout << "_________________________________________\n";
        }
    }
    s.print();
    return;
}

void testStockOrders() {
    testMatchingPriority(true);
    testMatchingPriority(false);
    testPartialFill(true, 5, 4);
    testPartialFill(false, 5, 4);
    return;
}

void testParser() {
    parser p;
    for(int i=0; i<5; i++) {
        string s = "1001|10|1|aaa|";
        order o = p.decryptPacket(s);
        cout << s << '\n';
        o.printOrder();
    }

}

int main() {
    testOrders(0.0,1); // test $0 price
    testOrders(-0.5,1); //test negative price
    testOrders(1.00,0); // test 0 quantity
    testOrders(1.00,-100); // test negative quantity
    
    testStockOrders();
    
    testParser();

    //used to test matching algorithm
    StockOrders s;
    order o1(1.01, 1, true, "aaa");
    order o2(1.02, 1, true, "aaa");
    order o3(1.03, 1, true, "aaa");
    order o4(1.02, 1, false, "aaa");
    s.processOrder(o1);
    s.processOrder(o2);
    s.processOrder(o3);
    s.processOrder(o4);
    s.print();

    return 0;
}