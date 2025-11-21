#include <powda/task/handle_inputs.hpp>
#include <powda/task/scheduler.hpp>

//clang-format off
#include <GLFW/glfw3.h>
// clang-format on
namespace powda
{

HandleInputs::HandleInputs(
    const std::shared_ptr<Window>& window, const std::shared_ptr<Scheduler>& scheduler
)
    : m_window{window}
    , m_scheduler{scheduler}
{
}

void HandleInputs::run()
{
    if (m_window->should_close())
    {
        m_scheduler->stop();
        return;
    }

    for (const int pressed_key : m_window->get_keyboard_info().m_current_pressed_keys)
    {
        if (pressed_key == GLFW_KEY_ESCAPE || pressed_key == GLFW_KEY_Q)
        {
            m_window->close();
            m_scheduler->stop();
        }
    }
}

} // namespace powda
