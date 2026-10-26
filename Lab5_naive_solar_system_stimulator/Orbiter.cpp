#include "Orbiter.h"

using std::cos;
using std::sin;

Orbiter::Orbiter(const string &name, double age, const CelestialObject &orbitee, double orbital_radius, double angular_speed): 
CelestialObject(name, age), m_orbitee(orbitee), m_orbital_radius(orbital_radius), m_angular_position(0), m_angular_speed(angular_speed){
}

Vector2 Orbiter::get_position() const {
    Vector2 temp(std::cos(m_angular_position), std::sin(m_angular_position));
    return Vector2::add(m_orbitee.get_position(), Vector2::scale(temp, m_orbital_radius));
}

void Orbiter::simulate(double years) {
    CelestialObject::simulate(years); //update age
    m_angular_position = m_angular_position + m_angular_speed * years;
}

double Orbiter::get_distance_from_star() const {
    Vector2 diff = Vector2::subtract(this->get_star().get_position(), this->get_position());
    return diff.get_magnitude();
}