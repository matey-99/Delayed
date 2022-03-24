#pragma once

#include "Renderer/RenderTarget.h"

class SSAOPass
{
private:
	struct SSAOSettings
	{
		float Intensity = 1.0f;
		int KernelSize = 64;
		float Radius = 0.5f;
		float Bias = 0.025f;
	};

public:
	SSAOPass();
	~SSAOPass();

	void Render();
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline const SSAOSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetFinalRenderTarget() const { return m_FinalRenderTarget; }

private:
	Ref<RenderTarget> m_FirstRenderTarget;
	Ref<RenderTarget> m_FinalRenderTarget;

	SSAOSettings m_Settings;

	std::vector<glm::vec3> m_SSAOKernel;
	std::vector<glm::vec3> m_SSAONoise;

	uint32_t m_NoiseTexture;

	friend class RendererSettingsPanel;
};