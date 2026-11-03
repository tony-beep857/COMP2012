#include <cassert>
#include <iostream>
using namespace std;

#include "Customer.h"
#include "Shop.h"
#include "Platform.h"
#include "Product.h"
#include "CustomerData.h"
#include "Rating.h"

void Test_1() {
    Product* p1 = new Product(1, 10, nullptr);
    Product* p2 = new Product(2, 20);
    p1->print();
    p2->print();
    delete p1;
    delete p2;
}

void Test_2() {
    Product* product = new Product(1, 100, nullptr);
    Product* replica = new Product(*product);
    Product* knockoff = new Product(*replica);
    product->print();
    replica->print();
    knockoff->print();
    delete product;
    delete replica;
    delete knockoff;
}

void Test_3() {
    Customer* customer = new Customer(1);
    assert(customer->get_ID() == 1);
    assert(customer->get_balance() == 0);
    assert(customer->get_num_of_belongings() == 0);
    assert(customer->get_belongings_capacity() == 2);
    customer->print();
    customer->print_belongings();
    delete customer;
}

void Test_4() {
    CustomerData* cd = new CustomerData(0);
    assert(cd->customer_ID == 0);
    assert(cd->amount_spent == 0);
    assert(cd->next == nullptr);
    cd->print();
    cd->amount_spent += 20;
    cd->print();
    delete cd;
}

void Test_5() {
    Shop* shop = new Shop(0);
    assert(shop->get_ID() == 0);
    assert(shop->get_balance() == 0);
    assert(shop->get_num_of_inventory() == 0);
    assert(shop->get_inventory_capacity() == 2);
    assert(shop->get_platform_ID() == -1);
    shop->print();
    shop->print_inventory();
    delete shop;
}

void Test_6() {
    Shop* shop = new Shop(1);
    Product* product = new Product(0, 15, shop);
    Product* replica = new Product(*product);
    
    product->print();
    replica->print();
    
    delete shop;
    delete product;
    delete replica;
}

void Test_7() {
    Customer* customer = new Customer(20);
    Shop* shop = new Shop(12);
    
    //Just testing the constructor, no need to worry about linked list insertion / deallocate in this test case :)
    Rating* r0 = new Rating(customer, shop, 50);
    r0->print(true, true);
    Rating* r1 = new Rating(customer, shop, 100);
    r1->print(true, true);
    
    delete r1;
    
    Rating* r2 = new Rating(customer, shop, 40);
    r2->print(true, true);
    
    delete r0;
    delete r2;
    delete customer;
    delete shop;
}

void Test_8() {
    Platform* platform = new Platform(0, 100, 0.25, 10);
    assert(platform->get_ID() == 0);
    assert(platform->get_balance() == 0);
    assert(platform->get_VIP_threshold() == 100);
    assert(platform->get_VIP_discount() == 0.25);
    assert(platform->get_subscription_fee() == 10);
    assert(platform->get_num_of_shops() == 0);
    assert(platform->get_shops_capacity() == 2);
    platform->print();
    delete platform;
}

void Test_9() {
    Customer* customer = new Customer(5);
    assert(customer->add_product(new Product(10,21)));
    assert(customer->get_num_of_belongings() == 1);
    assert(customer->get_belongings_capacity() == 2);
    customer->print();
    customer->print_belongings();
    delete customer;
}

void Test_10() {
    Customer* customer = new Customer(38);
    int correct_size[3] = {2,2,4};
    for (int i = 0; i < 3; i++) {
        customer->add_product(new Product(12,34));
        assert(customer->get_num_of_belongings() == i + 1);
        assert(customer->get_belongings_capacity() == correct_size[i]);
        customer->print_belongings();
    }
    delete customer;
}

void Test_11() {
    Shop* shop = new Shop(23);
    int ids[5] = {1,3,0,4,2}; 
    int prices[5] = {30,20,10,50,40};
    int correct_size[5] = {2,2,4,4,8};
    for (int i = 0; i < 5; i++) {
        shop->stock_product(new Product(ids[i], prices[i]));
        assert(shop->get_num_of_inventory() == i + 1);
        assert(shop->get_inventory_capacity() == correct_size[i]);
        shop->print_inventory();
    }
    delete shop;
}

void Test_12() {
    Shop* shop = new Shop(14);
    Product* product = new Product(0,10);
    shop->stock_product(product);
    shop->print_inventory();
    
    Product* result = shop->remove_product(0);
    assert(result->get_ID() == product->get_ID());
    assert(result->get_origin_ID() == shop->get_ID());
    assert(shop->get_num_of_inventory() == 0);
    assert(shop->get_inventory_capacity() == 2);
    shop->print_inventory();
    result->print();
    
    delete product;
    delete shop;
}

void Test_13() {
    Shop* shop = new Shop(12);
    for (int i = 0; i < 3; i++) { shop->stock_product(new Product(4-i,i*5)); }
    shop->print_inventory();
    Product* product = shop->remove_product(10);
    assert(product == nullptr);
    shop->print_inventory();
    delete shop;
}

void Test_14() {
    Shop* shop = new Shop(11);
    for (int i = 0; i < 5; i++) { shop->stock_product(new Product(5-i,i*8)); }
    shop->print_inventory();

    Product* temp[5] = {nullptr, nullptr, nullptr, nullptr, nullptr};
    int removal_order[5] = {2,1,4,3,5};
    for (int i = 0; i < 5; i++) {
        temp[i] = shop->remove_product(removal_order[i]);
        assert(temp[i]->get_ID() == removal_order[i]);
    }
    shop->print_inventory();
    
    shop->stock_product(temp[4]);
    shop->stock_product(temp[0]);
    shop->stock_product(temp[2]);
    shop->stock_product(temp[1]);
    shop->stock_product(temp[3]);
    shop->print_inventory();
    
    delete shop;
}

void Test_15() {
    Platform* platform = new Platform(0,500,0.5,100);
    Shop* shop = new Shop(1,platform,0.125);
    assert(shop->get_platform_ID() == platform->get_ID());
    assert(platform->get_num_of_shops() == 1);
    assert(platform->get_shops_capacity() == 2);
    shop->print();
    platform->print();
    platform->print_shops(false);
    delete platform;
}

void Test_16() {
    Platform* platform = new Platform(1,200,0.15,50);
    int correct_size[3] = {2,2,4};
    for (int i = 0; i < 3; i++) {
        Shop* shop = new Shop(i+1, platform, 0.125 * i);
        assert(platform->get_num_of_shops() == i+1);
        assert(platform->get_shops_capacity() == correct_size[i]);
        shop->print();
    }
    platform->print();
    platform->print_shops(false);
    delete platform;
}

void Test_17() {
    Platform* platform = new Platform(1,100,0.5,50);
    Shop* shop = new Shop(2,platform,0.25);
    (void)shop;
    platform->print_shops(false);
    assert(platform->delete_shop(2));
    assert(platform->get_num_of_shops() == 0);
    assert(platform->get_shops_capacity() == 2);
    platform->print_shops(false);
    delete platform;
}

void Test_18() {
    Platform* platform = new Platform(1,600,0.5,100);
    platform->print_customer_data();
    assert(platform->update_customer_data(1,50) == 50);
    platform->print_customer_data();
    delete platform;
}

void Test_19() {
    Platform* platform = new Platform(0,100,0.5,100);
    assert(platform->update_customer_data(0,50) == 50);
    platform->print_customer_data();
    assert(platform->update_customer_data(0,100) == 150); //VIP Time!
    platform->print_customer_data();
    delete platform;
}

void Test_20() {
    Platform* platform = new Platform(1,100,0.75,40);
    for (int i = 0; i < 3; i++) {
        assert(platform->update_customer_data(i+1,i*100+50) == i*100+50);
        platform->print_customer_data();
    }
    delete platform;
}

void Test_21() {
    Platform* platform = new Platform(1,500,0.25,150);
    platform->print_customer_data();
    assert(platform->update_customer_data(2,200) == 200);
    platform->print_customer_data();
    assert(platform->delete_customer_data(2));
    platform->print_customer_data();
    delete platform;
}

void Test_22() {
    Platform* platform = new Platform(1,750,0.5,150);
    for (int i = 0; i < 3; i++) { platform->update_customer_data(i+2,400 - i*100); }
    
    platform->print_customer_data();
    assert(platform->delete_customer_data(3));
    platform->print_customer_data();
    delete platform;
}

void Test_23() {
    Platform* platform = new Platform(1,100,0.75,200);
    assert(platform->update_customer_data(0,10) == 10);
    assert(!platform->check_if_VIP(0));
    assert(platform->update_customer_data(0,89) == 99);
    assert(!platform->check_if_VIP(0));
    assert(platform->update_customer_data(0,1) == 100);
    assert(platform->check_if_VIP(0));
    assert(platform->update_customer_data(0,1) == 101);
    assert(platform->check_if_VIP(0));
    assert(platform->update_customer_data(0,99) == 200);
    assert(platform->check_if_VIP(0));
    delete platform;
    cout << "Test completed." << endl;

}

void Test_24() {
    Customer* customer = new Customer(2);
    Shop* shop = new Shop(1);
    Rating* rating = new Rating(customer, shop, 25);
    shop->set_ratings_head(rating);
    rating->customer = nullptr; //Manual decoupling to ensure destruction during shop destruction
    shop->print_ratings();
    
    delete shop;
    delete customer;
}

void Test_25() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(2);
    customer->add_product(new Product(1,1,shop));
    
    assert(customer->rate_shop(shop,60));
    
    customer->print_ratings();
    shop->print_ratings();
    
    delete customer;
    delete shop;
}

void Test_26() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(0);
    customer->add_product(new Product(20,12,shop));
    
    customer->rate_shop(shop,40);
    
    customer->print_ratings();
    shop->print_ratings();
    
    delete customer;
    shop->print_ratings();
    
    delete shop;
}

void Test_27() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(0);
    customer->add_product(new Product(10,21,shop));
    
    customer->rate_shop(shop,20);
    
    customer->print_ratings();
    shop->print_ratings();
    
    delete shop;
    customer->print_ratings();
    
    delete customer;
}

void Test_28() {
    Customer* customer = new Customer(0);
    Shop** shops = new Shop*[3];
    for (int i = 0; i < 3; i++) {
        shops[i] = new Shop(i);
        customer->add_product(new Product(i,30,shops[i]));
    }
    
    int rate_order[3] = {2,0,1};
    for (int i = 0; i < 3; i++) { assert(customer->rate_shop(shops[rate_order[i]], i * 10)); }
    customer->print_ratings();
    for (int i = 0; i < 3; i++) { shops[i]->print_ratings(); }
    
    for (int i = 0; i < 3; i++) { delete shops[i]; }
    delete [] shops;
    delete customer;
}

void Test_29() {
    Customer** customers = new Customer*[3];
    Shop* shop = new Shop(3);
    for (int i = 0; i < 3; i++) {
        customers[i] = new Customer(i);
        customers[i]->add_product(new Product(i,20,shop));
    }
    
    int rate_order[3] = {0,2,1};
    for (int i = 0; i < 3; i++) { assert(customers[rate_order[i]]->rate_shop(shop, i * 10)); }
    for (int i = 0; i < 3; i++) { customers[i]->print_ratings(); }
    shop->print_ratings();
    
    for (int i = 0; i < 3; i++) { delete customers[i]; }
    delete [] customers;
    delete shop;
}

void Test_30() {
    Customer* customer = new Customer(10);
    Shop** shops = new Shop*[5];
    for (int i = 0; i < 5; i++) {
        shops[i] = new Shop(i);
        customer->add_product(new Product(i,0,shops[i]));
    }
    
    for (int i = 0; i < 5; i++) { assert(customer->rate_shop(shops[i], i * 10)); }
    customer->print_ratings();
    for (int i = 0; i < 5; i++) { shops[i]->print_ratings(); }

    assert(customer->rate_shop(shops[0], 25));
    //cout << "runs until here" << endl;

    customer->print_ratings();
    for (int i = 0; i < 5; i++) { shops[i]->print_ratings(); }
    
    for (int i = 0; i < 5; i++) { delete shops[i]; }
    delete [] shops;
    delete customer;
}

void Test_31() {
    Customer** customers = new Customer*[5];
    Shop* shop = new Shop(10);
    for (int i = 0; i < 5; i++) {
        customers[i] = new Customer(i);
        customers[i]->add_product(new Product(i,0,shop));
    }
    
    for (int i = 0; i < 5; i++) { assert(customers[i]->rate_shop(shop, i * 12)); }
    shop->print_ratings();
    for (int i = 0; i < 5; i++) { customers[i]->print_ratings(); }
    
    assert(customers[0]->rate_shop(shop, 30));
    shop->print_ratings();
    for (int i = 0; i < 5; i++) { customers[i]->print_ratings(); }
    
    for (int i = 0; i < 5; i++) { delete customers[i]; }
    delete [] customers;
    delete shop;
}

void Test_32() {
    Customer* customer = new Customer(2);
    Shop* shop = new Shop(3);
    customer->add_product(new Product(0,10,shop));
    
    assert(customer->get_i_th_rating(0) == nullptr);
    assert(shop->get_i_th_rating(0) == nullptr);
    
    assert(customer->rate_shop(shop,25));

    const Rating* c_r0 = customer->get_i_th_rating(0);
    const Rating* s_r0 = shop->get_i_th_rating(0);
    
    c_r0->print(true, true);
    s_r0->print(true, true);
    assert(c_r0->customer == customer);
    assert(s_r0->shop == shop);
    assert(c_r0 == s_r0);
    
    delete customer;
    delete shop;
}

void Test_33() {
    Shop* shop = new Shop(33);
    assert(shop->get_average_score() == -1);
    delete shop;
    cout << "Test completed." << endl;

}

void Test_34() {
    Customer** customers = new Customer*[3];
    Shop* shop = new Shop(7);
    for (int i = 0; i < 3; i++) {
        customers[i] = new Customer(i);
        customers[i]->add_product(new Product(i,40,shop));
    }
    
    int rate_score[3] = {65,0,10};
    for (int i = 0; i < 3; i++) { assert(customers[i]->rate_shop(shop, rate_score[i])); }
    
    assert(shop->get_average_score() == 25);
    
    for (int i = 0; i < 3; i++) { delete customers[i]; }
    delete [] customers;
    delete shop;
    cout << "Test completed." << endl;

}

void Test_35() {
    Customer* customer = new Customer(0);
    Shop* shop = new Shop(1);
    customer->add_product(new Product(20,12,shop));
    
    assert(customer->rate_shop(shop, 100));
    customer->print_ratings();
    shop->print_ratings();
    
    assert(shop->remove_rating(0));
    customer->print_ratings();
    shop->print_ratings();
    
    delete customer;
    delete shop;
}

void Test_36() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(0);
    customer->add_product(new Product(10,21,shop));
    
    assert(customer->rate_shop(shop,90));
    customer->print_ratings();
    shop->print_ratings();
    
    assert(customer->delete_rating(0));
    customer->print_ratings();
    shop->print_ratings();
    
    assert(customer->rate_shop(shop,100)); //Re-rate to ensure rating_ID is different
    customer->print_ratings();
    shop->print_ratings();
    
    delete customer;
    delete shop;
}

void Test_37() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(1);
    customer->add_product(new Product(20,11,shop));
    
    assert(customer->rate_shop(shop,100));
    customer->print_ratings();
    shop->print_ratings();
    
    delete customer;
    shop->print_ratings();
    
    assert(shop->remove_rating(1)); //Deallocate Rating as well since decoupled from both sides
    shop->print_ratings();
    
    delete shop;
}

void Test_38() {
    Customer* customer = new Customer(0);
    Shop* shop = new Shop(0);
    
    customer->change_balance(5);
    shop->stock_product(new Product(0,5));
    int order[] = {0};
    assert(shop->handle_order(order,1,customer));
    assert(customer->get_balance() == 0);
    //cout<<"runs until here"<< endl;
    assert(customer->get_num_of_belongings() == 1);
    assert(customer->get_belongings_capacity() == 2);
    assert(shop->get_balance() == 5);
    assert(shop->get_num_of_inventory() == 0);
    assert(shop->get_inventory_capacity() == 2);
    customer->print();
    customer->print_belongings();
    shop->print();
    shop->print_inventory();
    
    delete shop;
    delete customer;
}

void Test_39() {
    Customer* customer = new Customer(1);
    Shop* shop = new Shop(1);
    
    customer->change_balance(15);
    shop->stock_product(new Product(0,15));
    
    int order[] = {1};
    assert(!shop->handle_order(order,1,customer));
    
    assert(customer->get_balance() == 15);
    assert(customer->get_num_of_belongings() == 0);
    assert(customer->get_belongings_capacity() == 2);
    assert(shop->get_balance() == 0);
    assert(shop->get_num_of_inventory() == 1);
    assert(shop->get_inventory_capacity() == 2);
    customer->print();
    customer->print_belongings();
    shop->print();
    shop->print_inventory();
    
    delete shop;
    delete customer;
}

void Test_40() {
    Customer* customer = new Customer(0);
    Shop* shop = new Shop(0);
    
    customer->change_balance(5);
    for (int i = 0; i < 3; i++) { shop->stock_product(new Product(i,5)); }
    
    int order[] = {0,1};
    assert(!shop->handle_order(order,2,customer));
    
    assert(customer->get_balance() == 5);
    assert(customer->get_num_of_belongings() == 0);
    assert(customer->get_belongings_capacity() == 2);
    assert(shop->get_balance() == 0);
    assert(shop->get_num_of_inventory() == 3);
    assert(shop->get_inventory_capacity() == 4);
    customer->print();
    customer->print_belongings();
    shop->print();
    shop->print_inventory();
    
    delete shop;
    delete customer;
}

void Test_41() {
    Platform* platform = new Platform(41,400,0.5,15);
    Shop* shop = new Shop(41,platform,0.5);
    
    shop->change_balance(20);
    
    platform->simulate_end_of_month();
    
    platform->print(); platform->print_shops();
    
    delete platform;
}

void Test_42() {
    Platform* platform = new Platform(42,100,0.125,10);
    
    Shop* shop = new Shop(0,platform,0.5);
    (void)shop;
    assert(platform->get_num_of_shops() == 1);
    assert(platform->get_shops_capacity() == 2);

    platform->simulate_end_of_month();
    platform->print(); platform->print_shops();
    assert(platform->get_balance() == 0);
    assert(platform->get_num_of_shops() == 0);
    assert(platform->get_shops_capacity() == 2);
    
    delete platform;
}

void Test_43() {
    Platform* platform = new Platform(1,500,0.25,200);
    Shop* shop = new Shop(1,platform,0.25);
    shop->change_balance(1000);
    Customer** customers = new Customer*[4];
    for (int i = 0; i < 4; i++) {
        customers[i] = new Customer(i);
        customers[i]->add_product(new Product(20,12,shop));
    }
    
    //3 Ratings && avg < 15
    for (int i = 0; i < 3; i++) { customers[i]->rate_shop(shop,5); } //Avg = 5, Score = 15
    platform->simulate_end_of_month();
    assert(platform->get_num_of_shops() == 1);
    assert(platform->get_shops_capacity() == 2);
    shop->print_ratings();
    platform->print(); platform->print_shops();
    
    //4 Ratings && avg > 15
    customers[3]->rate_shop(shop,85); //Avg = 25
    platform->simulate_end_of_month();
    assert(platform->get_num_of_shops() == 1);
    assert(platform->get_shops_capacity() == 2);
    shop->print_ratings();
    platform->print(); platform->print_shops();
    
    //4 Ratings && avg < 15
    customers[3]->rate_shop(shop,5); //Avg = 5
    platform->simulate_end_of_month();
    assert(platform->get_num_of_shops() == 0);
    assert(platform->get_shops_capacity() == 2);
    platform->print(); platform->print_shops();
    
    delete platform;
    for (int i = 0; i < 4; i++) { delete customers[i]; }
    delete [] customers;
}

void Test_44() {
    Platform* platform = new Platform(1,100,0.25,50);
    Customer* customer = new Customer(1);
    Shop** shops = new Shop*[5];
    int scores[5] = {5,50,-1,10,25}; //Shop[2] is not rated
    for (int i = 0; i < 5; i++) {
        shops[i] = new Shop(i, platform, 0.25);
        shops[i]->change_balance(200);
        customer->add_product(new Product(0,50,shops[i]));
        if (i != 2) { customer->rate_shop(shops[i], scores[i]); }
    }
    
    platform->simulate_end_of_month();
    platform->print(); platform->print_shops();
    
    delete platform;
    delete customer;
    delete [] shops;
}

void Execute_Test_Case(int i) {
    switch (i) {
        case 1: Test_1(); break;
        case 2: Test_2(); break;
        case 3: Test_3(); break;
        case 4: Test_4(); break;
        case 5: Test_5(); break;
        case 6: Test_6(); break;
        case 7: Test_7(); break;
        case 8: Test_8(); break;
        case 9: Test_9(); break;
        case 10: Test_10(); break;
        case 11: Test_11(); break;
        case 12: Test_12(); break;
        case 13: Test_13(); break;
        case 14: Test_14(); break;
        case 15: Test_15(); break;
        case 16: Test_16(); break;
        case 17: Test_17(); break;
        case 18: Test_18(); break;
        case 19: Test_19(); break;
        case 20: Test_20(); break;
        case 21: Test_21(); break;
        case 22: Test_22(); break;
        case 23: Test_23(); break;
        case 24: Test_24(); break;
        case 25: Test_25(); break;
        case 26: Test_26(); break;
        case 27: Test_27(); break;
        case 28: Test_28(); break;
        case 29: Test_29(); break;
        case 30: Test_30(); break;
        case 31: Test_31(); break;
        case 32: Test_32(); break;
        case 33: Test_33(); break;
        case 34: Test_34(); break;
        case 35: Test_35(); break;
        case 36: Test_36(); break;
        case 37: Test_37(); break;
        case 38: Test_38(); break;
        case 39: Test_39(); break;
        case 40: Test_40(); break;
        case 41: Test_41(); break;
        case 42: Test_42(); break;
        case 43: Test_43(); break;
        case 44: Test_44(); break;
        default: cout << "Not a Test Case" << endl;
    }
}

//-fsanitize=address,leak,undefined
int main() {
    int index;
    cin >> index; //Enter the case number!
    Execute_Test_Case(index);
    
    return 0;
}