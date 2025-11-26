#include <powda/simulation/gravity_simulation.hpp>

#include <algorithm>
#include <functional>

#include <powda/simulation/world.hpp>
#include <powda/task/simulate_powders.hpp>
#include <powda/simulation/material.hpp>

namespace powda
{

GravitySimulation::GravitySimulation(const std::shared_ptr<World>& world)
    : m_world{world}
    , m_powders{}
    , m_liquids{}
{
    size_t i = 0;
    for (const auto& mat : *m_world)
    {
        unsigned int x = i % m_world->width();
        unsigned int y = i / m_world->width();
        if (mat.m_type == Material::Type::Powder)
        {
            m_powders.emplace_back(x, y);
        }
        else if (mat.m_type == Material::Type::Liquid)
        {
            m_liquids.emplace_back(x, y);
        }
        ++i;
    }
}

void GravitySimulation::next()
{
    World new_world{m_world->width(), m_world->height()};
    auto  current_powders{m_powders};
    m_powders.clear();

    auto current_liquids{m_liquids};
    m_liquids.clear();

    using namespace std::placeholders;
    auto is_empty = [](std::initializer_list<Material::Type> range) -> bool {
        return std::all_of(
            range.begin(),
            range.end(),
            std::bind(std::equal_to<Material::Type>{}, _1, Material::Type::Empty)
        );
    };

    for (const auto [x, y] : current_powders)
    {
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = m_world->get(x, down_offset);
        const auto below_next  = new_world.get(x, down_offset);
        if (y == 0)
        {
            new_world.set(x, y, Material::Type::Powder);
            m_powders.emplace_back(x, down_offset);
            continue;
        }

        if (is_empty({below, below_next}))
        {
            new_world.set(x, down_offset, Material::Type::Powder);
            m_powders.emplace_back(x, down_offset);
            continue;
        }

        // if (below == Material::Type::Liquid)
        // {
        //     new_world.set(x, down_offset, Material::Type::Powder);
        //     new_world.set(x, y, Material::Type::Liquid);
        //     current_liquids.erase(
        //         std::find(current_liquids.begin(), current_liquids.end(), coord{x, down_offset})
        //     );
        //     continue;
        // }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto left        = m_world->get(left_offset, y);
        const auto right       = m_world->get(right_offset, y);
        const auto below_left  = m_world->get(left_offset, down_offset);
        const auto below_right = m_world->get(right_offset, down_offset);

        const auto right_next       = new_world.get(right_offset, y);
        const auto left_next        = new_world.get(left_offset, y);
        const auto below_left_next  = new_world.get(left_offset, down_offset);
        const auto below_right_next = new_world.get(right_offset, down_offset);

        if (is_empty({left, left_next, below_left, below_left_next}))
        {
            new_world.set(left_offset, down_offset, Material::Type::Powder);
            m_powders.emplace_back(left_offset, down_offset);
        }
        else if (is_empty({right, right_next, below_right, below_right_next}))
        {
            new_world.set(right_offset, down_offset, Material::Type::Powder);
            m_powders.emplace_back(right_offset, down_offset);
        }
        else
        {
            new_world.set(x, y, Material::Type::Powder);
            m_powders.emplace_back(x, y);
        }
    }

    for (const auto [x, y] : current_liquids)
    {
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = m_world->get(x, down_offset);
        const auto below_next  = new_world.get(x, down_offset);

        if (y != 0 && is_empty({below, below_next}))
        {
            new_world.set(x, down_offset, Material::Type::Liquid);
            m_liquids.emplace_back(x, down_offset);
            continue;
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto left        = m_world->get(left_offset, y);
        const auto right       = m_world->get(right_offset, y);
        const auto below_left  = m_world->get(left_offset, down_offset);
        const auto below_right = m_world->get(right_offset, down_offset);

        const auto right_next       = new_world.get(right_offset, y);
        const auto left_next        = new_world.get(left_offset, y);
        const auto below_left_next  = new_world.get(left_offset, down_offset);
        const auto below_right_next = new_world.get(right_offset, down_offset);

        if (y != 0 && is_empty({right, right_next, below_right, below_right_next}))
        {
            new_world.set(right_offset, down_offset, Material::Type::Liquid);
            m_liquids.emplace_back(right_offset, down_offset);
        }
        else if (y != 0 && is_empty({left, left_next, below_left, below_left_next}))
        {
            new_world.set(left_offset, down_offset, Material::Type::Liquid);
            m_liquids.emplace_back(left_offset, down_offset);
        }
        else if (is_empty({right, right_next}))
        {
            new_world.set(right_offset, y, Material::Type::Liquid);
            m_liquids.emplace_back(right_offset, y);
        }
        else if (is_empty({left, left_next}))
        {
            new_world.set(left_offset, y, Material::Type::Liquid);
            m_liquids.emplace_back(left_offset, y);
        }
        else
        {
            new_world.set(x, y, Material::Type::Liquid);
            m_liquids.emplace_back(x, y);
        }
    }

    *m_world = new_world;
}

} // namespace powda
