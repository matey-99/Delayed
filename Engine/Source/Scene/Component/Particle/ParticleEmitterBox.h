#pragma once

#include "ParticleEmitterShape.h"

class ParticleEmitterBox : public ParticleEmitterShape
{
public:
	ParticleEmitterBox();

	virtual glm::vec3 GenerateParticlePosition(glm::vec3 emitterPosition) override;

	inline glm::vec3 GetSize() const { return m_Size; }
	inline void SetSize(glm::vec3 size) { m_Size = size; }

private:
	glm::vec3 m_Size;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};