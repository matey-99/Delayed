#include "Light.h"

#include "Scene/Scene.h"

Light::Light(Actor* owner, Ref<UniformBuffer> vertexUniformBuffer, Ref<UniformBuffer> fragmentUniformBuffer)
	: RenderComponent(owner), m_VertexUniformBuffer(vertexUniformBuffer), m_FragmentUniformBuffer(fragmentUniformBuffer)
{
	m_Color = glm::vec3(1.0f);
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
	if (m_Owner->GetScene()->IsChangedSinceLastFrame())
	{
		Use();

		if (m_ShadowsEnabled)
			RenderShadowMap();
	}
}

void Light::Render()
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
