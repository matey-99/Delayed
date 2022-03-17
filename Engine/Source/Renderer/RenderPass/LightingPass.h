#pragma once

#include "Renderer/Framebuffer.h"
#include "Scene/Scene.h"

class LightingPass
{
public:
	LightingPass();
	~LightingPass();

	void Render();

private:
	void InitializeQuad();

public:
	uint32_t m_LightingTexture;

private:
	uint32_t m_Framebuffer;

	uint32_t m_QuadVAO;
	uint32_t m_QuadVBO;
};