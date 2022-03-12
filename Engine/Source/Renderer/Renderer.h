#pragma once

#include <mutex>
#include <glm/glm.hpp>
#include <iostream>
#include <glad/glad.h>

#define CHECK_OPENGL_ERRORS()	while (GLenum error = glGetError()) \
								{ std::cout << "OpenGL Error: " << error << std::endl; __debugbreak(); }
					
#define GAUSSIAN_BLUR_RADIUS 6
#define GAUSSIAN_BLUR_KERNEL_SIZE (GAUSSIAN_BLUR_RADIUS * 2 + 1)

class Framebuffer;
class UniformBuffer;
class Scene;
class DirectionalLight;
class PointLight;
class SpotLight;

class Renderer
{
public:
	uint32_t m_DepthMap;
	uint32_t m_DepthCubemap;
private:
	static Ref<Renderer> s_Instance;
	static std::mutex s_Mutex;

	Ref<Framebuffer> m_MainSceneFramebuffer;
	Ref<Framebuffer> m_PostProcessingFramebuffer;
	Ref<Framebuffer> m_ThresholdFramebuffer;
	Ref<Framebuffer> m_ScaleFramebuffers[2];
	Ref<Framebuffer> m_BlurFramebuffer;

	Ref<Framebuffer> m_DirectionalLightShadowMapFramebuffer;
	Ref<Framebuffer> m_PointLightShadowMapFramebuffer;
	Ref<Framebuffer> m_SpotLightShadowMapFramebuffer;

	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;

	uint32_t m_PointLightShadowMapFramebufferObject;
	uint32_t m_SpotLightShadowMapFramebufferObject;

	uint32_t m_PostProcessingVAO;
	uint32_t m_PostProcessingVBO;

	uint32_t m_PointLightShadowMapsPlaceholders[16];
	uint32_t m_SpotLightShadowMapsPlaceholders[16];

	bool m_PostProcessing;
	
	bool m_Bloom;
	float m_BloomIntensity;
	float m_BloomThreshold;
	float m_BloomLimit;
	float m_BlurSigma;

	float m_Gamma;
	float m_Exposure;

	glm::vec2 m_GaussianBlurHorizontalCache[GAUSSIAN_BLUR_KERNEL_SIZE];
	glm::vec2 m_GaussianBlurVerticalCache[GAUSSIAN_BLUR_KERNEL_SIZE];

public:
	Renderer();
	~Renderer();

	Renderer(Renderer& other) = delete;
	void operator=(const Renderer&) = delete;

	static Ref<Renderer> GetInstance();

	void Initialize();

	void InitializeMainSceneFramebuffer();
	void InitializePostProcessingFramebuffer();
	void InitializeShadowMapFramebuffers();

	void InitializePostProcessing();

	void RenderScene(Ref<Scene> scene);
	void AddPostProcessingEffects();

	void RenderShadowMap(Scene* scene, DirectionalLight* source);
	void RenderShadowMap(Scene* scene, PointLight* source);
	void RenderShadowMap(Scene* scene, SpotLight* source);

	void RenderQuad();

	inline Ref<Framebuffer> GetMainSceneFramebuffer() const { return m_MainSceneFramebuffer; }
	inline Ref<Framebuffer> GetPostProcessingFramebuffer() const { return m_PostProcessingFramebuffer; }

	inline Ref<Framebuffer> GetThresholdFramebuffer() const { return m_ThresholdFramebuffer; }
	inline Ref<Framebuffer> GetScaleFramebuffers(int index) const { return m_ScaleFramebuffers[index]; }
	inline Ref<Framebuffer> GetBlurFramebuffer() const { return m_BlurFramebuffer; }

	inline Ref<Framebuffer> GetDirectionalLightShadowMapFramebuffer() const { return m_DirectionalLightShadowMapFramebuffer; }
	inline Ref<Framebuffer> GetPointLightShadowMapFramebuffer() const { return m_PointLightShadowMapFramebuffer; }
	inline Ref<Framebuffer> GetSpotLightShadowMapFramebuffer() const { return m_SpotLightShadowMapFramebuffer; }

	inline Ref<UniformBuffer> GetCameraVertexUniformBuffer() const { return m_CameraVertexUniformBuffer; }
	inline Ref<UniformBuffer> GetCameraFragmentUniformBuffer() const { return m_CameraFragmentUniformBuffer; }

	inline uint32_t GetPostProcessingVAO() const { return m_PostProcessingVAO; }

	inline uint32_t GetPointLightShadowMapPlaceholder(int index) const { return m_PointLightShadowMapsPlaceholders[index]; }
	inline uint32_t GetSpotLightShadowMapPlaceholder(int index) const { return m_SpotLightShadowMapsPlaceholders[index]; }


	inline bool IsPostProcessing() const { return m_PostProcessing; }

private:
	void CreateShadowMapsPlaceholders();

	void ComputeGaussianBlurKernel(float sigma, float width, float height);

	friend class RendererSettingsPanel;
};