#ifndef POWDA_SIMULATE_POWDERS_HPP
#define POWDA_SIMULATE_POWDERS_HPP

#include <powda/simulation/world.hpp>

namespace powda
{

class SimulatePowders
{
  public:
    SimulatePowders(const WorldPtr& world);

    void run();

  private:
    WorldPtr m_world;
};

} // namespace powda

#endif // POWDA_SIMULATE_POWDERS_HPP
