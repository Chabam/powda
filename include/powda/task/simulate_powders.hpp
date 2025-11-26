#ifndef POWDA_SIMULATE_POWDERS_HPP
#define POWDA_SIMULATE_POWDERS_HPP

#include <memory>

namespace powda
{

class GravitySimulation;

class SimulatePowders
{
  public:
    SimulatePowders(const std::shared_ptr<GravitySimulation>& gravity_simulation);

    void run();
  private:
    std::shared_ptr<GravitySimulation> m_gravity_simulation;
};

} // namespace powda

#endif // POWDA_SIMULATE_POWDERS_HPP
