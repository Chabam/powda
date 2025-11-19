#include <glad/gl.h>

#include <powda/pixel_grid.hpp>
#include <powda/shader.hpp>

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
    : m_pbo_id{}
    , m_tex_id{}
    , m_shader_program{}
    , m_vbo{}
    , m_vao{}
    , m_width{width}
    , m_height{height}
    , m_pixel_count{m_width * m_height}
    , m_pixels{}
{
    m_pixels.resize(m_pixel_count);
    std::fill(m_pixels.begin(), m_pixels.end(), 0xFFFFFFFF);

    glCreateTextures(GL_TEXTURE_2D, 1, &m_tex_id);
    glBindTexture(GL_TEXTURE_2D, m_tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        m_width,
        m_height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        (GLvoid*)m_pixels.data()
    );

    glGenBuffers(1, &m_pbo_id);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo_id);

    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_width * m_height, 0, GL_STREAM_DRAW);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

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
        {-1.f, -1.f, 0.f, 0.f}, // Top left
        {-1.f, 1.f, 0.f, 1.f},  // Bottom left
        {1.f, 1.f, 1.f, 1.f},   // Bottom right
        {-1.f, -1.f, 0.f, 0.f}, // Top left
        {1.f, -1.f, 1.f, 0.f},  // Top right
        {1.f, 1.f, 1.f, 1.f}    // Bottom right
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(viewport_quad), viewport_quad, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat))
    );
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}

PixelGrid::~PixelGrid()
{
    glDeleteBuffers(1, &m_vbo);
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_pbo_id);
    glDeleteTextures(1, &m_tex_id);
}

void PixelGrid::set(unsigned int x, unsigned int y, unsigned int color)
{
    m_pixels[x + (y * m_width)] = color;
}

void PixelGrid::render() const
{
    m_shader_program.use();
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo_id);

    glBufferData(GL_PIXEL_UNPACK_BUFFER, m_pixel_count * 4, 0, GL_STREAM_DRAW);
    auto ptr = static_cast<unsigned int*>(glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY));
    std::copy(m_pixels.begin(), m_pixels.end(), ptr);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    glBindTexture(GL_TEXTURE_2D, m_tex_id);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glBindVertexArray(m_vao);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

} // namespace powda
