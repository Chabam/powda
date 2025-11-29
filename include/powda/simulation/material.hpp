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

    Material(Type type);

    void reset_inertia() { m_inertia.reset(); }
    void set_inertia(Direction dir) { m_inertia.emplace(dir); }
    const std::optional<Direction>& inertia() const { return m_inertia; }

    Type               type() const { return m_type; }
    Category           category() const { return m_category; }
    double             density() const { return m_density; }

    friend void        swap(Material& lhs, Material& rhs);
    static const char* type_to_string(Material::Type mat);

  private:
    Type                     m_type;
    Category                 m_category;
    std::optional<Direction> m_inertia;
    double                   m_density;
};

} // namespace powda

#endif // MATERIAL_HPP
