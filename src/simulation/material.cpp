#include <powda/simulation/material.hpp>

namespace powda
{

const char* Material::type_to_string(Material::Type mat)
{
    switch (mat)
    {
    case Material::Type::Liquid:
        return "Liquid";
    case Material::Type::Powder:
        return "Powder";
    case Material::Type::Wall:
        return "Wall";
    default:
        return "Empty";
    }
}

} // namespace powda
