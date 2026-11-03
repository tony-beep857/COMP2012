#ifndef SHOP_H
#define SHOP_H

class Platform;
class Customer;
class Product;
class Rating;

class Shop {
    private:
        const int shop_ID;
        int balance;
        Platform* const platform;
        const double cut_rate;
        Product** inventory;
        int num_of_inventory;
        int inventory_capacity;
        Rating* ratings;    
        
    public:
        //To be implemented
        Shop(int shop_ID, Platform* platform = nullptr, double cut_rate = 0);
        ~Shop();
        bool stock_product(Product* product);
        Product* remove_product(int product_ID);
        bool handle_order(const int* order, int order_size, Customer* customer);
        void set_ratings_head(Rating* r);
        const Rating* get_i_th_rating(int i);
        int get_average_score() const; 
        bool remove_rating(int customer_ID);
        void print_ratings() const;
        
        //Given
        int get_ID() const {return shop_ID;}
        int get_balance() const {return balance;}
        int get_platform_ID() const;
        void change_balance(int amount) {balance += amount;}
        int get_num_of_inventory() const {return num_of_inventory;}
        int get_inventory_capacity() const {return inventory_capacity;}
        
        void print() const;
        void print_inventory() const;
        
        //Deleted
        Shop(const Shop&) = delete;
        Shop(Shop&&) = delete;
};

#endif