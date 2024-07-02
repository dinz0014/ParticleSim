#include <iostream>
#include <algorithm>
#include <ranges>

#include "utils/utils.h"

#include "particle.h"

namespace sim {

int Particle::nextID = 0;

Particle::Particle(const sim::Vec2f& position, int region, float radius)
{
    position_ = position;
    acceleration_ = {0, G};
    radius_ = radius;
    region_ = region;
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

void Particle::rebound(int axis)
{
    const float delta = std::abs(velocity_[axis] - velocity_[axis] * DAMPING_CONSTANT);
    if (delta > epsilon)
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

float Particle::distanceFrom(Particle& other)
{
    sim::Vec2f displacement = other.position() - position();
    return displacement.magnitude();
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
}

const Particle& ParticleManager::createParticleAtCursor(const sf::Event::MouseButtonEvent& mouse_event)
{
    sim::Vec2f position{static_cast<float>(mouse_event.x), static_cast<float>(mouse_event.y)};

    // We maintain a spatial grid and particles inside it for easier collision handling
    auto cell = computeFlattenedLocation(position);
    auto region = computeRegion(position);
    auto new_particle = std::make_unique<Particle>(position, region);

    particle_store_[cell] = std::move(new_particle);

    return *particle_store_[cell];
}

int ParticleManager::computeRegion(const sim::Vec2f& position)
{
    const auto hash_row = int(position.y / (window_height_ / GRID_SIZE));
    const auto hash_col = int(position.x / (window_width_ / GRID_SIZE));
    return hash_row * GRID_SIZE + hash_col;
}

int ParticleManager::computeFlattenedLocation(const sim::Vec2f& position)
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

void ParticleManager::updateParticles()
{
    for (auto& particle : deref_non_null_view(particle_store_))
    {
        auto new_position = particle.nextPosition(timestep_);
        resolveOutOfBounds(particle, new_position);
    }

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

    // for (auto& region : particle_store_)
    // {
    //     for (auto& particle : region)
    //     {
    //         resolveCollisions(*particle);
    //     }
    // }
}

void ParticleManager::resolveOutOfBounds(Particle& particle, sim::Vec2f& new_pos)
{
    // Window Bound Checking
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

void ParticleManager::resolveCollisions(Particle& particle)
{
    // const auto grid_loc = particle.gridLocation();

    // for (auto offset : neighbour_offsets_)
    // {
    //     auto& curr_region = particle_store_[grid_loc + offset];

    //     for (auto& neighbour : curr_region)
    //     {
    //         if (*neighbour == particle)
    //         {
    //             continue;
    //         }

    //         const auto distance = particle.distanceFrom(*neighbour);

    //         if (distance > particle.radius() + neighbour->radius())
    //         {
    //             continue;
    //         }

    //         sim::Vec2f relative_velocity = neighbour->velocity() - particle.velocity();
    //         const auto p1_mass = particle.mass();
    //         const auto p2_mass = neighbour->mass();
    //         sim::Vec2f p1_impulse = relative_velocity * p2_mass * (1 + restitution_coeff);
    //         sim::Vec2f p2_impulse = relative_velocity * p1_mass * (1 + restitution_coeff);
    //         sim::Vec2f p1_new_vel =  particle.velocity() + p1_impulse/(p1_mass + p2_mass);
    //         sim::Vec2f p2_new_vel = neighbour->velocity() - p2_impulse/(p1_mass + p2_mass);

    //         particle.setVelocity(p1_new_vel);
    //         neighbour->setVelocity(p2_new_vel);
    //     }
    // }
}

const ParticleManager::ParticleStore& ParticleManager::particles() const
{
    return particle_store_;
}

}
