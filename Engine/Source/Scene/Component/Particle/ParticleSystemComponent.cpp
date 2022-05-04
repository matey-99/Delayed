#include "ParticleSystemComponent.h"

#include "Assets/AssetManager.h"
#include "Material/ShaderLibrary.h"
#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Math/Math.h"

#include "ParticleEmitterSphere.h"
#include "ParticleEmitterBox.h"

ParticleSystemComponent::ParticleSystemComponent(Actor* owner)
	: RenderComponent(owner)
{
	m_SimulationTime = 0.0f;
	m_EmissionTime = 0.0f;
	m_LastUsedParticleIndex = 0;

	m_Duration = 5.0f;
	m_Looping = true;
	m_Sprite = AssetManager::LoadTexture("Textures/Default/Particle.png");
	m_EmissionRateOverTime = 10.0f;
	m_EmitterShape = CreateRef<ParticleEmitterSphere>();
	m_MaxParticles = 100;
	m_MinParticleSize = 0.1f;
	m_MaxParticleSize = 0.3f;
	m_EndParticleSize = 0.0f;
	m_MinParticleVelocity = glm::vec3(0.0f, 0.1f, 0.0f);
	m_MaxParticleVelocity = glm::vec3(0.0f, 0.5f, 0.0f);
	m_EndParticleVelocity = glm::vec3(0.0f, 0.0f, 0.5f);
	m_MinParticleLifeTime = 1.0f;
	m_MaxParticleLifeTime = 2.0f;
	m_StartParticleColor = glm::vec4(1.0f);
	m_EndParticleColor = glm::vec4(0.0f);

	m_Shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Particle, "Particle");

	Initialize();
	Reset();
}

void ParticleSystemComponent::Start()
{
}

void ParticleSystemComponent::Update(float deltaTime)
{
	m_SimulationTime += deltaTime;

	bool shouldEmitParticles = true;
	if (m_SimulationTime > m_Duration)
	{
		if (m_Looping)
			m_SimulationTime = 0.0f;
		else
			shouldEmitParticles = false;
	}

	if (shouldEmitParticles)
	{
		m_EmissionTime += deltaTime;
		if (m_EmissionTime > 1.0f / m_EmissionRateOverTime)
		{
			int index = FindUnusedParticle();

			if (index != -1)
				EmitParticle(index);

			m_EmissionTime = 0.0f;
		}
	}

	for (int i = 0; i < m_MaxParticles; i++)
	{
		Particle& particle = m_Particles[i];
		if (particle.CurrentLifeTime > 0.0f)
		{
			particle.CurrentVelocity = Math::Lerp(particle.CurrentVelocity, m_EndParticleVelocity, 1 / particle.InitialLifeTime * deltaTime);
			particle.Position += particle.CurrentVelocity * deltaTime;
			particle.CurrentSize = Math::Lerp(particle.CurrentSize, m_EndParticleSize, 1 / particle.InitialLifeTime * deltaTime);
			particle.Color = Math::Lerp(particle.Color, m_EndParticleColor, 1 / particle.InitialLifeTime * deltaTime);
			particle.CurrentLifeTime -= deltaTime;

			m_ParticlesPositions[i] = glm::vec4(particle.Position, particle.CurrentSize);
			m_ParticlesColors[i] = particle.Color;
		}
		else
		{
			m_ParticlesPositions[i] = glm::vec4(0.0f);
			m_ParticlesColors[i] = glm::vec4(0.0f);
		}
	}
}

void ParticleSystemComponent::PreRender()
{
}

void ParticleSystemComponent::Render(Material::BlendMode blendMode)
{
	// Render particles only in Forward Pass
	if (blendMode == Material::BlendMode::Opaque)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_MaxParticles, &m_ParticlesPositions[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * m_MaxParticles, &m_ParticlesColors[0], GL_DYNAMIC_DRAW);

	glDisable(GL_CULL_FACE);

	m_Sprite->Bind(0);

	m_Shader->Use();
	m_Shader->SetInt("u_Sprite", 0);

	glBindVertexArray(m_VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, m_MaxParticles);
	glBindVertexArray(0);

	m_Sprite->Unbind();

	glEnable(GL_CULL_FACE);
}

void ParticleSystemComponent::Destroy()
{
}

void ParticleSystemComponent::Initialize()
{
	float quadVertices[] =
	{
		-1.0f,  1.0f, 0.0f,		0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 0.0f,

		-1.0f,  1.0f, 0.0f, 	0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 	1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 	1.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_PositionVBO);
	glGenBuffers(1, &m_ColorVBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionVBO);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(2, 1);

	glBindBuffer(GL_ARRAY_BUFFER, m_ColorVBO);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(3, 1);

	glBindVertexArray(0);
}

void ParticleSystemComponent::Reset()
{
	for (int i = 0; i < m_MaxParticles; i++)
	{
		m_Particles.push_back(ParticleSystemComponent::Particle());
		m_ParticlesPositions.push_back(glm::vec4(0.0f));
		m_ParticlesColors.push_back(glm::vec4(0.0f));
	}
}

void ParticleSystemComponent::ChangeSprite(std::string path)
{
	m_Sprite = AssetManager::LoadTexture(path);
}

void ParticleSystemComponent::SetMaxParticles(uint32_t count)
{
	m_MaxParticles = count;
	Reset();
}

int ParticleSystemComponent::FindUnusedParticle()
{
	for (int i = m_LastUsedParticleIndex; i < m_MaxParticles; i++)
	{
		if (m_Particles[i].CurrentLifeTime <= 0.0f)
		{
			m_LastUsedParticleIndex = i;
			return i;
		}
	}

	for (int i = 0; i < m_LastUsedParticleIndex; i++)
	{
		if (m_Particles[i].CurrentLifeTime <= 0.0f)
		{
			m_LastUsedParticleIndex = i;
			return i;
		}
	}

	return -1;
}

void ParticleSystemComponent::EmitParticle(int index)
{
	glm::vec3 emitterPosition = m_Owner->GetTransform()->GetWorldPosition();

	Particle particle;
	particle.Position = m_EmitterShape->GenerateParticlePosition(emitterPosition);

	float velocityX = m_MinParticleVelocity.x + ((float)rand() / (RAND_MAX / (m_MaxParticleVelocity.x - m_MinParticleVelocity.x)));
	float velocityY = m_MinParticleVelocity.y + ((float)rand() / (RAND_MAX / (m_MaxParticleVelocity.y - m_MinParticleVelocity.y)));
	float velocityZ = m_MinParticleVelocity.z + ((float)rand() / (RAND_MAX / (m_MaxParticleVelocity.z - m_MinParticleVelocity.z)));
	particle.InitialVelocity = glm::vec3(velocityX, velocityY, velocityZ);
	particle.CurrentVelocity = particle.InitialVelocity;

	particle.Color = m_StartParticleColor;

	particle.InitialSize = m_MinParticleSize + ((float)rand() / (RAND_MAX / (m_MaxParticleSize - m_MinParticleSize)));
	particle.CurrentSize = particle.InitialSize;

	particle.InitialLifeTime = m_MinParticleLifeTime + ((float)rand() / (RAND_MAX / (m_MaxParticleLifeTime - m_MinParticleLifeTime)));
	particle.CurrentLifeTime = particle.InitialLifeTime;

	m_Particles[index] = particle;
	m_ParticlesPositions[index] = glm::vec4(particle.Position, particle.CurrentSize);
	m_ParticlesColors[index] = particle.Color;
}
