#pragma once

#include <string>
#include <glm/glm.hpp>

#include "Scene/Component/RenderComponent.h"
#include "Scene/Actor.h"
#include "Renderer/Shader.h"
#include "Renderer/UniformBuffer.h"

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

#define GLSL_SCALAR_SIZE 4
#define GLSL_VEC3_SIZE 16
#define GLSL_MAT4_SIZE 64

#define GLSL_DIRECTIONAL_LIGHT_SIZE 32
#define GLSL_POINT_LIGHT_SIZE 48
#define GLSL_SPOT_LIGHT_SIZE 80

#define GLSL_LIGHTS_COUNTS_OFFSET 0
#define GLSL_DIRECTIONAL_LIGHT_OFFSET 16
#define GLSL_POINT_LIGHTS_OFFSET (GLSL_DIRECTIONAL_LIGHT_OFFSET + GLSL_DIRECTIONAL_LIGHT_SIZE)
#define GLSL_SPOT_LIGHTS_OFFSET (GLSL_POINT_LIGHTS_OFFSET + (GLSL_POINT_LIGHT_SIZE * MAX_POINT_LIGHTS))

class UniformBuffer;

class Light : public RenderComponent
{
public:
	Light(Actor* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void PreRender() override;
	virtual void Render(Material::BlendMode blendMode) override;
	virtual void Destroy() override;

	virtual void Use() = 0;
	virtual void SwitchOff() = 0;

	inline Actor* GetOwner() const { return m_Owner; }
	inline glm::vec3 GetColor() const { return m_Color; }
	inline glm::mat4 GetLightSpace() const { return m_LightSpace; }
	inline bool IsCastingShadows() const { return m_CastShadows; }

	void SetColor(glm::vec3 color);

	friend class ActorDetailsPanel;

protected:
	Ref<UniformBuffer> m_VertexUniformBuffer;
	Ref<UniformBuffer> m_FragmentUniformBuffer;

	glm::vec3 m_Color;
	float m_Intensity;

	bool m_CastShadows;
	glm::mat4 m_LightSpace;
	uint32_t m_ShadowMap;
};
