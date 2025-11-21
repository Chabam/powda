#include <powda/graphics/window.hpp>
#include <powda/logger.hpp>

// clang-format off
#include <glad/gl.h>
#include <GLFW/glfw3.h>
// clang-format on

namespace powda
{

void Window::init()
{
    Logger init_logger{"Window Init"};
    init_logger.debug("Init glfw");

    if (!glfwInit())
    {
        throw std::runtime_error("Could not initialize GLWF!");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    init_logger.debug("glfw ok!");
}

Window::Window(unsigned int width, unsigned int height, const char* title)
    : m_width{width}
    , m_height{height}
    , m_title{title}
    , m_ptr{glfwCreateWindow(width, height, title, nullptr, nullptr), glfwDestroyWindow}
{
    if (!m_ptr)
    {
        throw std::runtime_error("Could not create a GLWF window! Call Window::init first!");
    }

    m_logger.add_subcategory(title);

    glfwSetWindowAspectRatio(m_ptr.get(), 16, 9);

    glfwSetWindowUserPointer(m_ptr.get(), this);

    glfwSetKeyCallback(
        m_ptr.get(),
        [](GLFWwindow*          glfwWindow,
           int                  key,
           [[maybe_unused]] int scancode,
           int                  action,
           [[maybe_unused]] int mods) {
            KeyboardInfo& keyboard_info =
                static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->m_keyboard_info;
            if (action == GLFW_PRESS)
            {
                keyboard_info.m_current_pressed_keys.push_back(key);
            }
            else if (action == GLFW_RELEASE)
            {
                keyboard_info.m_current_pressed_keys.erase(
                    std::ranges::find(keyboard_info.m_current_pressed_keys, key)
                );
            }
        }
    );

    glfwSetWindowSizeCallback(m_ptr.get(), [](GLFWwindow* glfwWindow, int width, int height) {
        Window* window   = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));
        window->m_width  = width;
        window->m_height = height;
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(m_ptr.get(), [](GLFWwindow* glfwWindow, double x_pos, double y_pos) {
        MouseInfo& mouse_info =
            static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->m_mouse_info;
        mouse_info.x_pos = x_pos;
        mouse_info.y_pos = y_pos;
    });

    glfwSetMouseButtonCallback(
        m_ptr.get(), [](GLFWwindow* glfwWindow, int button, int action, [[maybe_unused]] int mods) {
            MouseInfo& mouse_info =
                static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow))->m_mouse_info;
            if (action == GLFW_PRESS)
            {
                mouse_info.m_current_pressed_buttons.push_back(button);
            }
            else if (action == GLFW_RELEASE)
            {
                mouse_info.m_current_pressed_buttons.erase(
                    std::ranges::find(mouse_info.m_current_pressed_buttons, button)
                );
            }
        }
    );

    m_logger.debug("Window created");
}

void Window::make_active()
{
    glfwMakeContextCurrent(m_ptr.get());
    glfwSwapInterval(0);
    m_logger.debug("Init glad");
    if (!gladLoadGL(static_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        throw std::runtime_error("Could not initialize glad!");
    }
    m_logger.debug("glad ok!");
}

const char* Window::title() const
{
    return m_title;
}

void Window::set_title(const char* title)
{
    m_title = title;
    glfwSetWindowTitle(m_ptr.get(), title);
}

void Window::set_size(int width, int height)
{
    m_width  = width;
    m_height = height;
}

void Window::close()
{
    m_logger.info("Closing window");
    glfwSetWindowShouldClose(m_ptr.get(), GLFW_TRUE);
}

bool Window::should_close()
{
    return glfwWindowShouldClose(m_ptr.get());
}

void Window::update()
{
    glfwSwapBuffers(m_ptr.get());
    glfwPollEvents();
}

unsigned int Window::width() const
{
    return m_width;
}

unsigned int Window::height() const
{
    return m_height;
}

const Window::MouseInfo& Window::mouse_info() const
{
    return m_mouse_info;
}

const Window::KeyboardInfo& Window::keyboard_info() const
{
    return m_keyboard_info;
}

} // namespace powda
