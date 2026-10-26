#include "InterstellarObject.h"

Vector2 InterstellarObject::get_position() const {
    return this->m_position;
}

void InterstellarObject::simulate(double years) {
    CelestialObject::simulate(years); // update the object age
    m_position = Vector2::add(m_position, Vector2::scale(m_velocity, years));
}