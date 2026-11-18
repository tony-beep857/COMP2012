#ifndef STAGE_H
#define STAGE_H

#include <queue>
#include <vector>
#include <utility>
#include <functional>
#include <unordered_set>
#include "MapView.h"
#include "Base.h"
#include "Payload.h"

using std::priority_queue;
using std::vector;
using std::pair;
using std::function;
using std::unordered_set;

class World {
    public:
    using ItemMap = map<string, int>;
    using ItemPair = ItemMap::value_type;
    using BaseMap = map<string, Base *>;
    using BasePair = BaseMap::value_type;
    using PayloadMap = map<string, Payload *>;
    using PayloadPair = PayloadMap::value_type;
    private:
    /**
     * @brief All bases in the stage.
     */
    map<string, Base *> m_bases;
    /**
     * @brief All active payloads in the stage.
     */
    map<string, Payload *> m_payloads;
    int m_time = 0;
    struct GreaterPayload {
        bool operator()(Payload *a, Payload *b) {
            return a->get_arrival_time() > b->get_arrival_time();
        }
    };
    /**
     * @brief Priority queue that treats the payload's arrival time as key,
     * can extract the smallest payload.
     */
    priority_queue<Payload *, vector<Payload *>, GreaterPayload> m_payload_queue;

    
    const MapView<Base> m_base_view;
    const MapView<Payload> m_payload_view;

    public:
    /**
     * @brief Initializes the stage.
     * 
     * @tparam Container An STL container, its value_type is assumed to be Base *,
     * and it is assumed to have begin() and end() for iterating through its value_types.
     * 
     * @param bases A collection of pointer to bases, all bases are assumed to be dynamically allocated, distinct, and have distinct ID.
     */
    template <class Container>
    World(const Container &bases): m_base_view{m_bases}, m_payload_view{m_payloads} {
        for (Base *b: bases)
            m_bases[b->get_id()] = b;
    }
    World(const World &) = delete;
    World(World &&) = delete;

    /**
     * @brief Deallocates all dynamically allocated resources,
     * i.e. all bases and payloads.
     */
    ~World();
    
    const MapView<Base> &get_bases() const { return m_base_view; }
    const MapView<Payload> &get_payloads() const { return m_payload_view; }

    private:
    /**
     * @brief Adds a new payload to the stage, and removes the payload's content from the sender base.
     * 
     * Conditions: its ID is unique, the bases' with the specified IDs exist, the sender base can afford all specified items.
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
     * 
     * @return true if and only if the conditions are met.
     */
    template <class Container>
    bool create_payload(const string &id, const string &sender_id, const string &receiver_id, const Container &items, int travel_time, int arrival_time);

    public:
    /**
     * @brief Refer to @ref create_payload(const string &, const string &, const string &, const Container &, int, int);
     * 
     * but the arrival time is the current time plus the travel time.
     */
    template <class Container>
    bool create_payload(const string &id, const string &sender_id, const string &receiver_id, const Container &items, int travel_time) {
        return create_payload(id, sender_id, receiver_id, items, travel_time, travel_time+m_time);
    }
    
    /**
     * @brief Lazy deletes the payload of specified ID.
     * This means marking the payload as deleted and removing the payload from m_payloads.
     * 
     * Condition: the payload with the speficied ID exists.
     * 
     * @param id ID of the payload.
     * 
     * @returns true if and only if the condition is met.
     */
    bool remove_payload(const string &id);
    
    private:
    /**
     * @brief Processes all payloads that have arrived.
     * Arrived means that the arrival_time of the payload is less than or equal to the current time.
     * 
     * For non-deleted payloads, transfer its content to the receiver base,
     * and print a message given in the skeleton code and webpage should be printed.
     * Make sure that the order of arrival is correct, i.e. payload with smaller arrival time arrives first.
     * 
     * Removes the arrived payloads from m_payloads and m_payload_queue,
     * and deallocates them.
     */
    void process_arrived_payloads();

    public:

    /**
     * @brief Adds time to the current time, m_time,
     * then processes payloads that have arrived.
     * 
     * @param time Amount of time to pass, assumed to be positive.
     */
    void elapse(int time);

    // // important: this should also change the travel time by transformation, and transformation is assumed to be non-decreasing s.t. time until arrival <= travel time
    // bool change_payload_time(const string &id, function<int(int)> transformation);

    /**
     * @brief Given in World.cpp. Do not modify it.
     * 
     * Recalls a payload by lazy deleting the existing one,
     * and reinserting another one with the same ID, but sends item from the receiver base to the sender base.
     * The content and travel time should remain the same, but the arrival time should be set to how long the original payload has travelled pre-recall.
     * 
     * Important: If the payload immediately arrives after this function is called,
     * it should be processed, i.e. base receives item, message from process_arrived_payloads printed.
     * 
     * Condition: The payload of the given ID exists.
     * 
     * @param id ID of the payload.
     * 
     * @return true if and only if the condition is met.
     */
    bool recall_payload(const string &id);

};

#include "World.tpp"

#endif