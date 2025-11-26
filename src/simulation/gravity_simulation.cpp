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
    World new_world{*m_world};
    auto  current_powders{m_powders};
    m_powders.clear();

    auto current_liquids{m_liquids};
    m_liquids.clear();

    for (const auto [x, y] : current_powders)
    {
        const auto current     = m_world->get(x, y);
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = m_world->get(x, down_offset);
        if (y == 0)
        {
            continue;
        }

        if (below.m_type == Material::Type::Empty || below.m_type == Material::Type::Liquid)
        {
            std::swap(new_world.get(x, y), new_world.get(x, down_offset));
            m_powders.emplace_back(x, down_offset);
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
            std::swap(new_world.get(x, y), new_world.get(left_offset, down_offset));
            m_powders.emplace_back(left_offset, down_offset);
        }
        else if (right.m_type == Material::Type::Empty && below_right.m_type == Material::Type::Empty)
        {
            std::swap(new_world.get(x, y), new_world.get(right_offset, down_offset));
            m_powders.emplace_back(right_offset, down_offset);
        }
    }

    for (const auto [x, y] : current_liquids)
    {
        const auto current     = m_world->get(x, y);
        const auto down_offset = std::max(static_cast<int>(y) - 1, 0);
        const auto below       = m_world->get(x, down_offset);
        if (y == 0)
        {
            continue;
        }

        if (below.m_type == Material::Type::Empty)
        {
            std::swap(new_world.get(x, y), new_world.get(x, down_offset));
            m_liquids.emplace_back(x, down_offset);
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
            std::swap(new_world.get(x, y), new_world.get(left_offset, down_offset));
            m_liquids.emplace_back(left_offset, down_offset);
        }
        else if (right.m_type == Material::Type::Empty && below_right.m_type == Material::Type::Empty)
        {
            std::swap(new_world.get(x, y), new_world.get(right_offset, down_offset));
            m_liquids.emplace_back(right_offset, down_offset);
        }
        else if (left.m_type == Material::Type::Empty)
        {
            std::swap(new_world.get(x, y), new_world.get(left_offset, y));
            m_liquids.emplace_back(left_offset, y);
        }
        else if (right.m_type == Material::Type::Empty)
        {
            std::swap(new_world.get(x, y), new_world.get(right_offset, y));
            m_liquids.emplace_back(right_offset, y);
        }
    }

    *m_world = new_world;
}

} // namespace powda
