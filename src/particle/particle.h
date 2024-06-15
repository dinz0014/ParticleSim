#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include "vector/vector.h"

constexpr inline float G = 19.6;

class Particle
{
public:
    using ShapeType = sf::CircleShape;

    Particle(const sim::Vec2f &position, float thickness = 5.0f);

    // It doesn't make sense to copy/move particles to other particles
    Particle(const Particle &other) = delete;
    Particle &operator=(const Particle &other) = delete;
    Particle(Particle &&other) noexcept = delete;
    Particle &operator=(Particle &&other) = delete;

    void update(float timestep);

    const ShapeType &shape() const
    {
        return shape_;
    }

    const sim::Vec2f &acceleration() const
    {
        return acceleration_;
    }

    const sim::Vec2f &velocity() const
    {
        return velocity_;
    }

    const sim::Vec2f &position() const
    {
        return position_;
    }

protected:
    sim::Vec2f position_{};
    sim::Vec2f acceleration_{0, G};
    sim::Vec2f velocity_{0, 0};
    float thickness_{0};

    ShapeType shape_;
};

class ParticleManager
{
public:
    using ParticleStore = std::vector<std::unique_ptr<Particle>>;
    ParticleManager(float timestep);
    const Particle &CreateParticleAtCursor(const sf::Event::MouseButtonEvent &mouse_event);
    void updateParticles();
    const ParticleStore &particles() const;

private:
    ParticleStore particles_;
    float timestep_{0.0f};
};