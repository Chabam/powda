#ifndef POWDA_ENGINE_HPP
#define POWDA_ENGINE_HPP

#include <glad/gl.h>

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
    Scheduler();

    void render(Window& window);

  private:
    unsigned int       m_target_fps;
    unsigned long long m_frame_count;
    WorldPtr           m_world;
    Renderer           m_renderer;
    Logger             m_logger{"engine"};

    static void handle_gl_error(
        GLenum        source,
        GLenum        type,
        GLuint        id,
        GLenum        severity,
        GLsizei       length,
        const GLchar* message,
        const void*   user_param
    );
};

} // namespace powda

#endif // POWDA_ENGINE_HPP
