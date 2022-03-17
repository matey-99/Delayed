#pragma once

#include "Renderer/Framebuffer.h"
#include "Scene/Scene.h"

class GBufferPass
{
public:
	GBufferPass();
	~GBufferPass();

	void Render(Ref<Scene> scene);

private:
	uint32_t m_Framebuffer;

public: // TEMP
	uint32_t m_GBuffer0;
	uint32_t m_GBuffer1;
	uint32_t m_GBuffer2;
	uint32_t m_GBuffer3;
};