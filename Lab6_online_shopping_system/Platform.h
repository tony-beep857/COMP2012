#ifndef PLATFORM_H
#define PLATFORM_H

class Customer;
class Shop;
struct CustomerData;

class Platform {
    private:
        const int platform_ID;
        int balance;
        int VIP_threshold;
        double VIP_discount;
        int subscription_fee;
        Shop** shops;
        int num_of_shops;
        int shops_capacity;
        CustomerData* customer_data;
    
    public:
        //To be implemented
        Platform(int platform_ID, int VIP_threshold, double VIP_discount, int subscription_fee);
        ~Platform();
        bool add_shop(Shop* shop);
        bool delete_shop(int shop_ID);
        bool check_if_VIP(int customer_ID) const;
        int update_customer_data(int customer_ID, int amount);
        bool delete_customer_data(int customer_ID);
        void simulate_end_of_month();
        void print_customer_data() const;
        
        //Provided
        int get_ID() const {return platform_ID;}
        int get_balance() const {return balance;}
        void change_balance(int amount) {balance += amount;}
        int get_VIP_threshold() const {return VIP_threshold;}
        void set_VIP_threshold(int amount) {VIP_threshold = amount;}
        double get_VIP_discount() const {return VIP_discount;}
        void set_VIP_discount(double rate) {VIP_discount = rate;}
        int get_subscription_fee() const {return subscription_fee;}
        void set_subscription_fee(int amount) {subscription_fee = amount;}
        int get_num_of_shops() const {return num_of_shops;}
        int get_shops_capacity() const {return shops_capacity;}
        
        void print() const;
        void print_shops(bool show_average = true) const;
        
        //Deleted
        Platform(const Platform&) = delete;
        Platform(Platform&&) = delete;
};

#endif