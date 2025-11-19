#include <powda/graphics/gl/shader.hpp>
#include <powda/utils/logger.hpp>

#include <filesystem>
#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace powda
{

GLenum convertToGlEnum(Shader::Type type)
{
    switch (type)
    {
    case Shader::Type::Fragment:
        return GL_FRAGMENT_SHADER;
    case Shader::Type::Vertex:
        return GL_VERTEX_SHADER;
    default:
        throw std::runtime_error("Invalid shader type provided");
    }
}

const char* shader_type_to_text(Shader::Type type)
{
    switch (type)
    {
    case Shader::Type::Fragment:
        return "Fragment";
    case Shader::Type::Vertex:
        return "Vertex";
    default:
        throw std::runtime_error("Invalid shader type provided");
    }
}

Shader::Shader(Type type, const char* source)
    : m_id(glCreateShader(convertToGlEnum(type)))
    , m_type(type)
{
    const char* shader_type_str = shader_type_to_text(m_type);
    m_logger.add_subcategory(std::vformat("{},id:{}", std::make_format_args(shader_type_str, m_id)));

    glShaderSource(m_id, 1, &source, 0);
    glCompileShader(m_id);

    GLint isCompiled = 0;
    glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        GLint max_length = 0;
        glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &max_length);

        std::vector<GLchar> info_log(max_length);
        glGetShaderInfoLog(m_id, max_length, &max_length, &info_log[0]);

        glDeleteShader(m_id);

        std::ostringstream out;
        out << shader_type_str;
        for (auto& character : info_log)
        {
            out << character;
        }
        m_logger.error("Compilation error: {}", out.str());

        throw std::runtime_error(out.str());
    }

    m_logger.debug("shader compiled from source successfully");
}

Shader::~Shader()
{
    m_logger.debug("shader destroyed");
    glDeleteShader(m_id);
}

Shader::Shader(Shader&& other)
{
    m_id = std::move(other.m_id);
    other.m_id = 0;
}

Shader& Shader::operator=(Shader&& other)
{
    m_id = std::move(other.m_id);
    other.m_id = 0;

    return *this;
}

std::optional<Shader> Shader::try_compile(Type type, const char* source) noexcept
{
    std::optional<Shader> shader_out;
    try
    {
        shader_out = Shader{type, source};
        Logger("Shader").debug("{} shader compiled with success", shader_type_to_text(type));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Shader compilation failed:\n" << e.what() << '\n';
    }

    return shader_out;
}

} // namespace powda
