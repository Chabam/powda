#ifndef RT_ENGINE_COMPONENT_H
#define RT_ENGINE_COMPONENT_H

namespace rt
{

struct TimeStep;

class EngineComponent
{
  public:
    virtual ~EngineComponent() = default;

    virtual void update(const TimeStep& time_step) = 0;
};

} // namespace rt

#endif // RT_ENGINE_COMPONENT_H