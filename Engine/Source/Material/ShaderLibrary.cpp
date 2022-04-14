#include "ShaderLibrary.h"

#include "Assets/AssetManager.h"

Ref<ShaderLibrary> ShaderLibrary::s_Instance{};
std::mutex ShaderLibrary::s_Mutex;

ShaderLibrary::ShaderLibrary()
{
	// OPAQUE MATERIAL SHADERS
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Standard", 
		AssetManager::LoadShader("Standard", "Shaders/Material/Standard.vert", "Shaders/GBuffer.frag")));

	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Lighting",
		AssetManager::LoadShader("Lighting", "Shaders/Lighting.vert", "Shaders/Lighting.frag")));

	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("OpaqueSkeletal",
		AssetManager::LoadShader("OpaqueSkeletal", "Shaders/Material/StandardSkeletal.vert", "Shaders/GBuffer.frag")));

	// TRANSPARENT MATERIAL SHADERS
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Transparent",
		AssetManager::LoadShader("Transparent", "Shaders/Material/Standard.vert", "Shaders/Material/Standard.frag")));

	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("TransparentSkeletal",
		AssetManager::LoadShader("TransparentSkeletal", "Shaders/Material/StandardSkeletal.vert", "Shaders/Material/Standard.frag")));

	// POST PROCESSING SHADERS
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Screen", 
		AssetManager::LoadShader("Screen", "Shaders/PostProcessing/Screen.vert", "Shaders/PostProcessing/Screen.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("SSAO",
		AssetManager::LoadShader("SSAO", "Shaders/SSAO.vert", "Shaders/SSAO.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("SSAOBlur",
		AssetManager::LoadShader("SSAOBlur", "Shaders/SSAO.vert", "Shaders/SSAOBlur.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("PostProcessing", 
		AssetManager::LoadShader("PostProcessing", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/PostProcessing.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("FXAA",
		AssetManager::LoadShader("FXAA", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/FXAA.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Vignette",
		AssetManager::LoadShader("Vignette", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/Vignette.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Blur",
		AssetManager::LoadShader("Blur", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/Blur.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthOfField",
		AssetManager::LoadShader("DepthOfField", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/DepthOfField.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Threshold", 
		AssetManager::LoadShader("Threshold", "Shaders/PostProcessing/Threshold.vert", "Shaders/PostProcessing/Threshold.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Scale", 
		AssetManager::LoadShader("Scale", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/Scale.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("BlurHorizontal", 
		AssetManager::LoadShader("BlurHorizontal", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/BlurHorizontal.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("BlurVertical",
		AssetManager::LoadShader("BlurVertical", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/BlurVertical.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapOrtographic", 
		AssetManager::LoadShader("DepthMapOrtographic", "Shaders/PostProcessing/DepthMapOrtographic.vert", "Shaders/PostProcessing/DepthMapOrtographic.frag")));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapPerspective",
		AssetManager::LoadShader("DepthMapPerspective", "Shaders/PostProcessing/DepthMapPerspective.vert", "Shaders/PostProcessing/DepthMapPerspective.frag")));

    m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthFog",
            AssetManager::LoadShader("DepthFog", "Shaders/PostProcessing/PostProcessing.vert", "Shaders/PostProcessing/DepthFog.frag")));

	// SKYBOX SHADERS
	m_SkyboxShaders.insert(std::make_pair<std::string, Ref<Shader>>("Skybox", 
		AssetManager::LoadShader("Skybox", "Shaders/Skybox/Skybox.vert", "Shaders/Skybox/Skybox.frag")));

	// CALCULATION SHADERS
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepth", 
		AssetManager::LoadShader("SceneDepth", "Shaders/Calculation/SceneDepth.vert", "Shaders/Calculation/SceneDepth.frag", "Shaders/Calculation/SceneDepth.geom")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthInstanced", 
		AssetManager::LoadShader("SceneDepthInstanced", "Shaders/Calculation/SceneDepthInstanced.vert", "Shaders/Calculation/SceneDepth.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPoint", 
		AssetManager::LoadShader("SceneDepthPoint", 
			"Shaders/Calculation/SceneDepthPoint.vert", 
			"Shaders/Calculation/SceneDepthPoint.frag", 
			"Shaders/Calculation/SceneDepthPoint.geom")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPointInstanced",
		AssetManager::LoadShader("SceneDepthPointInstanced", 
			"Shaders/Calculation/SceneDepthPointInstanced.vert", 
			"Shaders/Calculation/SceneDepthPoint.frag", 
			"Shaders/Calculation/SceneDepthPoint.geom")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("EquirectangularToCubemap", 
		AssetManager::LoadShader("EquirectangularToCubemap", 
			 "Shaders/Calculation/EquirectangularToCubemap.vert",
			 "Shaders/Calculation/EquirectangularToCubemap.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Irradiance", 
		AssetManager::LoadShader("Irradiance", 
			 "Shaders/Calculation/Irradiance.vert",
			 "Shaders/Calculation/Irradiance.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Prefilter", 
		AssetManager::LoadShader("Prefilter", 
			 "Shaders/Calculation/Prefilter.vert",
			 "Shaders/Calculation/Prefilter.frag")));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("BRDF", 
		AssetManager::LoadShader("BRDF", 
			 "Shaders/Calculation/BRDF.vert",
			 "Shaders/Calculation/BRDF.frag")));

	// PARTICLE SHADERS
	m_ParticleShaders.insert(std::make_pair<std::string, Ref<Shader>>("StandardParticle", 
		AssetManager::LoadShader("StandardParticle", 
			 "Shaders/Particle/StandardParticle.vert",
			 "Shaders/Particle/StandardParticle.frag")));

	// UI SHADERS
	m_UIShaders.insert(std::make_pair<std::string, Ref<Shader>>("UI",
		AssetManager::LoadShader("UI",
			 "Shaders/UI/UI.vert",
			 "Shaders/UI/UI.frag")));

	// COMPUTE SHADERS
	m_ComputeShaders.insert(std::make_pair<std::string, Ref<ComputeShader>>("StandardParticle",
		AssetManager::LoadComputeShader("StandardParticle", "Shaders/Particle/StandardParticle.comp")));
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
	case ShaderType::Material:
		return m_MaterialShaders.find(name)->second;
	case ShaderType::PostProcessing:
		return m_PostProcessingShaders.find(name)->second;
	case ShaderType::Skybox:
		return m_SkyboxShaders.find(name)->second;
	case ShaderType::Calculations:
		return m_CalculationShaders.find(name)->second;
	case ShaderType::Particle:
		return m_ParticleShaders.find(name)->second;
	case ShaderType::UI:
		return m_UIShaders.find(name)->second;
	}

	return Ref<Shader>();
}

Ref<ComputeShader> ShaderLibrary::GetComputeShader(std::string name)
{
	return m_ComputeShaders.find(name)->second;
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
