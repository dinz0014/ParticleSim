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

    void reset();

private:
    Vec2i getCell(const Vec2f& position);
    size_t getIndex(const Vec2i& cell);

private:
    using StoreType = std::vector<double_linked_list<Particle::id_type>>;
    StoreType grid_;
    Vec2f top_left_{0.0f, 0.0f};
    Vec2f bottom_right_{0.0f, 0.0f};

    int rows_{0};
    int cols_{0};

    static constexpr int CELL_SIZE = static_cast<int>(2 * MAX_RADIUS);

};

}