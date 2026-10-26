#ifndef RATING_H
#define RATING_H

class Customer;
class Shop;

class Rating {
    private:
        static int rating_count;
        
    public:
        const int rating_ID;
        
        Customer* customer;
        Shop* shop;
        int score;
        
        const int customer_ID; // ID data for printing / searching
        const int shop_ID;
    
        Rating* customer_next; // Points to the next Rating node along the Customer's linked list
        Rating* shop_next; // Points to the next Rating node along the Shop's linked list
        
        //To be implemented
        Rating(Customer* customer, Shop* shop, int score);
            
        //Given
        ~Rating() = default;
        void print(bool show_customer, bool show_shop) const;
        
        //Deleted
        Rating(const Rating&) = delete;
        Rating(Rating&&) = delete;
};

#endif