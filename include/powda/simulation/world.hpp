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
    const std::vector<coord>& settled_powders() const { return m_settled_powders; }
    const std::vector<coord>& walls() const { return m_walls; }

  private:
    using Walls = std::vector<coord>;
    using Powders = std::vector<coord>;
    unsigned int             m_width;
    unsigned int             m_height;
    size_t                   m_material_count;
    Powders                  m_powders;
    Powders                  m_settled_powders;
    Walls                    m_walls;

    Materials get(unsigned int x, unsigned int y, const Powders& powders, const Powders& settled_powders, const Walls& walls) const;
};

using WorldPtr = std::shared_ptr<World>;

} // namespace powda

#endif // POWDA_WORLD_HPP
