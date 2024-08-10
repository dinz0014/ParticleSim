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
    const Vec2f offset{size_.x / 2.0f, size_.y / 2.0f};
    const Vec2f margin_vec{margin, margin};
    assign_piecewise(position_ - offset + margin_vec, x_min, y_min);
    assign_piecewise(position_ + offset - margin_vec, x_max, y_max);

    return {Vec2f{x_min, x_max}, Vec2f{y_min, y_max}};
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