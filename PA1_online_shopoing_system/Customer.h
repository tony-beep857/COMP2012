#ifndef CUSTOMER_H
#define CUSTOMER_H

class Shop;
class Product;
class Rating;

class Customer {
    private:
        const int customer_ID;
        int balance;
        Product** belongings;
        int num_of_belongings;
        int belongings_capacity;
        Rating* ratings;
        
    public:
        //To be implemented
        Customer(int customer_ID);
        ~Customer();
        bool add_product(Product* product);
        bool rate_shop(Shop* shop, int score);
        const Rating* get_i_th_rating(int i);
        bool delete_rating(int shop_ID);
        void print_ratings() const;

        //Given
        int get_ID() const {return customer_ID;}
        int get_balance() const {return balance;}
        void change_balance(int amount) {balance += amount;}
        int get_num_of_belongings() const {return num_of_belongings;}
        int get_belongings_capacity() const {return belongings_capacity;}
        
        void print() const;
        void print_belongings() const;
        
        //Deleted
        Customer(const Customer&) = delete;
        Customer(Customer&&) = delete;
};

#endif