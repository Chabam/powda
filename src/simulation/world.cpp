#include <algorithm>
#include <functional>
#include <map>

#include <powda/simulation/materials.hpp>
#include <powda/simulation/world.hpp>

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

    using Cache = std::map<coord, Materials>;
    const auto lookup =
        [this](unsigned int x, unsigned int y, Cache& cache, const Powders& powders) -> Materials {
        const coord c{x, y};
        if (cache.contains(c))
            return cache[c];

        const auto mat = get(x, y, powders, m_walls);
        cache[c]       = mat;
        return mat;
    };

    Cache snapshot_cache;
    Cache future_cache;
    using namespace std::placeholders;
    auto lookup_snapshot =
        std::bind(lookup, _1, _2, std::ref(snapshot_cache), std::cref(snapshot_powders));
    auto lookup_future = std::bind(lookup, _1, _2, std::ref(future_cache), std::cref(m_powders));

    for (const auto [x, y] : snapshot_powders)
    {
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = lookup_snapshot(x, down_offset);

        if (below == Materials::Empty || lookup_future(x, down_offset) == Materials::Empty)
        {
            if (below != Materials::Empty)
                future_cache[{x, down_offset}] = Materials::Powder;

            m_powders.emplace_back(x, down_offset);
            continue;
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_width - 1);

        const auto left        = lookup_snapshot(left_offset, y);
        const auto right       = lookup_snapshot(right_offset, y);
        const auto below_left  = lookup_snapshot(left_offset, down_offset);
        const auto below_right = lookup_snapshot(right_offset, down_offset);

        if (right == Materials::Empty && below_right == Materials::Empty)
        {
            m_powders.emplace_back(right_offset, down_offset);
        }
        else if (left == Materials::Empty && below_left == Materials::Empty)
        {
            m_powders.emplace_back(left_offset, down_offset);
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
    unsigned int   x,
    unsigned int   y,
    const Powders& powders,
    const Walls&   walls
) const
{
    coord c{x, y};
    auto  walls_it = std::find(walls.begin(), walls.end(), c);
    if (walls_it != walls.end())
    {
        return Materials::Wall;
    }

    auto powders_it         = std::find(powders.begin(), powders.end(), c);
    if (powders_it != powders.end())
    {
        return Materials::Powder;
    }

    return Materials::Empty;
}

} // namespace powda
