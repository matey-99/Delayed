#include "ComputeShader.h"

#include <glad/glad.h>

ComputeShader::ComputeShader(const char* path)
    : m_Uniforms(std::vector<ShaderUniform>())
{
    std::string source;
    std::ifstream filestream;

    filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream buffer;

        filestream.open(path);
        buffer << filestream.rdbuf();
        source = buffer.str();
        filestream.close();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Reading shader failed." << std::endl;
    }

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
        uniform.name = uniformName;
        switch (type)
        {
        case GL_BOOL:
            uniform.type = ShaderUniformType::BOOL;
            break;
        case GL_INT:
            uniform.type = ShaderUniformType::INT;
            break;
        case GL_FLOAT:
            uniform.type = ShaderUniformType::FLOAT;
            break;
        case GL_FLOAT_VEC3:
            uniform.type = ShaderUniformType::VEC3;
            break;
        case GL_FLOAT_VEC4:
            uniform.type = ShaderUniformType::VEC4;
            break;
        case GL_FLOAT_MAT3:
            uniform.type = ShaderUniformType::MAT3;
            break;
        case GL_FLOAT_MAT4:
            uniform.type = ShaderUniformType::MAT4;
            break;
        case GL_SAMPLER_2D:
            uniform.type = ShaderUniformType::SAMPLER_2D;
            break;
        case GL_SAMPLER_CUBE:
            uniform.type = ShaderUniformType::SAMPLER_CUBE;
            break;
        }

        m_Uniforms.push_back(uniform);
    }
}
