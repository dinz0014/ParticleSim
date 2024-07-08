#include "common/utils.h"

#include "particle_manager.h"

namespace sim {

ParticleManager::ParticleManager(Container& container)
    : container_{container}
{
}

const Particle& ParticleManager::createParticleAtCursor(float x, float y)
{
    const float radius = 10.0f;
    const auto& [x_bounds, y_bounds] = container_.getBounds(radius);
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;


    Vec2f position{x, y};
    position.clamp({x_min, x_max}, {y_min, y_max});

    // We maintain a spatial grid and particles inside it for easier collision handling
    auto cell = computeFlattenedLocation(position);
    auto region = computeRegion(position);
    auto new_particle = std::make_unique<Particle>(position, region, radius);
    particle_store_[cell] = std::move(new_particle);

    return *particle_store_[cell];
}

int ParticleManager::computeRegion(const Vec2f& position)
{
    const auto& [x_bounds, y_bounds] = container_.getBounds();
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;

    const auto hash_row = int((position.y - y_min) / (container_.getHeight() / GRID_SIZE));
    const auto hash_col = int((position.x - x_min) / (container_.getWidth() / GRID_SIZE));
    return hash_row * GRID_SIZE + hash_col;
}

int ParticleManager::computeFlattenedLocation(const Vec2f& position)
{
    const auto region_start = computeRegion(position) * MAX_PER_CELL;

    int cell = region_start;
    while (particle_store_[cell])
    {
        cell++;
    }

    // When the number of particles in a cell exceeds MAX_PER_CELL, we simply overwrite the last one.
    return std::clamp(cell, region_start, region_start + MAX_PER_CELL - 1);
}

void ParticleManager::updateParticles(float dt)
{
    updateGrid();

    for (auto& particle : deref_non_null_view(particle_store_))
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
    for (auto& particle : non_null_view(particle_store_))
    {
        const auto old_region = particle->region();
        const auto new_region = computeRegion(particle->position());

        if (old_region != new_region)
        {
            particle->setRegion(new_region);

            const auto new_cell = computeFlattenedLocation(particle->position());
            particle_store_[new_cell] = std::move(particle);
        }
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
    const auto region = particle.region();

    for (auto offset : neighbour_offsets_)
    {
        auto curr_region_start = (region + offset) * MAX_PER_CELL;

        for (int i = curr_region_start; i < curr_region_start + MAX_PER_CELL; i++)
        {
            if (i >= particle_store_.size() || !particle_store_[i] || *particle_store_[i] == particle)
            {
                continue;
            }

            auto& neighbour = *particle_store_[i];
            const auto axis = particle.position() - neighbour.position();
            const auto dist2 = vec_dot(axis, axis);
            const auto min_dist = particle.radius() + neighbour.radius();

            if (dist2 > min_dist * min_dist)
            {
                continue;
            }

            const auto dist = std::sqrt(dist2);
            Vec2f norm = axis / dist;
            const float delta = 0.5 * std::abs(dist - min_dist);
            particle.move(norm * delta);
            neighbour.move(norm * -delta);

            // Assuming mass is equal
            Vec2f delta_vel = vec_dot(norm, particle.velocity() - neighbour.velocity()) * norm ;
            particle.changeVelocity(-delta_vel);
            neighbour.changeVelocity(delta_vel);
        }
    }
}

const ParticleManager::ParticleStore& ParticleManager::particles() const
{
    return particle_store_;
}

}
