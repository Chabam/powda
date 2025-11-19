#ifndef POWDA_SHADER_PROGRAM_H
#define POWDA_SHADER_PROGRAM_H

#include <powda/logger.hpp>

#include <glad/gl.h>
#include <memory>
#include <vector>


namespace powda
{

class Shader;

class ShaderProgram
{
  public:
    ShaderProgram();
    ~ShaderProgram();

    void attach_shader(const std::shared_ptr<Shader>& shader);
    void link();

    void use() const;
  private:
    GLuint m_id;
    std::vector<std::shared_ptr<Shader>> m_attached_shaders;
    Logger m_logger{"ShaderProgram"};
};

} // namespace powda

#endif // POWDA_SHADER_PROGRAM_H
