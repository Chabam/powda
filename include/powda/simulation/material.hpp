#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <optional>

namespace powda
{

struct Material
{
    enum class Type
    {
        Empty,
        Powder,
        Liquid,
        Wall
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
    std::optional<Direction> m_current_inertia = {};

    static const char*
                type_to_string(Material::Type mat);
    friend void swap(Material& lhs, Material& rhs);
};

} // namespace powda

#endif // MATERIAL_HPP
