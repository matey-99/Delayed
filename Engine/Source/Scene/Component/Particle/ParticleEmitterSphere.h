#pragma once

#include "ParticleEmitterShape.h"

class ParticleEmitterSphere : public ParticleEmitterShape
{
public:
	ParticleEmitterSphere();

	virtual glm::vec3 GenerateParticlePosition(glm::vec3 emitterPosition) override;

	inline float GetRadius() const { return m_Radius; }
	inline void SetRadius(float radius) { m_Radius = radius; }

private:
	float m_Radius;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};