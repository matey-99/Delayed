#pragma once

#include "Renderer/RenderTarget.h"

#define GAUSSIAN_BLUR_RADIUS 6
#define GAUSSIAN_BLUR_KERNEL_SIZE (GAUSSIAN_BLUR_RADIUS * 2 + 1)

class PostProcessingPass
{
private:
	struct PostProcessingSettings
	{
		//glm::vec2 Resolution = glm::vec2(1920, 1080);

		// Basic
		//float Gamma = 2.2f;
		float Gamma = 0.0f;
		float Gain = 0.0f;
		float Lift = 0.0f;
		float Offset = 0.0f;
		float Exposure = 1.0f;
		float Contrast = 1.0f;
		float ContrastPivot = 0.5f;


		// Bloom
		bool BloomEnabled = true;

		float BloomThreshold = 2.0f;
		float BloomLimit = 20.0f;
		float BloomIntensity = 0.8f;
		float BloomBlurSigma = 3.6f;

		// Lens Flare

		// Color Grading
		float Saturation = 1.0f;
		float Temperature = 66.0f;
		float Hue = 180.0f;
	};

public:
	PostProcessingPass();
	~PostProcessingPass();

	void Render(uint32_t input);
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline const PostProcessingSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetMainRenderTarget() const { return m_MainRenderTarget; }

private:
	void ComputeGaussianBlurKernel(float sigma, float width, float height);

private:
	PostProcessingSettings m_Settings;

	Ref<RenderTarget> m_MainRenderTarget;

	// BLOOM
	Ref<RenderTarget> m_ThresholdRenderTarget;
	Ref<RenderTarget> m_DownscaleRenderTargets[2];
	Ref<RenderTarget> m_BlurRenderTargets[2];
	Ref<RenderTarget> m_UpscaleRenderTargets[2];

	glm::vec2 m_GaussianBlurHorizontalCache[GAUSSIAN_BLUR_KERNEL_SIZE];
	glm::vec2 m_GaussianBlurVerticalCache[GAUSSIAN_BLUR_KERNEL_SIZE];

	friend class SceneSerializer;
	friend class RendererSettingsPanel;
};