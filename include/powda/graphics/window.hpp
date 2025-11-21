#ifndef POWDA_WINDOW_HPP
#define POWDA_WINDOW_HPP

#include <powda/logger.hpp>

#include <functional>
#include <vector>

struct GLFWwindow;

namespace powda
{

class Window
{
  public:
    Window(unsigned int width, unsigned int height, const char* title);

    static void init();

    void update();

    void         make_active();
    void         close();
    bool         should_close();
    void         set_title(const char* title);
    const char*  title() const;
    void         set_size(int width, int height);
    unsigned int width() const;
    unsigned int height() const;

    struct MouseInfo
    {
        std::vector<int> m_current_pressed_buttons;
        double           x_pos;
        double           y_pos;
    };

    struct KeyboardInfo
    {
        std::vector<int> m_current_pressed_keys;
    };

    const MouseInfo&    mouse_info() const;
    const KeyboardInfo& keyboard_info() const;

  private:
    Logger       m_logger{"Window"};
    unsigned int m_width;
    unsigned int m_height;
    const char*  m_title;

    MouseInfo    m_mouse_info;
    KeyboardInfo m_keyboard_info;

    std::unique_ptr<GLFWwindow, std::function<void(GLFWwindow*)>> m_ptr;
};

} // namespace powda

#endif // POWDA_WINDOW_HPP
