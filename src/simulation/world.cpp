#include <algorithm>
#include <cassert>
#include <functional>
#include <iterator>
#include <map>
#include <type_traits>

#include <powda/simulation/material.hpp>
#include <powda/simulation/world.hpp>
#include <powda/logger.hpp>

namespace powda
{

World::World(unsigned int width, unsigned int height)
    : m_width{width}
    , m_height{height}
    , m_material_count{m_width * m_height}
    , m_powders{}
    , m_liquids{}
    , m_walls{}
{
}

void World::next_step()
{
    auto current_powders{m_powders};
    m_powders.clear();

    auto current_liquids{m_liquids};
    m_liquids.clear();

    using Cache       = std::map<coord, Material::Type>;
    const auto lookup = [this](
                            unsigned int              x,
                            unsigned int              y,
                            Cache&                    cache,
                            const std::vector<coord>& powders,
                            const std::vector<coord>& liquids
                        ) -> Material::Type {
        const coord c{x, y};
        if (cache.contains(c))
            return cache[c];

        const auto mat = get(x, y, powders, liquids, m_walls);
        cache[c]       = mat;
        return mat;
    };

    Cache current_cache;
    Cache next_cache;
    using namespace std::placeholders;
    auto lookup_next =
        std::bind(lookup, _1, _2, std::ref(next_cache), std::cref(m_powders), std::cref(m_liquids));
    auto lookup_current = std::bind(
        lookup,
        _1,
        _2,
        std::ref(current_cache),
        std::cref(current_powders),
        std::cref(current_liquids)
    );

    auto set_next_material =
        [&next_cache](auto& materials, unsigned int x, unsigned int y, Material::Type mat) {
            materials.emplace_back(x, y);
            next_cache[{x, y}] = mat;
        };

    auto is_empty = [](std::initializer_list<Material::Type> range) -> bool {
        return std::all_of(
            range.begin(), range.end(), std::bind(std::equal_to<Material::Type>{}, _1, Material::Type::Empty)
        );
    };

    for (const auto [x, y] : current_powders)
    {
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = lookup_current(x, down_offset);
        const auto below_next  = lookup_next(x, down_offset);
        if (y == 0)
        {
            set_next_material(m_powders, x, y, Material::Type::Powder);
            continue;
        }

        if (is_empty({below, below_next}))
        {
            set_next_material(m_powders, x, down_offset, Material::Type::Powder);
            continue;
        }

        if (below == Material::Type::Liquid)
        {
            set_next_material(m_powders, x, down_offset, Material::Type::Powder);
            set_next_material(m_liquids, x, y, Material::Type::Liquid);
            current_liquids.erase(
                std::find(current_liquids.begin(), current_liquids.end(), coord{x, down_offset})
            );
            continue;
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_width - 1);

        const auto left        = lookup_current(left_offset, y);
        const auto right       = lookup_current(right_offset, y);
        const auto below_left  = lookup_current(left_offset, down_offset);
        const auto below_right = lookup_current(right_offset, down_offset);

        const auto right_next       = lookup_next(right_offset, y);
        const auto left_next        = lookup_next(left_offset, y);
        const auto below_left_next  = lookup_next(left_offset, down_offset);
        const auto below_right_next = lookup_next(right_offset, down_offset);

        if (is_empty({left, left_next, below_left, below_left_next}))
        {
            set_next_material(m_powders, left_offset, down_offset, Material::Type::Powder);
        }
        else if (is_empty({right, right_next, below_right, below_right_next}))
        {
            set_next_material(m_powders, right_offset, down_offset, Material::Type::Powder);
        }
        else
        {
            set_next_material(m_powders, x, y, Material::Type::Powder);
        }
    }

    for (const auto [x, y] : current_liquids)
    {
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = lookup_current(x, down_offset);
        const auto below_next  = lookup_next(x, down_offset);

        if (y != 0 && is_empty({below, below_next}))
        {
            set_next_material(m_liquids, x, down_offset, Material::Type::Liquid);
            continue;
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_width - 1);

        const auto left        = lookup_current(left_offset, y);
        const auto right       = lookup_current(right_offset, y);
        const auto below_left  = lookup_current(left_offset, down_offset);
        const auto below_right = lookup_current(right_offset, down_offset);

        const auto right_next       = lookup_next(right_offset, y);
        const auto left_next        = lookup_next(left_offset, y);
        const auto below_left_next  = lookup_next(left_offset, down_offset);
        const auto below_right_next = lookup_next(right_offset, down_offset);


        if (y != 0 && is_empty({right, right_next, below_right, below_right_next}))
        {
            set_next_material(m_liquids, right_offset, down_offset, Material::Type::Liquid);
        }
        else if (y != 0 && is_empty({left, left_next, below_left, below_left_next}))
        {
            set_next_material(m_liquids, left_offset, down_offset, Material::Type::Liquid);
        }
        else if (is_empty({right, right_next}))
        {
            set_next_material(m_liquids, right_offset, y, Material::Type::Liquid);
        }
        else if (is_empty({left, left_next}))
        {
            set_next_material(m_liquids, left_offset, y, Material::Type::Liquid);
        }
        else
        {
            set_next_material(m_liquids, x, y, Material::Type::Liquid);
        }
    }
}

void World::set(unsigned int x, unsigned int y, Material::Type mat)
{
    const auto set_material = [this](const coord& c, Material::Type mat) {
        if (mat == Material::Type::Powder)
            m_powders.push_back(c);
        else if (mat == Material::Type::Liquid)
            m_liquids.push_back(c);
        else if (mat == Material::Type::Wall)
            m_walls.push_back(c);
    };

    coord c{x, y};
    auto  walls_it = std::find(m_walls.begin(), m_walls.end(), c);
    if (walls_it != m_walls.end())
    {
        if (mat == Material::Type::Wall)
            return;

        m_walls.erase(walls_it);
        set_material(c, mat);
        return;
    }

    auto powders_it = std::find(m_powders.begin(), m_powders.end(), c);
    if (powders_it != m_powders.end())
    {
        if (mat == Material::Type::Powder)
            return;

        m_powders.erase(powders_it);
        set_material(c, mat);
        return;
    }

    auto liquids_it = std::find(m_liquids.begin(), m_liquids.end(), c);
    if (liquids_it != m_liquids.end())
    {
        if (mat == Material::Type::Liquid)
            return;

        m_liquids.erase(liquids_it);
        set_material(c, mat);
        return;
    }

    set_material(c, mat);
}

    Material::Type World::get(unsigned int x, unsigned int y) const
    {
        return get(x, y, m_powders, m_liquids, m_walls);
    }

    Material::Type World::get(
        unsigned int              x,
        unsigned int              y,
        const std::vector<coord>& powders,
        const std::vector<coord>& liquids,
        const std::vector<coord>& walls
    ) const
    {
        coord      c{x, y};
        const auto is_material = [&c](const auto& materials) {
            return std::find(materials.begin(), materials.end(), c) != materials.end();
        };

        if (is_material(walls))
            return Material::Type::Wall;

        if (is_material(powders))
            return Material::Type::Powder;

        if (is_material(liquids))
            return Material::Type::Liquid;

        return Material::Type::Empty;
    }

} // namespace powda
