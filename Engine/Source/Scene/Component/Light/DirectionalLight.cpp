#include "DirectionalLight.h"

#include "Renderer/UniformBuffer.h"
#include <glm/gtc/type_ptr.hpp>
#include "Material/ShaderLibrary.h"
#include "Scene/Scene.h"

DirectionalLight::DirectionalLight(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
}

DirectionalLight::~DirectionalLight()
{
	SwitchOff();
}

void DirectionalLight::Use()
{
	glm::vec3 direction = glm::normalize(m_Owner->GetWorldRotation());

	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(direction));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_FragmentUniformBuffer->SetUniform(offset + (GLSL_VEC3_SIZE * 2) - GLSL_SCALAR_SIZE, sizeof(bool), &m_ShadowsEnabled);
	m_FragmentUniformBuffer->Unbind();

	glm::vec3 position = glm::clamp(direction * -10.0f, glm::vec3(0.0001f), glm::vec3(10000.0f));
	glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 5.0f, 20.0f);
	glm::mat4 lightView = glm::lookAt(position,
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	m_LightSpace = lightProjection * lightView;
	m_VertexUniformBuffer->Bind();
	m_VertexUniformBuffer->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(m_LightSpace));
	m_VertexUniformBuffer->Unbind();
}

void DirectionalLight::SwitchOff()
{
	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(glm::vec3(0.0f)));
	m_FragmentUniformBuffer->Unbind();
}

void DirectionalLight::RenderShadowMap()
{
	Renderer::GetInstance()->RenderShadowMap(m_Owner->GetScene(), this);
}
