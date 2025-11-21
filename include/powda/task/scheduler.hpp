#ifndef POWDA_ENGINE_HPP
#define POWDA_ENGINE_HPP

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
    void register_task(const SchedulingTask& task);
    void stop() { m_should_stop = true; }

  private:
    bool m_should_stop;
    unsigned int                m_target_fps;
    std::vector<SchedulingTask> m_tasks;
    Logger                      m_logger{"engine"};
};

using SchedulerPtr = std::shared_ptr<Scheduler>;

} // namespace powda

#endif // POWDA_ENGINE_HPP
