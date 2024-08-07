#include "container.h"

namespace sim {

void Container::centerInside(std::pair<float, float> x_bounds, std::pair<float, float> y_bounds)
{
    const auto [x_min, x_max] = x_bounds;
    const auto [y_min, y_max] = y_bounds;
    position_ = midpoint(Vec2f{x_min, y_max}, Vec2f{x_max, y_min});
}

Container::BoundsType Container::getBounds(float margin)
{
    // Container is centered, so we add/subtract half the size to get bounds
    float x_min, x_max, y_min, y_max;
    const Vec2f offset = Vec2f::from(size_ / 2u);
    const Vec2f margin_vec{margin, margin};
    assign_piecewise(position_ - offset + margin_vec, x_min, y_min);
    assign_piecewise(position_ + offset - margin_vec, x_max, y_max);

    return {Vec2f{x_min, x_max}, Vec2f{y_min, y_max}};
}

void Container::handleResize(sf::Event::KeyEvent& key_event)
{
    switch(key_event.code)
    {
        case sf::Keyboard::Up    : size_[1] += sizeTick; break;
        case sf::Keyboard::Right : size_[0] += sizeTick; break;
        case sf::Keyboard::Down  : size_[1] -= sizeTick; break;
        case sf::Keyboard::Left  : size_[0] -= sizeTick; break;
        default: break;
    }
}

bool Container::intersects(float x, float y)
{
    const auto& [x_bounds, y_bounds] = getBounds();
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;

    if (x >= x_min && x <= x_max && y >= y_min && y <= y_max)
    {
        return true;
    }

    return false;
}

}