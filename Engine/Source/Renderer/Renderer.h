#pragma once

#include <mutex>
#include <glm/glm.hpp>
#include <iostream>
#include <glad/glad.h>

#include "Patterns/Singleton.h"

#define CHECK_OPENGL_ERRORS()	while (GLenum error = glGetError()) \
								{ std::cout << "OpenGL Error: " << error << std::endl; __debugbreak(); }
					
#define GAUSSIAN_BLUR_RADIUS 6
#define GAUSSIAN_BLUR_KERNEL_SIZE (GAUSSIAN_BLUR_RADIUS * 2 + 1)

class GBufferPass;
class LightingPass;
class ShadowsPass;
class ForwardPass;
class SSAOPass;
class PostProcessingPass;
class FXAAPass;
class VignettePass;
class DepthOfFieldPass;
class UIPass;
class DepthFogPass;
class UniformBuffer;
class Scene;
class Camera;
class DirectionalLight;
class PointLight;
class SpotLight;

class Renderer : public Singleton<Renderer>
{
public:
	Ref<GBufferPass> m_GBufferPass;
	Ref<ShadowsPass> m_ShadowsPass;
	Ref<SSAOPass> m_SSAOPass;
	Ref<LightingPass> m_LightingPass;
	Ref<ForwardPass> m_ForwardPass;
	Ref<PostProcessingPass> m_PostProcessingPass;
	Ref<FXAAPass> m_FXAAPass;
	Ref<VignettePass> m_VignettePass;
	Ref<DepthOfFieldPass> m_DepthOfFieldPass;
	Ref<UIPass> m_UIPass;
    Ref<DepthFogPass> m_DepthFogPass;

	struct RendererSettings
	{
		bool PostProcessingEnabled = true;
		bool FXAAEnabled = true;
		bool VignetteEnabled = true;
		bool DepthOfFieldEnabled = true;
	};

private:
	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;

	RendererSettings m_Settings;

	uint32_t m_WindowWidth;
	uint32_t m_WindowHeight;

	uint32_t m_Output[2];

public:
	Renderer();
	~Renderer();

	void Initialize();

	void Render(Ref<Scene> scene, Ref<Camera> camera, uint32_t outputIndex = 0);
	void Display();

	uint32_t GetOutput(uint32_t index = 0);
	void ResizeWindow(uint32_t width, uint32_t height);

	inline const RendererSettings& GetSettings() const { return m_Settings; }
	inline void SetSettings(const RendererSettings& settings) { m_Settings = settings; }

	inline Ref<UniformBuffer> GetCameraVertexUniformBuffer() const { return m_CameraVertexUniformBuffer; }
	inline Ref<UniformBuffer> GetCameraFragmentUniformBuffer() const { return m_CameraFragmentUniformBuffer; }

	inline uint32_t GetWindowWidth() const { return m_WindowWidth; }
	inline uint32_t GetWindowHeight() const { return m_WindowHeight; }

	friend class RendererSettingsPanel;
};