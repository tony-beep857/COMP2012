#ifndef CELESTIAL_OBJECT_H
#define CELESTIAL_OBJECT_H

#include <string>
#include "Vector2.h"
using std::string;

/**
 * @brief The very base class for any objects in the solar system.
 */
class CelestialObject {
    private:
    /**
     * @brief The name of the celestial object.
     */
    string m_name;
    /**
     * @brief The age of the celestial object in years.
     */
    double m_age;

    public:
    /**
     * @brief Initializes the members of the celestial object.
     * 
     * @param name Name of the celestial object.
     * 
     * @param age Initial age of the object in years.
     */
    CelestialObject(const string &name, double age): m_name{name}, m_age{age} {

    }
    virtual ~CelestialObject() = default;

    /**
     * @return The name of the celestial object.
     */
    const string &get_name() const { return m_name; }
    /**
     * @return The age of the celestial object in years.
     */
    double get_age() const { return m_age; }

    /**
     * @brief A function that returns the position of a celestial object.
     * 
     * @return Position of the object.
     */
    virtual Vector2 get_position() const = 0;

    /**
     * @brief Simulates passage of time by increasing the age of it.
     * 
     * @param years How many years to simulate.
     */
    virtual void simulate(double years) {
        m_age += years;
    }
};


#endif