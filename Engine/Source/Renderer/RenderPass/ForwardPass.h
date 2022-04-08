#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class ForwardPass
{
public:
	ForwardPass();
	~ForwardPass();

	void Render(Ref<Scene> scene);

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	Ref<RenderTarget> m_RenderTarget;
};