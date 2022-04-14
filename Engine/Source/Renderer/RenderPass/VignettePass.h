#pragma once

#include "Renderer/RenderTarget.h"

class VignettePass
{
private:
	struct VignetteSettings
	{
		float Intensity = 15.0f;
		float Size = 0.25f;
	};

public:
	VignettePass();
	~VignettePass();

	void Render(uint32_t input);
	void UpdateRenderTarget(uint32_t width, uint32_t height);

	inline const VignetteSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	VignetteSettings m_Settings;

	Ref<RenderTarget> m_RenderTarget;

	friend class RendererSettingsPanel;
};