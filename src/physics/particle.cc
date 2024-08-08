#include <iostream>
#include <algorithm>
#include <ranges>

#include "particle.h"

namespace sim {

int Particle::nextID = 0;

Particle::Particle(const Vec2f& position, float radius)
    : position_{position}
    , acceleration_{0, G}
    , radius_{radius}
    , id_{nextID++}
{
}

void Particle::setVelocity(Vec2f new_velocity)
{
    new_velocity.clamp({-MAX_VEL, MAX_VEL}, {-MAX_VEL, MAX_VEL});
    velocity_ = new_velocity;
}

const Vec2f& Particle::nextPosition(float timestep)
{
    Vec2f v_half = velocity_ + 0.5f * timestep * acceleration_;
    changeVelocity(acceleration_ * timestep);
    position_ += v_half * timestep;
    return position();
}

void Particle::rebound(int axis)
{
    const float speed_along_axis = std::abs(velocity_[axis]);
    const float delta = speed_along_axis * (1 - speed_along_axis);
    const float rel_delta = delta / speed_along_axis;

    if (rel_delta > 0.05f)
    {
        velocity_.reflect(axis);
        velocity_.dilate(axis, DAMPING_CONSTANT);
    }
    else
    {
        velocity_[axis] = 0;
        acceleration_[axis] = 0;
    }
}

void Particle::move(const Vec2f& pos_delta)
{
    position_ += pos_delta;
}

}
