#include <unordered_map>
#include <utility>
#include <powda/simulation/material.hpp>

namespace powda
{

static const std::unordered_map<Material::Type, Material::Category> g_category_mapping{
    {Material::Type::Sand, Material::Category::Powder},
    {Material::Type::Gravel, Material::Category::Powder},
    {Material::Type::Water, Material::Category::Liquid},
    {Material::Type::Oil, Material::Category::Liquid},
    {Material::Type::Smoke, Material::Category::Gas},
    {Material::Type::Metal, Material::Category::Wall}
};

static const std::unordered_map<Material::Type, double> g_density_mapping{
    {Material::Type::Sand, 1.0},
    {Material::Type::Gravel, 1.5},
    {Material::Type::Water, 0.5},
    {Material::Type::Oil, 0.4},
    {Material::Type::Smoke, 0.3},
    {Material::Type::Metal, 10.0}
};

static const std::unordered_map<Material::Type, unsigned int> g_spread_mapping{
    {Material::Type::Sand, 0},
    {Material::Type::Gravel, 0},
    {Material::Type::Water, 1},
    {Material::Type::Oil, 2},
    {Material::Type::Smoke, 2},
    {Material::Type::Metal, 0}
};

static const std::unordered_map<Material::Type, const char*> g_string_mapping{
    {Material::Type::Sand, "Sand"},
    {Material::Type::Gravel, "Gravel"},
    {Material::Type::Water, "Water"},
    {Material::Type::Oil, "Oil"},
    {Material::Type::Smoke, "Smoke"},
    {Material::Type::Metal, "Metal"}
};

Material::Material(Type type)
    : m_type{type}
    , m_category{g_category_mapping.at(m_type)}
    , m_density{g_density_mapping.at(m_type)}
    , m_spread{g_spread_mapping.at(m_type)}
{
}

void swap(Material& lhs, Material& rhs)
{
    std::swap(lhs.m_type, rhs.m_type);
    std::swap(lhs.m_category, rhs.m_category);
    std::swap(lhs.m_density, rhs.m_density);
    std::swap(lhs.m_spread, rhs.m_spread);
}

const char* Material::type_to_string(Material::Type type)
{
    return g_string_mapping.at(type);
}

} // namespace powda
