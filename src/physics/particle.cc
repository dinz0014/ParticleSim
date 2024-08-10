#include <iostream>
#include <algorithm>
#include <ranges>

#include "particle.h"

namespace sim {

int Particle::nextID = 0;

Particle::Particle(const Vec2f& position, float radius, float max_1d_spd)
    : id_{nextID++}
    , position_{position}
    , radius_{radius}
    , max_spd_{max_1d_spd}
{
}

void Particle::setVelocity(Vec2f new_velocity)
{
    new_velocity.clamp({-max_spd_, max_spd_}, {-max_spd_, max_spd_});
    velocity_ = new_velocity;
}

const Vec2f& Particle::nextPosition(float timestep)
{
    Vec2f v_half = velocity_ + 0.5f * timestep * acceleration_;
    changeVelocity(acceleration_ * timestep);
    position_ += v_half * timestep;
    acceleration_ = {0.0f, 0.0f};
    return position();
}

void Particle::rebound(int axis, float damp_bounce, float threshold)
{
    const float speed_along_axis = std::abs(velocity_[axis]);
    const float delta = speed_along_axis * (1 - damp_bounce);
    const float rel_delta = delta / speed_along_axis;

    if (rel_delta > threshold)
    {
        velocity_.reflect(axis);
        velocity_.dilate(axis, damp_bounce);
    }
    else
    {
        velocity_[axis] = 0;
    }
}

void Particle::move(const Vec2f& pos_delta)
{
    position_ += pos_delta;
}

}
