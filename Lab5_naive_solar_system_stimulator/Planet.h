#ifndef PLANET_H
#define PLANET_H

#include "Orbiter.h"

/**
 * @brief Celestial object that orbits a star.
 */
class Planet: public Orbiter {
    public:
    /**
     * @brief Initializes the planet.
     * 
     * See also: @ref Orbiter::Orbiter
     * 
     * @param orbitee The star that this planet orbits.
     */
    Planet(const string &name, double age, const Star &orbitee, double orbital_radius, double angular_speed);
    
    /**
     * @return The star that this planet orbits.
     */
    virtual const Star &get_star() const override;
};

#endif