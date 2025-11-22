#include <powda/task/handle_inputs.hpp>
#include <powda/task/scheduler.hpp>
#include <powda/graphics/window.hpp>
#include <powda/simulation/world.hpp>
#include "powda/simulation/materials.hpp"

//clang-format off
#include <GLFW/glfw3.h>
// clang-format on
namespace powda
{

HandleInputs::HandleInputs(
    const std::shared_ptr<Window>&    window,
    const std::shared_ptr<Scheduler>& scheduler,
    const std::shared_ptr<World>&     world

)
    : m_window{window}
    , m_scheduler{scheduler}
    , m_world{world}
    , m_selected_material{Materials::Powder}
    , m_logger{"input handler"}
{
}

void HandleInputs::run()
{
    if (m_window->should_close())
    {
        m_scheduler->stop();
        return;
    }

    for (int pressed_key : m_window->keyboard_info().m_current_pressed_keys)
    {
        if (pressed_key == GLFW_KEY_ESCAPE || pressed_key == GLFW_KEY_Q)
        {
            m_window->close();
            m_scheduler->stop();
        }

        if (pressed_key == GLFW_KEY_1)
        {
            m_selected_material = Materials::Powder;
            m_logger.debug("Powder selected");
        }

        if (pressed_key == GLFW_KEY_2)
        {
            m_selected_material = Materials::Wall;
            m_logger.debug("Wall selected");
        }

        if (pressed_key == GLFW_KEY_3)
        {
            m_selected_material = Materials::Liquid;
            m_logger.debug("Liquid selected");
        }
    }

    auto mouse_info = m_window->mouse_info();
    auto left_click = std::find(
        mouse_info.m_current_pressed_buttons.begin(),
        mouse_info.m_current_pressed_buttons.end(),
        GLFW_MOUSE_BUTTON_LEFT
    );
    auto right_click = std::find(
        mouse_info.m_current_pressed_buttons.begin(),
        mouse_info.m_current_pressed_buttons.end(),
        GLFW_MOUSE_BUTTON_RIGHT
    );

    unsigned int x_idx = (mouse_info.x_pos / m_window->width()) * m_world->width();
    unsigned int y_idx = ((m_window->height() - mouse_info.y_pos) / m_window->height()) * m_world->height();
    if (left_click != mouse_info.m_current_pressed_buttons.end())
    {
        m_world->set(x_idx, y_idx, m_selected_material);
    }
    else if (right_click != mouse_info.m_current_pressed_buttons.end())
    {
        m_world->set(x_idx, y_idx, Materials::Empty);
    }
}

} // namespace powda
