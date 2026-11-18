#ifndef BASE_H
#define BASE_H

#include <map>
#include <string>
#include <utility>
#include <vector>

using std::map;
using std::string;
using std::pair;
using std::initializer_list;
using std::vector;

class Payload;

class Base {
    public:
    using ItemMap = map<string, int>;
    using ItemPair = ItemMap::value_type;
    private:
    const string m_id;
    /**
     * @brief The list of items.
     * The invariant that any items' quantity is positive should be held.
     */
    ItemMap m_items;

    public:
    /**
     * @brief Initializes data members.
     * 
     * @tparam Container An STL container, its value_type is assumed to be ItemPair,
     * and it is assumed to have begin() and end() for iterating through its value_types.
     * 
     * @param id ID of the base.
     * 
     * @param initial_items Items that should be in the base initially.
     * Items with non-positive quantity will not be considered.
     */
    template <class Container>
    Base(const string &id, const Container &initial_items);
    Base(const Base &) = default;
    Base(Base &&) = default;

    /**
     * @return The ID of the base.
     */
    const string &get_id() const { return m_id; }
    /**
     * @return A const reference to the item map.
     */
    const ItemMap &get_items() const { return m_items; }

    /**
     * @brief Adds an item to the base.
     * Adding an item with non-positive quantity does nothing
     * 
     * @param arg Name and quantity of item to be added.
     */
    void add_item(ItemPair arg);

    /**
     * @param item_name Name of the item.
     * 
     * @return 0 if the item is not in the base.
     * Otherwise, the item's quantity.
     */
    int operator [](const string &item_name) const;
    
    /**
     * @brief Receives the content of a payload.
     * 
     * @param payload The payload to receive from.
     * 
     * @return Reference to the base itself.
     */
    Base &operator +=(const Payload &payload);

    template <class Container>
    bool can_afford(const Container &items) const;

    /**
     * @brief Sends items to a payload.
     * Removes items with 0 quantity.
     * 
     * @param payload The payload to send to.
     * 
     * @return Reference to the base itself.
     * 
     * @throw std::runtime_error Thrown if the base cannot afford the content of the payload.
     */
    Base &operator -=(const Payload &payload);

};

#include "Base.tpp"

#endif