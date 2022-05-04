#include "ParticleEmitterBox.h"

ParticleEmitterBox::ParticleEmitterBox()
{
	m_Size = glm::vec3(1.0f);
}

glm::vec3 ParticleEmitterBox::GenerateParticlePosition(glm::vec3 emitterPosition)
{
	float x = emitterPosition.x + ((float)rand() / RAND_MAX) * m_Size.x;
	float y = emitterPosition.y + ((float)rand() / RAND_MAX) * m_Size.y;
	float z = emitterPosition.z + ((float)rand() / RAND_MAX) * m_Size.z;

	return glm::vec3(x, y, z);
}
