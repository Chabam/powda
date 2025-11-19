#include <powda/world.hpp>
#include "powda/materials.hpp"

namespace powda
{

World::World(unsigned int width, unsigned int height)
    : m_materials{}
    , m_width{width}
    , m_height{height}
    , m_material_count{m_width * m_height}
{
    m_materials.resize(m_material_count);
    std::fill(m_materials.begin(), m_materials.end(), Materials::Empty);
}

void World::next_step()
{
    const container snapshot{m_materials};
    std::fill(m_materials.begin(), m_materials.end(), Materials::Empty);

    for (unsigned int y = 0; y < m_height; ++y)
    {
        for (unsigned int x = 0; x < m_width; ++x)
        {
            const auto current = get(x, y, snapshot);
            if (current == Materials::Powder)
            {
                const auto below = get(x, y - 1, snapshot);
                if (below == Materials::Empty || get(x, y - 1) == Materials::Empty)
                {
                    if (get(x, y - 1) == Materials::Powder)
                        set(x, y, Materials::Powder);
                    else
                        set(x, y - 1, Materials::Powder);
                    continue;
                }

                const auto left        = get(x - 1, y, snapshot);
                const auto right       = get(x + 1, y, snapshot);
                const auto below_left  = get(x - 1, y - 1, snapshot);
                const auto below_right = get(x + 1, y - 1, snapshot);

                if (right == Materials::Empty && below_right == Materials::Empty)
                {
                    // if (get(x + 1, y) == Materials::Powder)
                    //     set(x, y, Materials::Powder);
                    // else
                    set(x + 1, y - 1, Materials::Powder);
                }
                else if (left == Materials::Empty && below_left == Materials::Empty)
                {
                    // if (get(x - 1, y) == Materials::Powder)
                    //     set(x, y, Materials::Powder);
                    // else
                        set(x - 1, y - 1, Materials::Powder);
                }
                else if (below == Materials::Wall || below == Materials::Powder)
                {
                    set(x, y, Materials::Powder);
                }
            }
            else if (current == Materials::Wall)
            {
                set(x, y, Materials::Wall);
            }
        }
    }
}

size_t World::xy_to_flat_idx(unsigned int x, unsigned int y) const
{
    return (x % m_width) + ((y % m_height) * m_width);
}

void World::set(unsigned int x, unsigned int y, Materials mat)
{
    m_materials[xy_to_flat_idx(x, y)] = mat;
}

Materials World::get(unsigned int x, unsigned int y) const
{
    return get(x, y, m_materials);
}

Materials World::get(unsigned long idx) const
{
    return get(idx, m_materials);
}

Materials World::get(unsigned long idx, const container& materials) const
{
    return materials[idx];
}

Materials World::get(unsigned int x, unsigned int y, const container& materials) const
{
    return get(xy_to_flat_idx(x, y), materials);
}

} // namespace powda
