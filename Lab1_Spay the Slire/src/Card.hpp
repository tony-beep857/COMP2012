#ifndef CARD_HPP
#define CARD_HPP

#include <string>

// Forward declaration of `GameState`. Definition can be found at 'GameState.hpp'.
class GameState;

/**
 * @brief An enum describing the type of actions a card can perform.
 */
enum CardType {
    // Deals X amount of damage to target.
    DEAL_DAMAGE,
    // Caster gains X amount of block.
    GAIN_BLOCK,
    // Caster draws X cards from the Deck (can only be played by the Player).
    DRAW_CARDS,
};

/**
 * @brief A card class that can be played by both the Player and the Enemy.
 *
 * This class represents a card that can be played, either by the Player or by
 * the Enemy. A card consists of its name, energy cost, and the action that can
 * be done when the card is played. The action itself is split into the type of
 * action, the "card type", and its "potency" which signals how strong the
 * action is.
 */
class Card {
   private:
    // The name of the card.
    std::string name;
    // The type of action the card can do.
    CardType card_type;
    // The strength of the action the card can do.
    int potency;
    // The energy cost of the card.
    int cost;

   public:
    /**
     * @brief `Card` class constructor.
     *
     * This constructor has been implemented for you.
     *
     * @param name      The name of the card.
     * @param card_type The type of action the card can do.
     * @param potency   The strength of the action the card can do.
     * @param cost      The energy cost of the card.
     */
    Card(std::string name, CardType card_type, int potency, int cost)
        : name(name), card_type(card_type), potency(potency), cost(cost) {}

    /**
     * @brief Accessor (getter) function for `name`.
     *
     * This function has been implemented for you.
     *
     * @return The name of the card.
     */
    std::string get_name() const { return name; }

    /**
     * @brief Accessor (getter) function for `card_type`.
     *
     * This function has been implemented for you.
     *
     * @return The action type of the card.
     */
    CardType get_card_type() const { return card_type; }

    /**
     * @brief Accessor (getter) function for `potency`.
     *
     * This function has been implemented for you.
     *
     * @return The potency of the card action.
     */
    int get_potency() const { return potency; }

    /**
     * @brief Accessor (getter) function for `cost`.
     *
     * This function has been implemented for you.
     *
     * @return The energy cost of the card.
     */
    int get_cost() const { return cost; }

    /**
     * @brief Deals damage to the target.
     *
     * __Task 2.1__
     *
     * When this function is called, deals damage to the target based on the
     * card's potency. If it is currently the Player's
     * turn, the caster is the Player and the target is the Enemy, and vice
     * versa if it is currently the Enemy's turn.
     *
     * Damage is calculated by the sum of the card's potency. When damage is
     * dealt, first, reduce the target's block by the amount of damage
     * calculated, up until block reaches 0. If the amount of block is not enough,
     * then, reduce the target's health by the remaining amount of damage.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param game_state The current game state which you are meant to modify.
     */
    void deal_damage(GameState* game_state) const;

    /**
     * @brief Gives block to the caster.
     *
     * __Task 2.2__
     *
     * When this function is called, the caster gains block based on the card's
     * potency. If it is currently the Player's turn, the caster is the Player,
     * and vice versa if it is currently the Enemy's turn.
     *
     * The amount of block gained equals to the card's potency.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param game_state The current game state which you are meant to modify.
     */
    void gain_block(GameState* game_state) const;

    /**
     * @brief Draws card(s) from the Draw Pile to the Player's Hand.
     *
     * When this function is called, the Player draws card(s) from the Draw Pile
     * to the Player's Hand equal to the card's potency. It is assumed that only
     * the Player can play any cards with this effect, i.e., the Enemy will
     * never hold any cards with the card type `DRAW_CARDS`.
     *
     * This function has been implemented for you over on 'lab_tasks.cpp'.
     *
     * @param game_state The current game state which you are meant to modify.
     */
    void draw_cards(GameState* game_state) const;

    /**
     * @brief Plays the card, which performs the corresponding action.
     *
     * This function switches upon the card's type to call the corresponding
     * function that performs that specific action.
     *
     * This function has been implemented for you.
     *
     * @param game_state The current game state which you are meant to modify.
     */
    void play_card(GameState* game_state) const {
        switch (card_type) {
            case DEAL_DAMAGE:
                return deal_damage(game_state);
            case GAIN_BLOCK:
                return gain_block(game_state);
            case DRAW_CARDS:
                return draw_cards(game_state);
            default:
                return;
        }
    }

    /**
     * @brief Generates a description for the card.
     *
     * This function has been implemented for you.
     *
     * @return The card's description.
     */
    std::string get_description() const {
        switch (card_type) {
            case DEAL_DAMAGE:
                return "Deal " + std::to_string(potency) + " damage.";
            case GAIN_BLOCK:
                return "Gain " + std::to_string(potency) + " block.";
            case DRAW_CARDS:
                return "Draw " + std::to_string(potency) + " card(s).";
            default:
                return "";
        }
    }
};

#endif
