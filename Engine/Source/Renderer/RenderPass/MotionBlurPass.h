#pragma once

#include "Renderer/RenderTarget.h"

class MotionBlurPass
{
private:
	struct MotionBlurSettings
	{
		//glm::vec2 Resolution = glm::vec2(1920, 1080);

		// Basic
		//bool BloomEnabled = true;
		//float Saturation = 1.0f;
		int Size = 2;
		float Separation = 2.f;


	};

public:
	MotionBlurPass();
	~MotionBlurPass();

	void Render(uint32_t input);
	void UpdateRenderTarget(uint32_t width, uint32_t height);

	inline const MotionBlurSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	MotionBlurSettings m_Settings;

	Ref<RenderTarget> m_RenderTarget;

	friend class SceneSerializer;
	friend class RendererSettingsPanel;
};