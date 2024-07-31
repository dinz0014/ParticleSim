#pragma once
#include <array>
#include <memory>
#include "particle.h"
#include "fixed_grid.h"

namespace sim {

class ParticleManager
{
public:
    using ParticleStore = std::vector<Particle>;

    ParticleManager(Container& container);

    const Particle& createParticleAtCursor(float x, float y);

    void resolveOutOfBounds(Particle& particle);
    void resolveCollisions(Particle& particle);
    void updateParticles(float dt);
    void updateGrid();

    const ParticleStore& particles() const;

    size_t particle_count() const;
    void clear();

private:
    using BoundsType = std::pair<Vec2f, Vec2f>;
    BoundsType getMinMaxBounds();

    FixedGrid partitioner_;
    ParticleStore particles_;
    Container& container_;
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
