#ifndef SCHEDULING_TASK_HPP
#define SCHEDULING_TASK_HPP

#include <variant>

#include <powda/task/render_world.hpp>
#include <powda/task/simulate_powders.hpp>
#include <powda/task/handle_inputs.hpp>

namespace powda
{

using SchedulingTask = std::variant<RenderWorld, SimulatePowders, HandleInputs>;

}
#endif // SCHEDULING_TASK_HPP
