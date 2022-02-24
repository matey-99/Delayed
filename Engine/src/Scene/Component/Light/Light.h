#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Scene/Component/RenderComponent.h"
#include "Scene/Entity.h"
#include "Renderer/Shader.h"
#include "Scene/Camera.h"
#include "Renderer/UniformBuffer.h"

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

#define GLSL_SCALAR_SIZE 4
#define GLSL_VEC3_SIZE 16
#define GLSL_MAT4_SIZE 64
#define GLSL_DIRECTIONAL_LIGHT_SIZE 32
#define GLSL_POINT_LIGHT_SIZE 48
#define GLSL_SPOT_LIGHT_SIZE 64

#define GLSL_LIGHTS_COUNTS_OFFSET 0
#define GLSL_DIRECTIONAL_LIGHT_OFFSET 16
#define GLSL_POINT_LIGHTS_OFFSET (GLSL_DIRECTIONAL_LIGHT_OFFSET + GLSL_DIRECTIONAL_LIGHT_SIZE)
#define GLSL_SPOT_LIGHTS_OFFSET (GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS))

class UniformBuffer;

class Light : public RenderComponent
{
public:
	Light(Entity* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);

	virtual void Begin() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void Destroy() override;

	virtual void Use() = 0;
	virtual void SwitchOff() = 0;

	virtual void RenderShadowMap() = 0;

	inline Entity* GetOwner() const { return m_Owner; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline glm::mat4 GetLightSpace() const { return m_LightSpace; }

	void SetColor(glm::vec3 color);

	friend class EntityDetailsPanel;

protected:
	Ref<UniformBuffer> m_VertexUniformBuffer;
	Ref<UniformBuffer> m_FragmentUniformBuffer;

	glm::vec3 m_Color;

	bool m_ShadowsEnabled;
	glm::mat4 m_LightSpace;
	uint32_t m_ShadowMap;
};
