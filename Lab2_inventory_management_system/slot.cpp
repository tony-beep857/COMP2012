#include "slot.h"
#include <cstring>
using namespace std;


Slot::Slot(const char* name, int max_items, Slot* prev) {
    m_count = 0;
    m_name = new char[strlen(name)+1];
    strcpy(m_name,name);
    m_max_items = max_items;
    if (prev != nullptr && prev->m_next == nullptr){
        m_prev = prev;
        prev->m_next = this;
        m_next = nullptr;
    } 
    else if (prev == nullptr){
        m_prev = nullptr;
        m_next = nullptr;
    }
    else if (prev->m_next != nullptr){
        m_prev = prev;
        m_next = prev->m_next;
        m_prev->m_next = this;
        m_next->m_prev = this;
    }
}

Slot::Slot(const Slot& other, Slot* prev) 
    : Slot(other.m_name, other.m_max_items, prev) { 

    m_count = other.m_count;
}

Slot::~Slot() {
    if (m_prev == nullptr && m_next != nullptr){
        m_next->m_prev = nullptr;
    }
    else if (m_prev != nullptr && m_next == nullptr){
        m_prev->m_next = nullptr;
    }
    else if (m_prev != nullptr && m_next != nullptr){
        m_prev->m_next = this->m_next;
        m_next->m_prev = this->m_prev;
    }
    delete[] m_name;
}
