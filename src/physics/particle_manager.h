#pragma once
#include <array>
#include <memory>
#include "particle.h"

namespace sim {

class ParticleManager
{
public:
    static constexpr int GRID_SIZE = 8;
    static constexpr int MAX_PER_CELL = 5;

    using ParticleStore = std::array<std::unique_ptr<Particle>, GRID_SIZE * GRID_SIZE * MAX_PER_CELL>;

    ParticleManager(Container& container);

    const Particle& createParticleAtCursor(float x, float y);

    void resolveOutOfBounds(Particle& particle);
    void resolveCollisions(Particle& particle);
    void updateParticles(float dt);
    void updateGrid();

    const ParticleStore& particles() const;

private:
    int computeRegion(const Vec2f& position);
    int computeFlattenedLocation(const Vec2f& position);

    using BoundsType = std::pair<Vec2f, Vec2f>;
    BoundsType getMinMaxBounds();

    ParticleStore particle_store_;

    Container& container_;

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
