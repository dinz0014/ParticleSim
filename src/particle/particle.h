#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

constexpr inline float G = -9.8;

class Particle
{
public:
    using ShapeType = sf::CircleShape;

    Particle(const sf::Vector2f &position, float thickness = 5.0f);

    // It doesn't make sense to copy/move particles to other particles
    Particle(const Particle &other) = delete;
    Particle &operator=(const Particle &other) = delete;
    Particle(Particle &&other) noexcept = delete;
    Particle &operator=(Particle &&other) = delete;

    const ShapeType &shape() const
    {
        return shape_;
    }

protected:
    sf::Vector2f position_{};
    sf::Vector2f acceleration_{0, G};
    sf::Vector2f velocity_{0, 0};
    float thickness_{0};

    ShapeType shape_;
};

class ParticleManager
{
public:
    using ParticleStore = std::vector<std::unique_ptr<Particle>>;
    ParticleManager(float target_fps);
    const Particle &CreateParticleAtCursor(const sf::Event::MouseButtonEvent &mouse_event);
    const ParticleStore &particles() const;

private:
    ParticleStore particles_;
    float timestep_{0.0f};
};