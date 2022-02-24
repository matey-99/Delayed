#include "Shader.h"

#include <glad/glad.h>

Shader::Shader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    : m_Name(name), m_Uniforms(std::vector<ShaderUniform>())
{
    std::string vertexSource, fragmentSource, geometrySource;
    std::ifstream filestream;

    filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        std::stringstream buffer;

        filestream.open(vertexPath);
        buffer << filestream.rdbuf();
        vertexSource = buffer.str();
        filestream.close();

        buffer.str(std::string());

        filestream.open(fragmentPath);
        buffer << filestream.rdbuf();
        fragmentSource = buffer.str();
        filestream.close();

        if (geometryPath)
        {
            buffer.str(std::string());

            filestream.open(geometryPath);
            buffer << filestream.rdbuf();
            geometrySource = buffer.str();
            filestream.close();
        }
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "Reading shader failed." << std::endl;
    }

    unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    uint32_t geometryShader;
    if (geometryPath)
        geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource.c_str());

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (geometryPath)
        glAttachShader(shaderProgram, geometryShader);

    glLinkProgram(shaderProgram);

    int result;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryPath)
        glDeleteShader(geometryShader);

    id = shaderProgram;

    LoadUniforms();
}

Shader::~Shader()
{
    glDeleteProgram(id);
}

void Shader::Use() const
{
    glUseProgram(id);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, glm::vec2& vec) const
{
    glUniform2f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y);
}

void Shader::SetVec3(const std::string& name, glm::vec3& vec) const
{
    glUniform3f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetVec4(const std::string& name, glm::vec4& vec) const
{
    glUniform4f(glGetUniformLocation(id, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetMat4(const std::string& name, glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

unsigned int Shader::CompileShader(unsigned int type, const char* source)
{
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int result;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << ((type == GL_VERTEX_SHADER) ? "Vertex" : (type == GL_FRAGMENT_SHADER) ? "Fragment" : "Geometry") << " Shader compilation failed: ";
        std::cout << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void Shader::LoadUniforms()
{
    m_Uniforms.clear();

    GLint uniformsCount;
    glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniformsCount);

    GLsizei length;
    GLint size;
    GLenum type;
    GLchar uniformName[32];
    for (GLuint i = 0; i < uniformsCount; i++)
    {
        glGetActiveUniform(id, i, 32, &length, &size, &type, uniformName);

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
