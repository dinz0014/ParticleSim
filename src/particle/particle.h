#pragma once
#include <array>
#include <algorithm>
#include <cmath>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "vector/vector.h"

namespace sim {

class ParticleStore;

constexpr inline float G = 800.0;
constexpr inline float DAMPING_CONSTANT = 0.85;
constexpr inline float restitution_coeff = 0.9;
constexpr inline float epsilon = 6.0;

class Particle
{
public:
    static int nextID;

    Particle(const sim::Vec2f& position, int grid_loc, float radius = 10.0f);

    // It doesn't make sense to copy/move particles to other particles
    Particle(const Particle& other) = delete;
    Particle& operator=(const Particle& other) = delete;
    Particle(Particle&& other) noexcept = delete;
    Particle& operator=(Particle&& other) = delete;

    const sim::Vec2f& nextPosition(float timestep);

    void move(const sim::Vec2f& pos_delta);

    const sf::CircleShape& shape() const
    {
        return shape_;
    }

    sim::Vec2f& position()
    {
        return position_;
    }

    const sim::Vec2f& acceleration() const
    {
        return acceleration_;
    }

    const sim::Vec2f& velocity() const
    {
        return velocity_;
    }

    void rebound(int axis);

    void setPosition(const sim::Vec2f& new_position)
    {
        position_ = new_position;
    }

    void setAcceleration(const sim::Vec2f& accel)
    {
        acceleration_ = accel;
    }

    void setRegion(int region)
    {
        region_ = region;
    }

    int region() const
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

    void setVelocity(const sim::Vec2f& new_velocity)
    {
        velocity_ = new_velocity;
    }

    void changeVelocity(const sim::Vec2f& delta_vel)
    {
        velocity_ += delta_vel;
    }

    float mass() const
    {
        return mass_;
    }

    bool operator==(const Particle& other) const
    {
        return id_ == other.id();
    }

protected:
    sim::Vec2f position_;
    sim::Vec2f prev_position_;
    sim::Vec2f acceleration_;
    sim::Vec2f velocity_;
    float radius_;
    float mass_;
    int region_;
    int id_;

    sf::CircleShape shape_;
};

class ParticleManager
{
public:
    static constexpr int GRID_SIZE = 8;
    static constexpr int MAX_PER_CELL = 5;

    using ParticleStore = std::array<std::unique_ptr<Particle>, GRID_SIZE * GRID_SIZE * MAX_PER_CELL>;

    ParticleManager(float timestep, int window_height, int window_width);

    const Particle& createParticleAtCursor(const sf::Event::MouseButtonEvent& mouse_event);
    void resolveOutOfBounds(Particle& particle);
    void resolveCollisions(Particle& particle);
    void updateParticles();
    void updateGrid();

    const ParticleStore& particles() const;

private:
    int computeRegion(const sim::Vec2f& position);
    int computeFlattenedLocation(const sim::Vec2f& position);

    ParticleStore particle_store_;

    float timestep_{0.0f};
    int window_height_;
    int window_width_;

    std::array<int, 9> neighbour_offsets_ = {
        -GRID_SIZE - 1,
            -GRID_SIZE,
        -GRID_SIZE + 1,
                    -1,
                     0,
                     1,
         GRID_SIZE - 1,
             GRID_SIZE,
         GRID_SIZE + 1,
    };
};

}

namespace std {
    template<>
    struct hash<std::unique_ptr<sim::Particle>> {
        std::size_t operator()(const std::unique_ptr<sim::Particle>& particle) const {
            return std::hash<int>()(particle->id());
        }
    };
}
