#ifndef POWDA_WORLD_HPP
#define POWDA_WORLD_HPP

#include <cstddef>
#include <vector>

#include <powda/simulation/material.hpp>

namespace powda
{

class World
{
  public:
    World(unsigned int width, unsigned int height);
    World(const World& other);
    World(World&& other);

    World& operator=(const World& other);
    World& operator=(World&& other);

    void            set(unsigned int x, unsigned int y, Material::Type mat);
    const Material& get(unsigned int x, unsigned int y) const;
    Material&       get(unsigned int x, unsigned int y);

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }
    unsigned int count() const { return m_material_count; }

    std::vector<Material>::const_iterator cbegin() const { return m_materials.cbegin(); }
    std::vector<Material>::const_iterator cend() const { return m_materials.cend(); }

    std::vector<Material>::iterator begin() { return m_materials.begin(); }
    std::vector<Material>::iterator end() { return m_materials.end(); }

  private:
    unsigned int          m_width;
    unsigned int          m_height;
    size_t                m_material_count;
    std::vector<Material> m_materials;

    size_t convert_to_flat_idx(unsigned int x, unsigned int y) const;
};

} // namespace powda

#endif // POWDA_WORLD_HPP
