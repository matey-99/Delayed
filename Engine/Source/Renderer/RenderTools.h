#pragma once

#include "Patterns/Singleton.h"
#include "Math/BoundingBox.h"
#include "Math/BoundingSphere.h"

class RenderTools : public Singleton<RenderTools>
{
public:
	RenderTools();

	void RenderQuad();
	void RenderSkybox();
	void RenderBoundingBox(BoundingBox box);
	void RenderBoundingSphere(BoundingSphere sphere);

	static uint32_t GenerateSimpleNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat);
	static uint32_t GenerateWorleyNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat);
	static uint32_t GeneratePerlinNoiseTexture(uint64_t seed, uint32_t width, uint32_t height, uint32_t depth, uint32_t internalFormat);

private:
	void Initialize();

private:
	uint32_t m_QuadVAO;
	uint32_t m_SkyboxVAO;
	uint32_t m_BoxVAO;
	uint32_t m_SphereVAO;
};