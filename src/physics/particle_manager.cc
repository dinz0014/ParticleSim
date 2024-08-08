#include "common/utils.h"

#include "particle_manager.h"

namespace sim {

ParticleManager::ParticleManager(Container& container)
    : container_{container}
{
    const auto& [x_bounds, y_bounds] = container_.getBounds();
    const Vec2f top_left{x_bounds[0], y_bounds[0]};
    const Vec2f bottom_right{x_bounds[1], y_bounds[1]};
    partitioner_ = FixedGrid{top_left, bottom_right};
}

const Particle& ParticleManager::createParticleAtCursor(float x, float y)
{
    const float radius = 10.0f;
    const auto& [x_bounds, y_bounds] = container_.getBounds(radius);
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;

    Vec2f position{x, y};
    position.clamp({x_min, x_max}, {y_min, y_max});

    Particle p{position, radius};
    partitioner_.add(p);
    particles_.push_back(std::move(p));

    return particles_.back();
}

void ParticleManager::updateParticles(float dt)
{
    updateGrid();

    for (auto& particle : particles_)
    {
        particle.setAcceleration(Vec2f{0, G});
        resolveCollisions(particle);
        particle.nextPosition(dt);
        resolveOutOfBounds(particle);
    }
}

void ParticleManager::updateGrid()
{
    // Move particles to their new regions
    for (auto& particle : particles_)
    {
        partitioner_.remove(particle);
        partitioner_.add(particle);
    }
}

void ParticleManager::resolveOutOfBounds(Particle& particle)
{
    // Window Bound Checking
    auto& pos = particle.position();
    const float radius = particle.radius();

    // Container is centered, so we add/subtract half the size to get bounds
    const auto& [x_bounds, y_bounds] = container_.getBounds(radius);
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;

    if (pos.x < x_min || pos.x > x_max)
    {
        particle.rebound(0);
    }

    if (pos.y < y_min || pos.y > y_max)
    {
        particle.rebound(1);
    }

    pos.clamp({x_min, x_max}, {y_min, y_max});
}

void ParticleManager::resolveCollisions(Particle& particle)
{
    auto neighbours = partitioner_.getNearby(particle);

    for (auto nbr : neighbours)
    {
        Particle& other = particles_[nbr];
        const auto axis = particle.position() - other.position();
        const auto dist2 = vec_dot(axis, axis);
        const auto min_dist = particle.radius() + other.radius();

        if (dist2 > min_dist * min_dist)
        {
            continue;
        }

        const auto dist = std::sqrt(dist2);
        Vec2f norm = axis / dist;
        const float delta = 0.5f * std::abs(dist - min_dist);
        particle.move(norm * delta);
        other.move(norm * -delta);

        // Assuming mass is equal
        Vec2f delta_vel = vec_dot(norm, particle.velocity() - other.velocity()) * norm;
        particle.changeVelocity(-delta_vel);
        other.changeVelocity(delta_vel);
    }
}

const ParticleManager::ParticleStore& ParticleManager::particles() const
{
    return particles_;
}

size_t ParticleManager::particle_count() const
{
    return particles_.size();
}

void ParticleManager::clear()
{
    particles_.clear();
    Particle::nextID = 0;
    partitioner_.reset();
}

}
