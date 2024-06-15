#pragma once
#include <SFML/System/Vector2.hpp>

namespace sim {

// A wrapper around the sf::Vector2<T> classes to support addition/subtraction between vectors as well as scalar multiplication 
template <typename T>
class Vec2d : public sf::Vector2<T>
{
public:
    // Constructors
    Vec2d(T x = 0, T y = 0) : sf::Vector2<T>(x, y) {}
    Vec2d(const sf::Vector2<T>& v) : sf::Vector2<T>(v) {}

    // Addition
    Vec2d operator+(const sf::Vector2<T>& other) const
    {
        return Vec2d(this->x + other.x, this->y + other.y);
    }

    // Subtraction
    Vec2d operator-(const sf::Vector2<T>& other) const
    {
        return Vec2d(this->x - other.x, this->y - other.y);
    }

    // Scalar Multiplication
    Vec2d operator*(T multiplier) const
    {
        return Vec2d(this->x * multiplier, this->y * multiplier);
    }

    Vec2d& operator+=(const sf::Vector2<T>& other)
    {
        this->x += other.x;
        this->y += other.y;
        return *this;
    }

    Vec2d& operator-=(const sf::Vector2<T>& other)
    {
        this->x -= other.x;
        this->y -= other.y;
        return *this;
    }

    Vec2d& operator*=(T multiplier)
    {
        this->x *= multiplier;
        this->y *= multiplier;
        return *this;
    }
};

using Vec2f = Vec2d<float>;
using Vec2i = Vec2d<int>;

}