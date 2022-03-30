#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class UIPass
{
private:
	struct UISettings
	{
		float Gamma = 2.2f;
		float Exposure = 1.0f;
	};

public:
	UIPass();
	~UIPass();

	void Render(Ref<Scene> scene, uint32_t input);
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline const UISettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	UISettings m_Settings;

	Ref<RenderTarget> m_RenderTarget;

	friend class RendererSettingsPanel;
};