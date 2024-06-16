#include "particle.h"

namespace sim {

int Particle::nextID = 0;

Particle::Particle(const sim::Vec2f& position, float radius)
    : position_{position}
    , acceleration_{0, G}
    , velocity_{0, 0}
    , radius_{radius}
    , id_{nextID++}
{
    shape_ = sf::CircleShape{radius};
    shape_.setFillColor(sf::Color::Cyan);
    shape_.setPosition(position);
}

const sim::Vec2f& Particle::nextPosition(float timestep)
{
    sim::Vec2f v_half = velocity_ + 0.5f * timestep * acceleration_;
    velocity_ += acceleration_ * timestep;
    position_ += v_half * timestep;

    return position();
}

void Particle::move(const sim::Vec2f& pos_delta)
{
    shape_.move(pos_delta);
}

ParticleManager::ParticleManager(float timestep, int window_height, int window_width)
    : timestep_{timestep}
    , window_height_{window_height}
    , window_width_{window_width}
{
    int total_grid_cells = SPATIAL_GRID_SIZE * SPATIAL_GRID_SIZE;
    particle_store_.reserve(total_grid_cells);
    particle_store_.resize(total_grid_cells);
}

const Particle& ParticleManager::createParticleAtCursor(const sf::Event::MouseButtonEvent& mouse_event)
{
    sim::Vec2f position{static_cast<float>(mouse_event.x), static_cast<float>(mouse_event.y)};
    auto new_particle = std::make_unique<Particle>(position);

    // We maintain a spatial grid and particles inside it for easier collision handling
    auto cell = computeSpatialGridLocation(new_particle->position());
    new_particle->setGridLocation(cell);
    const auto& ret = particle_store_[cell].insert(std::move(new_particle));

    return **ret.first;
}

int ParticleManager::computeSpatialGridLocation(const sim::Vec2f& position)
{
    auto hash_row = int(position.x / (window_width_ / SPATIAL_GRID_SIZE));
    auto hash_col = int(position.y / (window_height_ / SPATIAL_GRID_SIZE));
    return hash_row * SPATIAL_GRID_SIZE + hash_col;
}

void ParticleManager::updateParticles()
{
    for (auto& region : particle_store_)
    {
        for (auto& particle : region)
        {
            auto new_position = particle->nextPosition(timestep_);
            handleCollisions(*particle, new_position);
        }
    }

    // Move particles to their new grid location
    for (auto& region : particle_store_)
    {
        for (auto it = region.begin(); it != region.end(); it++)
        {
            auto& particle = *it;
            const int old_grid_loc = particle->gridLocation();
            const int new_grid_loc = computeSpatialGridLocation(particle->position());

            if (old_grid_loc != new_grid_loc)
            {
                // Since we are dealing with unique pointers as keys, we have to extract the node from old and insert into new
                // We can't simply insert or emplace because unique pointers cannot be copied
                particle->setGridLocation(new_grid_loc);
                auto& old_region = particle_store_[old_grid_loc];
                particle_store_[new_grid_loc].insert(old_region.extract(it));
            }
        }

    }
}

void ParticleManager::handleCollisions(Particle& particle, sim::Vec2f& new_pos)
{
    // 1. Check that the new position for this particle is not going out of bounds
    // 2. Check the neighbouring grid cells to see if it collides with any particles
    const float radius = particle.radius();
    const float x_min = radius;
    const float x_max = window_width_ - 2*radius;
    const float y_min = radius;
    const float y_max = window_height_ - 2*radius;

    if (new_pos.x < x_min || new_pos.x > x_max)
    {
        particle.rebound(0);
    }

    if (new_pos.y < y_min || new_pos.y > y_max)
    {
        particle.rebound(1);
    }

    new_pos.clamp({x_min, x_max}, {y_min, y_max});
    particle.setPosition(new_pos);

}

const ParticleManager::ParticleStore& ParticleManager::particles() const
{
    return particle_store_;
}

}
