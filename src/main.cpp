#include <powda/window.hpp>
#include <powda/scheduler.hpp>
#include "powda/logger.hpp"

int main()
{
    powda::Logger::set_global_level(powda::Logger::Level::Debug);

    powda::Window::init();
    powda::Window main_window{1280, 720, "powda"};
    main_window.make_active();
    powda::Scheduler engine;
    engine.render(main_window);
}
