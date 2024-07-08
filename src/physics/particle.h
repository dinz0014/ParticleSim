#pragma once
#include <SFML/Graphics.hpp>

#include "common/constants.h"
#include "common/vector.h"

#include "container.h"

namespace sim {

class ParticleStore;

class Particle
{
public:
    static int nextID;
    static const sf::Color slowColour;
    static const sf::Color fastColour;
    static const sf::Color midColour;

    using id_type = uint16_t;

    Particle(const Vec2f& position, float radius = 10.0f);

    const Vec2f& nextPosition(float timestep);

    void move(const Vec2f& pos_delta);

    const Vec2f& position() const
    {
        return position_;
    }

    Vec2f& position()
    {
        return position_;
    }

    const Vec2f& acceleration() const
    {
        return acceleration_;
    }

    const Vec2f& velocity() const
    {
        return velocity_;
    }

    void rebound(int axis);

    void setPosition(const Vec2f& new_position)
    {
        position_ = new_position;
    }

    void setAcceleration(const Vec2f& accel)
    {
        acceleration_ = accel;
    }

    void setRegion(const Vec2i& region)
    {
        region_ = region;
    }

    const Vec2i& region() const
    {
        return region_;
    }

    float radius() const
    {
        return radius_;
    }

    int id() const
    {
        return id_;
    }

    void setVelocity(Vec2f new_velocity);

    void changeVelocity(const Vec2f& delta_vel)
    {
        setVelocity(velocity_ + delta_vel);
    }

    float mass() const
    {
        return mass_;
    }

    bool operator==(const Particle& other) const
    {
        return id_ == other.id();
    }

private:
    Vec2f position_;
    Vec2f prev_position_;
    Vec2f acceleration_;
    Vec2f velocity_;
    float radius_;
    float mass_;
    Vec2i region_;
    int id_;
};

}
