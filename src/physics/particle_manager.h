#pragma once
#include <array>
#include <memory>
#include <nlohmann/json.hpp>
#include "particle.h"
#include "fixed_grid.h"
#include "types.h"

namespace sim {

using json = nlohmann::json;

class ParticleManager
{
public:
    using ParticleStore = std::vector<Particle>;

    ParticleManager(const json& cfg, Container& container);

    static ValidationResult ValidateConfig(const json& manager_cfg);
    void LoadConfig();

    const Particle& createParticleAtCursor(float x, float y);

    void resolveOutOfBounds(Particle& particle);
    void resolveCollisions(Particle& particle);
    void updateParticles(float dt);
    void updateGrid();

    const ParticleStore& particles() const;

    size_t particle_count() const;
    void clear();

private:
    json cfg_;

    ParticleStore particles_;
    Container& container_;
    std::unique_ptr<FixedGrid> grid_;

    float grav_accel_;
    float particle_radius_;
    float wall_collision_damping_;
    float rel_rebound_spd_change_threshold_;
    float max_1d_spd_;
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
