#ifndef POWDA_WORLD_H
#define POWDA_WORLD_H

#include <cstddef>
#include <memory>
#include <vector>

#include <powda/materials.hpp>

namespace powda
{

class World
{
  public:
    using container = std::vector<Materials>;
    using iterator = container::iterator;
    using const_iterator = container::const_iterator;

    World(unsigned int width, unsigned int height);

    void next_step();

    void         set(unsigned int x, unsigned int y, Materials mat);
    Materials    get(unsigned int x, unsigned int y) const;
    Materials    get(unsigned long idx) const;

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }
    unsigned int count() const { return m_material_count; }
    const_iterator cbegin() const { return m_materials.cbegin(); }
    const_iterator cend() const { return m_materials.cend(); }

  private:
    unsigned int           m_width;
    unsigned int           m_height;
    size_t                 m_material_count;
    container              m_materials;

    size_t xy_to_flat_idx(unsigned int x, unsigned int y) const;
    Materials    get(unsigned int x, unsigned int y, const container& grid) const;
    Materials    get(unsigned long idx, const container& grid) const;

};

using WorldPtr = std::shared_ptr<World>;

} // namespace powda

#endif // POWDA_WORLD_H
