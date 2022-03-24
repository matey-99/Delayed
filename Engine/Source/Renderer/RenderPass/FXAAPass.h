#pragma once

#include "Renderer/RenderTarget.h"

class FXAAPass
{
private:
	struct FXAASettings
	{
		// TO DO
	};

public:
	FXAAPass();
	~FXAAPass();

	void Render(uint32_t input);
	void UpdateRenderTarget(uint32_t width, uint32_t height);

	inline const FXAASettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	FXAASettings m_Settings;

	Ref<RenderTarget> m_RenderTarget;

	friend class RendererSettingsPanel;
};