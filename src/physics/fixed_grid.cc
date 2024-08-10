#include "fixed_grid.h"

namespace sim {

FixedGrid::FixedGrid(int max_particle_radius, const Vec2f& top_left, const Vec2f& bottom_right)
    : top_left_{top_left}
    , bottom_right_{bottom_right}
    , m_cell_size{2 * max_particle_radius}
{
    rows_ = static_cast<int>(( bottom_right_.y - top_left_.y ) / m_cell_size);
    cols_ = static_cast<int>(( bottom_right_.x - top_left_.x ) / m_cell_size);
    grid_.resize(rows_ * cols_);
}

void FixedGrid::reset()
{
    rows_ = static_cast<int>(( bottom_right_.y - top_left_.y ) / m_cell_size);
    cols_ = static_cast<int>(( bottom_right_.x - top_left_.x ) / m_cell_size);
    grid_.clear();
    grid_.resize(rows_ * cols_);
}

Vec2i FixedGrid::getCell(const Vec2f& position)
{
    const Vec2f rel_pos = position - top_left_;
    const int c = static_cast<int>(rel_pos.x / m_cell_size);
    const int r = static_cast<int>(rel_pos.y / m_cell_size);
    return Vec2i{r, c};
}

size_t FixedGrid::getIndex(const Vec2i& cell)
{
    return cell[0] * cols_ + cell[1];
}

void FixedGrid::add(Particle& entity)
{
    const Vec2i cell = getCell(entity.position());
    entity.setRegion(cell);
    const auto idx = getIndex(cell);
    grid_[idx].push_back(entity.id());
}

void FixedGrid::remove(Particle& entity)
{
    const Vec2i cell = entity.region();
    const auto idx = getIndex(cell);
    auto it = grid_[idx].find(entity.
    id());
    grid_[idx].erase(it);
}

std::vector<Particle::id_type> FixedGrid::getNearby(Particle& entity)
{
    std::vector<Particle::id_type> neighbours;

    const Vec2i cell = entity.region();

    // This loop adds all neighbours from top-left, top, left and same cell
    for (int dx = -1; dx <= 0; ++dx)
    {
        for (int dy = -1; dy <= 0; ++dy)
        {
            const Vec2i new_cell = cell + Vec2i{dx, dy};
            if (new_cell.x < 0 || new_cell.x >= rows_ || new_cell.y < 0 || new_cell.y >= cols_)
            {
                continue;
            }

            const auto idx = getIndex(new_cell);

            for (const auto id : grid_[idx])
            {
                if (id == entity.id()) { continue; }
                neighbours.push_back(id);
            }
        }
    }

    // Add neighbours from bottom-left cell
    const Vec2i bl_cell{cell.x + 1, cell.y - 1};
    if (bl_cell.x < rows_ && bl_cell.y > 0)
    {
        const auto idx = getIndex(bl_cell);
        for (const auto id : grid_[idx])
            {
                neighbours.push_back(id);
            }
    }

    return neighbours;
}


}