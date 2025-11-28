#include <unordered_map>
#include <utility>
#include <powda/simulation/material.hpp>

namespace powda
{

static const std::unordered_map<Material::Type, Material::Category> g_category_mapping{
    {Material::Type::Empty, Material::Category::Empty},
    {Material::Type::Sand, Material::Category::Powder},
    {Material::Type::Gravel, Material::Category::Powder},
    {Material::Type::Water, Material::Category::Liquid},
    {Material::Type::Oil, Material::Category::Liquid},
    {Material::Type::Smoke, Material::Category::Gas},
    {Material::Type::Metal, Material::Category::Wall}
};

const Material::Category Material::get_type_category(Material::Type mat)
{
    return g_category_mapping.at(mat);
}

void swap(Material& lhs, Material& rhs)
{
    std::swap(lhs.m_type, rhs.m_type);
    std::swap(lhs.m_inertia, rhs.m_inertia);
    std::swap(lhs.m_density, rhs.m_density);
}

} // namespace powda
