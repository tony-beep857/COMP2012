#include <iostream>
#include <string>

#include "Card.hpp"
#include "GameState.hpp"

int main() {
    // Gets input to initialize the kinds of cards.
    std::cout << "How many cards do you want to enter? ";
    int card_count;
    std::cin >> card_count;
    std::cout << std::endl;

    Card** cards = new Card*[card_count];

    for (int i = 0; i < card_count; i++) {
        std::cout << "Data for card #" << i << ":" << std::endl;

        std::cout << "  Card Name (no whitespaces)? ";
        std::string name;
        std::cin >> name;

        std::cout << "  Card Type (0-2)? ";
        CardType card_type;
        int input;
        std::cin >> input;
        card_type = static_cast<CardType>(input);

        std::cout << "  Card Potency? ";
        int potency;
        std::cin >> potency;

        std::cout << "  Card Cost? ";
        int cost;
        std::cin >> cost;

        cards[i] = new Card(name, card_type, potency, cost);
        std::cout << std::endl;
    }

    // Gets input to initialize the Player Deck.
    std::cout << "How many cards do you want to add to the Player's Deck? ";
    int player_card_count;
    std::cin >> player_card_count;
    std::cout << std::endl;

    Card** player_cards = new Card*[player_card_count];

    for (int i = 0; i < player_card_count; i++) {
        std::cout << "  Card #" << i << ": Index of card (0-" << card_count - 1 << ")? ";
        int card_index;
        std::cin >> card_index;
        player_cards[i] = cards[card_index];
    }
    std::cout << std::endl;

    // Gets input to initialize the Enemy Deck.
    std::cout << "How many cards do you want to add to the Enemy's Deck? ";
    int enemy_card_count;
    std::cin >> enemy_card_count;
    std::cout << std::endl;

    Card** enemy_cards = new Card*[enemy_card_count];

    for (int i = 0; i < enemy_card_count; i++) {
        std::cout << "  Card #" << i << ": Index of card (0-" << card_count - 1 << ")? ";
        int card_index;
        std::cin >> card_index;
        enemy_cards[i] = cards[card_index];
    }
    std::cout << std::endl;

    // Initializes the game state.
    GameState game{
        player_cards,
        player_card_count,
        enemy_cards,
        enemy_card_count,
    };

    // The game loop.
    while (true) {
        game.start_player_turn();

        while (true) {
            game.print_game_state();
            game.print_current_hand();

            std::cout << "Other Options:" << std::endl;
            std::cout << "  [e] End your turn." << std::endl;
            std::cout << "  [q] Quit the game." << std::endl;
            std::cout << std::endl;

            std::cout << "What do you want to do? ";
            char player_input;
            if (!(std::cin >> player_input)) {
                delete[] enemy_cards;
                delete[] player_cards;
                for (int i = 0; i < card_count; i++) {
                    delete cards[i];
                }
                delete[] cards;
                return 0;
            }
            std::cout << std::endl;

            if (player_input == 'e') {
                break;
            } else if (player_input == 'q') {
                std::cout << "You quit the game." << std::endl;

                // Deallocates all dynamically allocated objects.
                delete[] enemy_cards;
                delete[] player_cards;
                for (int i = 0; i < card_count; i++) {
                    delete cards[i];
                }
                delete[] cards;

                return 0;
            } else if (game.play_card(player_input - '0')) {
                if (game.get_result() != ONGOING) break;
            }
        }

        if (game.get_result() != ONGOING) break;
        game.start_enemy_turn();
        if (game.get_result() != ONGOING) break;
    }

    if (game.get_result() == PLAYER_WIN) {
        std::cout << "You defeated the Enemy!" << std::endl;
    } else {  // if (game.get_result() == ENEMY_WIN)
        std::cout << "You were defeated by the Enemy..." << std::endl;
    }

    // Deallocates all dynamically allocated objects.
    delete[] enemy_cards;
    delete[] player_cards;
    for (int i = 0; i < card_count; i++) {
        delete cards[i];
    }
    delete[] cards;

    return 0;
}
