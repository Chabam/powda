#include <powda/graphics/window.hpp>
#include <powda/task/scheduler.hpp>
#include <powda/logger.hpp>
#include <powda/simulation/world.hpp>
#include <powda/task/handle_inputs.hpp>

int main()
{
    using namespace powda;
    Logger::set_global_level(Logger::Level::Debug);

    Window::init();
    std::shared_ptr<Window> main_window = std::make_shared<Window>(1280, 720, "powda");
    main_window->make_active();

    std::shared_ptr<World> world = std::make_shared<World>(160, 90);
    std::shared_ptr<Scheduler> sched    = std::make_shared<Scheduler>(60);
    std::shared_ptr<Renderer>  renderer = std::make_shared<Renderer>(world);

    sched->register_background_task(RenderWorld{renderer, main_window});
    sched->register_render_task(SimulatePowders{world});
    sched->register_background_task(HandleInputs{main_window, sched, world});
    sched->start();
}

