#include "Light.h"

#include "Scene/Scene.h"

Light::Light(Actor* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: RenderComponent(owner), m_VertexUniformBuffer(vertexUniformBuffer), m_FragmentUniformBuffer(fragmentUniformBuffer)
{
	m_Color = glm::vec3(1.0f);
	m_Intensity = 1.0f;
	m_ShadowsEnabled = true;

	owner->GetScene()->SetChangedSinceLastFrame(true);
}

void Light::Start()
{
}

void Light::Update(float deltaTime)
{
	
}

void Light::PreRender()
{
	Use();
}

void Light::Render(Material::BlendMode blendMode)
{
}

void Light::Destroy()
{
	SwitchOff();
}

void Light::SetColor(glm::vec3 color)
{
	m_Color = color;

	m_Owner->GetScene()->SetChangedSinceLastFrame(true);
}
