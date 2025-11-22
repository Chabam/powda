#ifndef POWDA_WORLD_HPP
#define POWDA_WORLD_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include <powda/simulation/materials.hpp>

namespace powda
{

class World
{
  public:
    using coord = std::pair<unsigned int, unsigned int>;
    World(unsigned int width, unsigned int height);

    void next_step();

    void      set(unsigned int x, unsigned int y, Materials mat);
    Materials get(unsigned int x, unsigned int y) const;

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }
    unsigned int count() const { return m_material_count; }

    const std::vector<coord>& powders() const { return m_powders; }
    const std::vector<coord>& walls() const { return m_walls; }
    const std::vector<coord>& liquids() const { return m_liquids; }

  private:
    unsigned int       m_width;
    unsigned int       m_height;
    size_t             m_material_count;
    std::vector<coord> m_powders;
    std::vector<coord> m_liquids;
    std::vector<coord> m_walls;

    Materials get(
        unsigned int              x,
        unsigned int              y,
        const std::vector<coord>& powders,
        const std::vector<coord>& liquids,
        const std::vector<coord>& walls
    ) const;
};

} // namespace powda

#endif // POWDA_WORLD_HPP
