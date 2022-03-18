#include "DirectionalLight.h"

#include "Renderer/UniformBuffer.h"
#include "Camera/CameraManager.h"
#include <glm/gtc/type_ptr.hpp>
#include "Material/ShaderLibrary.h"
#include "Scene/Scene.h"

DirectionalLight::DirectionalLight(Actor* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: Light(owner, vertexUniformBuffer, fragmentUniformBuffer)
{
}

DirectionalLight::~DirectionalLight()
{
	SwitchOff();
}

void DirectionalLight::Use()
{
	glm::vec3 direction = m_Owner->GetTransform()->GetForward();

	uint32_t offset = GLSL_DIRECTIONAL_LIGHT_OFFSET;
	m_FragmentUniformBuffer->Bind();
	m_FragmentUniformBuffer->SetUniform(offset, sizeof(glm::vec3), glm::value_ptr(direction));
	m_FragmentUniformBuffer->SetUniform(offset + GLSL_VEC3_SIZE, sizeof(glm::vec3), glm::value_ptr(m_Color));
	m_FragmentUniformBuffer->Unbind();

	auto camera = CameraManager::GetInstance()->GetMainCamera();

	m_ShadowCascadeLevels = { camera->GetFarClipPlane() / 50.0f, camera->GetFarClipPlane() / 25.0f, camera->GetFarClipPlane() / 10.0f, camera->GetFarClipPlane() / 2.0f };
	
	m_LightSpaceMatrices.clear();
	for (int i = 0; i < m_ShadowCascadeLevels.size() + 1; i++)
	{
		if (i == 0)
			m_LightSpaceMatrices.push_back(CalculateLightSpaceMatrix(camera->GetNearClipPlane(), m_ShadowCascadeLevels[i]));
		else if (i < m_ShadowCascadeLevels.size())
			m_LightSpaceMatrices.push_back(CalculateLightSpaceMatrix(m_ShadowCascadeLevels[i - 1], m_ShadowCascadeLevels[i]));
		else
			m_LightSpaceMatrices.push_back(CalculateLightSpaceMatrix(m_ShadowCascadeLevels[i - 1], camera->GetFarClipPlane()));
	}

	m_VertexUniformBuffer->Bind();

	for (int i = 0; i < m_LightSpaceMatrices.size(); i++)
		m_VertexUniformBuffer->SetUniform(i * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_LightSpaceMatrices[i]));

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

std::vector<glm::vec4> DirectionalLight::GetFrustumCornersWorldSpace(const glm::mat4& projection, const glm::mat4& view)
{
	auto inversed = glm::inverse(projection * view);

	std::vector<glm::vec4> corners;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				glm::vec4 point = inversed * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
				corners.push_back(point / point.w);
			}
		}
	}
	return corners;
}

glm::mat4 DirectionalLight::CalculateLightSpaceMatrix(float nearClipPlane, float farClipPlane)
{
	auto camera = CameraManager::GetInstance()->GetMainCamera();

	// LIGHT VIEW MATRIX
	glm::vec3 center = glm::vec3(0.0f);
	glm::vec2 aspectRatio = camera->GetAspectRatio();
	glm::mat4 projection = glm::perspective(glm::radians(camera->GetFieldOfView()), aspectRatio.x / aspectRatio.y, nearClipPlane, farClipPlane);
	auto corners = GetFrustumCornersWorldSpace(projection, camera->GetViewMatrix());
	for (auto corner : corners)
		center += glm::vec3(corner);
	center /= corners.size();

	glm::mat4 lightView = glm::lookAt(center - m_Owner->GetTransform()->GetForward(), center, glm::vec3(0.0f, 1.0f, 0.0f));

	// LIGHT PROJECTION MATRIX
	glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
	for (auto corner : corners)
	{
		auto temp = lightView * corner;
		min = { glm::min(min.x, temp.x), glm::min(min.y, temp.y), glm::min(min.z, temp.z) };
		max = { glm::max(max.x, temp.x), glm::max(max.y, temp.y), glm::max(max.z, temp.z) };
	}

	const float zMultiplier = 10.0f;
	if (min.z < 0)
		min.z *= zMultiplier;
	else
		min.z /= zMultiplier;

	if (max.z < 0)
		max.z /= zMultiplier;
	else
		max.z *= zMultiplier;

	glm::mat4 lightProjection = glm::ortho(min.x, max.x, min.y, max.y, min.z, max.z);

	return lightProjection * lightView;
}
