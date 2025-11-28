#include <powda/graphics/window.hpp>
#include <powda/simulation/gravity_simulation.hpp>
#include <powda/simulation/material.hpp>
#include <powda/simulation/world.hpp>
#include <powda/task/handle_inputs.hpp>
#include <powda/task/scheduler.hpp>

//clang-format off
#include <GLFW/glfw3.h>
// clang-format on
namespace powda
{

HandleInputs::HandleInputs(
    const std::shared_ptr<Window>&            window,
    const std::shared_ptr<Scheduler>&         scheduler,
    const std::shared_ptr<World>&             world,
    const std::shared_ptr<GravitySimulation>& gravity_simulation
)
    : m_window{window}
    , m_scheduler{scheduler}
    , m_world{world}
    , m_gravity_simulation{gravity_simulation}
    , m_selected_material{Material::Type::Sand}
    , m_last_x_idx{static_cast<unsigned int>(-1)}
    , m_last_y_idx{static_cast<unsigned int>(-1)}
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
            m_selected_material = Material::Type::Sand;
            m_logger.debug("Sand selected");
        }

        if (pressed_key == GLFW_KEY_2)
        {
            m_selected_material = Material::Type::Metal;
            m_logger.debug("Metal selected");
        }

        if (pressed_key == GLFW_KEY_3)
        {
            m_selected_material = Material::Type::Water;
            m_logger.debug("Water selected");
        }

        if (pressed_key == GLFW_KEY_4)
        {
            m_selected_material = Material::Type::Smoke;
            m_logger.debug("Smoke eselected");
        }

        if (pressed_key == GLFW_KEY_5)
        {
            m_selected_material = Material::Type::Gravel;
            m_logger.debug("Gravel eselected");
        }

        if (pressed_key == GLFW_KEY_6)
        {
            m_selected_material = Material::Type::Oil;
            m_logger.debug("Oil eselected");
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
    unsigned int y_idx =
        ((m_window->height() - mouse_info.y_pos) / m_window->height()) * m_world->height();

    if (x_idx == m_last_x_idx && y_idx == m_last_y_idx)
        return;

    m_last_x_idx = x_idx;
    m_last_y_idx = y_idx;

    if (left_click != mouse_info.m_current_pressed_buttons.end())
    {
        const auto cat = Material::get_type_category(m_selected_material);
        if (cat == Material::Category::Powder)
            m_gravity_simulation->add_powder(x_idx, y_idx);
        else if (cat == Material::Category::Liquid)
            m_gravity_simulation->add_liquid(x_idx, y_idx);
        else if (cat == Material::Category::Gas)
            m_gravity_simulation->add_gas(x_idx, y_idx);


        m_world->set(x_idx, y_idx, m_selected_material);
    }
    else if (right_click != mouse_info.m_current_pressed_buttons.end())
    {
        m_world->set(x_idx, y_idx, Material::Type::Empty);
    }
}

} // namespace powda
