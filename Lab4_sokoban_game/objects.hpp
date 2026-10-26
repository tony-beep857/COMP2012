#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include "GameObject.hpp"

class GameBoard;

/**
 * @brief A player class that inherits from `GameObject`.
 *
 * A player (" P ") is a collidable and movable object. It is assumed that there
 * can only be one player object at a time, and it can be controlled by user
 * input.
 */
class Player : public GameObject {
   public:
    /**
     * @brief `Player` class constructor.
     *
     * __Task 1.1__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Player`'s sprite is " P ", with a rendering priority of 6.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate position of the object.
     * @param y The y-coordinate position of the object.
     */
    Player(int x, int y);
};

/**
 * @brief A wall class that inherits from `GameObject`.
 *
 * A wall ("###") is a collidable but immovable object. Walls cannot be pushed
 * around nor can they be passed through.
 */
class Wall : public GameObject {
   public:
    /**
     * @brief `Wall` class constructor.
     *
     * __Task 1.2__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Wall`'s sprite is "###", with a rendering priority of 5.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate position of the object.
     * @param y The y-coordinate position of the object.
     */
    Wall(int x, int y);
};

/**
 * @brief A box class that inherits from `GameObject`.
 *
 * A box (" X ") is a collidable and movable object. They can be pushed around
 * by players or even by other boxes.
 */
class Box : public GameObject {
   public:
    /**
     * @brief `Box` class constructor.
     *
     * __Task 1.3__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Box`'s sprite is " X ", with a rendering priority of 4.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate position of the object.
     * @param y The y-coordinate position of the object.
     */
    Box(int x, int y);
};

/**
 * @brief A door class that inherits from `GameObject`.
 *
 * A door ("[ ]" when opened, "[#]" when closed) is an immovable object. A door
 * is uncollidable when it is opened, and collidable when it is closed.
 */
class Door : public GameObject {
   public:
    /**
     * @brief `Door` class constructor.
     *
     * __Task 1.4__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Door`'s default state is closed, and its sprite is "[#]", with a
     * rendering priority of 3.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate position of the object.
     * @param y The y-coordinate position of the object.
     */
    Door(int x, int y);

    /**
     * @brief Opens the door.
     *
     * __Task 2.3__
     *
     * When this function is called, the sprite of the object is modified to
     * "[ ]" to indicate that the door is open, and the door is set to no longer
     * be collidable.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     */
    void open_door();

    /**
     * @brief Closes the door.
     *
     * __Task 2.4__
     *
     * When this function is called, if the door closes successfully, the sprite
     * of the object is modified to "[#]" to indicate that the door is closed,
     * and the door is set back to be collidable.
     *
     * The door can be jammed and fail to close when there are any collidable
     * objects other than the door itself that are in the same position as the
     * door, as it blocks the way for the door to be closed.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param board The board containing all the game objects.
     */
    void close_door(const GameBoard& board);
};

/**
 * @brief A button class that inherits from `GameObject`.
 *
 * A button ("___" when pressed, "---" when not) is an uncollidable and
 * immovable object. When pressed, it opens the door that is linked to it.
 */
class Button : public GameObject {
   private:
    // The pointer to the connected `Door` object.
    Door* door;

   public:
    /**
     * @brief `Button` class constructor.
     *
     * __Task 1.5__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Button`'s default sprite is "---", with a rendering priority of 2.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x    The x-coordinate position of the object.
     * @param y    The y-coordinate position of the object.
     * @param door The pointer to the connected `Door` object.
     */
    Button(int x, int y, Door* door);

    /**
     * @brief Checks if button is pressed and opens or closes the door accordingly.
     *
     * __Task 2.5__
     *
     * This function checks if there are any collidable objects that are in the
     * same position as the button (in other words, standing on the button), and
     * if so, opens the door linked to the button. If no collidable objects are
     * standing on the button, attempts to close the door instead.
     *
     * Furthermore, whenever the button is pressed or released, modifies the
     * sprite of this object to be "___" or "---" respectively.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param board The board containing all the game objects.
     */
    void update_button(const GameBoard& board);
};

/**
 * @brief A flag class that inherits from `GameObject`.
 *
 * A flag ("!!!") is an uncollidable and immovable object. It is assumed that
 * there can only be one flag object at a time. When stood on, you win the game.
 */
class Flag : public GameObject {
   public:
    /**
     * @brief `Flag` class constructor.
     *
     * __Task 1.6__
     *
     * Calls the parent constructor and sets all the data members accordingly.
     *
     * `Flag`'s sprite is "!!!", with a rendering priority of 1.
     *
     * Implement this constructor over on 'lab_tasks.cpp'.
     *
     * @param x The x-coordinate position of the object.
     * @param y The y-coordinate position of the object.
     */
    Flag(int x, int y);
};

#endif
