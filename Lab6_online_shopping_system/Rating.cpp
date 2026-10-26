#include <iostream>
#include <string>
using namespace std;

#include "Customer.h"
#include "Shop.h"
#include "Rating.h"

int Rating::rating_count = 0; //Static data member initialization

// To do: Implement the constructor here!
Rating::Rating(Customer* customer, Shop* shop, int score):rating_ID(rating_count),customer_ID(customer->get_ID()),shop_ID(shop->get_ID()){
    rating_count++;
    this->customer = customer;
    this->shop = shop;
    this->score = score;

    if (this->score < 0){
        score = 0;
    }
    else if (this->score > 100){
        score = 100;
    }

    customer_next = nullptr;
    shop_next = nullptr;
}
// End of to-do

// PLEASE DO NOT CHANGE!
void Rating::print(bool show_customer, bool show_shop) const {
    cout << " - Rating " << rating_ID << "; ";
    if (show_customer) cout << "By Customer ID: "<< customer_ID << (customer != nullptr ? "" : " (DELETED)") << "; ";
    if (show_shop) cout << "For Shop ID: " << shop_ID << (shop != nullptr ? "" : " (DELETED)") << "; ";
    cout << "Score: " << score << endl;
}