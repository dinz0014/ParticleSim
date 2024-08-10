#include "common/utils.h"
#include <iostream>

#include "particle_manager.h"

namespace sim {

ParticleManager::ParticleManager(const json& cfg, Container& container)
    : container_{container}
{
    cfg_ = cfg;
    LoadConfig();

    // Initialise the fixed grid
    const auto& [x_bounds, y_bounds] = container_.getBounds();
    const Vec2f top_left{x_bounds[0], y_bounds[0]};
    const Vec2f bottom_right{x_bounds[1], y_bounds[1]};
    grid_ = std::make_unique<FixedGrid>(static_cast<int>(particle_radius_), top_left, bottom_right);
}

ValidationResult ParticleManager::ValidateConfig(const json& manager_cfg)
{
    if (!manager_cfg.contains("particle"))
    {
        return "particle config is required, but absent";
    }

    if (!manager_cfg.contains("grav_accel"))
    {
        return "grav_accel is required, but absent";
    }

    if (manager_cfg.contains("wall_collision_damping_constant")
        && (!manager_cfg["wall_collision_damping_constant"].is_number_float() || !(manager_cfg["wall_collision_damping_constant"] > 0.0f)))
    {
        return "wall_collision_damping_constant must be a positive floating point number";
    }

    if (manager_cfg.contains("max_1d_spd")
        && (!manager_cfg["max_1d_spd"].is_number_float() || !(manager_cfg["max_1d_spd"] > 0.0f)))
    {
        return "max_1d_spd must be a positive floating point number";
    }

    const json& particle_cfg = manager_cfg["particle"];

    if (!particle_cfg.contains("radius"))
    {
        return "radius is required in the particle config, but absent";
    }

    if (!particle_cfg["radius"].is_number_float() && !(particle_cfg["radius"] > 0.0f))
    {
        return "the radius in particle config must be a positive floating point number";
    }

    if (!manager_cfg["grav_accel"].is_number_float() && !(manager_cfg["grav_accel"] > 0.0f))
    {
        return "grav_accel must be a positive floating point number";
    }

    if (particle_cfg.contains("rel_rb_spd_change_threshold")
        && (!particle_cfg["rel_rb_spd_change_threshold"].is_number_float() || !(particle_cfg["rel_rb_spd_change_threshold"] >= 0.0f)))
    {
        return "rel_rb_spd_change_threshold must be a positive floating point number";
    }

    return "";
}

void ParticleManager::LoadConfig()
{
    grav_accel_ = cfg_["grav_accel"];

    const json& particle_cfg = cfg_["particle"];
    particle_radius_ = static_cast<float>(particle_cfg["radius"]);

    rel_rebound_spd_change_threshold_ = get(particle_cfg, "rel_rb_spd_change_threshold", 0.01f);
    wall_collision_damping_ = get(cfg_, "wall_collision_damping_constant", 1.0f);
    max_1d_spd_ = get(cfg_, "max_1d_spd", 1000.0f);
}

const Particle& ParticleManager::createParticleAtCursor(float x, float y)
{
    const auto& [x_bounds, y_bounds] = container_.getBounds(particle_radius_);
    const auto& [x_min, x_max] = x_bounds;
    const auto& [y_min, y_max] = y_bounds;

    Vec2f position{x, y};
    position.clamp({x_min, x_max}, {y_min, y_max});

    Particle p{position, particle_radius_, max_1d_spd_};
    grid_->add(p);
    particles_.push_back(std::move(p));

    return particles_.back();
}

void ParticleManager::updateParticles(float dt)
{
    updateGrid();

    for (auto& particle : particles_)
    {
        particle.setAcceleration(Vec2f{0, grav_accel_});
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
        grid_->remove(particle);
        grid_->add(particle);
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
        particle.rebound(0, wall_collision_damping_, rel_rebound_spd_change_threshold_);
    }

    if (pos.y < y_min || pos.y > y_max)
    {
        particle.rebound(1, wall_collision_damping_, rel_rebound_spd_change_threshold_);
    }

    pos.clamp({x_min, x_max}, {y_min, y_max});
}

void ParticleManager::resolveCollisions(Particle& particle)
{
    auto neighbours = grid_->getNearby(particle);

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
    grid_->reset();
}

}
