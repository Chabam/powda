#include <utility>
#include <powda/simulation/material.hpp>

namespace powda
{

const char* Material::type_to_string(Material::Type mat)
{
    switch (mat)
    {
    case Material::Type::Powder:
        return "Powder";
    case Material::Type::Liquid:
        return "Liquid";
    case Material::Type::Gas:
        return "Gas";
    case Material::Type::Wall:
        return "Wall";
    default:
        return "Empty";
    }
}

void swap(Material& lhs, Material& rhs)
{
    std::swap(lhs.m_type, rhs.m_type);
    std::swap(lhs.m_current_inertia, rhs.m_current_inertia);
}

} // namespace powda
