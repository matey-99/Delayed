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
#include "RenderPass/LightingPass.h"
#include "RenderPass/PostProcessingPass.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Renderer::Renderer()
{
	m_RenderOutputType = RenderOutputType::PostProcessing;

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
	m_LightingPass = CreateRef<LightingPass>();
	m_PostProcessingPass = CreateRef<PostProcessingPass>();
}

void Renderer::Render(Ref<Scene> scene)
{
	// Pre-Render
	scene->PreRender();

	// Camera
	auto camera = CameraManager::GetInstance()->GetMainCamera();

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

	// Lighting
	m_LightingPass->Render();

	// Post Processing
	 m_PostProcessingPass->Render();
}

uint32_t Renderer::GetOutput()
{
	switch (m_RenderOutputType)
	{
	case RenderOutputType::Normal:
		return m_LightingPass->GetRenderTarget()->GetTargets()[0];
	case RenderOutputType::PostProcessing:
		return m_PostProcessingPass->GetMainRenderTarget()->GetTargets()[0];
	}
}

void Renderer::ResizeWindow(uint32_t width, uint32_t height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	// GBuffer
	m_GBufferPass->GetRenderTarget()->Update(width, height);

	// Lighting
	m_LightingPass->GetRenderTarget()->Update(width, height);

	// POST PROCESSING
	m_PostProcessingPass->UpdateRenderTargets(width, height);
}