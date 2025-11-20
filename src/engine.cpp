#include <cassert>
#include <chrono>
#include <format>
#include <memory>
#include <stdexcept>

#include <powda/engine.hpp>
#include <powda/engine_component.hpp>
#include <powda/logger.hpp>
#include <powda/shader.hpp>
#include <powda/shader_program.hpp>
#include <powda/time_step.hpp>
#include "powda/materials.hpp"

#include <GLFW/glfw3.h>

namespace powda
{

Engine::Engine()
    : m_target_fps{1000000}
    , m_frame_count{}
    , m_world{std::make_shared<World>(160, 90)}
    , m_pixel_grid{m_world}
{
}

void Engine::render(Window& window)
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(handle_gl_error, nullptr);

    auto last_frame            = std::chrono::system_clock::now();
    auto start_fps_count_timer = std::chrono::system_clock::now();
    glEnable(GL_DEPTH_TEST);

    const char* original_title = window.get_title();

    for (size_t x = 0; x < m_world->width(); ++x)
    {
        m_world->set(x, 0, Materials::Wall);
    }

    const auto middle = m_world->width() / 2;
    while (!window.should_close())
    {
        using namespace std::chrono_literals;

        auto before_render = std::chrono::system_clock::now();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_pixel_grid.render();
        window.update();

        m_world->next_step();
        m_world->set(middle, m_world->height() - 1, Materials::Powder);

        TimeStep   time_step;
        const auto after_render = std::chrono::system_clock::now();
        time_step.m_render_time = after_render - last_frame;
        last_frame              = after_render;

        const auto elapsed              = after_render - before_render;
        const auto max_time_per_frame   = std::chrono::system_clock::duration(1s) / m_target_fps;
        time_step.m_next_frame_deadline = after_render + (max_time_per_frame - elapsed);

        for (const int pressed_key : window.get_keyboard_info().m_current_pressed_keys)
        {
            if (pressed_key == GLFW_KEY_ESCAPE || pressed_key == GLFW_KEY_Q)
            {
                m_logger.info("Closing!");
                window.close();
            }
        }

        ++m_frame_count;
        if (after_render - start_fps_count_timer > 1s)
        {
            window.set_title(
                std::vformat("{}: {} fps", std::make_format_args(original_title, m_frame_count))
                    .c_str()
            );
            m_frame_count         = 0;
            start_fps_count_timer = std::chrono::system_clock::now();
        }

        while (std::chrono::system_clock::now() < time_step.m_next_frame_deadline)
            ;
    }
}

void Engine::handle_gl_error(
    GLenum        source,
    GLenum        type,
    GLuint        id,
    GLenum        severity,
    GLsizei       length,
    const GLchar* message,
    const void*   user_param
)
{
    Logger::Level level;
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        level = Logger::Level::Error;
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        level = Logger::Level::Warn;
        break;
    default:
    case GL_DEBUG_SEVERITY_LOW:
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        level = Logger::Level::Debug;
        return;
    }
    Logger{"OpenGL"}.log(level, "{}", message);

    throw std::runtime_error("OpenGL fatal error!");
}

} // namespace powda
