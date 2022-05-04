#include "ParticleEmitterSphere.h"

ParticleEmitterSphere::ParticleEmitterSphere()
{
    m_Radius = 1.0f;
}

glm::vec3 ParticleEmitterSphere::GenerateParticlePosition(glm::vec3 emitterPosition)
{
	float u = ((float)rand() / RAND_MAX) * m_Radius;
	float v = ((float)rand() / RAND_MAX) * m_Radius;
	float theta = 2.0f * glm::pi<float>() * u;
	float phi = acos(2.0f * v - 1.0f);
	float r = cbrt((float)rand() / RAND_MAX) * m_Radius;
	float sinTheta = sin(theta);
	float cosTheta = cos(theta);
	float sinPhi = sin(phi);
	float cosPhi = cos(phi);
	float x = emitterPosition.x + r * sinPhi * cosTheta;
	float y = emitterPosition.y + r * sinPhi * sinTheta;
	float z = emitterPosition.z + r * cosPhi;

	return glm::vec3(x, y, z);
}
