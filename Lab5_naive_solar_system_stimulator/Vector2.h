#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>
using std::sqrt;


/**
 * @brief 2-dimensional vector for representing the positions of celestial objects.
 */
class Vector2 {

    private:
    /**
     * @brief The X-coordinate.
     */
    double m_x;
    /**
     * @brief The Y-coordinate.
     */
    double m_y;

    public:
    /**
     * @brief Initializes a vector.
     * 
     * @param x X-coordinate.
     * 
     * @param y Y-coordinate. 
     */
    Vector2(double x, double y): m_x{x}, m_y{y} {}

    Vector2(const Vector2 &) = default;

    /**
     * @return The X-coordinate.
     */
    double get_x() const { return m_x; }
    /**
     * @return The y-coordinate.
     */
    double get_y() const { return m_y; }

    /**
     * @return The magnitude of the vector, given by sqrt(x^2+y^2).
     */
    double get_magnitude() const { return sqrt(m_x*m_x+m_y*m_y); }

    /**
     * @return The result of addition of 2 vectors.
     */  
    static Vector2 add(const Vector2 &v1, const Vector2 &v2) {
        return Vector2{v1.m_x+v2.m_x, v1.m_y+v2.m_y};
    }
 
    /**
     * @return The result of subtraction of 2 vectors.
     */
    static Vector2 subtract(const Vector2 &v1, const Vector2 &v2) {
        return Vector2{v1.m_x-v2.m_x, v1.m_y-v2.m_y};
    }

    /**
     * @return the result of multiplying a vector by a scalar.
     */
    static Vector2 scale(const Vector2 &v, double multiplier) {
        return Vector2{v.m_x * multiplier, v.m_y * multiplier};
    }
};

#endif
