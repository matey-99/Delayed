#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class GBufferPass
{
public:
	GBufferPass();
	~GBufferPass();

	void Render(Ref<Scene> scene);

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	Ref<RenderTarget> m_RenderTarget;
};