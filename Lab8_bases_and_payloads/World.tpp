template <class Container>
bool World::create_payload(const string &id, const string &sender_id, const string &receiver_id, const Container&items, int travel_time, int time_until_arrival) {
    // check the ID is unique
    if (m_payloads.find(id) != m_payloads.end())
        return false; // ID can be found

    // the bases' with the specified IDs exist
    map<string, Base *>::const_iterator sender_it = m_bases.find(sender_id);
    map<string, Base *>::const_iterator receiver_it = m_bases.find(receiver_id);
    
    if (sender_it == m_bases.end() || receiver_it == m_bases.end()) 
        return false;

    Base *sender_base = sender_it->second;
    if (!sender_base->can_afford(items)) {
        return false;
    }

    Payload *new_payload = new Payload(id, sender_id, receiver_id, items, travel_time, time_until_arrival);
    *sender_base -= *new_payload;
    m_payloads[id] = new_payload;
    m_payload_queue.push(new_payload);
    return true;
}