#include "Planet.h"

Planet::Planet(const string &name, double age, const Star &orbitee, double orbital_radius, double angular_speed) :
Orbiter(name, age, orbitee, orbital_radius, angular_speed){
}

const Star &Planet::get_star() const {
    return dynamic_cast<const Star &>(Orbiter::get_orbitee());
}