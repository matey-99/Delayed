#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class LightingPass
{
public:
	LightingPass();
	~LightingPass();

	void Render(Ref<Scene> scene);

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	Ref<RenderTarget> m_RenderTarget;
};