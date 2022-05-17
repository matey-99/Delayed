#include "ShaderLibrary.h"

#include "Assets/AssetManager.h"

ShaderLibrary::ShaderLibrary()
{
	/* Material Shaders */

	Ref<Shader> opaque = AssetManager::LoadShader("Shaders/Opaque.glsl");
	m_MaterialShaders.insert({ opaque->GetName(), opaque });

	Ref<Shader> transparent = AssetManager::LoadShader("Shaders/Transparent.glsl");
	m_MaterialShaders.insert({ transparent->GetName(), transparent });

	Ref<Shader> transparentFresnel = AssetManager::LoadShader("Shaders/TransparentFresnel.glsl");
	m_MaterialShaders.insert({ transparentFresnel->GetName(), transparentFresnel });

	Ref<Shader> opaqueSkeletal = AssetManager::LoadShader("Shaders/OpaqueSkeletal.glsl");
	m_MaterialShaders.insert({ opaqueSkeletal->GetName(), opaqueSkeletal });

	Ref<Shader> transparentSkeletal = AssetManager::LoadShader("Shaders/TransparentSkeletal.glsl");
	m_MaterialShaders.insert({ transparentSkeletal->GetName(), transparentSkeletal });

	/* Post-Processing Shaders */

	Ref<Shader> viewport = AssetManager::LoadShader("Shaders/Viewport.glsl");
	m_PostProcessingShaders.insert({ viewport->GetName(), viewport });

	Ref<Shader> ssao = AssetManager::LoadShader("Shaders/SSAO.glsl");
	m_PostProcessingShaders.insert({ ssao->GetName(), ssao });

	Ref<Shader> ssr = AssetManager::LoadShader("Shaders/SSR.glsl");
	m_PostProcessingShaders.insert({ ssr->GetName(), ssr });

	Ref<Shader> ssaoBlur = AssetManager::LoadShader("Shaders/SSAOBlur.glsl");
	m_PostProcessingShaders.insert({ ssaoBlur->GetName(), ssaoBlur });

	Ref<Shader> postProcessing = AssetManager::LoadShader("Shaders/PostProcessing.glsl");
	m_PostProcessingShaders.insert({ postProcessing->GetName(), postProcessing });

	Ref<Shader> fxaa = AssetManager::LoadShader("Shaders/FXAA.glsl");
	m_PostProcessingShaders.insert({ fxaa->GetName(), fxaa });

	Ref<Shader> vignette = AssetManager::LoadShader("Shaders/Vignette.glsl");
	m_PostProcessingShaders.insert({ vignette->GetName(), vignette });

	Ref<Shader> blur = AssetManager::LoadShader("Shaders/Blur.glsl");
	m_PostProcessingShaders.insert({ blur->GetName(), blur });

	Ref<Shader> dof = AssetManager::LoadShader("Shaders/DepthOfField.glsl");
	m_PostProcessingShaders.insert({ dof->GetName(), dof });

	Ref<Shader> threshold = AssetManager::LoadShader("Shaders/Threshold.glsl");
	m_PostProcessingShaders.insert({ threshold->GetName(), threshold });

	Ref<Shader> scale = AssetManager::LoadShader("Shaders/Scale.glsl");
	m_PostProcessingShaders.insert({ scale->GetName(), scale });

	Ref<Shader> gaussianBlurHorizontal = AssetManager::LoadShader("Shaders/GaussianBlurHorizontal.glsl");
	m_PostProcessingShaders.insert({ gaussianBlurHorizontal->GetName(), gaussianBlurHorizontal });

	Ref<Shader> gaussianBlurVertical = AssetManager::LoadShader("Shaders/GaussianBlurVertical.glsl");
	m_PostProcessingShaders.insert({ gaussianBlurVertical->GetName(), gaussianBlurVertical });

	Ref<Shader> depthMapOrtographic = AssetManager::LoadShader("Shaders/DepthMapOrtographic.glsl");
	m_PostProcessingShaders.insert({ depthMapOrtographic->GetName(), depthMapOrtographic });

	Ref<Shader> depthMapPerspective = AssetManager::LoadShader("Shaders/DepthMapPerspective.glsl");
	m_PostProcessingShaders.insert({ depthMapPerspective->GetName(), depthMapPerspective });

	Ref<Shader> depthFog = AssetManager::LoadShader("Shaders/DepthFog.glsl");
	m_PostProcessingShaders.insert({ depthFog->GetName(), depthFog });

	/* Skybox Shaders */

	Ref<Shader> skybox = AssetManager::LoadShader("Shaders/Skybox.glsl");
	m_SkyboxShaders.insert({ skybox->GetName(), skybox });

	Ref<Shader> irradiance = AssetManager::LoadShader("Shaders/Irradiance.glsl");
	m_SkyboxShaders.insert({ irradiance->GetName(), irradiance });

	Ref<Shader> prefilter = AssetManager::LoadShader("Shaders/Prefilter.glsl");
	m_SkyboxShaders.insert({ prefilter->GetName(), prefilter });

	/* Calculation Shaders */

	Ref<Shader> lighting = AssetManager::LoadShader("Shaders/Lighting.glsl");
	m_CalculationShaders.insert({ lighting->GetName(), lighting });

	Ref<Shader> brdf = AssetManager::LoadShader("Shaders/BRDF.glsl");
	m_CalculationShaders.insert({ brdf->GetName(), brdf });

	Ref<Shader> sceneDepth = AssetManager::LoadShader("Shaders/SceneDepth.glsl");
	m_CalculationShaders.insert({ sceneDepth->GetName(), sceneDepth });

	Ref<Shader> sceneDepthPoint = AssetManager::LoadShader("Shaders/SceneDepthPoint.glsl");
	m_CalculationShaders.insert({ sceneDepthPoint->GetName(), sceneDepthPoint });

	/* Particle Shaders */

	Ref<Shader> particle = AssetManager::LoadShader("Shaders/Particle.glsl");
	m_ParticleShaders.insert({ particle->GetName(), particle });

	/* UI Shaders */
	
	Ref<Shader> ui = AssetManager::LoadShader("Shaders/UI.glsl");
	m_UIShaders.insert({ ui->GetName(), ui });

	/* Compute Shaders */

	Ref<ComputeShader> computeParticles = AssetManager::LoadComputeShader("Shaders/ComputeShaders/Particle.comp");
	m_ComputeShaders.insert({ computeParticles->GetName(), computeParticles });
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
