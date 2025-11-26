#ifndef POWDA_GRAVITY_SIMULATION_HPP
#define POWDA_GRAVITY_SIMULATION_HPP

#include <memory>
#include <vector>

namespace powda
{

class World;

class GravitySimulation
{
  public:
    GravitySimulation(const std::shared_ptr<World>& world);

    void next();

    void add_liquid(unsigned int x, unsigned int y) { m_liquids.emplace_back(x, y); }
    void add_powder(unsigned int x, unsigned int y) { m_powders.emplace_back(x, y); }
  private:
    using coord = std::pair<unsigned int, unsigned int>;
    std::shared_ptr<World> m_world;
    std::vector<coord>     m_powders;
    std::vector<coord>     m_liquids;
};

} // namespace powda

#endif // POWDA_GRAVITY_SIMULATION_HPP
