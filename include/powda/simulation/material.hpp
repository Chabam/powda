#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <optional>

namespace powda
{

class Material
{
  public:
    enum class Category
    {
        Powder,
        Liquid,
        Gas,
        Wall
    };

    enum class Type
    {
        Sand,
        Gravel,
        Water,
        Oil,
        Smoke,
        Metal
    };

    Material(Type type);

    Type     type() const { return m_type; }
    Category category() const { return m_category; }
    double   density() const { return m_density; }
    unsigned spread() const { return m_spread; }

    friend void        swap(Material& lhs, Material& rhs);
    static const char* type_to_string(Material::Type mat);

  private:
    Type         m_type;
    Category     m_category;
    double       m_density;
    unsigned int m_spread;
};

} // namespace powda

#endif // MATERIAL_HPP
