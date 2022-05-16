#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Mesh.h"
#include "Scene/Component/Light/Light.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"
#include "Scene/Component/TransformComponent.h"
#include "Camera/CameraManager.h"
#include "RenderPass/GBufferPass.h"
#include "RenderPass/ShadowsPass.h"
#include "RenderPass/SSAOPass.h"
#include "RenderPass/LightingPass.h"
#include "RenderPass/ForwardPass.h"
#include "RenderPass/PostProcessingPass.h"
#include "RenderPass/FXAAPass.h"
#include "RenderPass/VignettePass.h"
#include "RenderPass/DepthOfFieldPass.h"
#include "RenderPass/UIPass.h"
#include "RenderPass/DepthFogPass.h"
#include "RenderTools.h"
#include "Analysis/Profiler.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
	m_WindowWidth = 1920;
	m_WindowHeight = 1080;
}

Renderer::~Renderer()
{
}

void Renderer::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_CameraVertexUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::mat4) * 3, 0);
	m_CameraFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_VEC3_SIZE + GLSL_SCALAR_SIZE, 2);

	m_GBufferPass = CreateRef<GBufferPass>();
	m_ShadowsPass = CreateRef<ShadowsPass>();
	m_SSAOPass = CreateRef<SSAOPass>();
	m_LightingPass = CreateRef<LightingPass>();
	m_ForwardPass = CreateRef<ForwardPass>();
	m_PostProcessingPass = CreateRef<PostProcessingPass>();
	m_FXAAPass = CreateRef<FXAAPass>();
	m_VignettePass = CreateRef<VignettePass>();
	m_DepthOfFieldPass = CreateRef<DepthOfFieldPass>();
	m_UIPass = CreateRef<UIPass>();
    m_DepthFogPass = CreateRef<DepthFogPass>();

	m_Output[0] = 0;
	m_Output[1] = 0;
}

void Renderer::Render(Ref<Scene> scene, Ref<Camera> camera, uint32_t outputIndex)
{
	// Pre-Render
	PROFILER_START("Pre-Render Time");
	scene->PreRender();
	PROFILER_STOP();

	PROFILER_START("Render Time");

	m_CameraVertexUniformBuffer->Bind();
	m_CameraVertexUniformBuffer->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjectionMatrix()));
	m_CameraVertexUniformBuffer->SetUniform(GLSL_MAT4_SIZE, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
	m_CameraVertexUniformBuffer->SetUniform(GLSL_MAT4_SIZE * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
	m_CameraVertexUniformBuffer->Unbind();

	m_CameraFragmentUniformBuffer->Bind();
	m_CameraFragmentUniformBuffer->SetUniform(0, sizeof(glm::vec3), glm::value_ptr(camera->GetWorldPosition()));

	float farClipPlane = camera->GetFarClipPlane();
	m_CameraFragmentUniformBuffer->SetUniform(sizeof(glm::vec3), sizeof(float), &farClipPlane);
	m_CameraFragmentUniformBuffer->Unbind();

	// GBuffer
	PROFILER_START("GBuffer Pass");
	m_GBufferPass->Render(scene);
	PROFILER_STOP();

	// Shadows
	PROFILER_START("Shadows Pass");
	m_ShadowsPass->Render(scene);
	PROFILER_STOP();

	// SSAO
	if (m_Settings.SSAOEnabled)
	{
		PROFILER_START("SSAO Pass");
		m_SSAOPass->Render();
		PROFILER_STOP();
	}
		
	// Lighting
	PROFILER_START("Lighting Pass");
	m_LightingPass->Render(scene);
	PROFILER_STOP();
	m_Output[outputIndex] = m_LightingPass->GetRenderTarget()->GetTargets()[0];

	// Depth Fog
	if (m_Settings.DepthFogEnabled)
	{
		PROFILER_START("Depth Fog Pass");
		m_DepthFogPass->Render(m_Output[outputIndex]);
		PROFILER_STOP();
		m_Output[outputIndex] = m_DepthFogPass->GetRenderTarget()->GetTargets()[0];
	}

	// Forward
	PROFILER_START("Forward Pass");
	Ref<RenderTarget> previousRT = m_Settings.DepthFogEnabled ? m_DepthFogPass->GetRenderTarget() : m_LightingPass->GetRenderTarget();
	m_ForwardPass->Render(scene, previousRT);
	PROFILER_STOP();

	// Post Processing
	if (m_Settings.PostProcessingEnabled)
	{
		PROFILER_START("Post Processing Pass");
		m_PostProcessingPass->Render(m_Output[outputIndex]);
		PROFILER_STOP();
		m_Output[outputIndex] = m_PostProcessingPass->GetMainRenderTarget()->GetTargets()[0];
	}

	// FXAA
	if (m_Settings.FXAAEnabled)
	{
		PROFILER_START("FXAA Pass");
		m_FXAAPass->Render(m_Output[outputIndex]);
		PROFILER_STOP();
		m_Output[outputIndex] = m_FXAAPass->GetRenderTarget()->GetTargets()[0];
	}

	if (m_Settings.VignetteEnabled)
	{
		PROFILER_START("Vignette Pass");
		m_VignettePass->Render(m_Output[outputIndex]);
		PROFILER_STOP();
		m_Output[outputIndex] = m_VignettePass->GetRenderTarget()->GetTargets()[0];
	}

	// Depth Of Field
	if (m_Settings.DepthOfFieldEnabled)
	{
		PROFILER_START("Depth Of Field Pass");
		m_DepthOfFieldPass->Render(m_Output[outputIndex]);
		PROFILER_STOP();
		m_Output[outputIndex] = m_DepthOfFieldPass->GetFinalRenderTarget()->GetTargets()[0];
	}

	// UI
	PROFILER_START("UI Pass");
	m_UIPass->Render(scene, m_Output[outputIndex]);
	PROFILER_STOP();
	m_Output[outputIndex] = m_UIPass->GetRenderTarget()->GetTargets()[0];

	PROFILER_STOP();
}

void Renderer::Display()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	auto screenShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Viewport");
	screenShader->Use();
	screenShader->SetInt("u_Screen", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_Output[0]);

	glDisable(GL_DEPTH_TEST);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
}

uint32_t Renderer::GetOutput(uint32_t index)
{
	return m_Output[index];
}

void Renderer::ResizeWindow(uint32_t width, uint32_t height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	// GBuffer
	m_GBufferPass->GetRenderTarget()->Update(width, height);

	// SSAO
	m_SSAOPass->UpdateRenderTargets(width, height);

	// Lighting
	m_LightingPass->GetRenderTarget()->Update(width, height);

	// Depth Fog
	m_DepthFogPass->UpdateRenderTarget(width, height);

	// Post Processing
	m_PostProcessingPass->UpdateRenderTargets(width, height);

	// FXAA
	m_FXAAPass->UpdateRenderTarget(width, height);

	// Vignette
	m_VignettePass->UpdateRenderTarget(width, height);

	// Depth Of Field
	m_DepthOfFieldPass->UpdateRenderTargets(width, height);

	// UI
	m_UIPass->UpdateRenderTargets(width, height);
}