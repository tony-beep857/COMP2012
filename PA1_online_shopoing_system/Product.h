#ifndef PRODUCT_H
#define PRODUCT_H

class Shop;

class Product {
    private:
        const int product_ID;
        const int price;
        int origin_ID; //-1 if not associated with a shop

    public:
        //To be implemented
        Product(int product_ID, int price, Shop* shop = nullptr);
        Product(const Product& other);

        //Given
        ~Product() = default;
        int get_ID() const {return product_ID;}
        int get_price() const {return price;}
        int get_origin_ID() const {return origin_ID;}
        void set_origin_ID(Shop* shop);
        void print() const;

        //Deleted
        Product(Product&&) = delete;
};

#endif