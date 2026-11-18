#include "World.h"
#include "Base.h"
#include "Payload.h"
#include <iostream>
using std::cout;

World::~World() {
    for (map<string, Base *>::iterator it = m_bases.begin(); it != m_bases.end(); ++it) {
        delete it->second;
    }

    while (!m_payload_queue.empty()) {
        Payload *p = m_payload_queue.top();
        m_payload_queue.pop();
        delete p;
    }
}

bool World::remove_payload(const string &id) {
    World::PayloadMap::iterator it = m_payloads.find(id);
    if (it == m_payloads.end())
        return false;
    
    it->second->lazy_delete();
    m_payloads.erase(it);
    return true;
}

void World::elapse(int time) {
    m_time += time;
    process_arrived_payloads();
}

void World::process_arrived_payloads() {
    while (!m_payload_queue.empty()) {
        Payload *p = m_payload_queue.top();
        
        if (p->get_arrival_time() > m_time) {
            break;
        }
        
        m_payload_queue.pop();
        
        if (!p->get_deleted()) {
            
            map<string, Base *>::iterator receiver_it = m_bases.find(p->get_receiver_id());
            if (receiver_it != m_bases.end()) {
                Base *receiver = receiver_it->second;
                *receiver += *p;
            }
            
            std::cout << "Payload " << p->get_id() << " has arrived.\n";
            
            m_payloads.erase(p->get_id());
        }
        delete p;
    }
}

// Given. You do not need to modify this function.
bool World::recall_payload(const string &id) {
    if (m_payloads.find(id) == m_payloads.end()) return false; // payload does not exist

    // back up the payload
    Payload *p = m_payloads[id];
    remove_payload(id); // this function should not cause the payload to actually get deleted

    // Re-add the items to the receiver before create_payload deducts them from the new sender.
    *m_bases[p->get_receiver_id()] += *p;

    create_payload(id, p->get_receiver_id(), p->get_sender_id(), p->get_items(), p->get_travel_time(), m_time + m_time - (p->get_arrival_time() - p->get_travel_time()));
    process_arrived_payloads();
    return true;
}