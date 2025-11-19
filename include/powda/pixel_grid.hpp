#ifndef POWDA_PIXEL_GRID
#define POWDA_PIXEL_GRID

#include <glad/gl.h>
#include <powda/shader_program.hpp>
#include <powda/world.hpp>

namespace powda
{

class PixelGrid
{
  public:
    PixelGrid(const WorldPtr& world);
    ~PixelGrid();

    void render();

  private:
    WorldPtr m_world;

    static constexpr auto                     s_buffer_count = 2;
    std::array<GLuint, s_buffer_count>        m_pbo_ids;
    GLuint                                    m_tex_id;
    ShaderProgram                             m_shader_program;
    GLuint                                    m_vbo;
    GLuint                                    m_vao;
    unsigned char                             m_current_buffer;
    std::array<unsigned int*, s_buffer_count> m_pixels_buffers;

    void write_world_to_pixel_buf();
};

} // namespace powda

#endif // POWDA_PIXEL_GRID
