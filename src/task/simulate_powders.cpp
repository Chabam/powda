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
    const auto middle = m_world->width() / 2;
    const auto size   = 20;

    m_world->set(middle - 40, m_world->height() - 1, Materials::Powder);
    m_world->set(middle, m_world->height() - 1, Materials::Powder);
    m_world->set(middle + 40, m_world->height() - 1, Materials::Powder);
}

} // namespace powda
