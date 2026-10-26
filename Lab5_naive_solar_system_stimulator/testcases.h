#include <iostream>
#include "Vector2.h"
#include "CelestialObject.h"
#include "Star.h"
#include "InterstellarObject.h"
#include "Orbiter.h"
#include "Planet.h"
#include "Moon.h"
#include "Orbiter.h"

using namespace std;

const double EPSILON = 0.001f;
const double PI = acos(-1.0);

#define NUM_TESTCASES 4

#define ASSERT_TRUE(boolean, reason) if (!(boolean)) cout << "Assertion fail at testcases.h line " << __LINE__ << ": " << reason << "\n";
#define ASSERT(expected, obtained, reason) if (expected != obtained) {ASSERT_TRUE(false, reason); cout << "Expected " << expected << "; but got " << obtained << "\n";}
#define ASSERT_DOUBLE(expected, obtained, reason) if (abs(expected-obtained) > EPSILON) {ASSERT_TRUE(false, reason);cout << "Expected " << expected << "; but got " << obtained << "; difference: " << abs(expected-obtained) <<"\n";}

/**
 * @brief InterstellarObject
 */
void test1() {
    cout << "Beginning of test 1" << endl;
    
    InterstellarObject probe{"Probe", 0, Vector2{0, 0}, 10, Vector2{3, 4}};
    ASSERT_DOUBLE(0, probe.get_position().get_x(), "Incorrect initial x for Probe's position.");
    ASSERT_DOUBLE(0, probe.get_position().get_y(), "Incorrect initial y for Probe's position.");

    Vector2 velocity{probe.get_velocity()};
    ASSERT_DOUBLE(10, velocity.get_magnitude(), "Incorrect velocity magnitude for Probe");
    ASSERT_DOUBLE(6, velocity.get_x(), "Incorrect x component for Probe's velocity");
    ASSERT_DOUBLE(8, velocity.get_y(), "Incorrect y component for Probe's velocity");

    probe.simulate(2);
    ASSERT_DOUBLE(2, probe.get_age(), "Incorrect age for Probe after simulating 2 years.");
    ASSERT_DOUBLE(12, probe.get_position().get_x(), "Incorrect x for Probe's position after 2 years.");
    ASSERT_DOUBLE(16, probe.get_position().get_y(), "Incorrect y for Probe's position after 2 years.");

    cout << "test 1 completed successfully." << endl;
}

/**
 * @brief InterstellarObject
 */
void test2() {
    cout << "Beginning of test 2" << endl;

    InterstellarObject asteroid{"Asteroid", 1000, Vector2{100, 100}, 200, Vector2{1, 2}};
    // ASSERT("Asteroid", asteroid.get_name(), "Incorrect name for Interstellar Object Asteroid.");
    ASSERT_DOUBLE(100, asteroid.get_position().get_x(), "Incorrect x for Asteroid's position.");
    ASSERT_DOUBLE(100, asteroid.get_position().get_y(), "Incorrect y for Asteroid's position.");

    Vector2 velocity{asteroid.get_velocity()};
    ASSERT_DOUBLE(200, velocity.get_magnitude(), "Incorrect velocity magnitude for Asteroid");
    ASSERT_DOUBLE(89.442719, velocity.get_x(), "Incorrect x component for Asteroid's velocity");
    ASSERT_DOUBLE(178.885438, velocity.get_y(), "Incorrect y component for Asteroid's velocity");


    asteroid.simulate(5);
    ASSERT_DOUBLE(1005, asteroid.get_age(), "Incorrect age for Asteroid after simulating 5 years.");
    ASSERT_DOUBLE(547.2135955, asteroid.get_position().get_x(), "Incorrect x for Asteroid's position after 5 years.");
    ASSERT_DOUBLE(994.427191, asteroid.get_position().get_y(), "Incorrect y for Asteroid's position after 5 years.");

    cout << "test 2 completed successfully." << endl;
}

/** 
 * @brief Orbiter, Planet
 */
void test3() {
    cout << "Beginning of test 3" << endl;

    Star sun{"Sun", 10000, Vector2{100, 300}};
    Planet earth{"Earth", 500, sun, 100, 2 * PI};

    ASSERT("Earth", earth.get_name(), "Incorrect name for Planet Earth.");
    ASSERT(sun.get_name(), earth.get_orbitee().get_name(), "Incorrect name for orbitee of Earth.");
    ASSERT(sun.get_name(), earth.get_star().get_name(), "Incorrect name for star of Earth.");
    ASSERT_DOUBLE(2 * PI, earth.get_angular_speed(), "Incorrect angular speed for Earth.");

    ASSERT_DOUBLE(100, earth.get_distance_from_star(), "Incorrect Earth's distance from star.");
    
    // Sun's position + Earth x at 0 radian = 100 + 100 = 200
    ASSERT_DOUBLE(200, earth.get_position().get_x(), "Incorrect x component for Earth's position.");
    // Sun's position + Earth y at 0 radian = 300 + 0 = 300
    ASSERT_DOUBLE(300, earth.get_position().get_y(), "Incorrect y component for Earth's position.");
    
    // 135 degree simulation
    earth.simulate(0.375);
    ASSERT_DOUBLE(500.375, earth.get_age(), "Incorrect age for earth after simulating 0.375 years.");

    // sun's position + Earth x at 135 degrees = 100 - 100*(sqrt2/2)
    ASSERT_DOUBLE(100 - 100/sqrt(2), earth.get_position().get_x(), "Incorrect x component for Earth's position after 135 degrees simulation.");
    // sun's position + Earth y at 135 degrees = 300+100*(sqrt2/2)
    ASSERT_DOUBLE(300 + 100/sqrt(2), earth.get_position().get_y(), "Incorrect y component for Earth's position after 135 degrees simulation.");
    // should remain 100 due to circular orbit
    ASSERT_DOUBLE(100, earth.get_distance_from_star(), "Incorrect Earth's distance from star.");

    cout << "test 3 completed successfully." << endl;
}

/**
 * @brief Orbiter, Moon
 */
void test4() {
    cout << "Beginning of test 4" << endl;

    Star sun{"Sun", 100000, Vector2{200, 500}};
    Planet earth{"Earth", 5000, sun, 100, 2 * PI};
    Moon moon{"Moon", 5000, earth, 50, 4 * PI};
    ASSERT("Moon", moon.get_name(), "Incorrect name for Moon.");
    ASSERT(earth.get_name(), moon.get_orbitee().get_name(), "Incorrect orbitee name for Moon.");
    ASSERT(sun.get_name(), moon.get_star().get_name(), "Incorrect star name for Moon.");

    // 200 + 100 + 50
    ASSERT_DOUBLE(350, moon.get_position().get_x(), "Incorrect x component for Moon's position.");
    // 500 + 0 + 0
    ASSERT_DOUBLE(500, moon.get_position().get_y(), "Incorrect y component for Moon's position.");

    // magnitude of <350-200, 500-500> = <150, 0>
    ASSERT_DOUBLE(150, moon.get_distance_from_star(), "Incorrect distance from Star to Moon.");

    // 270 degrees
    // note: we only simulate Moon, but the hidden test case would definitely simulate both the moon and earth.
    moon.simulate(5.375);
    ASSERT_DOUBLE(5005.375, moon.get_age(), "Incorrect age for moon after simulating 5.375 years.");
    // 200 + 100 + 0 = 300
    ASSERT_DOUBLE(300, moon.get_position().get_x(), "Incorrect x component for Moon's position after 270-degree simulation.");
    // 500 + 0 - 50 = 450
    ASSERT_DOUBLE(450, moon.get_position().get_y(), "Incorrect y component for Moon's position after 270-degree simulation.");

    ASSERT_DOUBLE(Vector2::subtract(Vector2{200, 500}, Vector2{300, 450}).get_magnitude(), moon.get_distance_from_star(),
        "Incorrect distance from star after 270 degrees simulation");

    cout << "test 4 completed successfully." << endl;
}
