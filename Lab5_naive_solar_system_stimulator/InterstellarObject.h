#ifndef INTERSTELLAR_OBJECT_H
#define INTERSTELLAR_OBJECT_H

#include "CelestialObject.h"
/**
 * @brief An object that moves past a Solar System in linear motion.
 */
class InterstellarObject: public CelestialObject {
    private:
    /**
     * @brief current position of the interstellar object.
     */
    Vector2 m_position;
    /**
     * @brief velocity in unit/year.
     */
    Vector2 m_velocity;

    public:
    /**
     * @brief Initializes the data members of the interstellar object.
     * 
     * See also: @ref CelestialObject::CelestialObject
     * 
     * @param direction the direction of the interstellar object's movement, students should normalize the vector.
     */
    InterstellarObject(const string &name, double age, Vector2 position, double speed, Vector2 direction):
        CelestialObject{name, age}, m_position{position}, m_velocity{Vector2::scale(Vector2::scale(direction, 1/direction.get_magnitude()), speed)} {

    }

    Vector2 get_velocity() const { return m_velocity; }

    /**
     * @return The current position of the interstellar object.
     */
    virtual Vector2 get_position() const override;


    /**
     * @brief Updates the object's age and position.
     * 
     * Note that new position = current position + year * velocity.
     * 
     * See also: @ref CelestialObject::simulate
     */
    virtual void simulate(double years) override;
};

#endif