#include "inventory.h"
#include <cstring>
using namespace std;

// m_head and m_tail are const pointers, so they must be initialized in the member initializer list.
// The nullptr values below are placeholders so that the skeleton compiles: replace them.
Inventory::Inventory(int capacity): m_head{new Slot(HEAD_NAME, 1, nullptr)}, m_tail{new Slot(TAIL_NAME, 1, m_head)} {
    m_count = 0;
    m_capacity = capacity;
}

// Same as above: replace the placeholder initializer list.
Inventory::Inventory(const Inventory& other): m_head{new Slot(HEAD_NAME, 1, nullptr)}, m_tail{new Slot(TAIL_NAME, 1, m_head)} {
    m_count = other.get_count();
    m_capacity = other.get_capacity();
    
    Slot* curr = other.m_head->get_next();

    while(curr != other.m_tail){
        new Slot(*curr, m_tail->get_prev());
        curr = curr->get_next();
    }
    
}

Inventory::~Inventory() {
    Slot* curr = m_head;
    while(curr != nullptr){
        Slot* next = curr->get_next();
        delete curr;
        curr = next;
    }
}

int Inventory::add(const char* name, int max_items, int amount) {
    if (m_count == m_capacity)
        return -1;
    
    Slot* newSlot = new Slot(name, max_items, m_tail->get_prev());
    m_count++;

    return newSlot->fill(amount);
}

int Inventory::take(int index, int amount) {

    Slot* target = get_ith(index);
    if (target == nullptr)
        return -1;
    
    int notTaken = target->take(amount);

    if (target->get_count() == 0){
    delete target;
    m_count--;
    }
    
    return notTaken;  

}
