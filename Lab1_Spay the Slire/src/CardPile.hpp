#ifndef CARD_PILE_HPP
#define CARD_PILE_HPP

#include "Card.hpp"

/**
 * @brief A class that holds a pile (array) of cards.
 *
 * This class holds an array of `Card` object pointers. The array is allocated
 * on the heap (i.e., via `new`) only once during the construction of the
 * object, its max size does not change during its lifetime.
 *
 * We store `Card` pointers instead of `Card` objects themselves since we want
 * to easily store multiple of the same card in the same pile.
 */
class CardPile {
   private:
    // Heap-allocated array of `Card` object pointers.
    Card** cards;
    // The current count of cards in the pile.
    int card_count;
    // The max amount of cards a pile can hold.
    int max_card_count;

   public:
    /**
     * @brief `CardPile` class constructor.
     *
     * __Task 1.1__
     *
     * This constructor allocates an array of `Card` object pointers on the heap
     * (i.e., via `new`) with the max size given by `max_card_count`. This
     * constructor copies the `Card` pointers from the `cards` parameter to this
     * new array.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param cards          Array of `Card` object pointers to copy from.
     * @param card_count     The count of cards found in the `cards` parameter.
     * @param max_card_count The max amount of cards a pile can hold.
     */
    CardPile(Card** cards, int card_count, int max_card_count);

    /**
     * @brief `CardPile` class destructor.
     *
     * __Task 1.2__
     *
     * This destructor deallocates the `cards` data member. Note that this class
     * does not own any of the `Card` object pointers and hence should not
     * deallocate them.
     *
     * Implement this destructor over on 'lab_tasks.cpp'.
     */
    ~CardPile();

    /**
     * @brief Accessor (getter) function for `card_count`.
     *
     * This function has been implemented for you.
     *
     * @return The size of the card pile.
     */
    int get_size() const { return card_count; }

    /**
     * @brief Adds a card to the card pile.
     *
     * __Task 1.3__
     *
     * This function adds a card to the card pile. The new card is added to the
     * end of this card pile.
     *
     * It is assumed that this function will never fail. I.e., whenever this
     * function is called, there will always be enough space in the card pile to
     * add a new card.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param card The card to be added.
     */
    void add_card(Card* card);

    /**
     * @brief Gets a card from the card pile.
     *
     * __Task 1.4__
     *
     * Returns a pointer to the `Card` object specified by the `index`
     * parameter. If `index` is invalid, return `nullptr`.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param index The index to the array of the card to get.
     * @return Pointer to the card we accessed.
     */
    Card* get_card(int index) const;

    /**
     * @brief Removes a card from the card pile.
     *
     * __Task 1.5__
     *
     * Returns a pointer to the `Card` object specified by the `index` parameter
     * and removes it from this card pile. When the card is removed, reorders
     * the remaining `Card` object pointers in this card pile such that they
     * remain in a contiguous sequence in memory. If `index` is invalid, return
     * `nullptr`.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param index The index to the array of the card to remove.
     * @return Pointer to the card we removed.
     */
    Card* remove_card(int index);

    /**
     * @brief Moves all cards from this pile to a target pile.
     *
     * __Task 1.6__
     *
     * This function moves all cards from this card pile to a target card pile.
     * The cards are appended to the end of the target card pile, in the same
     * order as was found in this card pile.
     *
     * It is assumed that this function will never fail. I.e., whenever this
     * function is called, there will always be enough space in the target pile
     * to fit all the cards from this pile.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param target_pile The target card pile to move all cards to.
     */
    void move_cards_to(CardPile& target_pile);
};

#endif
