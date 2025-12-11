#include <GLFW/glfw3.h>
#include <algorithm>
#include <functional>
#include <iostream>
#include <format>
#include <random>

#include <powda/simulation/gravity_simulation.hpp>
#include <powda/simulation/material.hpp>
#include <powda/simulation/world.hpp>
#include <powda/task/simulate_powders.hpp>

namespace powda
{

GravitySimulation::GravitySimulation(const std::shared_ptr<World>& world)
    : m_world{world}
    , m_powders{}
    , m_liquids{}
    , m_gasses{}
    , m_random_device{}
{
    size_t i = 0;
    for (const auto& mat : *m_world)
    {
        if (!mat)
            continue;
        unsigned int x   = i % m_world->width();
        unsigned int y   = i / m_world->width();
        const auto   cat = mat->category();
        if (cat == Material::Category::Powder)
        {
            m_powders.emplace(x, y);
        }
        else if (cat == Material::Category::Liquid)
        {
            m_liquids.emplace(x, y);
        }
        else if (cat == Material::Category::Gas)
        {
            m_gasses.emplace(x, y);
        }
        ++i;
    }
}

void GravitySimulation::reset()
{
    m_powders.clear();
    m_liquids.clear();
    m_gasses.clear();
}

void GravitySimulation::remove(unsigned int x, unsigned int y)
{
    coord c{x, y};
    if (m_powders.erase(c) != 0)
        return;
    if (m_liquids.erase(c) != 0)
        return;
    if (m_gasses.erase(c) != 0)
        return;
}

void GravitySimulation::next()
{
    World new_world{*m_world};

    auto update_new_grid =
        [this, &new_world](
            unsigned int a_x, unsigned int a_y, unsigned int b_x, unsigned int b_y, auto& materials
        ) {
            if (!materials.contains({b_x, b_y}))
            {
                std::swap(new_world.get(a_x, a_y), new_world.get(b_x, b_y));
                materials.emplace(b_x, b_y);
            }
            else
            {
                // A material was already placed here! Leaving in
                // place and will update on the next step.
                materials.emplace(a_x, a_y);
            }
        };
    using namespace std::placeholders;

    const auto current_powders{m_powders};
    m_powders.clear();
    for (const auto [x, y] : current_powders)
    {
        auto& current = new_world.get(x, y);

        if (!current)
            continue;

        const auto  down_offset    = std::max(static_cast<int>(y) - 1, 0);
        const auto& below          = m_world->get(x, down_offset);
        auto        update_powders = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_powders));

        if (y == 0)
        {
            continue;
        }

        if (!below || below->density() < current->density())
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

        if (!left && !below_left)
        {
            update_powders(left_offset, down_offset);
        }
        else if (!right && !below_right)
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
        const auto& current = m_world->get(x, y);

        if (!current)
            continue;

        const auto down_offset    = std::max(static_cast<int>(y) - 1, 0);
        auto       update_liquids = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_liquids));
        if (y != 0)
        {
            const auto& below = m_world->get(x, down_offset);

            if (!below || below->density() < current->density())
            {
                update_liquids(x, down_offset);
                continue;
            }
        }

        auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        auto right_offset = std::min(x + 1, m_world->width() - 1);

        for (unsigned char i = 0; i < current->spread(); ++i)
        {
            if (m_world->get(left_offset, y))
                break;

            left_offset = std::max(static_cast<int>(--left_offset) - 1, 0);
        }

        for (unsigned char i = 0; i < current->spread(); ++i)
        {
            if (m_world->get(right_offset, y))
                break;

            right_offset = std::min(++right_offset, m_world->width() - 1);
        }

        const auto& left        = m_world->get(left_offset, y);
        const auto& right       = m_world->get(right_offset, y);
        const auto& below_left  = m_world->get(left_offset, down_offset);
        const auto& below_right = m_world->get(right_offset, down_offset);

        if (!right && !left)
        {
            const auto rand_dir =
                std::uniform_int_distribution<unsigned char>(0, 1)(m_random_device);
            if (rand_dir == 0)
            {
                update_liquids(left_offset, y);
            }
            else
            {
                update_liquids(right_offset, y);
            }
        }
        else if (!right)
        {
            update_liquids(right_offset, y);
        }
        else if (!left)
        {
            update_liquids(left_offset, y);
        }
        else
        {
            m_liquids.emplace(x, y);
        }
    }

    const auto current_gasses{m_gasses};
    m_gasses.clear();
    for (const auto [x, y] : current_gasses)
    {
        const auto& current = m_world->get(x, y);

        if (!current)
            continue;

        const auto up_offset     = std::min(y + 1, m_world->height() - 1);
        auto       update_gasses = std::bind(update_new_grid, x, y, _1, _2, std::ref(m_gasses));

        if (y != m_world->height() - 1)
        {
            const auto& above = m_world->get(x, up_offset);

            if (!above || above->density() > current->density())
            {
                update_gasses(x, up_offset);
                continue;
            }
        }

        const auto left_offset  = std::max(static_cast<int>(x) - 1, 0);
        const auto right_offset = std::min(x + 1, m_world->width() - 1);

        const auto& left        = m_world->get(left_offset, y);
        const auto& right       = m_world->get(right_offset, y);
        const auto& above_left  = m_world->get(left_offset, up_offset);
        const auto& above_right = m_world->get(right_offset, up_offset);

        if (!right && !left)
        {
            const auto rand_dir =
                std::uniform_int_distribution<unsigned char>(0, 1)(m_random_device);
            if (rand_dir == 0)
            {
                update_gasses(left_offset, y);
            }
            else
            {
                update_gasses(right_offset, y);
            }
        }
        else if (!left && !above_left)
        {
            update_gasses(left_offset, up_offset);
        }
        else if (!right && !above_right)
        {
            update_gasses(right_offset, up_offset);
        }
        else if (!right)
        {
            update_gasses(right_offset, y);
        }
        else if (!left)
        {
            update_gasses(left_offset, y);
        }
        else
        {
            m_liquids.emplace(x, y);
        }
    }

    *m_world = std::move(new_world);
}

} // namespace powda
