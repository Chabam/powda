#ifndef POWDA_GRAVITY_SIMULATION_HPP
#define POWDA_GRAVITY_SIMULATION_HPP

#include <memory>
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

    void add_powder(unsigned int x, unsigned int y) { m_powders.emplace(x, y); }
    void add_liquid(unsigned int x, unsigned int y) { m_liquids.emplace(x, y); }
    void add_gas(unsigned int x, unsigned int y) { m_gasses.emplace(x, y); }

  private:
    using coord = std::pair<unsigned int, unsigned int>;
    std::shared_ptr<World> m_world;
    std::set<coord>        m_powders;
    std::set<coord>        m_liquids;
    std::set<coord>        m_gasses;
};

} // namespace powda

#endif // POWDA_GRAVITY_SIMULATION_HPP
