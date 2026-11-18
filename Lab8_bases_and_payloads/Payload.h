#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <utility>
#include <map>
#include <string>
#include <vector>

using std::map;
using std::pair;
using std::string;
using std::vector;

class Payload {
    public:
    using ItemMap = map<string, int>;
    using ItemPair = ItemMap::value_type;

    private:
    const string m_id;
    /**
     * @brief ID of the base that sends the payaload.
     */
    string m_sender_id;
    /**
     * @brief ID of the base that receives the payload.
     */
    string m_receiver_id;
    /**
     * @brief Content of the payload.
     */
    ItemMap m_items;
    /**
     * @brief How long it takes to travel from the sender to the receiver base.
     */
    int m_travel_time;

    /**
     * @brief Time when the payload will arrive at the receiver base.
     */
    const int m_arrival_time;
    private:
    /**
     * @brief true if the base has been lazy deleted.
     */
    bool m_deleted = false;
    

    public:
    /**
     * @brief Initializes the members.
     * 
     * @tparam Container An STL container, its value_type is assumed to be ItemPair,
     * and it is assumed to have begin() and end() for iterating through its value_types.
     * 
     * @param id ID of the payload.
     * 
     * @param sender_id ID of the base which will send the payload.
     * 
     * @param receiver_id ID of the base which will receive the payload. You may assume that both base parameters will not be the same.
     * 
     * @param items A collection of items to send, each item's name is assumed to be distinct, and the quantity is assumed to be positive.
     * 
     * @param travel_time The time it takes for the payload to travel to the receiving base, assumed to be positive.
     * 
     * @param arrival_time Time in which the payload reaches the receiving base, assumed to be positive.
     */ 
    template <class Container>
    Payload(const string &id, string sender_id, string receiver_id, const Container &items, int travel_time, int arrival_time);
    Payload(const Payload &) = default;
    Payload(Payload &&) = default;

    /**
     * @return The ID of the payload.
     */
    const string &get_id() const { return m_id; }
    /**
     * @return The ID of the base which sent the payload.
     */
    const string &get_sender_id() const { return m_sender_id; }
    /**
     * @return The ID of the base which will receive the payload.
     */
    const string &get_receiver_id() const { return m_receiver_id; }
    /**
     * @return The map of items this payload stores.
     */
    const ItemMap &get_items() const { return m_items; }
    /**
     * @return How long it takes to travel from the sender base to the receiver base.
     */
    int get_travel_time() const { return m_travel_time; }
    /**
     * @return The time in which the payload arrives at the receiver base.
     */
    int get_arrival_time() const { return m_arrival_time; }

    /**
     * @brief Marks the payload as deleted.
     */
    void lazy_delete() {
        m_deleted = true;
    }

    /**
     * @return true if the payload is lazy deleted.
     */
    bool get_deleted() const { return m_deleted; }
};

#include "Payload.tpp"

#endif