#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>

/**
 * @brief A struct that represents either the Player or the Enemy.
 *
 * A simple struct that represents an entity and their stats. We use a struct
 * here and not a class to make accessing the data members more convenient.
 */
struct Entity {
    // The entity's health, an entity dies when their health reaches 0 or lower.
    int health;
    // The entity's block, block reduces the damage taken by the entity.
    int block;
    
    /**
     * @brief Gets the stats of the entity in string format.
     *
     * This function has been implemented for you.
     *
     * @return The entity's stats.
     */
    std::string get_stats() const {
        std::string result;
        result += "Health: " + std::to_string(health) + ", ";
        result += "Block: " + std::to_string(block) + ", ";
        return result;
    }
};

#endif
