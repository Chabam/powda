#include <powda/simulation/gravity_simulation.hpp>

#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>

#include <powda/simulation/world.hpp>
#include <powda/task/simulate_powders.hpp>
#include <powda/simulation/material.hpp>

namespace powda
{

GravitySimulation::GravitySimulation(const std::shared_ptr<World>& world)
    : m_world{world}
    , m_powders{}
    , m_liquids{}
    , m_gasses{}
{
    size_t i = 0;
    for (const auto& mat : *m_world)
    {
        unsigned int x = i % m_world->width();
        unsigned int y = i / m_world->width();
        const auto cat = Material::get_type_category(mat.m_type);
        if (cat == Material::Category::Powder)
        {
            m_powders.emplace(x, y);
        }
        else if (cat == Material::Category::Liquid)
        {
            m_liquids.emplace(x, y);
        }
        ++i;
    }
}

void GravitySimulation::next()
{
    World new_world{*m_world};

    auto update_new_grid = [this, &new_world](
                               unsigned int a_x,
                               unsigned int a_y,
                               unsigned int b_x,
                               unsigned int b_y,
                               auto&        materials_to_track
                           ) {
        std::swap(new_world.get(a_x, a_y), new_world.get(b_x, b_y));
        materials_to_track.emplace(b_x, b_y);
    };
    using namespace std::placeholders;

    const auto current_powders{m_powders};
    m_powders.clear();
    for (const auto [x, y] : current_powders)
    {
        auto&       current        = new_world.get(x, y);
        const auto  down_offset    = std::max(static_cast<int>(y) - 1, 0);
        const auto& below          = m_world->get(x, down_offset);
        auto        update_powders = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_powders));

        if (y == 0)
        {
            m_powders.emplace(x, y);
            continue;
        }

        if (below.m_type == Material::Type::Empty)
        {
            update_powders(x, down_offset);
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
            update_powders(left_offset, down_offset);
        }
        else if (right.m_type == Material::Type::Empty &&
                 below_right.m_type == Material::Type::Empty)
        {
            update_powders(right_offset, down_offset);
        }
        else
        {
            m_powders.emplace(x, y);
        }
    }

    const auto current_liquids{m_liquids};
    m_liquids.clear();
    for (const auto [x, y] : current_liquids)
    {
        const auto  before_size    = m_liquids.size();
        const auto& current        = m_world->get(x, y);
        const auto  down_offset    = std::max(static_cast<int>(y) - 1, 0);
        auto        update_liquids = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_liquids));

        if (y != 0)
        {
            const auto& below = m_world->get(x, down_offset);

            if (below.m_type == Material::Type::Empty)
            {
                new_world.get(x, y).m_inertia.reset();
                update_liquids(x, down_offset);
                continue;
            }
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto& left  = m_world->get(left_offset, y);
        const auto& right = m_world->get(right_offset, y);

        if (right.m_type == Material::Type::Empty &&
            (!current.m_inertia || current.m_inertia == Material::Direction::Right))
        {
            new_world.get(x, y).m_inertia = {Material::Direction::Right};
            update_liquids(right_offset, y);
        }
        else if (left.m_type == Material::Type::Empty &&
                 (!current.m_inertia || current.m_inertia == Material::Direction::Left))
        {
            new_world.get(x, y).m_inertia = {Material::Direction::Left};
            update_liquids(left_offset, y);
        }
        else
        {
            new_world.get(x, y).m_inertia.reset();
            m_liquids.emplace(x, y);
        }
    }

    const auto current_gasses{m_gasses};
    m_gasses.clear();
    for (const auto [x, y] : current_gasses)
    {
        const auto& current       = m_world->get(x, y);
        const auto up_offset     = std::min(y + 1, m_world->height() - 1);
        auto       update_gasses = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_gasses));

        if (y != m_world->height() - 1)
        {
            const auto& above = m_world->get(x, up_offset);

            if (above.m_type == Material::Type::Empty)
            {
                update_gasses(x, up_offset);
                new_world.get(x, up_offset).m_inertia = {Material::Direction::Right};
                continue;
            }
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto& left  = m_world->get(left_offset, y);
        const auto& right = m_world->get(right_offset, y);

        if (right.m_type == Material::Type::Empty &&
            (!current.m_inertia || current.m_inertia == Material::Direction::Right))
        {
            new_world.get(x, y).m_inertia = {Material::Direction::Right};
            update_gasses(right_offset, y);
        }
        else if (left.m_type == Material::Type::Empty &&
                 (!current.m_inertia || current.m_inertia == Material::Direction::Left))
        {
            new_world.get(x, y).m_inertia = {Material::Direction::Left};
            update_gasses(left_offset, y);
        }
        else
        {
            new_world.get(x, y).m_inertia.reset();
            m_gasses.emplace(x, y);
        }
    }

    *m_world = new_world;
}

} // namespace powda
