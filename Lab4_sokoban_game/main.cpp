#include <iostream>

#include "GameObject.hpp"
#include "GameBoard.hpp"
#include "objects.hpp"

/**
 * @brief Renders the entire screen once.
 *
 * Both `screen` and `current_priority` are 2d grids with width `3 * width`
 * (remember that sprites are 3 chars wide) and height `height`.
 *
 * When this function is called, it iterates over all objects and calls their
 * respective `render()` functions. It then prints all of the chars in the
 * `screen` variable onto the terminal.
 *
 * This function has been implemented for you.
 *
 * @param board            The board containing all the game objects.
 * @param screen           A grid of chars that will be printed on the terminal.
 * @param current_priority A grid of priorities that help manage overlapping sprites.
 * @param width            The width of the game grid.
 * @param height           The height of the game grid.
 */
void render_screen(const GameBoard& board, char** screen, int** current_priority, int width, int height) {
    // Resets the screen to an empty slate.
    for (int x = 0; x < 3 * width; x++) {
        for (int y = 0; y < height; y++) {
            screen[x][y] = ' ';
            current_priority[x][y] = 0;
        }
    }

    // Iterates over all objects in the game.
    for (int i = 0; i < board.get_object_count(); i++) {
        board.get_object(i)->render(screen, current_priority, width, height);
    }

    // Prints the screen onto the terminal.
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < 3 * width; x++) {
            std::cout << screen[x][y];
        }
        std::cout << std::endl;
    }
}

int main() {
    // Gets input to initialize the game grid.
    int width;
    int height;
    std::cout << "Width of the game grid? ";
    std::cin >> width;
    std::cout << "Height of the game grid? ";
    std::cin >> height;
    std::cout << std::endl;

    char** screen = new char*[3 * width];
    int** priority = new int*[3 * width];
    for (int x = 0; x < 3 * width; x++) {
        screen[x] = new char[height];
        priority[x] = new int[height];
    }

    // Gets input to initialize the wall objects.
    std::cout << "How many walls to add? ";
    int wall_count;
    std::cin >> wall_count;
    std::cout << std::endl;
    Wall** walls = new Wall*[wall_count];

    for (int i = 0; i < wall_count; i++) {
        int x;
        int y;
        std::cout << "  Wall #" << i << ": X-coordinate value? ";
        std::cin >> x;
        std::cout << "  Wall #" << i << ": Y-coordinate value? ";
        std::cin >> y;
        walls[i] = new Wall(x, y);
    }
    std::cout << std::endl;

    // Gets input to initialize the box objects.
    std::cout << "How many boxes to add? ";
    int box_count;
    std::cin >> box_count;
    std::cout << std::endl;
    Box** boxes = new Box*[box_count];

    for (int i = 0; i < box_count; i++) {
        int x;
        int y;
        std::cout << "  Box #" << i << ": X-coordinate value? ";
        std::cin >> x;
        std::cout << "  Box #" << i << ": Y-coordinate value? ";
        std::cin >> y;
        boxes[i] = new Box(x, y);
    }
    std::cout << std::endl;

    // Gets input to initialize the door and button objects.
    // Doors and buttons are linked one-to-one.
    std::cout << "How many doors and buttons to add? ";
    int door_and_button_count;
    std::cin >> door_and_button_count;
    std::cout << std::endl;

    int object_count = wall_count + box_count + 2 * door_and_button_count + 2;
    GameBoard board(object_count);

    for (int i = 0; i < wall_count; i++) board.add_object(walls[i]);
    for (int i = 0; i < box_count; i++) board.add_object(boxes[i]);
    delete[] walls;
    delete[] boxes;

    int button_count = door_and_button_count;
    Button** buttons = new Button*[button_count];

    for (int i = 0; i < door_and_button_count; i++) {
        int x;
        int y;

        std::cout << "  Door #" << i << ": X-coordinate value? ";
        std::cin >> x;
        std::cout << "  Door #" << i << ": Y-coordinate value? ";
        std::cin >> y;
        Door* door = new Door(x, y);
        board.add_object(door);

        std::cout << "  Button #" << i << ": X-coordinate value? ";
        std::cin >> x;
        std::cout << "  Button #" << i << ": Y-coordinate value? ";
        std::cin >> y;
        buttons[i] = new Button(x, y, door);
        board.add_object(buttons[i]);
    }
    std::cout << std::endl;

    // Gets input to initialize the player object.
    int player_x;
    int player_y;
    std::cout << "Player: X-coordinate value? ";
    std::cin >> player_x;
    std::cout << "Player: Y-coordinate value? ";
    std::cin >> player_y;
    std::cout << std::endl;
    Player* player = new Player(player_x, player_y);
    board.add_object(player);

    // Gets input to initialize the flag object.
    int flag_x;
    int flag_y;
    std::cout << "Flag: X-coordinate value? ";
    std::cin >> flag_x;
    std::cout << "Flag: Y-coordinate value? ";
    std::cin >> flag_y;
    std::cout << std::endl;
    Flag* flag = new Flag(flag_x, flag_y);
    board.add_object(flag);

    // The game loop.
    while (!(player->get_x() == flag->get_x() && player->get_y() == flag->get_y())) {
        // Update buttons.
        for (int i = 0; i < button_count; i++) {
            buttons[i]->update_button(board);
        }
        // Render the screen.
        render_screen(board, screen, priority, width, height);
        std::cout << std::endl;

        // Get player input.
        Direction direction;
        while (true) {
            std::cout << "What is your next move (\"wasd\" to move, or \"q\" to quit)? ";
            char input;
            std::cin >> input;
            if (input == 'w') {
                direction = NORTH;
                break;
            }
            if (input == 'a') {
                direction = WEST;
                break;
            }
            if (input == 's') {
                direction = SOUTH;
                break;
            }
            if (input == 'd') {
                direction = EAST;
                break;
            }
            if (input == 'q') {
                std::cout << std::endl << "You quit the game." << std::endl;

                // The board automatically deletes all game objects.
                delete[] buttons;
                for (int x = 0; x < 3 * width; x++) {
                    delete[] priority[x];
                    delete[] screen[x];
                }
                delete[] priority;
                delete[] screen;

                return 0;
            }
        }
        std::cout << std::endl;

        // Move the player.
        if (!player->try_move(board, direction)) {
            std::cout << "You could not move!" << std::endl;
            std::cout << std::endl;
        }
    }

    // Render the screen once more before declaring victory.
    render_screen(board, screen, priority, width, height);
    std::cout << std::endl << "You won the game!" << std::endl;

    // The board automatically deletes all game objects.
    delete[] buttons;
    for (int x = 0; x < 3 * width; x++) {
        delete[] priority[x];
        delete[] screen[x];
    }
    delete[] priority;
    delete[] screen;

    return 0;
}
