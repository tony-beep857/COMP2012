#include <iostream>
using namespace std;

#include "CustomerData.h"

// To do: Implement the constructor here!
CustomerData::CustomerData(int customer_ID):customer_ID{customer_ID}{
    amount_spent = 0;
    next = nullptr;
}

// End of to-do



// PLEASE DO NOT CHANGE!
void CustomerData::print() const {
    cout << "- Customer ID: " << customer_ID << " spent $" << amount_spent << endl;
}