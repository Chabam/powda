#include <cassert>
#include <chrono>

#include <powda/task/scheduler.hpp>
#include <powda/logger.hpp>
#include <powda/graphics/shader.hpp>
#include <powda/graphics/shader_program.hpp>
#include <powda/task/time_step.hpp>

namespace powda
{

Scheduler::Scheduler(unsigned int target_fps)
    : m_should_stop{false}
    , m_target_fps{target_fps}
{
}

void Scheduler::start()
{
    auto last_frame            = std::chrono::system_clock::now();
    auto start_fps_count_timer = std::chrono::system_clock::now();

    unsigned long long    frame_count = 0;
    static constexpr auto task_runner = [](auto& task) {
        task.run();
    };
    while (!m_should_stop)
    {
        using namespace std::chrono_literals;

        auto before_render = std::chrono::system_clock::now();

        for (auto& task : m_render_tasks)
            std::visit(task_runner, task);

        ++frame_count;

        TimeStep   time_step;
        const auto after_render = std::chrono::system_clock::now();
        time_step.m_render_time = after_render - last_frame;
        last_frame              = after_render;

        const auto elapsed              = after_render - before_render;
        const auto max_time_per_frame   = std::chrono::system_clock::duration(1s) / m_target_fps;
        time_step.m_next_frame_deadline = after_render + (max_time_per_frame - elapsed);

        if (after_render - start_fps_count_timer > 1s)
        {
            m_logger.debug("{} fps (target {})", frame_count, m_target_fps);
            frame_count           = 0;
            start_fps_count_timer = std::chrono::system_clock::now();
        }

        while (std::chrono::system_clock::now() < time_step.m_next_frame_deadline)
        {
            for (auto& task : m_background_tasks)
            {
                std::visit(task_runner, task);
                if (m_should_stop)
                    return;
            }
        }
    }
}

void Scheduler::register_render_task(const SchedulingTask& task)
{
    m_render_tasks.push_back(task);
}

void Scheduler::register_background_task(const SchedulingTask& task)
{
    m_background_tasks.push_back(task);
}

} // namespace powda
