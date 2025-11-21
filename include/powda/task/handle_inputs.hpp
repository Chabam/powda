#ifndef POWDA_HANDLE_INPUTS_HPP
#define POWDA_HANDLE_INPUTS_HPP

#include <powda/graphics/window.hpp>

namespace powda
{

class Scheduler;

class HandleInputs
{
  public:
    HandleInputs(
        const std::shared_ptr<Window>& window, const std::shared_ptr<Scheduler>& scheduler
    );

    void run();

  private:
    std::shared_ptr<Window>    m_window;
    std::shared_ptr<Scheduler> m_scheduler;
};

} // namespace powda
#endif // POWDA_HANDLE_INPUTS_HPP
