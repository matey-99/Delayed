#pragma once

#include "Renderer/RenderTarget.h"

class SSRPass
{
private:
	struct SSRSettings
	{
		float RayStep = 0.1;
		float MinRayStep = 0.1;
		float ReflectionSpecularFalloffExponent = 3.0;
		int	  MaxSteps = 30;
		int   NumBinarySearchSteps = 5;
	};

public:
	SSRPass();
	~SSRPass();

	void Render(uint32_t input);
	void UpdateRenderTargets(uint32_t width, uint32_t height);

	inline const SSRSettings& GetSettings() const { return m_Settings; }

	inline Ref<RenderTarget> GetRenderTarget() const { return m_RenderTarget; }

private:
	Ref<RenderTarget> m_RenderTarget;

	SSRSettings m_Settings;


	friend class SceneSerializer;
	friend class RendererSettingsPanel;
};