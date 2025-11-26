#include <powda/task/simulate_powders.hpp>
#include <powda/simulation/gravity_simulation.hpp>

namespace powda
{

SimulatePowders::SimulatePowders(const std::shared_ptr<GravitySimulation>& gravity_simulation)
    : m_gravity_simulation{gravity_simulation}
{
}

void SimulatePowders::run()
{
    m_gravity_simulation->next();
}

} // namespace powda
