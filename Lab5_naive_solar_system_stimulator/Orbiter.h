#ifndef ORBITER_H
#define ORBITER_H

#include "CelestialObject.h"
#include "Star.h"
#include <cmath>

using std::cos;
using std::sin;

/**
 * @brief A celestial object that orbits another.
 */
class Orbiter: public CelestialObject {
    private: 
    /**
     * @brief The celestial object that is being orbited.
     */
    const CelestialObject &m_orbitee;
    /**
     * @brief How far the orbiter is from the celestial object in radian.
     */
    const double m_orbital_radius;
    /**
     * @brief The angular position of the celestial object.
     */
    double m_angular_position;
    /**
     * @brief How fast the orbiter orbits in unit radian per year.
     */
    const double m_angular_speed;
    

    public:
    /**
     * @brief Initializes the members of Orbiter.
     * Initial angular position should be 0 radian.
     * 
     * See also: @ref CelestialObject::CelestialObject
     * 
     * @param orbitee Reference to the celestial object this orbiter orbits.
     * 
     * @param orbital_radius The orbital radius of the orbit, assumed to be positive.
     * 
     * @param angular_speed The angular speed of the orbit in radian per year, assumed to be non-zero.
     * Positive means anti-clockwise, negative means clockwise.
     */
    Orbiter(const string &name, double age, const CelestialObject &orbitee, double orbital_radius, double angular_speed);

    /**
     * @return The celestial object this orbiter orbits.
     */
    const CelestialObject &get_orbitee() const { return m_orbitee; }
    /**
     * @return The orbital radius of this celestial object.
     */
    const double get_orbital_radius() const { return m_orbital_radius; }
    /**
     * @return The angular speed of the orbit in radian per year.
     */
    const double get_angular_speed() const { return m_angular_speed; }

    /**
     * @brief Computes the position of the orbiter via its orbitee's position, its orbital radius, and its angular position.
     * 
     * position = orbitee's position + radius * [cos(angular position), sin(angular position)]
     * 
     * @return The computed angular position.
     */
    virtual Vector2 get_position() const override final;

    /**
     * @brief Updates age and angular position.
     * 
     * new angular position = current angular position + angular speed * time
     * 
     * See also: @ref CelestialObject::simulate
     */
    virtual void simulate(double years) override final;

    /**
     * @return The underlying star in the solar system.
     */
    virtual const Star &get_star() const = 0;

    /**
     * @brief Computes the distance from the solar system's Star to the Orbiter.
     * 
     * distance of 2 vectors = magnitude of (vector a - vector b)
     * 
     * @return The computed distance.
     */
    double get_distance_from_star() const;
};

#endif
