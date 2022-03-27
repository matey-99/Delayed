#include "Renderer.h"
#include "Scene/Scene.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
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
#include "RenderPass/PostProcessingPass.h"
#include "RenderPass/FXAAPass.h"
#include "RenderPass/DepthOfFieldPass.h"
#include "RenderPass/UIPass.h"

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


	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_CameraVertexUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::mat4) * 3, 0);
	m_CameraFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_VEC3_SIZE + GLSL_SCALAR_SIZE, 2);

	m_GBufferPass = CreateRef<GBufferPass>();
	m_ShadowsPass = CreateRef<ShadowsPass>();
	m_SSAOPass = CreateRef<SSAOPass>();
	m_LightingPass = CreateRef<LightingPass>();
	m_PostProcessingPass = CreateRef<PostProcessingPass>();
	m_FXAAPass = CreateRef<FXAAPass>();
	m_DepthOfFieldPass = CreateRef<DepthOfFieldPass>();
	m_UIPass = CreateRef<UIPass>();

	m_Output[0] = 0;
	m_Output[1] = 0;
}

void Renderer::Render(Ref<Scene> scene, Ref<Camera> camera, uint32_t outputIndex)
{
	// Pre-Render
	scene->PreRender();

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
	m_GBufferPass->Render(scene);

	// Shadows
	m_ShadowsPass->Render(scene);

	// SSAO
	m_SSAOPass->Render();
		
	// Lighting
	m_LightingPass->Render();
	m_Output[outputIndex] = m_LightingPass->GetRenderTarget()->GetTargets()[0];

	// Post Processing
	if (m_Settings.PostProcessingEnabled)
	{
		m_PostProcessingPass->Render();
		m_Output[outputIndex] = m_PostProcessingPass->GetMainRenderTarget()->GetTargets()[0];
	}

	// FXAA
	if (m_Settings.FXAAEnabled)
	{
		m_FXAAPass->Render(m_Output[outputIndex]);
		m_Output[outputIndex] = m_FXAAPass->GetRenderTarget()->GetTargets()[0];
	}

	// Depth Of Field
	if (m_Settings.DepthOfFieldEnabled)
	{
		m_DepthOfFieldPass->Render(m_Output[outputIndex]);
		m_Output[outputIndex] = m_DepthOfFieldPass->GetFinalRenderTarget()->GetTargets()[0];
	}

	// UI
	m_UIPass->Render(scene, m_Output[outputIndex]);
	m_Output[outputIndex] = m_UIPass->GetRenderTarget()->GetTargets()[0];
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

	// Post Processing
	m_PostProcessingPass->UpdateRenderTargets(width, height);

	// FXAA
	m_FXAAPass->UpdateRenderTarget(width, height);

	// Depth Of Field
	m_DepthOfFieldPass->UpdateRenderTargets(width, height);

	// UI
	m_UIPass->UpdateRenderTargets(width, height);
}