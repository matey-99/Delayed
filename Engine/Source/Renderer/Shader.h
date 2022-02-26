#pragma once

#include <glm/glm.hpp>

enum class ShaderUniformType
{
	BOOL, INT, FLOAT, VEC3, VEC4, MAT3, MAT4, SAMPLER_2D, SAMPLER_CUBE
};

struct ShaderUniform
{
public:
	std::string name;
	ShaderUniformType type;
};

class Shader
{
public:
	unsigned int id;

private:
	std::string m_Name;
	std::vector<ShaderUniform> m_Uniforms;

public:
	Shader(std::string name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
	~Shader();
	void Use() const;

	inline std::string GetName() const { return m_Name; }
	inline std::vector<ShaderUniform> GetUniforms() const { return m_Uniforms; }

	// uniforms
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, glm::vec2& vec) const;
	void SetVec3(const std::string& name, glm::vec3& vec) const;
	void SetVec4(const std::string& name, glm::vec4& vec) const;
	void SetMat4(const std::string& name, glm::mat4& mat) const;
	
private:
	unsigned int CompileShader(unsigned int type, const char* source);
	void LoadUniforms();
};