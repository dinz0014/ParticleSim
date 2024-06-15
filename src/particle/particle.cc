#include "particle.h"

Particle::Particle(const sim::Vec2f &position, float thickness)
    : position_{position}, thickness_{thickness}
{
    shape_ = ShapeType{thickness};
    shape_.setFillColor(sf::Color::Cyan);
    shape_.setPosition(position_);
}

void Particle::update(float timestep)
{
    sim::Vec2f v_half = velocity_ + 0.5f * timestep * acceleration_;
    position_ += v_half * timestep;
    velocity_ += acceleration_ * timestep;
    shape_.setPosition(position_);
}

ParticleManager::ParticleManager(float timestep) : timestep_{timestep}
{
}

const Particle &ParticleManager::CreateParticleAtCursor(const sf::Event::MouseButtonEvent &mouse_event)
{
    sim::Vec2f position{static_cast<float>(mouse_event.x), static_cast<float>(mouse_event.y)};
    auto particle = std::make_unique<Particle>(position);
    const auto &ret = *particle;
    particles_.push_back(std::move(particle));

    return ret;
}

void ParticleManager::updateParticles()
{
    for (auto &particle : particles_)
    {
        particle->update(timestep_);
    }
}

const ParticleManager::ParticleStore &ParticleManager::particles() const
{
    return particles_;
}