#include <iostream>
using namespace std;

#include "Customer.h"
#include "Shop.h"
#include "Platform.h"
#include "Product.h"
#include "Rating.h"

// To do: Implement the member functions here!
Shop::Shop(int shop_ID, Platform* platform, double cut_rate):shop_ID(shop_ID), platform(platform), cut_rate(cut_rate)
{
    balance = 0;
    inventory = new Product*[2];
    num_of_inventory = 0;
    inventory_capacity = 2;
    ratings = nullptr;

    if (platform != nullptr){
        platform->add_shop(this);
    }
}

Shop::~Shop(){
    for (int i = 0; i < num_of_inventory; i++){
        delete inventory[i];
    }
    delete [] inventory;

    Rating* curr = this->ratings;
    while(curr != nullptr){
        Rating* next_node = curr->shop_next;
        curr->shop = nullptr;
        if (curr->customer == nullptr){
            delete curr;
        }
        curr = next_node;
    }
}

bool Shop::stock_product(Product* product){
    if (product == nullptr)
        return false;
    for (int i = 0; i < num_of_inventory; i++){
        if (inventory[i] == product)
            return false;
    }

    product->set_origin_ID(this);
    if(num_of_inventory == inventory_capacity){
        inventory_capacity = inventory_capacity*2;
        Product** new_inventory = new Product*[inventory_capacity];
        for (int i = 0; i < num_of_inventory; i++){
            new_inventory[i] = inventory[i];
        }
        delete []inventory;
        inventory = new_inventory;
    }

    int new_product_ID = product->get_ID();
    int insert_idx = num_of_inventory;
    for (int i = 0; i < num_of_inventory; i++){
        int current_ID = inventory[i]->get_ID();
        if (new_product_ID<= current_ID){
            insert_idx = i;
            break;
        }
    }
    for (int i = num_of_inventory; i > insert_idx; i--){
        inventory[i] = inventory[i-1];
    }

    inventory[insert_idx] = product;
    num_of_inventory++;
    return true;
}

Product* Shop::remove_product(int product_ID){
    int product_idx = -1;
    for (int i = 0; i < num_of_inventory; i++){
        if (inventory[i]->get_ID() == product_ID){
            product_idx = i;
            break;
        }
    }
    if (product_idx == -1)
        return nullptr;
    
    Product* return_product = inventory[product_idx];
    for (int i = product_idx; i < num_of_inventory-1; i++){
        inventory[i] = inventory[i+1];
    }
    num_of_inventory--;
    return return_product;
}

// Only write in the to-do section, PLEASE DO NOT CHANGE ANYTHING ELSE!
void Shop::print_ratings() const {
    cout << "----------------------- SHOP RATING DATA ----------------------" << endl;
    cout << "Ratings for Shop ID: " << shop_ID << endl;
    if (ratings == nullptr) {
        cout << "- No reviews available." << endl;
    } else {
        // To-do: Implement the Rating printing here!
        for (Rating* ptr = this->ratings; ptr != nullptr; ptr = ptr->shop_next){
            ptr->print(true, false);
        }
        // End of to-do
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}

void Shop::set_ratings_head(Rating* r){
    if(r == nullptr || r->shop_ID != this->get_ID())
        return;
    if(this->ratings == r)
        return;

    Rating* prev = nullptr;
    Rating* cur = ratings;
    while (cur != nullptr){
        if(cur == r){
            prev->shop_next = r->shop_next;
            break;
        }
        prev = cur;
        cur = cur->shop_next;
    }
  // move in shop chain
    r->shop_next = ratings;
    ratings = r;
}

const Rating* Shop::get_i_th_rating(int i){
    if (i < 0)
        return nullptr;
    int curr_idx = 0;
    for(const Rating* ptr = this->ratings; ptr != nullptr; ptr = ptr->shop_next){
        if ( curr_idx == i){
            return ptr;
        }
        curr_idx++;
    }
    return nullptr;
}

int Shop::get_average_score() const{
    if(this->ratings == nullptr)
        return -1;
    int total = 0;
    int count = 0;
    for(const Rating* ptr = this->ratings; ptr != nullptr; ptr = ptr->shop_next){
        total += ptr->score;
        count++;
    }
    return total / count;
}

bool Shop::remove_rating(int customer_ID){

    Rating* prev = nullptr;
    Rating* curr = ratings;
    while(curr != nullptr){
        if (curr->customer_ID == customer_ID)
            break;
        prev = curr;
        curr = curr->shop_next;
    }

    if(curr == nullptr){
        return false;
    }
    if (prev == nullptr){
        ratings = curr->shop_next;
    } else {
        prev->shop_next = curr->shop_next;
    }
    curr->shop = nullptr;
    if (curr->customer == nullptr){
        delete curr;
    }
    return true;
}

bool Shop::handle_order(const int* order, int order_size, Customer* customer){
    if (order == nullptr || order_size <= 0 || customer == nullptr)
        return false;
    
    int sum_of_prices = 0;// the customer can afford the calculated cost?
    for(int i = 0; i < order_size; i++){
        int price = -1;
        int product_ID = order[i];
        for(int j = 0; j < this->get_num_of_inventory(); j++){
            if(product_ID == inventory[j]->get_ID()){
                price = inventory[j]->get_price();
                break;
            }
        }
        if (price == -1){
            return false;
        }
        sum_of_prices += price;
    }
    
    int cost_of_order = sum_of_prices;
    int revenue_cut = 0;

    if (this->platform != nullptr){
        if(this->platform->check_if_VIP(customer->get_ID())){ // if the customer is a vip
            cost_of_order = cost_of_order * platform->get_VIP_discount();
            revenue_cut = cost_of_order * cut_rate /2;
        } else{ // the customer is not a vip
            revenue_cut = cost_of_order * cut_rate;
        }

    } 
        
    if (cost_of_order > customer->get_balance())
        return false;    

    if(platform != nullptr){
        this->platform->change_balance(revenue_cut);
        balance += cost_of_order - revenue_cut;
    } else{// the shop is not associate with platform
        balance += cost_of_order;
    }
    
    customer->change_balance(-cost_of_order);
    for(int i = 0; i < order_size; i++){
        customer->add_product(this->remove_product(order[i]));
    }
    if(platform!= nullptr)
        this->platform->update_customer_data(customer->get_ID(), cost_of_order);

    return true;
}

// End of to-do



// PLEASE DO NOT CHANGE!
int Shop::get_platform_ID() const {
    if (platform == nullptr) {
        return -1;
    } else {
        return platform->get_ID();
    }
}

void Shop::print() const {
    cout << "-------------------------- SHOP DATA --------------------------" << endl;
    cout << "Shop ID: " << shop_ID;
    if (platform != nullptr) {
        cout << "; Associated platform ID: " << platform->get_ID() << " (Cut Rate: " << cut_rate*100 << "%)" << endl;
    } else {
        cout << "; No associated platform."  << endl;
    }
    cout << "- Balance: $" << balance << endl;
    cout << "- Number of Products in Inventory: " << num_of_inventory << endl;
    cout << "----------------------------- END -----------------------------" << endl << endl;
}

void Shop::print_inventory() const {
    cout << "------------------------ INVENTORY DATA -----------------------" << endl;
    cout << "Inventory of Shop ID: " << shop_ID << endl;
    if (num_of_inventory == 0) cout << "- Out of stock." << endl;
    for (int i = 0; i < num_of_inventory; i++) {
        inventory[i]->print();
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}