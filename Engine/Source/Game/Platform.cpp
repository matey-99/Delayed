#include "Platform.h"

#include "Scene/Actor.h"
#include "Scene/Component/TransformComponent.h"
#include "Math/Math.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Material/MaterialInstance.h"
#include "Button.h"
#include "Scene/Component/AudioSourceComponent.h"

Platform::Platform(Actor* owner)
	: GameComponent(owner)
{
	m_Active = false;
	m_Direction = glm::vec3(0.0f, 1.0f, 0.0f);
	m_DefaultPosition = glm::vec3(0.0f);
	m_Distance = 5.0f;
	m_Speed = 5.0f;
}

Platform::~Platform()
{
}

void Platform::Start()
{
	m_DefaultPosition = m_Owner->GetTransform()->GetWorldPosition();

	m_AudioSource = m_Owner->GetComponent<AudioSourceComponent>();

	if (auto mesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		Ref<Material> material = mesh->GetMaterials()[0];
		m_Material = MaterialInstance::Create(material);

		mesh->SetMaterial(0, m_Material);
	}
}

void Platform::Update(float deltaTime)
{
	float step = m_Speed * deltaTime;
	glm::vec3 currentPosition = m_Owner->GetTransform()->GetWorldPosition();

	if (ShouldBeActive())
	{
		if (!Math::IsNearlyEqual(currentPosition, m_DefaultPosition + (m_Direction * m_Distance), step))
		{
			if (m_Material)
			{
				float currentEmissive = m_Material->GetFloatParameter("u_Material.emissiveStrength");
				m_Material->SetFloatParameter("u_Material.emissiveStrength", currentEmissive + step);
			}

			glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
			newPosition += m_Direction * step;
			m_Owner->GetTransform()->SetLocalPosition(newPosition);
		}
	}
	else
	{
		if (!Math::IsNearlyEqual(currentPosition, m_DefaultPosition, step))
		{
			if (m_Material)
			{
				float currentEmissive = m_Material->GetFloatParameter("u_Material.emissiveStrength");
				m_Material->SetFloatParameter("u_Material.emissiveStrength", currentEmissive - step);
			}

			glm::vec3 newPosition = m_Owner->GetTransform()->GetLocalPosition();
			newPosition -= m_Direction * step;
			m_Owner->GetTransform()->SetLocalPosition(newPosition);
		}
	}
}

void Platform::FixedUpdate()
{
}

void Platform::AddButton(Button* button)
{
	m_Buttons.push_back(button);
}

bool Platform::ShouldBeActive()
{
	bool result = true;
	for (auto& button : m_Buttons)
	{
		if (!button->IsPressed())
			result = false;
	}

	return result;
}
