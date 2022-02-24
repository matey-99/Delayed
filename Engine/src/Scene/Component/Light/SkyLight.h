#pragma once

#include <vector>
#include <string>

#include "Scene/Component/RenderComponent.h"
#include "Renderer/Shader.h"
#include "typedefs.h"

class SkyLight : public RenderComponent
{
private:
	std::string m_Path;
	bool m_SkyVisibility;
	float m_Intensity;

	uint32_t m_ID;
	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_IrradianceMap;
	uint32_t m_PrefilterMap;
	uint32_t m_BRDFLUT;

	uint32_t m_CaptureFBO;
	uint32_t m_CaptureRBO;

	Ref<Shader> m_Shader;

public:
	SkyLight(Entity* entity, std::string path);
	~SkyLight();

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Load(std::string path);

	inline std::string GetPath() const { return m_Path; }
	inline float GetIntensity() const { return m_Intensity; }
	inline uint32_t GetID() const { return m_ID; }
	inline unsigned int GetIrradianceMap() const { return m_IrradianceMap; }
	inline unsigned int GetPrefilterMap() const { return m_PrefilterMap; }
	inline unsigned int GetBRDFLUT() const { return m_BRDFLUT; }

private:
	void SetupMesh();

	friend class EntityDetailsPanel;
};