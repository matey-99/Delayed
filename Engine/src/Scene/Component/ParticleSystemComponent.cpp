#include "ParticleSystemComponent.h"
#include <GLFW/glfw3.h>
#include "Material/ShaderLibrary.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"

ParticleSystemComponent::ParticleSystemComponent(Entity* owner)
	: RenderComponent(owner)
{
	m_ParticlesCount = 10000;
	m_Radius = 2.0f;
	m_MinVelocity = glm::vec3(0.0f, 0.1f, 0.0f);
	m_MaxVelocity = glm::vec3(0.0f, 0.5f, 0.0f);
	m_ParticleLifeTime = 3.0f;
	m_ParticlesLifeTimeCounter = 0.0f;

	Reset();
}

void ParticleSystemComponent::Begin()
{
}

void ParticleSystemComponent::Update()
{
	m_ParticlesLifeTimeCounter += 0.016f;
	if (m_ParticlesLifeTimeCounter > m_ParticleLifeTime)
	{
		m_ParticlesLifeTimeCounter = 0.0f;

		m_PositionBuffer->Bind();
		glm::vec4* positions = m_PositionBuffer->Map();
		for (size_t i = 0; i < m_ParticlesCount; i++)
		{
			glm::vec3 center = m_Owner->GetWorldPosition();

			float u = ((float)rand() / RAND_MAX) * m_Radius;
			float v = ((float)rand() / RAND_MAX) * m_Radius;
			float theta = 2.0f * glm::pi<float>() * u;
			float phi = acos(2.0f * v - 1.0f);
			float r = cbrt((float)rand() / RAND_MAX) * m_Radius;
			float sinTheta = sin(theta);
			float cosTheta = cos(theta);
			float sinPhi = sin(phi);
			float cosPhi = cos(phi);
			float x = center.x + r * sinPhi * cosTheta;
			float y = center.y + r * sinPhi * sinTheta;
			float z = center.z + r * cosPhi;

			positions[i] = glm::vec4(x, y, z, 1.0f);
		}
		m_PositionBuffer->Unmap();
		m_PositionBuffer->Unbind();
	}


	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_VelocityBuffer->GetID());

	m_ComputeShader->Use();

	glDispatchCompute(m_ParticlesCount / 128, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleSystemComponent::PreRender()
{
}

void ParticleSystemComponent::Render()
{
	auto particleShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PARTICLE, "StandardParticle");
	particleShader->Use();
	particleShader->SetMat4("u_Model", m_Owner->GetTransform().ModelMatrix);
	particleShader->SetMat4("u_View", m_Owner->GetScene()->GetCamera()->GetViewMatrix());
	particleShader->SetMat4("u_Projection", m_Owner->GetScene()->GetCamera()->GetProjectionMatrix());
	particleShader->SetVec2("u_SpriteSize", glm::vec2(0.03f, 0.03f));
	particleShader->SetBool("u_IsSprite", false);
	particleShader->SetInt("u_Sprite", 0);
	particleShader->SetVec4("u_Color", glm::vec4(0.4f, 6.0f, 7.0f, std::clamp((m_ParticleLifeTime - m_ParticlesLifeTimeCounter), 0.0f, 1.0f)));

	glDisable(GL_CULL_FACE);


	glBindVertexArray(m_ParticleVAO);

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_PositionBuffer->GetID());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer->GetID());

	glDrawElements(GL_TRIANGLES, m_ParticlesCount * 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

	glBindVertexArray(0);

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
}

void ParticleSystemComponent::Destroy()
{
}

void ParticleSystemComponent::Reset()
{
	m_PositionBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_VelocityBuffer = CreateRef<ShaderStorageBuffer<glm::vec4>>(m_ParticlesCount);
	m_IndexBuffer = CreateRef<ShaderStorageBuffer<uint32_t>>(m_ParticlesCount * 6);

	m_PositionBuffer->Bind();
	glm::vec4* positions = m_PositionBuffer->Map();
	for (size_t i = 0; i < m_ParticlesCount; i++)
	{
		glm::vec3 center = m_Owner->GetWorldPosition();

		float u = ((float)rand() / RAND_MAX) * m_Radius;
		float v = ((float)rand() / RAND_MAX) * m_Radius;
		float theta = 2.0f * glm::pi<float>() * u;
		float phi = acos(2.0f * v - 1.0f);
		float r = cbrt((float)rand() / RAND_MAX) * m_Radius;
		float sinTheta = sin(theta);
		float cosTheta = cos(theta);
		float sinPhi = sin(phi);
		float cosPhi = cos(phi);
		float x = center.x + r * sinPhi * cosTheta;
		float y = center.y + r * sinPhi * sinTheta;
		float z = center.z + r * cosPhi;

		positions[i] = glm::vec4(x, y, z, 1.0f);
	}
	m_PositionBuffer->Unmap();
	m_PositionBuffer->Unbind();

	m_VelocityBuffer->Bind();
	glm::vec4* velocities = m_VelocityBuffer->Map();
	for (size_t i = 0; i < m_ParticlesCount; i++)
	{
		float x = (float)rand() / (RAND_MAX / (m_MaxVelocity.x - m_MinVelocity.x));
		float y = (float)rand() / (RAND_MAX / (m_MaxVelocity.y - m_MinVelocity.y));
		float z = (float)rand() / (RAND_MAX / (m_MaxVelocity.z - m_MinVelocity.z));

		velocities[i] = glm::vec4(x, y, z, 1.0f);
	}
	m_VelocityBuffer->Unmap();
	m_VelocityBuffer->Unbind();

	m_IndexBuffer->Bind();
	uint32_t* indices = m_IndexBuffer->Map();
	for (size_t i = 0; i < m_ParticlesCount; i++)
	{
		size_t index = i << 2;
		*(indices++) = index;
		*(indices++) = index + 1;
		*(indices++) = index + 2;
		*(indices++) = index;
		*(indices++) = index + 2;
		*(indices++) = index + 3;
	}
	m_IndexBuffer->Unmap();
	m_IndexBuffer->Unbind();

	m_ComputeShader = CreateRef<ComputeShader>("res/shaders/Particle/StandardParticle.comp");
	m_ComputeShader->Use();
	m_ComputeShader->SetUint("u_ParticlesCount", m_ParticlesCount);

	if (m_ParticleVAO)
	{
		glDeleteVertexArrays(1, &m_ParticleVAO);
	}

	glGenVertexArrays(1, &m_ParticleVAO);
}

void ParticleSystemComponent::SetParticlesCount(uint32_t count)
{
	m_ParticlesCount = count;
	Reset();
}

void ParticleSystemComponent::SetParticleLifeTime(float lifeTime)
{
	m_ParticleLifeTime = lifeTime;
	Reset();
}

void ParticleSystemComponent::SetRadius(float radius)
{
	m_Radius = radius;
	Reset();
}

void ParticleSystemComponent::SetMinVelocity(glm::vec3 minVelocity)
{
	m_MinVelocity = minVelocity;
	Reset();
}

void ParticleSystemComponent::SetMaxVelocity(glm::vec3 maxVelocity)
{
	m_MaxVelocity = maxVelocity;
	Reset();
}
