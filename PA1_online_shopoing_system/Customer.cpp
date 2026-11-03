#include <iostream>
using namespace std;

#include "Customer.h"
#include "Shop.h"
#include "Product.h"
#include "Rating.h"

// To do: Implement the member functions here!
Customer::Customer(int customer_ID):customer_ID(customer_ID){
    balance = 0;
    belongings = new Product*[2];
    num_of_belongings = 0;
    belongings_capacity = 2;
    ratings = nullptr;
}

Customer::~Customer(){
    for (int i = 0; i < num_of_belongings; i++){
        delete belongings[i];
    }
    delete [] belongings;

    Rating* curr = ratings;
    while(curr != nullptr){
        Rating* next_node = curr->customer_next;
        curr->customer = nullptr;
        if(curr->shop == nullptr){
            delete curr;
        }
        curr = next_node;
    }
}

bool Customer::add_product(Product* product){
    if (product == nullptr)
        return false;
    for (int i = 0; i < num_of_belongings; i++){
        if (belongings[i] == product)
            return false;
    }

    if (num_of_belongings == belongings_capacity){
        belongings_capacity = belongings_capacity * 2;
        Product** new_belongings = new Product* [belongings_capacity];
        for (int i = 0; i < num_of_belongings; i++){
            new_belongings[i] = belongings[i];
        }
        delete [] belongings;
        belongings = new_belongings;
    }

    belongings[num_of_belongings] = product;
    num_of_belongings++;
    return true;
}

bool Customer::rate_shop(Shop* shop, int score){
    bool allowed = false;
    for (int i = 0; i < num_of_belongings; i++){
        if (belongings[i]->get_origin_ID() == shop->get_ID()){
            allowed = true;
            break;
        }
    }
    if (!allowed || shop == nullptr)
        return false;
    
    if (score < 0 || score > 100)
        return false;
    // check if the customer already have the rating of the shop
    Rating* target = nullptr;
    Rating* prev = nullptr; //prev maybe a null ptr!!!
    for (Rating* ptr = this->ratings; ptr != nullptr; ptr = ptr->customer_next){
        if (ptr->shop_ID == shop->get_ID()){
            target = ptr;
            break;
        }
        prev = ptr;
    }   
    if (target != nullptr){
        target->score = score;
        shop->set_ratings_head(target);
        if(prev != nullptr){
            prev->customer_next = target->customer_next;// make it the head of customer rating
            target->customer_next = this->ratings;
            this->ratings = target;
        }
        return true;
    }

    target = new Rating(this, shop, score);
    target->customer_next = ratings;
    ratings = target;
    shop->set_ratings_head(target);

    return true;
}

const Rating* Customer::get_i_th_rating(int i){
    if (i < 0){
        return nullptr;
    }

    int curr_idx = 0;
    for (const Rating* ptr = ratings; ptr != nullptr; ptr = ptr->customer_next) {
        if (curr_idx == i) {
            return ptr;
        }
        curr_idx++;
    }
    return nullptr;
}

bool Customer::delete_rating(int shop_ID){

    Rating* prev = nullptr;
    Rating* curr = this->ratings;

    while(curr != nullptr){
        if (curr->shop_ID == shop_ID){
            break;
        }
        prev = curr;
        curr = curr->customer_next;
    }
    if(curr == nullptr) //no rating is found
        return false;
    if (prev == nullptr) {// is the first element
        ratings = curr->customer_next;
    } else {
        prev->customer_next = curr->customer_next;
    }
    curr->customer = nullptr;
    curr->shop->remove_rating(this->get_ID());
    return true;
}

// Only write in the to-do section, PLEASE DO NOT CHANGE ANYTHING ELSE!
void Customer::print_ratings() const {
    cout << "--------------------- CUSTOMER RATING DATA --------------------" << endl;
    cout << "Reviews by Customer ID: " << customer_ID << endl;
    if (ratings == nullptr) { 
        cout << "- No reviews written." << endl;
    } else {
        // To-do: Implement the Rating printing here!
        for(Rating* ptr = ratings; ptr != nullptr; ptr = ptr->customer_next){
            ptr->print(false, true);
        }
        // End of to-do
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}
// End of to-do



// PLEASE DO NOT CHANGE!
void Customer::print() const {
    cout << "------------------------ CUSTOMER DATA ------------------------" << endl;
    cout << "Customer ID: " << customer_ID << endl;
    cout << "- Balance: $" << balance << endl;
    cout << "- Number of Products in Belongings: " << num_of_belongings << endl;
    cout << "----------------------------- END -----------------------------" << endl << endl;
}

void Customer::print_belongings() const {
    cout << "----------------------- BELONGINGS DATA -----------------------" << endl;
    cout << "Belongings of Customer ID: " << customer_ID << endl;
    if (num_of_belongings == 0) cout << "- Has no belongings." << endl;
    for (int i = 0; i < num_of_belongings; i++) {
        belongings[i]->print();
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}