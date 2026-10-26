#ifndef STAR_H
#define STAR_H

#include "CelestialObject.h"

/**
 * @brief The center of a solar system, assumed to only have 1 per solar system, ignoring binary stars.
 * It is also assumed to be stationary.
 */
class Star: public CelestialObject {
    private:
    /**
     * @brief The position of the star, assumed to be fixed.
     */
    const Vector2 m_position;

    public:
    /**
     * @brief Initializes the data members of a star.
     * 
     * See also: @ref CelestialObject::CelestialObject
     * 
     * @param position Position of the star.
     */
    Star(const string &name, double age, Vector2 position): CelestialObject{name, age}, m_position{position} {

    }

    /**
     * @return The position of the star.
     */
    virtual Vector2 get_position() const override;
};

#endif