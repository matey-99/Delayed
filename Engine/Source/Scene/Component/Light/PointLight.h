#pragma once

#include "Light.h"

class PointLight : public Light
{
private:
	int m_Index;

	uint32_t m_ShadowMap;
	std::vector<glm::mat4> m_LightViews;
	float m_FarPlane;

public:
	PointLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~PointLight();

	virtual void Use() override;
	virtual void SwitchOff() override;

	virtual void RenderShadowMap() override;

	inline void SetIndex(int index) { m_Index = index; }

	inline uint32_t GetShadowMap() const { return m_ShadowMap; }
	inline std::vector<glm::mat4> GetLightViews() const { return m_LightViews; }
	inline float GetFarPlane() const { return m_FarPlane; }

	friend class EntityDetailsPanel;
};
