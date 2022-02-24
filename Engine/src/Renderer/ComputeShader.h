#pragma once

#include <glm/glm.hpp>
#include "Shader.h"

class ComputeShader
{
public:
	ComputeShader(const char* path);
	~ComputeShader();

	void Use() const;

	inline uint32_t GetID() const { return m_ID; }
	inline std::vector<ShaderUniform> GetUniforms() const { return m_Uniforms; }

	// uniforms
	void SetBool(const std::string& name, bool value) const;
	void SetInt(const std::string& name, int value) const;
	void SetUint(const std::string& name, unsigned int value) const;
	void SetFloat(const std::string& name, float value) const;
	void SetVec2(const std::string& name, glm::vec2& vec) const;
	void SetVec3(const std::string& name, glm::vec3& vec) const;
	void SetVec4(const std::string& name, glm::vec4& vec) const;
	void SetMat4(const std::string& name, glm::mat4& mat) const;
	
private:
	unsigned int CompileShader(const char* source);
	void LoadUniforms();

private:
	uint32_t m_ID;

	std::vector<ShaderUniform> m_Uniforms;
};