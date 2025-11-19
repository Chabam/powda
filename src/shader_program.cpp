#include <powda/shader.hpp>
#include <powda/shader_program.hpp>
#include <powda/logger.hpp>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>

namespace powda
{

ShaderProgram::ShaderProgram()
    : m_id(glCreateProgram())
{
    m_logger.add_subcategory("id:" + std::to_string(m_id));
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::attach_shader(const std::shared_ptr<Shader>& shader)
{
    m_attached_shaders.push_back(shader);
}

void ShaderProgram::link()
{
    for (const auto& shader : m_attached_shaders)
    {
        m_logger.debug("Attaching Shader ({})", shader->get_id());
        glAttachShader(m_id, shader->get_id());
    }

    glLinkProgram(m_id);

    GLint isLinked = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, (int*)&isLinked);
    if (isLinked == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(m_id, maxLength, &maxLength, &infoLog[0]);

        std::ostringstream out;
        out << "Linking error: ";
        for (auto& character : infoLog)
        {
            out << character;
        }
        m_logger.error(out.str());
        throw std::runtime_error(out.str());
    }
    m_logger.debug("linked succesfully");
}

void ShaderProgram::use() const
{
    glUseProgram(m_id);
}

} // namespace powda
