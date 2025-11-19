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

void ShaderProgram::set_uniform(const char* varName, const glm::mat4& matrix) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::set_uniform(const char* varName, const glm::mat3& matrix) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void ShaderProgram::set_uniform(const char* varName, const glm::vec4& vector) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniform4f(uniform_location, vector.r, vector.g, vector.b, vector.a);
}

void ShaderProgram::set_uniform(const char* varName, const glm::vec3& vector) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniform3f(uniform_location, vector.r, vector.g, vector.b);
}

void ShaderProgram::set_uniform(const char* varName, float value) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniform1f(uniform_location, value);
}

void ShaderProgram::set_uniform(const char* varName, int value) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniform1i(uniform_location, value);
}

void ShaderProgram::set_uniform(const char* varName, unsigned int value) const
{
    const int uniform_location = glGetUniformLocation(m_id, varName);
    glUniform1ui(uniform_location, value);
}

} // namespace powda
