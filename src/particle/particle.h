#pragma once
#include <memory>
#include <tuple>
#include <unordered_set>
#include <SFML/Graphics.hpp>
#include "vector/vector.h"

namespace sim {

constexpr inline float G = 800.0;
constexpr inline float DAMPING_CONSTANT = 0.85;
constexpr inline float epsilon = 6.0;

class Particle
{
public:
    static int nextID;

    Particle(const sim::Vec2f& position, float radius = 10.0f);

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

    const sim::Vec2f& acceleration() const
    {
        return acceleration_;
    }

    const sim::Vec2f& velocity() const
    {
        return velocity_;
    }

    const sim::Vec2f& position() const
    {
        return position_;
    }

    void rebound(int axis)
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

    void setPosition(const sim::Vec2f& new_position)
    {
        position_ = new_position;
        shape_.setPosition(position_);
    }

    void setGridLocation(int cell_no)
    {
        grid_location_ = cell_no;
    }

    int gridLocation() const
    {
        return grid_location_;
    }

    float radius() const
    {
        return radius_;
    }

    int id() const
    {
        return id_;
    }

    bool operator==(const Particle& other) const
    {
        return id_ == other.id();
    }

protected:
    sim::Vec2f position_;
    sim::Vec2f acceleration_;
    sim::Vec2f velocity_;
    float radius_;
    int grid_location_;
    int id_;

    sf::CircleShape shape_;
};

class ParticleManager
{
public:
    static constexpr int SPATIAL_GRID_SIZE = 16;

    using ParticleStore = std::vector<std::unordered_set<std::unique_ptr<Particle>>>;

    ParticleManager(float timestep, int window_height, int window_width);

    const Particle& createParticleAtCursor(const sf::Event::MouseButtonEvent& mouse_event);
    void handleCollisions(Particle& particle, sim::Vec2f& new_pos);
    void updateParticles();

    const ParticleStore& particles() const;

private:
    int computeSpatialGridLocation(const sim::Vec2f& particle);

    ParticleStore particle_store_;

    float timestep_{0.0f};
    int window_height_;
    int window_width_;
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
