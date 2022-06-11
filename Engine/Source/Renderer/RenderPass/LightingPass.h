#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class LightingPass
{
public:
	struct LightingSettings
	{
		// Shadows
		int ShadowsPCFSize = 1;
		float ShadowsBias = 0.0f;
	};

public:
	LightingPass();
	~LightingPass();

	void Render(Ref<Scene> scene);

	inline const LightingSettings& GetSettings() const { return m_Settings; }
	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	LightingSettings m_Settings;
	Ref<RenderTarget> m_RenderTarget;

	friend class RendererSettingsPanel;
};