#include <iostream>
#include <algorithm>
#include <ranges>

#include "utils/utils.h"

#include "particle.h"

namespace sim {

int Particle::nextID = 0;
const sf::Color Particle::slowColour = sf::Color::Cyan;
const sf::Color Particle::fastColour = sf::Color::Magenta;
const sf::Color Particle::midColour = sf::Color(173, 255, 47); // Pastel Green

Particle::Particle(const sim::Vec2f& position, int region, float radius)
    : position_{position}
    , acceleration_{0, G}
    , radius_{radius}
    , region_{region}
    , id_{nextID++}
{
    shape_ = sf::CircleShape{radius};
    shape_.setFillColor(sf::Color::Cyan);
    shape_.setOrigin(radius, radius);
    shape_.setPosition(position);
}

void Particle::setVelocity(sim::Vec2f new_velocity)
{
    new_velocity.clamp({-MAX_VEL, MAX_VEL}, {-MAX_VEL, MAX_VEL});
    velocity_ = new_velocity;
    shape_.setFillColor(computeFillColour());
}

sf::Color Particle::computeFillColour()
{
    const auto normalised_speed = std::clamp(vec_dot(velocity_, velocity_) / (MAX_VEL * MAX_VEL), 0.0f, 1.0f);

    auto interpolate_colour = [](float start, float end, float val)
    {
        return static_cast<sf::Uint8>(start + val * (end - start));
    };

    auto final_colour = [&](sf::Color start_colour, sf::Color end_colour)
    {
        return sf::Color(
            interpolate_colour(start_colour.r, end_colour.r, normalised_speed),
            interpolate_colour(start_colour.g, end_colour.g, normalised_speed),
            interpolate_colour(start_colour.b, end_colour.b, normalised_speed)
        );
    };

    sf::Color curr_colour = shape_.getFillColor();

    if (normalised_speed < 0.5f)
    {
        return final_colour(slowColour, midColour);
    }
    else
    {
        return final_colour(midColour, fastColour);
    }
}

const sim::Vec2f& Particle::nextPosition(float timestep)
{
    sim::Vec2f v_half = velocity_ + 0.5f * timestep * acceleration_;
    changeVelocity(acceleration_ * timestep);
    position_ += v_half * timestep;
    shape_.setPosition(position_);
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

    shape_.setFillColor(computeFillColour());
}

void Particle::move(const sim::Vec2f& pos_delta)
{
    position_ += pos_delta;
}

ParticleManager::ParticleManager(float timestep, int window_height, int window_width)
    : timestep_{timestep}
    , window_height_{window_height}
    , window_width_{window_width}
{
}

const Particle& ParticleManager::createParticleAtCursor(const sf::Event::MouseButtonEvent& mouse_event)
{
    auto x = static_cast<float>(mouse_event.x);
    auto y = static_cast<float>(mouse_event.y);
    const float radius = 10.0f;

    const float x_min = radius;
    const float x_max = window_width_ -  radius;
    const float y_min = radius;
    const float y_max = window_height_ - radius;


    sim::Vec2f position{x, y};
    position.clamp({x_min, x_max}, {y_min, y_max});

    // We maintain a spatial grid and particles inside it for easier collision handling
    auto cell = computeFlattenedLocation(position);
    auto region = computeRegion(position);
    auto new_particle = std::make_unique<Particle>(position, region, radius);
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
        particle.setAcceleration(sim::Vec2f{0, G});
        particle.nextPosition(timestep_);
        resolveOutOfBounds(particle);

        updateGrid();

        resolveCollisions(particle);
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
    const float x_min = radius;
    const float x_max = window_width_ -  radius;
    const float y_min = radius;
    const float y_max = window_height_ - radius;

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
            sim::Vec2f norm = axis / dist;
            const float delta = 0.5 * std::abs(dist - min_dist);
            particle.move(norm * delta);
            neighbour.move(norm * -delta);

            // Assuming mass is equal
            sim::Vec2f delta_vel = vec_dot(norm, particle.velocity() - neighbour.velocity()) * norm ;
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
