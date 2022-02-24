#include "PointLight.h"

#include <glm/gtc/type_ptr.hpp>

#include "Scene/Scene.h"
#include "Renderer/UniformBuffer.h"

PointLight::PointLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
	m_Index = owner->GetScene()->GetComponentsCount<PointLight>();

	for (int i = 0; i < 6; i++)
		m_LightViews.push_back(glm::mat4(0.0f));

	m_FarPlane = 250.0f;

	const uint32_t shadowWidth = 1024, shadowHeight = 1024;

	glGenTextures(1, &m_ShadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ShadowMap);

	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

PointLight::~PointLight()
{
	SwitchOff();
}

void PointLight::Use()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(m_Owner->GetWorldPosition()));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2) - GLSL_SCALAR_SIZE, sizeof(bool), &m_ShadowsEnabled);
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2), sizeof(float), &m_FarPlane);
	m_FragmentUniformBuffer->Unbind();

	glm::mat4 lightProjection = glm::perspective(glm::radians(90.0f), 1.0f, 1.0f, m_FarPlane);
	m_LightViews.at(0) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(1) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(2) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0)));
	m_LightViews.at(3) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0)));
	m_LightViews.at(4) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0)));
	m_LightViews.at(5) = (lightProjection * glm::lookAt(m_Owner->GetWorldPosition(), m_Owner->GetWorldPosition() + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0)));
}

void PointLight::SwitchOff()
{
	uint32_t offset = GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * m_Index);
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->Unbind();
}

void PointLight::RenderShadowMap()
{
	Renderer::GetInstance()->RenderShadowMap(m_Owner->GetScene(), this);
}
