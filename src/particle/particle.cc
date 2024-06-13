#include "particle.h"

Particle::Particle(const sf::Vector2f &position, float thickness)
    : position_{position}, thickness_{thickness}
{
    shape_ = ShapeType{thickness};
    shape_.setFillColor(sf::Color::Cyan);
    shape_.setPosition(position_);
}

ParticleManager::ParticleManager(float target_fps) : timestep_{1.0f / target_fps}
{
}

const Particle &ParticleManager::CreateParticleAtCursor(const sf::Event::MouseButtonEvent &mouse_event)
{
    sf::Vector2f position{static_cast<float>(mouse_event.x), static_cast<float>(mouse_event.y)};
    auto particle = std::make_unique<Particle>(position);
    const auto &ret = *particle;
    particles_.push_back(std::move(particle));

    return ret;
}

const ParticleManager::ParticleStore &ParticleManager::particles() const
{
    return particles_;
}