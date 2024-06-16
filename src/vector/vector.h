#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace sim {

// A wrapper around the sf::Vector2<T> classes to support addition/subtraction between vectors as well as scalar multiplication
template <typename T>
class Vec2d : public sf::Vector2<T>
{
public:
    // Constructors
    Vec2d(T x = 0, T y = 0) : sf::Vector2<T>(x, y) {}
    Vec2d(const sf::Vector2<T>& v) : sf::Vector2<T>(v) {}

    using MinMaxLimits = std::pair<T, T>;

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

    T& operator[](int axis)
    {
        if (axis != 0 && axis != 1)
        {
            throw std::invalid_argument("axis must be 0 or 1");
        }

        if (axis == 0)
        {
            return this->x;
        }

        return this->y;
    }

    const T& operator[](int axis) const
    {
        if (axis != 0 && axis != 1)
        {
            throw std::invalid_argument("axis must be 0 or 1");
        }

        if (axis == 0)
        {
            return this->x;
        }

        return this->y;
    }

    void reflect(int axis)
    {
        if (axis != 0 && axis != 1)
        {
            throw std::invalid_argument("axis must be 0 or 1");
        }

        if (axis == 0)
        {
            this->x *= -1;
        }
        else if (axis == 1)
        {
            this->y *= -1;
        }
        // TODO: review LOG here when logging is implemented
    }

    void dilate(int axis, float factor)
    {
        if (axis != 0 && axis != 1)
        {
            throw std::invalid_argument("axis must be 0 or 1");
        }

        if (axis == 0)
        {
            this->x *= factor;
        }
        else if (axis == 1)
        {
            this->y *= factor;
        }
        // TODO: review LOG here when logging is implemented
    }

    void clamp(MinMaxLimits x_limits, MinMaxLimits y_limits)
    {
        const auto& [x_min, x_max] = x_limits;
        const auto& [y_min, y_max] = y_limits;

        this->x = std::min(std::max(this->x, x_min), x_max);
        this->y = std::min(std::max(this->y, y_min), y_max);
    }

private:
};

using Vec2f = Vec2d<float>;
using Vec2i = Vec2d<int>;

}