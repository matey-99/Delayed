#pragma once

#include "Core.h"

class ParticleEmitterShape
{
public:
	virtual glm::vec3 GenerateParticlePosition(glm::vec3 emitterPosition) = 0;
};