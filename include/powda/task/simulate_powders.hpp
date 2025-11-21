#ifndef POWDA_SIMULATE_POWDERS_HPP
#define POWDA_SIMULATE_POWDERS_HPP

#include <memory>

namespace powda
{

class World;

class SimulatePowders
{
  public:
    SimulatePowders(const std::shared_ptr<World>& world);

    void run();

  private:
    std::shared_ptr<World> m_world;
};

} // namespace powda

#endif // POWDA_SIMULATE_POWDERS_HPP
