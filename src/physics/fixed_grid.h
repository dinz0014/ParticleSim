#pragma once

#include "common/constants.h"
#include "include/doubly_linked_list.h"

#include "particle.h"

namespace sim {

class FixedGrid
{
public:
    FixedGrid() = default;
    FixedGrid(const Vec2f& top_left, const Vec2f& bottom_right);

    void add(Particle& entity);
    void remove(Particle& entity);
    std::vector<Particle::id_type> getNearby(Particle& entity);

private:
    Vec2i getCell(const Vec2f& position);
    size_t getIndex(const Vec2i& cell);

private:
    using StoreType = std::vector<double_linked_list<Particle::id_type>>;
    StoreType grid_;
    Vec2f top_left_{0.0f, 0.0f};
    Vec2f bottom_right_{0.0f, 0.0f};

    uint8_t rows_{0};
    uint8_t cols_{0};

    static constexpr uint8_t CELL_SIZE = static_cast<uint8_t>(2 * MAX_RADIUS);

};

}