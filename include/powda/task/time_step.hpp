#ifndef POWDA_TIME_STEP_HPP
#define POWDA_TIME_STEP_HPP

#include <chrono>

namespace powda
{

struct TimeStep
{
    std::chrono::system_clock::duration m_render_time;
    std::chrono::time_point<std::chrono::system_clock> m_next_frame_deadline;
};

} // namespace powda

#endif // POWDA_TIME_STEP_HPP
