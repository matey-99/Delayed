#pragma once

#include "Renderer/RenderTarget.h"

class DepthOfFieldPass
{
private:
	struct DepthOfFieldSettings
	{
		float MinDistance = 1.5f;
		float MaxDistance = 64;
		int Size = 2;
	};

public:
	DepthOfFieldPass();
	~DepthOfFieldPass();

	void Render(uint32_t input);
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline const DepthOfFieldSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetFinalRenderTarget() const { return m_FinalRenderTarget; }

private:
	Ref<RenderTarget> m_BlurRenderTarget;
	Ref<RenderTarget> m_FinalRenderTarget;

	DepthOfFieldSettings m_Settings;

	friend class RendererSettingsPanel;
};