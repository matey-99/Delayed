#include "Renderer.h"
#include "Framebuffer.h"
#include "Scene/Scene.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/InstanceRenderedMeshComponent.h"
#include "Mesh.h"
#include "Scene/Component/Light/Light.h"
#include "Scene/Component/Light/PointLight.h"
#include "Scene/Component/Light/SpotLight.h"
#include "Scene/Component/Light/SkyLight.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

Ref<Renderer> Renderer::s_Instance{};
std::mutex Renderer::s_Mutex;

Renderer::Renderer()
{
	m_Gamma = 2.2f;
	m_Exposure = 1.0f;

	m_Bloom = true;
	m_BloomThreshold = 1.0f;
	m_BloomIntensity = 0.8f;
	m_BlurWidth = 0.8f;
}

Renderer::~Renderer()
{
}

Ref<Renderer> Renderer::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (s_Instance == nullptr)
		s_Instance = CreateRef<Renderer>();

	return s_Instance;
}

void Renderer::Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	m_CameraVertexUniformBuffer = CreateRef<UniformBuffer>(sizeof(glm::mat4) * 3, 0);
	m_CameraFragmentUniformBuffer = CreateRef<UniformBuffer>(GLSL_VEC3_SIZE, 2);
}

void Renderer::InitializeMainSceneFramebuffer()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_RGBA32F;
	textureConfig.Format = GL_RGBA;
	textureConfig.MinFilter = GL_LINEAR;
	textureConfig.MagFilter = GL_LINEAR;

	FramebufferRenderbufferConfig renderbufferConfig;
	renderbufferConfig.InternalFormat = GL_DEPTH_STENCIL;
	
	FramebufferConfig config;
	config.Width = 1920;
	config.Height = 1080;
	config.Textures.push_back(textureConfig);
	config.Renderbuffers.push_back(renderbufferConfig);

	m_MainSceneFramebuffer = Framebuffer::Create(config);
}

void Renderer::InitializePostProcessingFramebuffer()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_RGBA32F;
	textureConfig.Format = GL_RGBA;
	textureConfig.MinFilter = GL_LINEAR;
	textureConfig.MagFilter = GL_LINEAR;
	textureConfig.WrapS = GL_CLAMP_TO_EDGE;
	textureConfig.WrapT = GL_CLAMP_TO_EDGE;

	FramebufferRenderbufferConfig renderbufferConfig;
	renderbufferConfig.InternalFormat = GL_DEPTH_STENCIL;

	FramebufferConfig config;
	config.Width = 1920;
	config.Height = 1080;
	config.Textures.push_back(textureConfig);
	config.Renderbuffers.push_back(renderbufferConfig);

	m_PostProcessingFramebuffer = Framebuffer::Create(config);

	FramebufferTextureConfig thresholdTextureConfig;
	thresholdTextureConfig.Target = GL_TEXTURE_2D;
	thresholdTextureConfig.InternalFormat = GL_RGBA32F;
	thresholdTextureConfig.Format = GL_RGBA;
	thresholdTextureConfig.MinFilter = GL_LINEAR;
	thresholdTextureConfig.MagFilter = GL_LINEAR;
	thresholdTextureConfig.WrapS = GL_CLAMP_TO_EDGE;
	thresholdTextureConfig.WrapT = GL_CLAMP_TO_EDGE;

	FramebufferConfig thresholdConfig;
	thresholdConfig.Width = m_PostProcessingFramebuffer->GetConfiguration().Width;
	thresholdConfig.Height = m_PostProcessingFramebuffer->GetConfiguration().Height;
	thresholdConfig.Textures.push_back(thresholdTextureConfig);

	m_ThresholdFramebuffer = Framebuffer::Create(thresholdConfig);

	FramebufferTextureConfig halfResolutionTextureConfig;
	halfResolutionTextureConfig.Target = GL_TEXTURE_2D;
	halfResolutionTextureConfig.InternalFormat = GL_RGBA32F;
	halfResolutionTextureConfig.Format = GL_RGBA;
	halfResolutionTextureConfig.MinFilter = GL_NEAREST;
	halfResolutionTextureConfig.MagFilter = GL_NEAREST;
	halfResolutionTextureConfig.WrapS = GL_CLAMP_TO_EDGE;
	halfResolutionTextureConfig.WrapT = GL_CLAMP_TO_EDGE;

	FramebufferConfig halfResolutionConfig;
	halfResolutionConfig.Width = m_PostProcessingFramebuffer->GetConfiguration().Width / 4;
	halfResolutionConfig.Height = m_PostProcessingFramebuffer->GetConfiguration().Height / 4;
	halfResolutionConfig.Textures.push_back(halfResolutionTextureConfig);

	m_HalfResolutionFramebuffer = Framebuffer::Create(halfResolutionConfig);

	FramebufferTextureConfig blurTextureConfig;
	blurTextureConfig.Target = GL_TEXTURE_2D;
	blurTextureConfig.InternalFormat = GL_RGBA32F;
	blurTextureConfig.Format = GL_RGBA;
	blurTextureConfig.MinFilter = GL_NEAREST;
	blurTextureConfig.MagFilter = GL_NEAREST;
	blurTextureConfig.WrapS = GL_CLAMP_TO_EDGE;
	blurTextureConfig.WrapT = GL_CLAMP_TO_EDGE;

	FramebufferConfig blurConfig;
	blurConfig.Width = m_PostProcessingFramebuffer->GetConfiguration().Width / 4;
	blurConfig.Height = m_PostProcessingFramebuffer->GetConfiguration().Height / 4;
	blurConfig.Textures.push_back(blurTextureConfig);

	m_BlurFramebuffer = Framebuffer::Create(blurConfig);
}

void Renderer::InitializeShadowMapFramebuffers()
{
	FramebufferTextureConfig textureConfig;
	textureConfig.Attachment = GL_DEPTH_ATTACHMENT;
	textureConfig.Target = GL_TEXTURE_2D;
	textureConfig.InternalFormat = GL_DEPTH_COMPONENT;
	textureConfig.Format = GL_DEPTH_COMPONENT;
	textureConfig.MinFilter = GL_NEAREST;
	textureConfig.MagFilter = GL_NEAREST;
	textureConfig.WrapS = GL_REPEAT;
	textureConfig.WrapT = GL_REPEAT;
	textureConfig.Type = GL_FLOAT;
	textureConfig.Border = true;

	FramebufferConfig config;
	config.Width = 1024;
	config.Height = 1024;
	config.Textures.push_back(textureConfig);

	m_DirectionalLightShadowMapFramebuffer = Framebuffer::Create(config);

	// POINT LIGHT
	glGenFramebuffers(1, &m_PointLightShadowMapFramebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// SPOT LIGHT
	glGenFramebuffers(1, &m_SpotLightShadowMapFramebufferObject);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	CreateShadowMapsPlaceholders();
}

void Renderer::InitializePostProcessing()
{
	float viewportVertices[] =
	{
		-1.0f,  1.0f,    0.0f, 1.0f,
		-1.0f, -1.0f,    0.0f, 0.0f,
		 1.0f, -1.0f,    1.0f, 0.0f,

		-1.0f,  1.0f,    0.0f, 1.0f,
		 1.0f, -1.0f,    1.0f, 0.0f,
		 1.0f,  1.0f,    1.0f, 1.0f,
	};

	glGenVertexArrays(1, &m_PostProcessingVAO);
	glBindVertexArray(m_PostProcessingVAO);

	glGenBuffers(1, &m_PostProcessingVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_PostProcessingVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(viewportVertices), viewportVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Renderer::RenderScene(Ref<Scene> scene)
{
	scene->PreRender();

	m_MainSceneFramebuffer->Bind();

	glClearColor(scene->GetBackgroundColor()->x, scene->GetBackgroundColor()->y, scene->GetBackgroundColor()->z, scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_CameraVertexUniformBuffer->Bind();
	m_CameraVertexUniformBuffer->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(scene->GetCurrentCamera()->GetViewProjectionMatrix()));
	m_CameraVertexUniformBuffer->SetUniform(GLSL_MAT4_SIZE, sizeof(glm::mat4), glm::value_ptr(scene->GetCurrentCamera()->GetViewMatrix()));
	m_CameraVertexUniformBuffer->SetUniform(GLSL_MAT4_SIZE * 2, sizeof(glm::mat4), glm::value_ptr(scene->GetCurrentCamera()->GetProjectionMatrix()));
	m_CameraVertexUniformBuffer->Unbind();

	m_CameraFragmentUniformBuffer->Bind();
	m_CameraFragmentUniformBuffer->SetUniform(0, sizeof(glm::vec3), glm::value_ptr(scene->GetCurrentCamera()->GetOwner()->GetWorldPosition()));
	m_CameraFragmentUniformBuffer->Unbind();

	scene->Render();
	
	m_MainSceneFramebuffer->Unbind();
}

void Renderer::AddPostProcessingEffects()
{
	if (m_Bloom)
	{
		m_ThresholdFramebuffer->Bind();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		auto thresholdShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Threshold");
		thresholdShader->Use();
		thresholdShader->SetInt("u_Screen", 0);
		thresholdShader->SetFloat("u_Threshold", m_BloomThreshold);

		glBindVertexArray(m_PostProcessingVAO);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_MainSceneFramebuffer->GetColorAttachment());

		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_ThresholdFramebuffer->Unbind();

		m_HalfResolutionFramebuffer->Bind();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ThresholdFramebuffer->GetColorAttachment());

		auto downfilterShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Downfilter");
		downfilterShader->Use();
		downfilterShader->SetInt("u_SourceTexture", 0);
		downfilterShader->SetVec2("u_TexelOffset", glm::vec2(1.0f / (float)m_HalfResolutionFramebuffer->GetConfiguration().Width,
			1.0f / (float)m_HalfResolutionFramebuffer->GetConfiguration().Height));

		glBindVertexArray(m_PostProcessingVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_HalfResolutionFramebuffer->Unbind();

		m_BlurFramebuffer->Bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_HalfResolutionFramebuffer->GetColorAttachment());

		auto blurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Blur");
		blurShader->Use();
		blurShader->SetInt("u_SourceTexture", 0);
		blurShader->SetVec2("u_TexelOffset", glm::vec2(m_BlurWidth / (float)m_HalfResolutionFramebuffer->GetConfiguration().Width, 0.0f));

		glBindVertexArray(m_PostProcessingVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_BlurFramebuffer->Unbind();

		m_HalfResolutionFramebuffer->Bind();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BlurFramebuffer->GetColorAttachment());

		blurShader->Use();
		blurShader->SetInt("u_SourceTexture", 0);
		blurShader->SetVec2("u_TexelOffset", glm::vec2(0.0f, m_BlurWidth / (float)m_HalfResolutionFramebuffer->GetConfiguration().Height));

		glBindVertexArray(m_PostProcessingVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		m_HalfResolutionFramebuffer->Unbind();
	}

	m_PostProcessingFramebuffer->Bind();
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_MainSceneFramebuffer->GetColorAttachment());

	if (m_Bloom)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_HalfResolutionFramebuffer->GetColorAttachment());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	auto postProcessingShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "PostProcessing");
	postProcessingShader->Use();
	postProcessingShader->SetInt("u_Screen", 0);
	postProcessingShader->SetBool("u_IsBloom", m_Bloom);
	postProcessingShader->SetFloat("u_BloomIntensity", m_BloomIntensity);
	if (m_Bloom)
		postProcessingShader->SetInt("u_BloomTexture", 1);

	postProcessingShader->SetFloat("u_Gamma", m_Gamma);
	postProcessingShader->SetFloat("u_Exposure", m_Exposure);

	glBindVertexArray(m_PostProcessingVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glBindVertexArray(0);
	m_PostProcessingFramebuffer->Unbind();
}

void Renderer::RenderShadowMap(Scene* scene, DirectionalLight* source)
{
	m_DirectionalLightShadowMapFramebuffer->Bind();

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();
	depthShader->SetMat4("u_LightSpace", source->GetLightSpace());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

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

	m_DirectionalLightShadowMapFramebuffer->Unbind();
}

void Renderer::RenderShadowMap(Scene* scene, PointLight* source)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_PointLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPoint");
	depthShader->Use();
	for (int i = 0; i < 6; i++)
		depthShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthShader->SetFloat("u_FarPlane", source->GetFarPlane());
	depthShader->SetVec3("u_LightPos", source->GetOwner()->GetWorldPosition());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

			for (auto mesh : smc->GetMeshes())
				mesh.Render();
		}
	}

	auto depthIstancedShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepthPointInstanced");
	depthIstancedShader->Use();
	for (int i = 0; i < 6; i++)
		depthIstancedShader->SetMat4("u_ShadowMatrices[" + std::to_string(i) + "]", source->GetLightViews().at(i));
	depthIstancedShader->SetFloat("u_FarPlane", source->GetFarPlane());
	depthIstancedShader->SetVec3("u_LightPos", source->GetOwner()->GetWorldPosition());
	
	for (auto c : scene->GetComponents<InstanceRenderedMeshComponent>())
	{
		auto irmc = Cast<InstanceRenderedMeshComponent>(c);
		for (auto mesh : irmc->GetMeshes())
			mesh.RenderInstanced(irmc->GetInstancesCount());
	}

	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderShadowMap(Scene* scene, SpotLight* source)
{
	glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, m_SpotLightShadowMapFramebufferObject);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, source->GetShadowMap(), 0);

	auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::CALCULATION, "SceneDepth");
	depthShader->Use();
	depthShader->SetMat4("u_LightSpace", source->GetLightSpace());

	glClear(GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_FRONT);

	for (auto e : scene->GetEntities())
	{
		if (auto smc = e->GetComponent<StaticMeshComponent>())
		{
			depthShader->SetMat4("u_Model", e->GetTransform().ModelMatrix);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderQuad()
{
	float vertices[] =
	{
		 1.0f,  1.0f, 0.0f,		1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,

		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
	};

	unsigned int vao, vbo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void Renderer::CreateShadowMapsPlaceholders()
{
	const uint32_t shadowWidth = 1024, shadowHeight = 1024;

	for (int i = 0; i < 16; i++)
	{
		glGenTextures(1, &m_PointLightShadowMapsPlaceholders[i]);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_PointLightShadowMapsPlaceholders[i]);
		for (int i = 0; i < 6; i++)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


		glGenTextures(1, &m_SpotLightShadowMapsPlaceholders[i]);
		glBindTexture(GL_TEXTURE_2D, m_SpotLightShadowMapsPlaceholders[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}


	
}
