#ifndef GAME_BOARD_HPP
#define GAME_BOARD_HPP

#include "GameObject.hpp"

/**
 * @brief Owns and provides access to all game objects.
 *
 * The board stores the objects in a fixed-size dynamic array. `add_object()`
 * transfers ownership of an object to the board, and the board deletes every
 * object when it is destroyed.
 */
class GameBoard {
   private:
    GameObject** objects;
    int object_count;

   public:
    GameBoard(int size) : objects(new GameObject*[size]), object_count(0) {}

    ~GameBoard() {
        for (int i = 0; i < object_count; i++) {
            delete objects[i];
        }
        delete[] objects;
    }

    // A board owns its objects, so it must not be copied.
    GameBoard(const GameBoard&) = delete;
    GameBoard& operator=(const GameBoard&) = delete;

    /** @brief Adds an object and transfers its ownership to the board. */
    void add_object(GameObject* object) { objects[object_count++] = object; }

    int get_object_count() const { return object_count; }
    GameObject* get_object(int index) const { return objects[index]; }

    /**
     * @brief Finds a collidable object at a given position.
     *
     * __Task 2.1__
     *
     * Search through the objects on the board and return the first collidable
     * object whose position is (`x`, `y`). Return `nullptr` if none is found.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate to search.
     * @param y The y-coordinate to search.
     * @return The collidable object at the position, or `nullptr` if none exists.
     */
    GameObject* find_collidable_object_at(int x, int y) const;
};

#endif
