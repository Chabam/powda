#ifndef MATERIAL_HPP
#define MATERIAL_HPP

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

    static const char* type_to_string(Material::Type mat);
    friend void swap(Material& lhs, Material& rhs);
};

} // namespace powda

#endif // MATERIAL_HPP
