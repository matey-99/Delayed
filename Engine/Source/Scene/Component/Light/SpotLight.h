#pragma once

#include "Light.h"

class SpotLight : public Light
{
private:
	int m_Index;

	float m_InnerCutOff;
	float m_OuterCutOff;

	uint32_t m_ShadowMap;
	float m_FarPlane;

public:
	SpotLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);
	~SpotLight();

	virtual void Use() override;
	virtual void SwitchOff() override;

	virtual void RenderShadowMap() override;

	inline void SetIndex(int index) { m_Index = index; }

	inline float GetInnerCutOff() const { return m_InnerCutOff; }
	inline float GetOuterCutOff() const { return m_OuterCutOff; }
	inline uint32_t GetShadowMap() const { return m_ShadowMap; }
	inline float GetFarPlane() const { return m_FarPlane; }

	void SetInnerCutOff(float innerCutOff);
	void SetOuterCutOff(float outerCutOff);

	friend class EntityDetailsPanel;
};
