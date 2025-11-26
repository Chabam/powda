#ifndef POWDA_HANDLE_INPUTS_HPP
#define POWDA_HANDLE_INPUTS_HPP

#include <memory>

#include <powda/simulation/material.hpp>
#include <powda/logger.hpp>

namespace powda
{

class Scheduler;
class Window;
class World;

class HandleInputs
{
  public:
    HandleInputs(
        const std::shared_ptr<Window>&    window,
        const std::shared_ptr<Scheduler>& scheduler,
        const std::shared_ptr<World>&     world
    );

    void run();

  private:
    std::shared_ptr<Window>    m_window;
    std::shared_ptr<Scheduler> m_scheduler;
    std::shared_ptr<World>     m_world;
    Material::Type             m_selected_material;
    Logger                     m_logger;
};

} // namespace powda
#endif // POWDA_HANDLE_INPUTS_HPP
