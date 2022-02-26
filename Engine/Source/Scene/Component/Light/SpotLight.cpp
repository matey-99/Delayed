#include "SpotLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"

SpotLight::SpotLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
	m_Index = owner->GetScene()->GetComponentsCount<SpotLight>();

	m_InnerCutOff = 0.91f;
	m_OuterCutOff = 0.82f;

	m_FarPlane = 250.0f;

	const uint32_t shadowWidth = 1024, shadowHeight = 1024;

	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

SpotLight::~SpotLight()
{
	SwitchOff();
}

void SpotLight::Use()
{
	glm::vec3 direction = glm::normalize(m_Owner->GetWorldRotation());

	uint32_t offset = GLSL_SPOT_LIGHTS_OFFSET + (GLSL_SPOT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Owner->GetWorldPosition()));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(direction));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3) - GLSL_SCALAR_SIZE, sizeof(float), &m_InnerCutOff);
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(float), &m_OuterCutOff);
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3) + GLSL_SCALAR_SIZE, sizeof(bool), &m_ShadowsEnabled);
	m_FragmentUniformBuffer->Unbind();

	glm::vec3 position = m_Owner->GetWorldPosition();
	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, m_FarPlane);
	glm::mat4 lightView = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));

	m_LightSpace = lightProjection * lightView;

	uint32_t vertexOffset = GLSL_MAT4_SIZE + (GLSL_MAT4_SIZE * m_Index);
	m_VertexUniformBuffer->Bind();
	m_VertexUniformBuffer->SetUniform(vertexOffset, sizeof(glm::mat4), glm::value_ptr(m_LightSpace));
	m_VertexUniformBuffer->Unbind();
}

void SpotLight::SwitchOff()
{
	uint32_t offset = GLSL_SPOT_LIGHTS_OFFSET + (GLSL_SPOT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3) - GLSL_SCALAR_SIZE, sizeof(float), (void*)0);
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 3), sizeof(float), (void*)0);
	m_FragmentUniformBuffer->Unbind();
}

void SpotLight::RenderShadowMap()
{
	Renderer::GetInstance()->RenderShadowMap(m_Owner->GetScene(), this);
}

void SpotLight::SetInnerCutOff(float innerCutOff)
{
	m_InnerCutOff = innerCutOff;

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}

void SpotLight::SetOuterCutOff(float outerCutOff)
{
	m_OuterCutOff = outerCutOff;

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}
