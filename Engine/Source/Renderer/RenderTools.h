#pragma once

#include "Patterns/Singleton.h"

class RenderTools : public Singleton<RenderTools>
{
public:
	RenderTools();

	void RenderQuad();

private:
	void Initialize();

private:
	uint32_t m_QuadVAO;
};