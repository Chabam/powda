#include <cassert>
#include <powda/engine.hpp>
#include <powda/engine_component.hpp>
#include <powda/time_step.hpp>
#include <powda/logger.hpp>

#include <GLFW/glfw3.h>
#include <chrono>
#include <format>
#include <memory>
#include <stdexcept>
#include "powda/shader.hpp"
#include "powda/shader_program.hpp"

namespace powda
{

static constexpr auto g_vert_shader = R"(
#version 460

// uniform sampler2D inTexture;
in vec2 inTexCoord;
out vec2 texCoord;

void main()
{
    texCoord = inTexCoord;
}
)";

static constexpr auto g_frag_shader = R"(
#version 460

in vec2 texCoord;
uniform sampler2D inTexture;
out vec4 fragColor;

void main()
{
    fragColor = texture(inTexture, texCoord);
}
)";

Engine::Engine()
    : m_target_fps{60}
    , m_frame_count{}
    , m_shader_program{}
    , m_vbo{}
    , m_vao{}
{
    auto vert = std::make_shared<Shader>(Shader::Type::Vertex, g_vert_shader);
    auto frag = std::make_shared<Shader>(Shader::Type::Fragment, g_frag_shader);
    m_shader_program.attach_shader(frag);
    m_shader_program.attach_shader(vert);
    m_shader_program.link();

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    constexpr float viewport_quad[6][2] = {
        {-1.f, -1.f}, {-1.f, 1.f}, {1.f, 1.f},
        {-1.f, -1.f}, {1.f, -1.f}, {1.f, 1.f}
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(viewport_quad), viewport_quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);
}

void Engine::render(Window& window)
{
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(handle_gl_error, nullptr);

    auto last_frame = std::chrono::system_clock::now();
    auto start_fps_count_timer = std::chrono::system_clock::now();
    glEnable(GL_DEPTH_TEST);

    const char* original_title = window.get_title();

    while (!window.should_close())
    {
        using namespace std::chrono_literals;

        auto before_render = std::chrono::system_clock::now();

        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        static const float black[] = {0.0f, 0.0f, 0.0f, 0.0f};
        glClearBufferfv(GL_COLOR, 0, black);

        glBindVertexArray(m_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        window.update();

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
            window.set_title(std::vformat("{}: {} fps", std::make_format_args(original_title, m_frame_count)).c_str());
            m_frame_count = 0;
            start_fps_count_timer = std::chrono::system_clock::now();
        }

        while(std::chrono::system_clock::now() < time_step.m_next_frame_deadline);
    }
}

void Engine::handle_gl_error(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                             const GLchar* message, const void* user_param)
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
