template <class Container>
Payload::Payload(const string &id, string sender_id, string receiver_id, const Container &items, int travel_time, int arrival_time): m_id{id}, m_sender_id{sender_id}, m_receiver_id{receiver_id}, m_items{items.begin(), items.end()}, m_travel_time{travel_time}, m_arrival_time{arrival_time} {

}