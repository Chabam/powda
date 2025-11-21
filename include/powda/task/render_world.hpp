#ifndef POWDA_RENDER_WORLD_HPP
#define POWDA_RENDER_WORLD_HPP

#include <memory>

namespace powda
{

class Window;
class Renderer;

class RenderWorld
{
  public:
    RenderWorld(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Window>& window);

    void run();

  private:
    std::shared_ptr<Renderer> m_renderer;
    std::shared_ptr<Window>   m_window;
};

} // namespace powda

#endif // POWDA_RENDER_WORLD_HPP
