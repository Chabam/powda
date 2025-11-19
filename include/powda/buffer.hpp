#ifndef RT_BUFFER_H
#define RT_BUFFER_H

#include <powda/graphics/gl/vertex.hpp>
#include <powda/utils/logger.hpp>

#include <glad/gl.h>
#include <span>
#include <vector>

namespace powda
{

class Buffer
{
  public:
    Buffer(const std::span<const Vertex>& vertices, const std::span<const unsigned int>& indices);
    ~Buffer();

    void bind() const;
    void unbind() const;

  private:
    GLuint m_vao;
    GLuint m_vbo;
    GLuint m_ibo;

    Logger m_logger{"Buffer"};
};

} // namespace powda

#endif // RT_BUFFER_H
