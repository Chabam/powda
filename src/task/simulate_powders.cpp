#include <powda/task/simulate_powders.hpp>
#include <powda/simulation/world.hpp>

namespace powda
{

SimulatePowders::SimulatePowders(const std::shared_ptr<World>& world)
    : m_world{world}
{
}

void SimulatePowders::run()
{
    m_world->next_step();
}

} // namespace powda
