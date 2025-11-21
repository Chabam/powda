#ifndef POWDA_ENGINE_H
#define POWDA_ENGINE_H

#include <powda/window.hpp>
#include <powda/logger.hpp>

#include <glad/gl.h>
#include <powda/shader_program.hpp>
#include <powda/renderer.hpp>
#include <powda/world.hpp>

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

#endif // POWDA_ENGINE_H
