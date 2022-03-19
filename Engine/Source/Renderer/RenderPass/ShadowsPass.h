#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class ShadowsPass
{
public:
	ShadowsPass();
	~ShadowsPass();

	void Render(Ref<Scene> scene);

	inline Ref<RenderTarget> GetDirectionalLightRenderTarget() const { return m_DirectionalLightRenderTarget; }

private:
	Ref<RenderTarget> m_DirectionalLightRenderTarget;
};