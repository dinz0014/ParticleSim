#pragma once
#include <SFML/Window/Event.hpp>
#include "common/constants.h"
#include "common/vector.h"

namespace sim {

/*
A simple container class that defines the boundaries of the world that the particles will use
*/
class Container
{
public:
    Container(const Vec2u& size)
        : size_{size}
        , default_size_{size}
    {
    }

    Container(unsigned int width, unsigned int height)
        : size_{width, height}
        , default_size_{width, height}
    {
    }

    const unsigned int getWidth() const
    {
        return size_.x;
    }

    const unsigned int getHeight() const
    {
        return size_.y;
    }

    const Vec2u& getSize() const
    {
        return size_;
    }

    const Vec2f& position() const
    {
        return position_;
    }

    void centerInside(std::pair<float, float> x_bounds, std::pair<float, float> y_bounds);

    void setPosition(const Vec2f& pos)
    {
        position_ = pos;
    }

    using BoundsType = std::pair<Vec2f, Vec2f>;
    BoundsType getBounds(float margin = 0.0f);

    void handleResize(sf::Event::KeyEvent& key_event);
    bool intersects(float x, float y);

private:
    static constexpr unsigned int sizeTick = 2 * static_cast<unsigned int>(MAX_RADIUS);
    Vec2u size_{0u, 0u};
    Vec2u default_size_{0u, 0u};
    Vec2f position_{0.0f, 0.0f};
};

}