#include <algorithm>
#include <powda/world.hpp>
#include "powda/materials.hpp"

namespace powda
{

World::World(unsigned int width, unsigned int height)
    : m_width{width}
    , m_height{height}
    , m_material_count{m_width * m_height}
    , m_powders{}
    , m_walls{}
{
}

void World::next_step()
{
    const auto snapshot_powders{m_powders};
    m_powders.clear();

    for (const auto [x, y] : snapshot_powders)
    {
        const auto below = get(x, (y - 1) % m_height, snapshot_powders, m_walls);
        if (below == Materials::Empty)
        {
            m_powders.emplace_back(x, (y - 1) % m_height);
            continue;
        }

        const auto left  = get((x - 1) % m_width, y, snapshot_powders, m_walls);
        const auto right = get((x + 1) % m_width, y, snapshot_powders, m_walls);
        const auto below_left =
            get((x - 1 % m_width), (y - 1) % m_height, snapshot_powders, m_walls);
        const auto below_right =
            get((x + 1) % m_width, (y - 1) % m_height, snapshot_powders, m_walls);

        if (right == Materials::Empty && below_right == Materials::Empty)
        {
            m_powders.emplace_back((x + 1) % m_width, (y - 1) % m_height);
        }
        else if (left == Materials::Empty && below_left == Materials::Empty)
        {
            m_powders.emplace_back((x - 1) % m_width, (y - 1) % m_height);
        }
        else if (below == Materials::Wall || below == Materials::Powder)
        {
            m_powders.emplace_back(x, y);
        }
    }
}

void World::set(unsigned int x, unsigned int y, Materials mat)
{
    coord c{x, y};
    auto  walls_it = std::find(m_walls.begin(), m_walls.end(), c);
    if (walls_it != m_walls.end() && mat != Materials::Wall)
    {
        m_walls.erase(walls_it);
    }

    auto powders_it = std::find(m_powders.begin(), m_powders.end(), c);
    if (powders_it != m_powders.end() && mat != Materials::Powder)
    {
        m_powders.erase(powders_it);
    }

    if (mat == Materials::Powder)
    {
        m_powders.push_back(c);
    }
    else if (mat == Materials::Wall)
    {
        m_walls.push_back(c);
    }
}

Materials World::get(unsigned int x, unsigned int y) const
{
    return get(x, y, m_powders, m_walls);
}

Materials World::get(
    unsigned int              x,
    unsigned int              y,
    const std::vector<coord>& powders,
    const std::vector<coord>& walls
) const
{
    coord c{x, y};
    auto  walls_it = std::find(walls.begin(), walls.end(), c);
    if (walls_it != walls.end())
    {
        return Materials::Wall;
    }

    auto powders_it = std::find(powders.begin(), powders.end(), c);
    if (powders_it != powders.end())
    {
        return Materials::Powder;
    }

    return Materials::Empty;
}

} // namespace powda
