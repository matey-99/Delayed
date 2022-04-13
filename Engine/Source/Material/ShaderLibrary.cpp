#include "ShaderLibrary.h"

#include "Content/ContentHelper.h"

Ref<ShaderLibrary> ShaderLibrary::s_Instance{};
std::mutex ShaderLibrary::s_Mutex;

ShaderLibrary::ShaderLibrary()
{
	// OPAQUE MATERIAL SHADERS
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Standard", 
		CreateRef<Shader>("Standard", 
			ContentHelper::GetAssetPath("Shaders/Material/Standard.vert"),
			ContentHelper::GetAssetPath("Shaders/GBuffer.frag"))));

	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Lighting",
		CreateRef<Shader>("Lighting",
			ContentHelper::GetAssetPath("Shaders/Lighting.vert"),
			ContentHelper::GetAssetPath("Shaders/Lighting.frag"))));

	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("OpaqueSkinned",
		CreateRef<Shader>("OpaqueSkinned",
			ContentHelper::GetAssetPath("Shaders/Material/StandardSkinned.vert"),
			ContentHelper::GetAssetPath("Shaders/GBuffer.frag"))));

	// TRANSPARENT MATERIAL SHADERS
	m_MaterialShaders.insert(std::make_pair<std::string, Ref<Shader>>("Transparent",
		CreateRef<Shader>("Transparent",
			ContentHelper::GetAssetPath("Shaders/Material/Standard.vert"),
			ContentHelper::GetAssetPath("Shaders/Material/Standard.frag"))));

	// POST PROCESSING SHADERS
	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Screen", 
		CreateRef<Shader>("Screen", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Screen.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Screen.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("SSAO",
		CreateRef<Shader>("SSAO",
			ContentHelper::GetAssetPath("Shaders/SSAO.vert"),
			ContentHelper::GetAssetPath("Shaders/SSAO.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("SSAOBlur",
		CreateRef<Shader>("SSAOBlur",
			ContentHelper::GetAssetPath("Shaders/SSAO.vert"),
			ContentHelper::GetAssetPath("Shaders/SSAOBlur.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("PostProcessing", 
		CreateRef<Shader>("PostProcessing", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("FXAA",
		CreateRef<Shader>("FXAA",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/FXAA.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Vignette",
		CreateRef<Shader>("Vignette",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Vignette.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Blur",
		CreateRef<Shader>("Blur",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/Blur.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthOfField",
		CreateRef<Shader>("DepthOfField",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/DepthOfField.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Threshold", 
		CreateRef<Shader>("Threshold", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Threshold.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Threshold.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("Scale", 
		CreateRef<Shader>("Scale", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/Scale.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("BlurHorizontal", 
		CreateRef<Shader>("BlurHorizontal",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/BlurHorizontal.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("BlurVertical",
		CreateRef<Shader>("BlurVertical",
			ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/BlurVertical.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapOrtographic", 
		CreateRef<Shader>("DepthMapOrtographic", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/DepthMapOrtographic.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/DepthMapOrtographic.frag"))));

	m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthMapPerspective",
		CreateRef<Shader>("DepthMapPerspective", 
			ContentHelper::GetAssetPath("Shaders/PostProcessing/DepthMapPerspective.vert"),
			ContentHelper::GetAssetPath("Shaders/PostProcessing/DepthMapPerspective.frag"))));

    m_PostProcessingShaders.insert(std::make_pair<std::string, Ref<Shader>>("DepthFog",
            CreateRef<Shader>("DepthFog",
                              ContentHelper::GetAssetPath("Shaders/PostProcessing/PostProcessing.vert"),
                              ContentHelper::GetAssetPath("Shaders/PostProcessing/DepthFog.frag"))));

	// SKYBOX SHADERS
	m_SkyboxShaders.insert(std::make_pair<std::string, Ref<Shader>>("Skybox", 
		CreateRef<Shader>("Skybox", 
			ContentHelper::GetAssetPath("Shaders/Skybox/Skybox.vert"),
			ContentHelper::GetAssetPath("Shaders/Skybox/Skybox.frag"))));

	// CALCULATION SHADERS
	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepth", 
		CreateRef<Shader>("SceneDepth", 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepth.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepth.frag"),
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepth.geom"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthInstanced", 
		CreateRef<Shader>("SceneDepthInstanced", 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthInstanced.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepth.frag"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPoint", 
		CreateRef<Shader>("SceneDepthPoint", 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPoint.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPoint.frag"), 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPoint.geom"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("SceneDepthPointInstanced",
		CreateRef<Shader>("SceneDepthPointInstanced", 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPointInstanced.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPoint.frag"), 
			ContentHelper::GetAssetPath("Shaders/Calculation/SceneDepthPoint.geom"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("EquirectangularToCubemap", 
		CreateRef<Shader>("EquirectangularToCubemap", 
			ContentHelper::GetAssetPath("Shaders/Calculation/EquirectangularToCubemap.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/EquirectangularToCubemap.frag"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Irradiance", 
		CreateRef<Shader>("Irradiance", 
			ContentHelper::GetAssetPath("Shaders/Calculation/Irradiance.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/Irradiance.frag"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("Prefilter", 
		CreateRef<Shader>("Prefilter", 
			ContentHelper::GetAssetPath("Shaders/Calculation/Prefilter.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/Prefilter.frag"))));

	m_CalculationShaders.insert(std::make_pair<std::string, Ref<Shader>>("BRDF", 
		CreateRef<Shader>("BRDF", 
			ContentHelper::GetAssetPath("Shaders/Calculation/BRDF.vert"),
			ContentHelper::GetAssetPath("Shaders/Calculation/BRDF.frag"))));

	// PARTICLE SHADERS
	m_ParticleShaders.insert(std::make_pair<std::string, Ref<Shader>>("StandardParticle", 
		CreateRef<Shader>("StandardParticle", 
			ContentHelper::GetAssetPath("Shaders/Particle/StandardParticle.vert"),
			ContentHelper::GetAssetPath("Shaders/Particle/StandardParticle.frag"))));

	// UI SHADERS
	m_UIShaders.insert(std::make_pair<std::string, Ref<Shader>>("UI",
		CreateRef<Shader>("UI",
			ContentHelper::GetAssetPath("Shaders/UI/UI.vert"),
			ContentHelper::GetAssetPath("Shaders/UI/UI.frag"))));

	// COMPUTE SHADERS
	m_ComputeShaders.insert(std::make_pair<std::string, Ref<ComputeShader>>("StandardParticle",
		CreateRef<ComputeShader>(ContentHelper::GetAssetPath("Shaders/Particle/StandardParticle.comp"))));
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
