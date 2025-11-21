#include <powda/task/render_world.hpp>

namespace powda
{
RenderWorld::RenderWorld(const std::shared_ptr<Renderer>& renderer, const std::shared_ptr<Window>& window)
    : m_renderer{renderer}
    , m_window{window}
{
}

void RenderWorld::run()
{
    m_renderer->render();
    m_window->update();
}

} // namespace powda
