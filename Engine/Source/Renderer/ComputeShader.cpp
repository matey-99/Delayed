#include "ComputeShader.h"

#include <glad/glad.h>

ComputeShader::ComputeShader(std::string name, const std::string& source)
    : m_Name(name)
{
    uint32_t computeShader = CompileShader(source.c_str());
    uint32_t shaderProgram = glCreateProgram();
    glProgramParameteri(shaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
    glAttachShader(shaderProgram, computeShader);
    glLinkProgram(shaderProgram);

    int result;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(computeShader);
    m_ID = shaderProgram;

    LoadUniforms();
}

ComputeShader::~ComputeShader()
{
    glDeleteProgram(m_ID);
}

Ref<ComputeShader> ComputeShader::Create(std::string name, const std::string& source)
{
    return CreateRef<ComputeShader>(name, source);
}

void ComputeShader::Use() const
{
    glUseProgram(m_ID);
}

void ComputeShader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void ComputeShader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeShader::SetUint(const std::string& name, unsigned int value) const
{
    glUniform1ui(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeShader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void ComputeShader::SetVec2(const std::string& name, glm::vec2& vec) const
{
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
}

void ComputeShader::SetVec3(const std::string& name, glm::vec3& vec) const
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
}

void ComputeShader::SetVec4(const std::string& name, glm::vec4& vec) const
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void ComputeShader::SetMat4(const std::string& name, glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int ComputeShader::CompileShader(const char* source)
{
    unsigned int shader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "Compute Shader compilation failed : ";
        std::cout << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void ComputeShader::LoadUniforms()
{
    m_Uniforms.clear();

    GLint uniformsCount;
    glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &uniformsCount);

    GLsizei length;
    GLint size;
    GLenum type;
    GLchar uniformName[32];
    for (GLuint i = 0; i < uniformsCount; i++)
    {
        glGetActiveUniform(m_ID, i, 32, &length, &size, &type, uniformName);

        ShaderUniform uniform;
        uniform.Name = uniformName;
        switch (type)
        {
        case GL_BOOL:
            uniform.Type = ShaderUniformType::BOOL;
            break;
        case GL_INT:
            uniform.Type = ShaderUniformType::INT;
            break;
        case GL_FLOAT:
            uniform.Type = ShaderUniformType::FLOAT;
            break;
        case GL_FLOAT_VEC3:
            uniform.Type = ShaderUniformType::VEC3;
            break;
        case GL_FLOAT_VEC4:
            uniform.Type = ShaderUniformType::VEC4;
            break;
        case GL_FLOAT_MAT3:
            uniform.Type = ShaderUniformType::MAT3;
            break;
        case GL_FLOAT_MAT4:
            uniform.Type = ShaderUniformType::MAT4;
            break;
        case GL_SAMPLER_2D:
            uniform.Type = ShaderUniformType::SAMPLER_2D;
            break;
        case GL_SAMPLER_CUBE:
            uniform.Type = ShaderUniformType::SAMPLER_CUBE;
            break;
        }

        m_Uniforms.push_back(uniform);
    }
}
