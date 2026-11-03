#include <iostream>
using namespace std;

#include "Product.h"
#include "Shop.h"

// To do: Implement the constructors here!
Product::Product(int product_ID, int price, Shop* shop):product_ID(product_ID), price(price)
{
    if (shop == nullptr)
        origin_ID = -1;
    else
        origin_ID = shop->get_ID();
}

Product::Product(const Product& other):product_ID(other.get_ID()), price(other.get_price())
{  
    origin_ID = other.get_origin_ID();
}
// End of to-do



// PLEASE DO NOT CHANGE!
void Product::set_origin_ID(Shop* shop) {
    if (shop != nullptr) {
        origin_ID = shop->get_ID();
    } else {
        origin_ID = -1;
    }
}

void Product::print() const {
    if (origin_ID == -1) {
        cout << " - Product ID: " << product_ID << " ($" << price << ")" << endl;
    } else {
        cout << " - Product ID: " << product_ID << " ($" << price << "; from Shop ID " << origin_ID << ")" << endl;
    }
}