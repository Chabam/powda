#ifndef POWDA_PIXEL_GRID
#define POWDA_PIXEL_GRID

#include <cstddef>
#include <glad/gl.h>
#include <powda/shader_program.hpp>

namespace powda
{

class PixelGrid
{
  public:
    PixelGrid(unsigned int width, unsigned int height);
    ~PixelGrid();

    void set(unsigned int x, unsigned int y, unsigned int color);
    void render();

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }

  private:
    static constexpr auto                     s_buffer_count = 2;
    std::array<GLuint, s_buffer_count>        m_pbo_ids;
    GLuint                                    m_tex_id;
    ShaderProgram                             m_shader_program;
    GLuint                                    m_vbo;
    GLuint                                    m_vao;
    unsigned int                              m_width;
    unsigned int                              m_height;
    size_t                                    m_pixel_count;
    unsigned char                             m_current_buffer;
    std::array<unsigned int*, s_buffer_count> m_pixels_buffers;
};

} // namespace powda

#endif // POWDA_PIXEL_GRID
