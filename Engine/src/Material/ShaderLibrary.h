#pragma once

#include <unordered_map>
#include <string>
#include <mutex>

#include "typedefs.h"
#include "Renderer/Shader.h"

enum class ShaderType
{
	MATERIAL, POST_PROCESSING, SKYBOX, CALCULATION, PARTICLE
};

class ShaderLibrary
{
public:
	ShaderLibrary();
	~ShaderLibrary() {};

	ShaderLibrary(ShaderLibrary& other) = delete;
	void operator=(const ShaderLibrary&) = delete;

	static Ref<ShaderLibrary> GetInstance();

	Ref<Shader> GetShader(ShaderType type, std::string name);

	std::vector<Ref<Shader>> GetAllMaterialShaders();
	inline std::unordered_map<std::string, Ref<Shader>> GetMaterialShaders() const { return m_MaterialShaders; }

private:
	static Ref<ShaderLibrary> s_Instance;
	static std::mutex s_Mutex;

	std::unordered_map<std::string, Ref<Shader>> m_MaterialShaders;
	std::unordered_map<std::string, Ref<Shader>> m_PostProcessingShaders;
	std::unordered_map<std::string, Ref<Shader>> m_SkyboxShaders;
	std::unordered_map<std::string, Ref<Shader>> m_CalculationShaders;
	std::unordered_map<std::string, Ref<Shader>> m_ParticleShaders;
};