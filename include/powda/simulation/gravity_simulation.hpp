#ifndef POWDA_GRAVITY_SIMULATION_HPP
#define POWDA_GRAVITY_SIMULATION_HPP

#include <memory>
#include <random>
#include <set>

namespace powda
{

class World;

class GravitySimulation
{
  public:
    GravitySimulation(const std::shared_ptr<World>& world);

    void next();
    void reset();

    void remove(unsigned int x, unsigned int y);

    void add_powder(unsigned int x, unsigned int y) { m_powders.emplace(x, y); }
    void add_liquid(unsigned int x, unsigned int y) { m_liquids.emplace(x, y); }
    void add_gas(unsigned int x, unsigned int y) { m_gasses.emplace(x, y); }

  private:
    using coord = std::pair<unsigned int, unsigned int>;

    void update_powders(World& new_world);
    void update_liquids(World& new_world);
    void update_gasses(World& new_world);
    static void update_new_grid(
        World&                              new_world,
        unsigned int                        a_x,
        unsigned int                        a_y,
        unsigned int                        b_x,
        unsigned int                        b_y,
        std::set<GravitySimulation::coord>& materials
    );

    std::shared_ptr<World>     m_world;
    std::set<coord>            m_powders;
    std::set<coord>            m_liquids;
    std::set<coord>            m_gasses;
    std::random_device         m_random_device;
};

} // namespace powda

#endif // POWDA_GRAVITY_SIMULATION_HPP
