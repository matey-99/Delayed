#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class UIPass
{
public:
	UIPass();
	~UIPass();

	void Render(Ref<Scene> scene, uint32_t input);
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	Ref<RenderTarget> m_RenderTarget;

	friend class RendererSettingsPanel;
};