#include <powda/graphics/window.hpp>
#include <powda/task/scheduler.hpp>
#include <powda/logger.hpp>
#include <powda/simulation/world.hpp>
#include <powda/simulation/gravity_simulation.hpp>
#include <powda/task/handle_inputs.hpp>

int main()
{
    using namespace powda;
    Logger::set_global_level(Logger::Level::Debug);

    Window::init();
    std::shared_ptr<Window> main_window = std::make_shared<Window>(1280, 720, "powda");
    main_window->make_active();

    auto world              = std::make_shared<World>(1600, 900);
    auto sched              = std::make_shared<Scheduler>(-1);
    auto renderer           = std::make_shared<Renderer>(world);
    auto gravity_simulation = std::make_shared<GravitySimulation>(world);

    sched->register_render_task(RenderWorld{renderer, main_window});
    sched->register_render_task(SimulatePowders{gravity_simulation});
    sched->register_render_task(HandleInputs{main_window, sched, world, gravity_simulation});
    sched->start();
}
