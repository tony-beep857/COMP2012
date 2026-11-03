#include <iostream>
using namespace std;

#include "Platform.h"
#include "Shop.h"
#include "Customer.h"
#include "CustomerData.h"

// To do: Implement the member functions here!
Platform::Platform(int platform_ID, int VIP_threshold, double VIP_discount, int subscription_fee):platform_ID(platform_ID)
{
    this->VIP_threshold = VIP_threshold;
    this->VIP_discount = VIP_discount;
    this->subscription_fee = subscription_fee;
    balance = 0;
    shops = new Shop*[2];
    num_of_shops = 0;
    shops_capacity = 2;
    customer_data = nullptr;
}

Platform::~Platform(){
    for (int i = 0; i < num_of_shops; i++){
        delete shops[i];
    }
    delete [] shops;

    CustomerData* curr = this->customer_data;
    while(curr!=nullptr){
        CustomerData* next = curr->next;
        delete curr;
        curr = next;
    }
}

bool Platform::add_shop(Shop* shop){
    if (shop == nullptr)
        return false;
    if (shop->get_platform_ID() != this->get_ID())
        return false;
    for (int i = 0; i < num_of_shops; i++){
        if (shops[i] == shop)
            return false;
    }
    if (num_of_shops == shops_capacity){
        shops_capacity = shops_capacity * 2;
        Shop** new_shops = new Shop*[shops_capacity];
        for (int i = 0; i < num_of_shops; i++){
            new_shops[i] = shops[i];
        }
        delete [] shops;
        shops = new_shops;
    }
    shops[num_of_shops] = shop;
    num_of_shops++;
    return true;
}

bool Platform::delete_shop(int shop_ID){
    int shop_idx = -1;
    for (int i = 0; i < num_of_shops; i++){
        if (shops[i]->get_ID() == shop_ID){
            shop_idx = i;
            break;
        }
    }
    if (shop_idx == -1)
        return false;
    
    delete shops[shop_idx];
    for (int i = shop_idx; i < num_of_shops -1; i++){
        shops[i] = shops[i+1];
    }
    num_of_shops--;
    return true;
}

int Platform::update_customer_data(int customer_ID, int amount){
    CustomerData* target = nullptr;
    for(CustomerData* ptr = customer_data; ptr != nullptr; ptr = ptr->next){
        if (ptr->customer_ID == customer_ID){
            target = ptr;
            break;
        }
    }
    if (target == nullptr){
        CustomerData* new_node = new CustomerData(customer_ID);
        new_node->amount_spent = amount;

        if(customer_data == nullptr || customer_ID < customer_data->customer_ID){
            new_node->next = customer_data;
            customer_data = new_node;
        } else{
            CustomerData* current = customer_data;

            while (current->next != nullptr && current->next->customer_ID < customer_ID){
                current = current->next;
            }

            new_node->next = current->next;
            current->next = new_node;
        }
        return amount;
        }
    
    target->amount_spent += amount;
    return  target->amount_spent;
}

bool Platform::delete_customer_data(int customer_ID){
    /*
    if (customer_data == nullptr){
        return false;
    } */
    if(customer_data->customer_ID == customer_ID){
        CustomerData* node_to_delete = customer_data;
        customer_data = customer_data->next;
        delete node_to_delete;
        return true;
    }
    for(CustomerData* ptr = customer_data; ptr->next != nullptr; ptr = ptr->next){
        if (ptr->next->customer_ID == customer_ID){
            CustomerData* node_to_delete = ptr->next;
            ptr->next = node_to_delete->next;
            delete node_to_delete;
            return true;
        }
    }
    return false;

}

bool Platform::check_if_VIP(int customer_ID) const{
    CustomerData* target = nullptr;
    for (CustomerData* ptr = customer_data; ptr != nullptr; ptr = ptr->next){
        if(ptr->customer_ID == customer_ID){
            target = ptr;
            break;
        }
    }
    if (target == nullptr)
        return false;
    if(target->amount_spent >= this->get_VIP_threshold())
        return true;
    else
        return false;
}

void Platform::simulate_end_of_month(){
    for(int i = num_of_shops - 1; i >= 0; i--){

        bool bankrupt = false;
        if (shops[i]->get_balance() >= this->get_subscription_fee()){//the shop have enough balance
            shops[i]->change_balance(-get_subscription_fee());
            this->change_balance(get_subscription_fee());
        } else { // the shop cannot pay the subscription fee, remove the shop
            int balance = shops[i]->get_balance();
            shops[i]->change_balance(-balance);
            change_balance(balance);
            this->delete_shop(shops[i]->get_ID());
            bankrupt = true;
        } 
        if(!bankrupt){
            int num_of_ratings = 0;
            while(shops[i]->get_i_th_rating(num_of_ratings) != nullptr){
                num_of_ratings++;
            }
            if(shops[i]->get_average_score() < 15 && num_of_ratings >3){
                this->delete_shop(shops[i]->get_ID());
            }
        }
    }

    for (int i = 0; i < num_of_shops -1; i++){
        for(int j = 0; j < num_of_shops -i -1; j++){
            bool swap = false;
            if(shops[j]->get_average_score() < shops[j+1]->get_average_score()){
                swap = true;
            }
            if(shops[j]->get_average_score() == shops[j+1]->get_average_score() && shops[j]->get_ID() > shops[j+1]->get_ID()){
                swap = true;
            }
            if (swap){
                Shop* temp = shops[j];
                shops[j] = shops[j+1];
                shops[j+1] = temp;
            }
        }
    }

}
// Only change the part in the to-do section, DO NOT CHANGE ANYTHING ELSE!
void Platform::print_customer_data() const {
    cout << "------------------------ CUSTOMER DATA ------------------------" << endl;
    cout << "Customer Data of Platform ID: " << platform_ID << endl;
    if (customer_data == nullptr) { 
        cout << "- Has no customer data." << endl; 
    } else {
        int total = 0; //The total amount of money spent on this Platform
        int count = 0; //The number of VIP Customers
        
        // To-do: Implement the CustomerData printing here!
        for (CustomerData* ptr = this->customer_data; ptr != nullptr; ptr = ptr->next){
            ptr->print();
            total += ptr->amount_spent;
            if (check_if_VIP(ptr->customer_ID))
                count++;
        }
        // End of to-do

        cout << "Customers Total Spending: $" << total << endl;
        cout << "Number of VIP Customers: " << count << endl;
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}
// End of to-do



// PLEASE DO NOT CHANGE!
void Platform::print() const {
    cout << "------------------------ PLATFORM DATA ------------------------" << endl;
    cout << "Platform ID: " << platform_ID << endl;
    cout << "- Balance: $" << balance << endl;
    cout << "- VIP Threshold: $" << VIP_threshold << " (Discount rate: " << (100-VIP_discount*100) << "% off)" << endl;
    cout << "- Subscription Fee: $" << subscription_fee << endl;
    cout << "- Number of shops: " << num_of_shops << endl;
    cout << "----------------------------- END -----------------------------" << endl << endl;
}

void Platform::print_shops(bool show_average) const {
    cout << "-------------------------- SHOPS DATA -------------------------" << endl;
    cout << "Shops of Platform ID: " << platform_ID << endl;
    if (num_of_shops == 0) cout << "- Has no shops." << endl;
    for (int i = 0; i < num_of_shops; i++) {
        if (show_average) {
            cout << i+1 << ". Shop ID: " << shops[i]->get_ID()
                 << " (Balance: $" << shops[i]->get_balance() 
                 << "; Average Rating: " << shops[i]->get_average_score() << ")" << endl;
        } else {
            cout << i+1 << ". Shop ID: " << shops[i]->get_ID()
                 << " (Balance: $" << shops[i]->get_balance() << ")" << endl;
        }
    }
    cout << "----------------------------- END -----------------------------" << endl << endl;
}
