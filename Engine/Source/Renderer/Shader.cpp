#include "Shader.h"

#include <glad/glad.h>
#include "Importer/ShaderImporter.h"

Shader::Shader(const std::string& name, const std::string& path, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
    : m_ID(0), m_Name(name), m_Path(path)
{
    Compile(vertexSource, fragmentSource, geometrySource);
}

Shader::~Shader()
{
    glDeleteProgram(m_ID);
}

Ref<Shader> Shader::Create(const std::string& name, const std::string& path, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
{
    return CreateRef<Shader>(name, path, vertexSource, fragmentSource, geometrySource);
}

void Shader::Use() const
{
    glUseProgram(m_ID);
}

void Shader::Compile(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource)
{
    if (m_ID)
        glDeleteProgram(m_ID);

    bool isGeometryShader = geometrySource != "";

    uint32_t vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
    uint32_t fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

    uint32_t geometryShader;
    if (isGeometryShader)
        geometryShader = CompileShader(GL_GEOMETRY_SHADER, geometrySource.c_str());

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (isGeometryShader)
        glAttachShader(shaderProgram, geometryShader);

    glLinkProgram(shaderProgram);

    int result;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << m_Name << ": Shader program linking failed: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (isGeometryShader)
        glDeleteShader(geometryShader);

    m_ID = shaderProgram;

    LoadUniforms();
}

void Shader::Recompile()
{
    ShaderSource source = ShaderImporter::ParseShader(m_Path);

    Compile(source.VertexSource, source.FragmentSource, source.GeometrySource);
}

void Shader::Remove()
{
    glDeleteProgram(m_ID);
}

void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::SetVec2(const std::string& name, glm::vec2& vec) const
{
    glUniform2f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y);
}

void Shader::SetVec3(const std::string& name, glm::vec3& vec) const
{
    glUniform3f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::SetVec4(const std::string& name, glm::vec4& vec) const
{
    glUniform4f(glGetUniformLocation(m_ID, name.c_str()), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetMat4(const std::string& name, glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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
        std::cout << m_Name << ((type == GL_VERTEX_SHADER) ? ": Vertex" : (type == GL_FRAGMENT_SHADER) ? ": Fragment" : ": Geometry") << " Shader compilation failed: ";
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
