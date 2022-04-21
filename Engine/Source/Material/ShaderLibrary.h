#pragma once

#include "Core.h"
#include "Patterns/Singleton.h"

#include "Renderer/Shader.h"
#include "Renderer/ComputeShader.h"

enum class ShaderType
{
	Material, PostProcessing, Skybox, Calculations, Particle, UI
};

class ShaderLibrary : public Singleton<ShaderLibrary>
{
public:
	ShaderLibrary();

	Ref<Shader> GetShader(ShaderType type, std::string name);
	Ref<ComputeShader> GetComputeShader(std::string name);

	std::vector<Ref<Shader>> GetAllMaterialShaders();
	inline std::unordered_map<std::string, Ref<Shader>> GetMaterialShaders() const { return m_MaterialShaders; }

private:
	std::unordered_map<std::string, Ref<Shader>> m_MaterialShaders;

	std::unordered_map<std::string, Ref<Shader>> m_PostProcessingShaders;
	std::unordered_map<std::string, Ref<Shader>> m_SkyboxShaders;
	std::unordered_map<std::string, Ref<Shader>> m_CalculationShaders;
	std::unordered_map<std::string, Ref<Shader>> m_ParticleShaders;
	std::unordered_map<std::string, Ref<Shader>> m_UIShaders;

	std::unordered_map<std::string, Ref<ComputeShader>> m_ComputeShaders;
};