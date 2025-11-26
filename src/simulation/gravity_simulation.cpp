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
            m_powders.emplace(x, y);
        }
        else if (mat.m_type == Material::Type::Liquid)
        {
            m_liquids.emplace(x, y);
        }
        ++i;
    }
}

void GravitySimulation::next()
{
    World new_world{*m_world};

    std::set<coord> cells_to_check;
    auto            update_surrounding = [this, &cells_to_check](unsigned int x, unsigned int y) {
        const auto     down_offset = std::max(static_cast<int>(y) - 1, 0);
        constexpr auto size        = 2;
        for (int dx = -size; dx != size; ++dx)
        {
            const auto hor_offset =
                std::clamp(static_cast<int>(x) + dx, 0, static_cast<int>(m_world->width() - 1));
            for (int dy = -size; dy != size; ++dy)
            {
                const auto ver_offset = std::clamp(
                    static_cast<int>(y) + dy, 0, static_cast<int>(m_world->height() - 1)
                );
                cells_to_check.emplace(hor_offset, ver_offset);
            }
        }
    };

    for (const auto [x, y] : m_powders)
    {
        auto&       current     = new_world.get(x, y);
        const auto  down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto& below       = m_world->get(x, down_offset);
        if (y == 0)
        {
            continue;
        }

        if (below.m_type == Material::Type::Empty || below.m_type == Material::Type::Liquid)
        {
            std::swap(current, new_world.get(x, down_offset));
            update_surrounding(x, down_offset);
            continue;
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto& left        = m_world->get(left_offset, y);
        const auto& right       = m_world->get(right_offset, y);
        const auto& below_left  = m_world->get(left_offset, down_offset);
        const auto& below_right = m_world->get(right_offset, down_offset);

        if (left.m_type == Material::Type::Empty && below_left.m_type == Material::Type::Empty)
        {
            std::swap(current, new_world.get(left_offset, down_offset));
            update_surrounding(left_offset, down_offset);
        }
        else if (right.m_type == Material::Type::Empty &&
                 below_right.m_type == Material::Type::Empty)
        {
            std::swap(current, new_world.get(right_offset, down_offset));
            update_surrounding(right_offset, down_offset);
        }
    }

    std::set<coord> new_liquids;
    for (const auto [x, y] : m_liquids)
    {
        auto&      current     = new_world.get(x, y);
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);

        if (y != 0)
        {
            const auto& below = m_world->get(x, down_offset);

            if (below.m_type == Material::Type::Empty)
            {
                current.m_current_inertia = {Material::Direction::Down};
                std::swap(current, new_world.get(x, down_offset));
                update_surrounding(x, down_offset);
                continue;
            }
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto& left        = m_world->get(left_offset, y);
        const auto& right       = m_world->get(right_offset, y);
        const auto& below_left  = m_world->get(left_offset, down_offset);
        const auto& below_right = m_world->get(right_offset, down_offset);

        if (right.m_type == Material::Type::Empty && left.m_type == Material::Type::Empty &&
            current.m_current_inertia)
        {
            if (*current.m_current_inertia == Material::Direction::Right)
            {
                std::swap(current, new_world.get(right_offset, y));
                update_surrounding(right_offset, y);
            }
            else if (*current.m_current_inertia == Material::Direction::Left)
            {
                std::swap(current, new_world.get(left_offset, y));
                update_surrounding(left_offset, y);
            }
        }
        else if (right.m_type == Material::Type::Empty)
        {
            current.m_current_inertia = {Material::Direction::Right};
            std::swap(current, new_world.get(right_offset, y));
            update_surrounding(right_offset, y);
        }
        else if (left.m_type == Material::Type::Empty)
        {
            current.m_current_inertia = {Material::Direction::Left};
            std::swap(current, new_world.get(left_offset, y));
            update_surrounding(left_offset, y);
        }
        else
        {
            current.m_current_inertia.reset();
            update_surrounding(x, y);
        }
    }

    *m_world = new_world;
    m_powders.clear();
    m_liquids.clear();
    for (const auto& coord : cells_to_check)
    {
        auto [x, y]          = coord;
        const auto& material = m_world->get(x, y);
        if (material.m_type == Material::Type::Powder)
            m_powders.emplace(coord);
        else if (material.m_type == Material::Type::Liquid)
            m_liquids.emplace(coord);
    }
}

} // namespace powda
