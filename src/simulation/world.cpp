#include <cassert>

#include <powda/logger.hpp>
#include <powda/simulation/material.hpp>
#include <powda/simulation/world.hpp>

namespace powda
{

World::World(unsigned int width, unsigned int height)
    : m_width{width}
    , m_height{height}
    , m_material_count{m_width * m_height}
    , m_materials{m_material_count, std::allocator<Material>{}}
{
}

World::World(const World& other)
    : m_width{other.m_width}
    , m_height{other.m_height}
    , m_material_count{other.m_material_count}
    , m_materials{other.m_materials}
{
}

World::World(World&& other)
    : m_width{std::move(other.m_width)}
    , m_height{std::move(other.m_height)}
    , m_material_count{std::move(other.m_material_count)}
    , m_materials{std::move(other.m_materials)}
{
}

World& World::operator=(const World& other)
{

    m_width          = other.m_width;
    m_height         = other.m_height;
    m_material_count = other.m_material_count;
    m_materials      = other.m_materials;

    return *this;
}

World& World::operator=(World&& other)
{

    m_width          = std::move(other.m_width);
    m_height         = std::move(other.m_height);
    m_material_count = std::move(other.m_material_count);
    m_materials      = std::move(other.m_materials);

    return *this;
}

void World::reset()
{
    m_materials =
        material_container{m_material_count, std::allocator<Material>{}};
}

void World::set(unsigned int x, unsigned int y, Material::Type mat)
{
    m_materials[convert_to_flat_idx(x, y)] = Material{mat};
}

void World::reset(unsigned int x, unsigned int y)
{
    m_materials[convert_to_flat_idx(x, y)].reset();
}

std::optional<Material>& World::get(unsigned int x, unsigned int y)
{
    return m_materials[convert_to_flat_idx(x, y)];
}

const std::optional<Material>& World::get(unsigned int x, unsigned int y) const
{
    return const_cast<std::optional<Material>&>(get(x, y));
}

size_t World::convert_to_flat_idx(unsigned int x, unsigned int y) const
{
    return x + (y * m_width);
}

} // namespace powda
