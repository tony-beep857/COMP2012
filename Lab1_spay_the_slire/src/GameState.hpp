#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <iostream>

#include "Card.hpp"
#include "CardPile.hpp"
#include "Entity.hpp"

// The max card pile size for both the Draw Pile and the Discard Pile.
const int MAX_DECK_SIZE = 30;
// The max card pile size for the Hand.
const int MAX_HAND_SIZE = 10;
// The amount of energy the Player has on the start of turn.
const int PLAYER_ENERGY_ON_TURN_START = 3;
// The number of cards the Player draws on the start of turn.
const int CARD_DRAW_ON_TURN_START = 5;
// The max card pile size for the Enemy's Deck.
const int MAX_ENEMY_CARD_COUNT = 5;

/**
 * @brief An enum describing the result of the game (if any).
 */
enum GameResult {
    // The game is still ongoing and no result has been set.
    ONGOING,
    // The Player has won, the Enemy's health has reached 0 or lower.
    PLAYER_WIN,
    // The Enemy has won, the Player's health has reached 0 or lower.
    ENEMY_WIN,
};

/**
 * @brief A class that represents the game's current state.
 *
 * This class represents the game state. If a function wants to perform some
 * action on the game, we can pass the `GameState` object to it as an argument.
 * Ideally, there should only ever exist one and only one object of this class
 * type in use.
 */
class GameState {
   private:
    // The Player entity.
    Entity player{100, 0};
    // The Player's remaining energy available.
    int player_energy;

    // The Player's Draw Pile.
    CardPile draw_pile;
    // The Player's Discard Pile.
    CardPile discard_pile{nullptr, 0, MAX_DECK_SIZE};
    // The Player's Hand.
    CardPile hand{nullptr, 0, MAX_HAND_SIZE};

    // The Enemy entity.
    Entity enemy{50, 0};
    // The Enemy's Deck.
    CardPile enemy_deck;

    // The current turn count (the Player and Enemy turns are counted as one turn combined).
    int turn_count = 0;
    // Whether or not it is currently the Player's turn.
    bool is_player_turn = true;

   public:
    /**
     * @brief `GameState` class constructor.
     *
     * Initializes the Draw Pile and the Enemy's Deck. The rest of the data
     * members have been initialized above.
     *
     * This constructor has been implemented for you.
     *
     * @param player_cards      Array of `Card` object pointers to copy to the Draw Pile.
     * @param player_card_count The count of cards found in the `player_cards` parameter.
     * @param enemy_cards       Array of `Card` object pointers to copy to the Enemy's Deck.
     * @param enemy_card_count  The count of cards found in the `enemy_cards` parameter.
     */
    GameState(Card** player_cards, int player_card_count, Card** enemy_cards, int enemy_card_count)
        : draw_pile(player_cards, player_card_count, MAX_DECK_SIZE),
          enemy_deck(enemy_cards, enemy_card_count, MAX_ENEMY_CARD_COUNT) {}

    /**
     * @brief Accessor (getter) function for `player`.
     *
     * This function has been implemented for you.
     *
     * @return A pointer to the Player entity to modify.
     */
    Entity* get_player() { return &player; }

    /**
     * @brief Accessor (getter) function for `enemy`.
     *
     * This function has been implemented for you.
     *
     * @return A pointer to the Enemy entity to modify.
     */
    Entity* get_enemy() { return &enemy; }

    /**
     * @brief Accessor (getter) function for `is_player_turn`.
     *
     * This function has been implemented for you.
     *
     * @return Whether or not it is currently the Player's turn.
     */
    bool get_is_player_turn() const { return is_player_turn; }

    /**
     * @brief Starts the Player's turn.
     *
     * When this function is called, the Player's remaining energy is reset, and
     * their Hand is discarded then redrawn.
     *
     * This function has been implemented for you.
     */
    void start_player_turn() {
        is_player_turn = true;
        player_energy = PLAYER_ENERGY_ON_TURN_START;
        std::cout << "Player's Turn:" << std::endl;
        std::cout << std::endl;

        std::cout << "Hand Setup:" << std::endl;
        while (hand.get_size() > 0) {
            discard_card(0);
        }
        for (int i = 0; i < CARD_DRAW_ON_TURN_START; i++) {
            draw_card();
        }

        std::cout << std::endl;
    }

    /**
     * @brief Starts the Enemy's turn.
     *
     * When this function is called, the Enemy plays one of their cards. The
     * card chosen is based on the order in which it appears in the Enemy's Deck
     * until it cycles back once the last card of said Deck is played.
     *
     * This function has been implemented for you.
     */
    void start_enemy_turn() {
        is_player_turn = false;
        std::cout << "Enemy's Turn:" << std::endl;

        const Card* enemy_card = enemy_deck.get_card(turn_count % enemy_deck.get_size());
        enemy_card->play_card(this);

        std::cout << std::endl;
        turn_count++;
    }

    /**
     * @brief Draws a card from the Draw Pile.
     *
     * When this function is called, it first ensures the Hand is not full and
     * that the Draw Pile is not empty. If the Draw Pile is empty, it moves the
     * cards from the Discard Pile back into the Draw Pile.
     *
     * Note that we do not actually shuffle (i.e., randomize) the Deck when we
     * move the cards from the Discard Pile back into the Draw Pile. This is to
     * ensure that the order of cards is predictable for easier testing.
     *
     * This function moves the first card in the Draw Pile (i.e., index 0), and
     * adds it to the Hand.
     *
     * This function has been implemented for you.
     *
     * @return Whether or not the function succeeded or not.
     */
    bool draw_card() {
        if (hand.get_size() >= MAX_HAND_SIZE) {
            std::cout << "  Hand is full, cannot draw more cards." << std::endl;
            return false;
        }

        if (draw_pile.get_size() <= 0) {
            std::cout << "  Draw Pile is empty, reshuffling the Deck..." << std::endl;
            discard_pile.move_cards_to(draw_pile);
        }

        Card* card = draw_pile.remove_card(0);
        hand.add_card(card);

        std::cout << "  Drew " << card->get_name() << "." << std::endl;
        return true;
    }

    /**
     * @brief Plays a card from the Hand, then discards it.
     *
     * When this function is called, it first checks for any invalid input. It
     * then checks that the Player has enough energy to play the card in the
     * first place.
     *
     * If the checks passed, energy is taken from the Player's remaining energy
     * and the card is played. Once the card is finished being played, it is
     * then discarded to the Discard Pile.
     *
     * This function has been implemented for you.
     *
     * @param card_index The index of the chosen card in the Hand.
     * @return Whether or not the function succeeded or not.
     */
    bool play_card(int card_index) {
        if (card_index < 0 || card_index >= hand.get_size()) {
            std::cout << "Invalid input." << std::endl;
            std::cout << std::endl;
            return false;
        }

        Card* card = hand.get_card(card_index);
        if (player_energy < card->get_cost()) {
            std::cout << "You do not have enough energy to play this card." << std::endl;
            std::cout << std::endl;
            return false;
        }
        player_energy -= card->get_cost();

        std::cout << "You played " << card->get_name() << "!" << std::endl;
        discard_card(card_index);
        card->play_card(this);

        
        std::cout << std::endl;
        return true;
    }

    /**
     * @brief Discards a card from the Hand.
     *
     * When this function is called, it removes the designated card from the
     * Hand, then adds it to the Discard Pile.
     *
     * This function has been implemented for you.
     *
     * @param card_index The index of the chosen card in the Hand.
     * @return Whether or not the function succeeded or not.
     */
    bool discard_card(int card_index) {
        if (card_index < 0 || card_index >= hand.get_size()) return false;

        Card* card = hand.remove_card(card_index);
        discard_pile.add_card(card);

        std::cout << "  Discarded " << card->get_name() << "." << std::endl;
        return true;
    }

    /**
     * @brief Gets the result of the game (if any is available).
     *
     * When this function is called, it checks if either one of the Player or
     * the Enemy's health has reached 0 or lower. If one of them does, we can
     * determine the winner. If not, we say that the game is still ongoing.
     *
     * This function has been implemented for you.
     *
     * @return The result of the game (if any).
     */
    GameResult get_result() const {
        if (enemy.health <= 0) {
            return PLAYER_WIN;
        } else if (player.health <= 0) {
            return ENEMY_WIN;
        } else {
            return ONGOING;
        }
    }

    /**
     * @brief Prints the current game's state.
     *
     * Prints useful information about the game's state for the Player. This
     * includes the Player and Enemy stats, the Enemy's plan for the next turn,
     * and the Player's remaining energy left.
     *
     * This function has been implemented for you.
     */
    void print_game_state() const {
        std::cout << "Current Game State:" << std::endl;
        std::cout << "  Player - " << player.get_stats() << std::endl;
        std::cout << "  Enemy - " << enemy.get_stats() << std::endl;
        const Card* next_enemy_card = enemy_deck.get_card(turn_count % enemy_deck.get_size());
        std::cout << "  Enemy's Next Turn: " << next_enemy_card->get_name() << " - "
                  << next_enemy_card->get_description() << std::endl;
        std::cout << "  Remaining Energy: " << player_energy << std::endl;
        std::cout << std::endl;
    }

    /**
     * @brief Prints the list of cards currently in the Player's Hand.
     *
     * Prints the list of cards currently in the Player's Hand, including its
     * energy cost, its description, and its index in the Hand.
     *
     * This function has been implemented for you.
     */
    void print_current_hand() const {
        std::cout << "Current Hand:" << std::endl;
        for (int i = 0; i < hand.get_size(); i++) {
            const Card* card = hand.get_card(i);
            std::cout << "  [" << i << "] " << card->get_name() << " (" << card->get_cost() << " cost) - "
                      << card->get_description() << std::endl;
        }
        std::cout << std::endl;
    }
};

#endif
