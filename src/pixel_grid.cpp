#include <powda/pixel_grid.hpp>
#include <powda/shader.hpp>
#include "glad/gl.h"

namespace powda
{

static constexpr auto g_vert_shader = R"(
#version 460

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTex;

out vec2 texCoord;

void main()
{
    gl_Position = vec4(inPos, 0.0, 1.0);
    texCoord = inTex;
})";

static constexpr auto g_frag_shader = R"(
#version 460

in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D ourTexture;

void main()
{
    fragColor = texture(ourTexture, texCoord);
}
)";

PixelGrid::PixelGrid(unsigned int width, unsigned int height)
    : m_tex_id{}
    , m_shader_program{}
    , m_vbo{}
    , m_vao{}
    , m_width{width}
    , m_height{height}
    , m_pixels{}
{
    auto vert = std::make_shared<Shader>(Shader::Type::Vertex, g_vert_shader);
    auto frag = std::make_shared<Shader>(Shader::Type::Fragment, g_frag_shader);
    m_shader_program.attach_shader(frag);
    m_shader_program.attach_shader(vert);
    m_shader_program.link();

    glGenBuffers(1, &m_vbo);
    glGenVertexArrays(1, &m_vao);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    constexpr float viewport_quad[6][4] = {
        {-1.f, -1.f, 0.f, 0.f}, {-1.f, 1.f, 0.f, 1.f}, {1.f, 1.f, 1.f, 1.f},
        {-1.f, -1.f, 0.f, 0.f}, {1.f, -1.f, 1.f, 0.f}, {1.f, 1.f, 1.f, 1.f}
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(viewport_quad), viewport_quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex_id);
    glTextureStorage2D(m_tex_id, 1, GL_RGBA8, m_width, m_height);
    glTextureParameteri(m_tex_id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    m_pixels.resize(m_width * m_height);
    std::fill(m_pixels.begin(), m_pixels.end(), 0xFFFFFF);
    // Investigate PBO:
    // 1. Generate and bind a PBO
    // GLuint pboId;
    // glGenBuffers(1, &pboId);
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pboId);

    // 2. Allocate storage for the PBO
    // glBufferData(GL_PIXEL_UNPACK_BUFFER, imageSize, 0, GL_STREAM_DRAW);

    // 3. Map the PBO to client memory and fill with data
    // void* ptr = glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
    // Copy image data into ptr
    // memcpy(ptr, imageData, imageSize);
    // glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    // 4. Upload data from PBO to texture
    // glBindTexture(GL_TEXTURE_2D, textureId);
    // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, 0); // Last argument is
    // 0, indicating data comes from bound PBO

    // 5. Unbind the PBO
    // glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

PixelGrid::~PixelGrid()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteTextures(1, &m_tex_id);
}

void PixelGrid::set(unsigned int x, unsigned int y, unsigned int color)
{
    m_pixels[x + (y * m_width)] = color;
}

void PixelGrid::render() const
{
    m_shader_program.use();
    glBindTextureUnit(0, m_tex_id);
    glTextureSubImage2D(
        m_tex_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_pixels.data()
    );
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

} // namespace powda
