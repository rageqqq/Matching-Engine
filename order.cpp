#include <string>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "order.h"
using namespace std;

int order::currID = 0;

order::order(float a, int b, bool c, string d):
    id(currID++) {
        if(a <= 0) {
            string s = "Price cannot be negative or zero: " + to_string(a);
            throw invalid_argument(s);
        }
        if(b < 1) {
            string s = "Quantity cannot be negative or zero: " + to_string(b);
            throw invalid_argument(s);
        }
        price = a;
        quantity = b;
        remainingQuantity = b;
        isBuy = c;
        name = d;
    }

void order::printOrder() {
    cout << "ID: " << id << " | Name: " << name << " | Price: " << price << " | Original quantity: " << quantity 
    << " | Remaining Quantity: " << remainingQuantity << " | isBuy: " << isBuy << '\n';
    return;
}

void order::reduceQuantity(int a) {
    if(a > remainingQuantity) {
        remainingQuantity = 0;
    } else {
        remainingQuantity = remainingQuantity - a;
    }
    return;
}

float order::getPrice() { return price;}

int order::getID() { return id;}

int order::getOriginalQuantity() { return quantity;}

int order::getRemainingQuantity() { return remainingQuantity;}

bool order::getIsBuy() { return isBuy;}

string order::getName() { return name;}

