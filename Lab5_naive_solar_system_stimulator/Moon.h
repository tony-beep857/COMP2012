#ifndef MOON_H
#define MOON_H

#include "Orbiter.h"
#include "Planet.h"

/**
 * @brief Celestial object that orbits a planet.
 */
class Moon: public Orbiter {
    public:
    /**
     * @brief Initializes the moon.
     * 
     * See also: @ref Orbiter::Orbiter
     * 
     * @param orbitee The planet the moon orbits.
     */
    Moon(const string &name, double age, const Planet &orbitee/*, double albedo*/, double orbital_radius, double angular_speed);

    /**
     * @return The star the moon's orbitee orbits.
     */
    virtual const Star &get_star() const override;
};

#endif