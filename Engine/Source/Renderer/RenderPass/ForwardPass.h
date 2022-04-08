#pragma once

#include "Renderer/RenderTarget.h"
#include "Scene/Scene.h"

class ForwardPass
{
public:
	ForwardPass();
	~ForwardPass();

	void Render(Ref<Scene> scene);
};