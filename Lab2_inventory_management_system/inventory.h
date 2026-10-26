#ifndef INVENTORY_H
#define INVENTORY_H

#include "slot.h"
#include <iomanip>


/**
 * @brief The name of the head dummy slot.
 *
 * You may use this like any constant.
 */
static const char* const HEAD_NAME = "@HEAD";
/**
 * @brief The name of the tail dummy slot.
 *
 * You may use this like any constant.
 */
static const char* const TAIL_NAME = "~TAIL";

/// @brief An inventory, i.e. a doubly-linked list of slots with a dummy head and a dummy tail, in the order the slots were added.
class Inventory {
    // data members
    private:
    /**
     * @brief A dummy slot pointing at the first slot. The first slot (Slot 0) is next to this.
     */
    Slot* const m_head;
    /**
     * @brief A dummy slot pointing at the last slot. The last slot is previous to this.
     */
    Slot* const m_tail;
    /**
     * @brief Number of slots currently used.
     */
    int m_count;
    /**
     * @brief Maximum number of slots.
     */
    int m_capacity;

    // constructors and destructor
    public:
    /**
     * @brief Initializes an empty inventory with only the head and tail dummy slots, linked to each other.
     *
     * The names of the head and tail dummy slots should be HEAD_NAME and TAIL_NAME respectively.
     * Their maximum number of items is irrelevant.
     *
     * @param capacity The capacity of the inventory, assumed to be positive.
     */
    explicit Inventory(int capacity);
    /**
     * @brief Initializes the inventory as a deep copy of other: same capacity, and a copy of each of its slots, in the same order.
     *
     * @param other The inventory to copy.
     */
    Inventory(const Inventory& other);
    /**
     * @brief Deallocates all dynamically allocated memory of this inventory, including all of its slots.
     */
    ~Inventory();

    // Assigning an inventory to another one is not needed in this lab.
    Inventory& operator=(const Inventory&) = delete;

    // given functions
    /**
     * @return The number of slots this inventory currently uses.
     */
    int get_count() const { return m_count; }
    /**
     * @return The maximum number of slots the inventory can have.
     */
    int get_capacity() const { return m_capacity; }
    /**
     * @return The head dummy slot of this inventory.
     */
    const Slot* get_head() const { return m_head; }
    /**
     * @return The tail dummy slot of this inventory.
     */
    const Slot* get_tail() const { return m_tail; }

    /**
     * @param i The index of the slot, where m_head->get_next() is the 0-th slot.
     *
     * @return The i-th slot, or nullptr if i is out of bounds.
     */
    const Slot* get_ith_const(int i) const {
        if (i >= m_count || i < 0) return nullptr;
        int curr = 0;
        Slot* return_item = m_head->get_next();
        while (curr < i) {
            curr++;
            return_item = return_item->get_next();
        }
        return return_item;
    }

    private:
    /**
     * @param i The index of the slot, where m_head->get_next() is the 0-th slot.
     *
     * @return The i-th slot, or nullptr if i is out of bounds.
     */
    Slot* get_ith(int i) {
        if (i >= m_count || i < 0) return nullptr;
        int curr = 0;
        Slot* return_item = m_head->get_next();
        while (curr < i) {
            curr++;
            return_item = return_item->get_next();
        }
        return return_item;
    }

    public:
    /**
     * @brief Fills up a single slot.
     *
     * @param index The index of the slot to fill.
     *
     * @param amount The number of items to fill up the slot with, assumed to be positive.
     *
     * @return -1 if index is out of bounds. Otherwise, the number of items not filled because the slot reached its maximum number of items.
     */
    int fill(int index, int amount) {
        Slot* to_fill = get_ith(index);
        if (to_fill == nullptr) {
            return -1;
        }
        return to_fill->fill(amount);
    }

    // debugging

    /**
     * @brief Prints the whole inventory, for debugging: how many slots it uses, then every slot
     * from the head to the tail, one per line, in order.
     *
     * So that it stays usable on a broken inventory, this function never follows a null pointer,
     * and gives up once it has seen more slots than the capacity, instead of looping forever.
     * It says where it stopped in both cases.
     *
     * This function is given only to help you debug, and the tests never call it.
     * Never leave a call to it in the code you hand in: the tests compare what your program prints,
     * so any extra output makes them fail.
     */
    void debug_print() const {
        std::cout << "Inventory: " << m_count << " of " << m_capacity << " slots used\n";
        if (m_head == nullptr || m_tail == nullptr) {
            std::cout << "  the head or the tail is null, there is nothing to walk through\n";
            return;
        }
        std::cout << "  head: ";
        m_head->debug_print();
        // walk forward, stopping at the tail, at a null pointer, or once past the capacity
        const Slot* curr = m_head->get_next();
        int i = 0;
        while (curr != nullptr && curr != m_tail && i <= m_capacity) {
            std::cout << "  " << std::setw(4) << i << ": ";
            curr->debug_print();
            curr = curr->get_next();
            i++;
        }
        if (curr == nullptr)
            std::cout << "  stopped: a next pointer is null before the tail\n";
        else if (curr != m_tail)
            std::cout << "  stopped: still not at the tail after " << i << " slots, more than the capacity\n";
        std::cout << "  tail: ";
        m_tail->debug_print();
    }

    // functions to implement
    /**
     * @brief Adds a new slot holding items with the given name at the end of the inventory, and fills it with amount items.
     *
     * Does nothing if the inventory is full. Remember to update m_count.
     *
     * @param name Name of the items.
     *
     * @param max_items Maximum number of items a slot can hold for these items.
     *
     * @param amount Number of items to fill the new slot with, assumed to be positive.
     *
     * @return -1 if the inventory is full. Otherwise, the number of items not filled because the new slot reached its maximum number of items.
     */
    int add(const char* name, int max_items, int amount);

    /**
     * @brief Takes items from a single slot. If the slot becomes empty, removes it from the inventory and updates m_count.
     *
     * @param index The index of the slot to take from.
     *
     * @param amount The number of items to take from the slot, assumed to be non-negative.
     *
     * @return -1 if index is out of bounds. Otherwise, the number of items not taken because the slot ran out of items.
     */
    int take(int index, int amount);
};

#endif
