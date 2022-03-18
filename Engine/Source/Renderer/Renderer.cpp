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
	m_LightingPass = CreateRef<LightingPass>();
	m_PostProcessingPass = CreateRef<PostProcessingPass>();
}

//void Renderer::InitializeShadowMapFramebuffers()
//{
//	glGenFramebuffers(1, &m_DirectionalLightShadowMapFramebufferObject);
//
//	glGenTextures(1, &m_DirectionalLightShadowMaps);
//	glBindTexture(GL_TEXTURE_2D_ARRAY, m_DirectionalLightShadowMaps);
//	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, 4096, 4096, 5, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//
//	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, borderColor);
//
//	glBindFramebuffer(GL_FRAMEBUFFER, m_DirectionalLightShadowMapFramebufferObject);
//	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DirectionalLightShadowMaps, 0);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//
//	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//		std::cout << "Directional Light Framebuffer is incomplete!" << std::endl;
//
//	// POINT LIGHT
//	glGenFramebuffers(1, &m_PointLightShadowMapFramebufferObject);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	// SPOT LIGHT
//	glGenFramebuffers(1, &m_SpotLightShadowMapFramebufferObject);
//	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);
//	glDrawBuffer(GL_NONE);
//	glReadBuffer(GL_NONE);
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	CreateShadowMapsPlaceholders();
//}

void Renderer::Render(Ref<Scene> scene)
{
	// PRE-RENDER
	scene->PreRender();

	// CAMERA
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

	// Lighting
	m_LightingPass->Render();

	// SHADOW
	// m_ShadowPass->Render();

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

	// GBUFFER
	m_GBufferPass->GetRenderTarget()->Update(width, height);

	// LIGHTING
	m_LightingPass->GetRenderTarget()->Update(width, height);

	// SHADOW
	// m_ShadowPass->Render();

	// POST PROCESSING
	m_PostProcessingPass->UpdateRenderTargets(width, height);
}

void Renderer::RenderShadowMap(Scene* scene, DirectionalLight* source)
{
	/*glViewport(0, 0, 4096, 4096);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DirectionalLightShadowMapFramebufferObject);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto a : scene->GetActors())
	{
		if (auto smc = a->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", a->GetTransform()->GetWorldModelMatrix());

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}


	}*/

	/*auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthInstanced");
	depthIstancedShader->Use();
	depthIstancedShader->SetMat4("u_LightSpace", source->GetLightSpace());
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}*/

	/*glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}

void Renderer::RenderShadowMap(Scene* scene, PointLight* source)
{
	/*glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPoint");
	depthShader->Use();
	for (int i = 0; i < 6; i++)
		depthShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthShader->SetFloat("u_FarPlane", source->GetFarClipPlane());
	depthShader->SetVec3("u_LightPos", source->GetOwner()->GetTransform()->GetWorldPosition());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetActors())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform()->GetWorldModelMatrix());

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}
	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPointInstanced");
	depthIstancedShader->Use();
	for (int i = 0; i < 6; i++)
		depthIstancedShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthIstancedShader->SetFloat("u_FarPlane", source->GetFarClipPlane());
	depthIstancedShader->SetVec3("u_LightPos", source->GetOwner()->GetTransform()->GetWorldPosition());
	
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}

void Renderer::RenderShadowMap(Scene* scene, SpotLight* source)
{
	/*glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();
	depthShader->SetMat4("u_LightSpace", source->GetLightSpace());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetActors())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform()->GetWorldModelMatrix());

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}
	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthInstanced");
	depthIstancedShader->Use();
	depthIstancedShader->SetMat4("u_LightSpace", source->GetLightSpace());
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);*/
}