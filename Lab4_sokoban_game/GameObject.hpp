#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

class GameBoard;

/**
 * @brief An enum that describes the direction of movement.
 */
enum Direction {
    // (x, y): (+0, -1)
    NORTH,
    // (x, y): (+0, +1)
    SOUTH,
    // (x, y): (+1, +0)
    EAST,
    // (x, y): (-1, +0)
    WEST,
};

/**
 * @brief A class that describes an abstract game object.
 *
 * A game object is something that has a position and some properties that can
 * be used to interact with other game objects in the game.
 *
 * While you technically can construct `GameObject` objects manually by setting
 * unique properties for each of them one at a time, we use inheritance instead
 * so that we can group multiple objects that have similar properties and
 * construct them in batches more easily.
 *
 * While derived classes can have specific functionalities that only these
 * derived class objects can do, there are also some functionalities that all
 * objects have in general.
 *
 * The first is the ability to be rendered on the terminal as a 3-char-wide
 * "sprite". Objects themselves (and therefore their sprites) can overlap with
 * one another. To distinguish between overlapping objects, objects have a
 * rendering priority, as well as transparent parts on their sprites (indicated
 * by a space character, i.e., ' ').
 *
 * The second is the ability to attempt to move. While only some objects can
 * move, all objects can *attempt* to move. Moreover, when an object moves and
 * collides with another object, if that object is movable, that object also
 * attempts to move. This means a player can push multiple boxes at once, for
 * example.
 */
class GameObject {
   private:
    // The x-coordinate position of the object.
    int x;
    // The y-coordinate position of the object.
    int y;
    // The sprite, i.e., the chars to print to the terminal to indicate that the object is there.
    char sprite[3];
    // The rendering priority of the sprite, the higher the number, the higher the priority.
    int render_priority;
    // A property that indicates whether or not other objects can collide to this object.
    bool is_collidable;
    // A property that indicates whether or not this object can be moved.
    bool is_movable;

   public:
    /**
     * @brief `GameObject` class constructor.
     *
     * This constructor has been implemented for you.
     *
     * @param x               The x-coordinate position of the object.
     * @param y               The y-coordinate position of the object.
     * @param sprite          The sprite, i.e., the chars to print to the terminal to indicate that the object is there.
     * @param render_priority The rendering priority of the sprite, the higher the number, the higher the priority.
     * @param is_collidable   A property that indicates whether or not other objects can collide to this object.
     * @param is_movable      A property that indicates whether or not this object can be moved.
     */
    GameObject(int x, int y, const char sprite[], int render_priority, bool is_collidable, bool is_movable)
        : x(x),
          y(y),
          sprite{sprite[0], sprite[1], sprite[2]},
          render_priority(render_priority),
          is_collidable(is_collidable),
          is_movable(is_movable) {}

    virtual ~GameObject() = default;

    /**
     * @brief Accessor (getter) function for `x`.
     *
     * This function has been implemented for you.
     *
     * @return The x-coordinate position of the object.
     */
    int get_x() const { return x; }

    /**
     * @brief Mutator (setter) function for `x`.
     *
     * This function has been implemented for you.
     *
     * @param new_x The new value for `x`.
     */
    void set_x(int new_x) { x = new_x; }

    /**
     * @brief Accessor (getter) function for `y`.
     *
     * This function has been implemented for you.
     *
     * @return The y-coordinate position of the object.
     */
    int get_y() const { return y; }

    /**
     * @brief Mutator (setter) function for `y`.
     *
     * This function has been implemented for you.
     *
     * @param new_y The new value for `y`.
     */
    void set_y(int new_y) { y = new_y; }

    /**
     * @brief Accessor (getter) function for `sprite`.
     *
     * This function has been implemented for you.
     *
     * @return The sprite of the object.
     */
    const char* get_sprite() const { return sprite; }

    /**
     * @brief Mutator (setter) function for `sprite`.
     *
     * Note that the `new_sprite` parameter is of `const char[]` type so that
     * you can call the function using a C-string as an argument for simplicity.
     * So for example, `set_sprite(" P ");` sets the sprite for the player
     * object.
     *
     * This function has been implemented for you.
     *
     * @param new_sprite The new value for `sprite`.
     */
    void set_sprite(const char new_sprite[]) {
        sprite[0] = new_sprite[0];
        sprite[1] = new_sprite[1];
        sprite[2] = new_sprite[2];
    }

    /**
     * @brief Accessor (getter) function for `render_priority`.
     *
     * This function has been implemented for you.
     *
     * @return The rendering priority of the sprite.
     */
    int get_render_priority() const { return render_priority; }

    /**
     * @brief Mutator (setter) function for `render_priority`.
     *
     * This function has been implemented for you.
     *
     * @param new_value The new value for `render_priority`.
     */
    void set_render_priority(int new_value) { render_priority = new_value; }

    /**
     * @brief Accessor (getter) function for `is_collidable`.
     *
     * This function has been implemented for you.
     *
     * @return Whether or not the object is collidable.
     */
    bool get_is_collidable() const { return is_collidable; }

    /**
     * @brief Mutator (setter) function for `is_collidable`.
     *
     * This function has been implemented for you.
     *
     * @param new_value The new value for `is_collidable`.
     */
    void set_is_collidable(bool new_value) { is_collidable = new_value; }

    /**
     * @brief Accessor (getter) function for `is_movable`.
     *
     * This function has been implemented for you.
     *
     * @return Whether or not the object is movable.
     */
    bool get_is_movable() const { return is_movable; }

    /**
     * @brief Mutator (setter) function for `is_movable`.
     *
     * This function has been implemented for you.
     *
     * @param new_value The new value for `is_movable`.
     */
    void set_is_movable(bool new_value) { is_movable = new_value; }

    /**
     * @brief Attempts to move the object in a specified direction.
     *
     * __Task 2.2__
     *
     * An object can only be moved when it is movable, obviously.
     *
     * Aside from that, you also have to account for any collidable objects
     * blocking your path when you move. If the blocking object can be moved,
     * then it means this object can also be moved.
     *
     * All objects are owned by a `GameBoard`. The board can be used to inspect
     * the other objects when checking for a collision.
     *
     * Implement this function over on 'lab_tasks.cpp'.
     *
     * @param board     The board containing all the game objects.
     * @param direction The direction to move towards.
     * @return Whether or not the move was successful.
     */
    bool try_move(GameBoard& board, Direction direction);

    /**
     * @brief Renders the object onto a `screen` variable.
     *
     * Both `screen` and `current_priority` are 2d grids with width `3 * width`
     * (remember that sprites are 3 chars wide) and height `height`.
     *
     * If we encounter any non-space chars with higher priority than the current
     * one on screen, we replace the previous char with the new one and set the
     * new rendering priority.
     *
     * The `screen` variable can then be seen being used over on `main.cpp`,
     * where each of the chars are printed onto the terminal once per game loop.
     *
     * Note that there is no restriction on the position of the object. Just
     * that if the object is out of the screen's width and height, we simply do
     * not render it.
     *
     * This function has been implemented for you.
     *
     * @param screen           A grid of chars that will eventually be printed on the terminal.
     * @param current_priority A grid of priorities that help manage overlapping sprites.
     * @param width            The width of the game grid.
     * @param height           The height of the game grid.
     */
    void render(char** screen, int** current_priority, int width, int height) const {
        if (x < 0 || x >= width) return;
        if (y < 0 || y >= height) return;
        if (sprite[0] != ' ' && render_priority > current_priority[3 * x + 0][y]) {
            screen[3 * x + 0][y] = sprite[0];
            current_priority[3 * x + 0][y] = render_priority;
        }
        if (sprite[1] != ' ' && render_priority > current_priority[3 * x + 1][y]) {
            screen[3 * x + 1][y] = sprite[1];
            current_priority[3 * x + 1][y] = render_priority;
        }
        if (sprite[2] != ' ' && render_priority > current_priority[3 * x + 2][y]) {
            screen[3 * x + 2][y] = sprite[2];
            current_priority[3 * x + 2][y] = render_priority;
        }
    }
};

#endif
