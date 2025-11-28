#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <optional>

namespace powda
{

struct Material
{
    enum class Category
    {
        Empty,
        Powder,
        Liquid,
        Gas,
        Wall
    };

    enum class Type
    {
        Empty,
        Sand,
        Gravel,
        Water,
        Oil,
        Smoke,
        Metal
    } m_type = Type::Empty;

    enum class Direction
    {
        Left,
        Right,
        Down,
        DownLeft,
        DownRight,
        Up,
        UpLeft,
        UpRight
    };
    std::optional<Direction> m_inertia = {};
    double m_density = 1.0;

    static const char*
                type_to_string(Material::Type mat);
    static const Category get_type_category(Material::Type mat);
    friend void swap(Material& lhs, Material& rhs);
};

} // namespace powda

#endif // MATERIAL_HPP
