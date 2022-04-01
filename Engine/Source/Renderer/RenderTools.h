#pragma once

#include "Patterns/Singleton.h"
#include "Math/BoundingBox.h"

class RenderTools : public Singleton<RenderTools>
{
public:
	RenderTools();

	void RenderQuad();
	void RenderBoundingBox(BoundingBox box);
	void RenderSphere();

private:
	void Initialize();

private:
	uint32_t m_QuadVAO;
	uint32_t m_BoxVAO;
	uint32_t m_SphereVAO;
};