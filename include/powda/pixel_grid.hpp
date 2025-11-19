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
    void render() const;

    unsigned int width() const { return m_width; }
    unsigned int height() const { return m_height; }

  private:
    GLuint                    m_pbo_id;
    GLuint                    m_tex_id;
    ShaderProgram             m_shader_program;
    GLuint                    m_vbo;
    GLuint                    m_vao;
    unsigned int              m_width;
    unsigned int              m_height;
    size_t                    m_pixel_count;
    std::vector<unsigned int> m_pixels;
};

} // namespace powda

#endif // POWDA_PIXEL_GRID
