#pragma once

#include <glm/glm.hpp>

enum class ShaderUniformType
{
	BOOL, INT, FLOAT, VEC3, VEC4, MAT3, MAT4, SAMPLER_2D, SAMPLER_CUBE
};

struct ShaderUniform
{
	std::string Name;
	ShaderUniformType Type;
};

struct ShaderSource
{
	std::string VertexSource;
	std::string FragmentSource;
	std::string GeometrySource;
};

class Shader
{
public:
	Shader(const std::string& name, const std::string& path, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource);
	~Shader();

	static Ref<Shader> Create(const std::string& name, const std::string& path, const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");

	void Use() const;
	void Compile(const std::string& vertexSource, const std::string& fragmentSource, const std::string& geometrySource = "");

	void Recompile();

	void Remove();

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

private:
	uint32_t m_ID;
	std::string m_Name;
	std::string m_Path;
	std::vector<ShaderUniform> m_Uniforms;
};