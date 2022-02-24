#include "ShaderLibrary.h"

Ref<ShaderLibrary> ShaderLibrary::s_Instance{};
std::mutex ShaderLibrary::s_Mutex;

ShaderLibrary::ShaderLibrary()
{
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Standard", CreateRef<Shader>("Standard", "Assets/Shaders/Material/Standard.vert", "Assets/Shaders/Material/Standard.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("StandardInstanced", CreateRef<Shader>("StandardInstanced", "Assets/Shaders/Material/StandardInstanced.vert", "Assets/Shaders/Material/Standard.frag")));
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("GrassInstanced", CreateRef<Shader>("GrassInstanced", "Assets/Shaders/Material/StandardInstanced.vert", "Assets/Shaders/Material/Grass.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Screen", CreateRef<Shader>("Screen", "Assets/Shaders/PostProcessing/Screen.vert", "Assets/Shaders/PostProcessing/Screen.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("PostProcessing", CreateRef<Shader>("PostProcessing", "Assets/Shaders/PostProcessing/PostProcessing.vert", "Assets/Shaders/PostProcessing/PostProcessing.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Threshold", CreateRef<Shader>("Threshold", "Assets/Shaders/PostProcessing/Threshold.vert", "Assets/Shaders/PostProcessing/Threshold.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Downfilter", CreateRef<Shader>("Downfilter", "Assets/Shaders/PostProcessing/PostProcessing.vert", "Assets/Shaders/PostProcessing/Downfilter.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Blur", CreateRef<Shader>("Blur", "Assets/Shaders/PostProcessing/PostProcessing.vert", "Assets/Shaders/PostProcessing/Blur.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapOrtographic", 
		CreateRef<Shader>("DepthMapOrtographic", "Assets/Shaders/PostProcessing/DepthMapOrtographic.vert", "Assets/Shaders/PostProcessing/DepthMapOrtographic.frag")));
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapPerspective",
		CreateRef<Shader>("DepthMapPerspective", "Assets/Shaders/PostProcessing/DepthMapPerspective.vert", "Assets/Shaders/PostProcessing/DepthMapPerspective.frag")));

	m_SkyboxShaders.insert(std::make_pair<std::string, Ref<Shader>>("Skybox", CreateRef<Shader>("Skybox", "Assets/Shaders/Skybox/Skybox.vert", "Assets/Shaders/Skybox/Skybox.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepth", CreateRef<Shader>("SceneDepth", 
		"Assets/Shaders/Calculation/SceneDepth.vert", "Assets/Shaders/Calculation/SceneDepth.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthInstanced", CreateRef<Shader>("SceneDepthInstanced",
		"Assets/Shaders/Calculation/SceneDepthInstanced.vert", "Assets/Shaders/Calculation/SceneDepth.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPoint", CreateRef<Shader>("SceneDepthPoint",
		"Assets/Shaders/Calculation/SceneDepthPoint.vert", "Assets/Shaders/Calculation/SceneDepthPoint.frag", "Assets/Shaders/Calculation/SceneDepthPoint.geom")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPointInstanced", CreateRef<Shader>("SceneDepthPointInstanced",
		"Assets/Shaders/Calculation/SceneDepthPointInstanced.vert", "Assets/Shaders/Calculation/SceneDepthPoint.frag", "Assets/Shaders/Calculation/SceneDepthPoint.geom")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("EquirectangularToCubemap", 
		CreateRef<Shader>("EquirectangularToCubemap", "Assets/Shaders/Calculation/EquirectangularToCubemap.vert", "Assets/Shaders/Calculation/EquirectangularToCubemap.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Irradiance", CreateRef<Shader>("Irradiance", "Assets/Shaders/Calculation/Irradiance.vert", "Assets/Shaders/Calculation/Irradiance.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Prefilter", CreateRef<Shader>("Prefilter", "Assets/Shaders/Calculation/Prefilter.vert", "Assets/Shaders/Calculation/Prefilter.frag")));
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("BRDF", CreateRef<Shader>("BRDF", "Assets/Shaders/Calculation/BRDF.vert", "Assets/Shaders/Calculation/BRDF.frag")));

	m_ParticleShaders.insert(std::make_pair<std::string, Ref<Shader>>("StandardParticle", CreateRef<Shader>("StandardParticle", "Assets/Shaders/Particle/StandardParticle.vert", "Assets/Shaders/Particle/StandardParticle.frag")));
}

Ref<ShaderLibrary> ShaderLibrary::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<ShaderLibrary>();

	return s_Instance;
}

Ref<Shader> ShaderLibrary::GetShader(ShaderType type, std::string name)
{
	switch (type)
	{
	case ShaderType::MATERIAL:
		return m_MaterialShaders.find(name)->second;
	case ShaderType::POST_PROCESSING:
		return m_PostProcessingShaders.find(name)->second;
	case ShaderType::SKYBOX:
		return m_SkyboxShaders.find(name)->second;
	case ShaderType::CALCULATION:
		return m_CalculationShaders.find(name)->second;
	case ShaderType::PARTICLE:
		return m_ParticleShaders.find(name)->second;
	}

	return Ref<Shader>();
}

std::vector<Ref<Shader>> ShaderLibrary::GetAllMaterialShaders()
{
	std::vector<Ref<Shader>> result = std::vector<Ref<Shader>>();
	for (auto shader : m_MaterialShaders)
	{
		result.push_back(shader.second);
	}

	return result;
}
