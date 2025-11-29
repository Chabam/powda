#ifndef POWDA_WORLD_HPP
#define POWDA_WORLD_HPP

#include <cstddef>
#include <memory>
#include <vector>

#include <powda/simulation/material.hpp>

namespace powda
{

class World
{
  public:
    using material_container = std::vector<std::optional<Material>>;
    using iterator           = material_container::iterator;
    using const_iterator     = material_container::const_iterator;

    World(unsigned int width, unsigned int height);
    World(const World& other);
    World(World&& other);

    World& operator=(const World& other);
    World& operator=(World&& other);

    void                           set(unsigned int x, unsigned int y, Material::Type mat);
    void                           reset(unsigned int x, unsigned int y);
    void                           reset();
    const std::optional<Material>& get(unsigned int x, unsigned int y) const;
    std::optional<Material>&       get(unsigned int x, unsigned int y);

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }
    unsigned int count() const { return m_material_count; }

    const_iterator cbegin() const { return m_materials.cbegin(); }

    const_iterator cend() const { return m_materials.cend(); }

    iterator begin() { return m_materials.begin(); }
    iterator end() { return m_materials.end(); }

  private:
    unsigned int       m_width;
    unsigned int       m_height;
    size_t             m_material_count;
    material_container m_materials;

    size_t convert_to_flat_idx(unsigned int x, unsigned int y) const;
};

} // namespace powda

#endif // POWDA_WORLD_HPP
