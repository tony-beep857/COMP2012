#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H

struct CustomerData {
    const int customer_ID;
    int amount_spent;
    CustomerData* next;
    
    //To be implemented    
    CustomerData(int customer_ID);
    
    //Given
    ~CustomerData() = default;
    void print() const;
    
    //Deleted
    CustomerData(const CustomerData&) = delete;
    CustomerData(CustomerData&&) = delete;
};

#endif