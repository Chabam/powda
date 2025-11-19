#ifndef POWDA_PIXEL_GRID
#define POWDA_PIXEL_GRID

#include <vector>
#include <glad/gl.h>

namespace powda
{

class PixelGrid
{
  public:
    PixelGrid(unsigned int width, unsigned int height);

    void set(unsigned int x, unsigned int y, unsigned int color);
  private:
    GLuint m_id;
    unsigned int m_width;
    unsigned int m_height;
    std::vector<unsigned int> m_pixels;
};

} // namespace powda

#endif // POWDA_PIXEL_GRID
