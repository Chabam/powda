#ifndef POWDA_SCHEDULER_HPP
#define POWDA_SCHEDULER_HPP

#include <powda/task/scheduling_task.hpp>
#include <powda/graphics/renderer.hpp>
#include <powda/graphics/shader_program.hpp>
#include <powda/graphics/window.hpp>
#include <powda/logger.hpp>
#include <powda/simulation/world.hpp>

namespace powda
{

class Scheduler
{
  public:
    Scheduler(unsigned int target_fps);

    void start();
    void register_render_task(const SchedulingTask& task);
    void register_background_task(const SchedulingTask& task);
    void stop() { m_should_stop = true; }

  private:
    bool m_should_stop;
    unsigned int                m_target_fps;
    std::vector<SchedulingTask> m_render_tasks;
    std::vector<SchedulingTask> m_background_tasks;
    Logger                      m_logger{"engine"};
};

} // namespace powda

#endif // POWDA_SCHEDULER_HPP
