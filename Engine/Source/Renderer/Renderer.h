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
class SSAOPass;
class PostProcessingPass;
class UniformBuffer;
class Scene;
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
	Ref<PostProcessingPass> m_PostProcessingPass;

	enum class RenderOutputType
	{
		Normal, PostProcessing
	};

private:
	Ref<UniformBuffer> m_CameraVertexUniformBuffer;
	Ref<UniformBuffer> m_CameraFragmentUniformBuffer;

	RenderOutputType m_RenderOutputType;

	uint32_t m_WindowWidth;
	uint32_t m_WindowHeight;

public:
	Renderer();
	~Renderer();

	void Initialize();

	void Render(Ref<Scene> scene);

	uint32_t GetOutput();
	void ResizeWindow(uint32_t width, uint32_t height);

	inline Ref<UniformBuffer> GetCameraVertexUniformBuffer() const { return m_CameraVertexUniformBuffer; }
	inline Ref<UniformBuffer> GetCameraFragmentUniformBuffer() const { return m_CameraFragmentUniformBuffer; }

	inline uint32_t GetWindowWidth() const { return m_WindowWidth; }
	inline uint32_t GetWindowHeight() const { return m_WindowHeight; }

	friend class RendererSettingsPanel;
};