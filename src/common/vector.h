#pragma once
#include <algorithm>
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace sim {

// A wrapper around the sf::Vector2<T> classes to support addition/subtraction between vectors as well as scalar multiplication
template <typename T>
class Vec2d : public sf::Vector2<T>
{
public:
    // Constructors
    Vec2d(T x_val = 0, T y_val = 0) : sf::Vector2<T>(x_val, y_val) {}
    Vec2d(const sf::Vector2<T>& v) : sf::Vector2<T>(v) {}

    template <typename OtherT>
    static Vec2d<T> from(const sf::Vector2<OtherT>& v)
    {
        T x = static_cast<T>(v.x);
        T y = static_cast<T>(v.y);
        return Vec2d(x, y);
    }

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

    Vec2d operator-() const
    {
        return Vec2d(-1 * this->x, -1 * this->y);
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
        const auto [x_min, x_max] = x_limits;
        const auto [y_min, y_max] = y_limits;

        this->x = std::clamp(this->x, x_min, x_max);
        this->y = std::clamp(this->y, y_min, y_max);
    }

    float magnitude() const
    {
        return std::sqrt(this->x * this->x + this->y * this->y);
    }

private:
};

using Vec2f = Vec2d<float>;
using Vec2i = Vec2d<int>;
using Vec2u = Vec2d<unsigned int>;

template <typename T>
T vec_dot(Vec2d<T> first, Vec2d<T> second)
{
    return first.x * second.x + first.y * second.y;
}

template <typename T>
void assign_piecewise(Vec2d<T> vec, T& first, T& second)
{
    first = vec.x;
    second = vec.y;
}

template <typename T>
Vec2d<T> midpoint(Vec2d<T> first, Vec2d<T> second)
{
    return ( first + second ) / 2.0f;
}

}