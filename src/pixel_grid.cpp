#include <powda/pixel_grid.hpp>

namespace powda
{

PixelGrid::PixelGrid(unsigned int width, unsigned int height)
    : m_id{}
    , m_width{width}
    , m_height{}
    , m_pixels()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, GL_RGB8, m_width, m_height);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_pixels.resize(m_width * m_height);
    std::fill(m_pixels.begin(), m_pixels.end(), 0x000000FF);

    glTextureSubImage2D(
        m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_pixels.data()
    );
    glBindTextureUnit(0, m_id);
}

void PixelGrid::set(unsigned int x, unsigned int y, unsigned int color)
{
    m_pixels[x + (y * m_width)] = color;
}

} // namespace powda
