#include "SphereColliderComponent.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Component/PlayerComponent.h"
#include "Scene/Component/TransformComponent.h"

SphereColliderComponent::SphereColliderComponent(Actor* owner)
	: ColliderComponent(owner)
{
	m_Center = glm::vec3(0.0f);
	m_Size = glm::vec3(1.0f);
}

void SphereColliderComponent::Start()
{
	if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		m_BoundingBox = BoundingBox(staticMesh->GetBoundingBox().Min * m_Size, staticMesh->GetBoundingBox().Max * m_Size);
	}
	else
	{
		m_BoundingBox = BoundingBox(-m_Size + m_Center, m_Size + m_Center);
	}

	m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void SphereColliderComponent::Update(float deltaTime)
{
	glm::vec3 deltaPosition = m_Owner->GetTransform()->GetWorldPosition() - m_OwnerLastPosition;
	m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();

	m_BoundingBox = BoundingBox(m_BoundingBox.Min + deltaPosition, m_BoundingBox.Max + deltaPosition);

	CheckCollisions();
}

void SphereColliderComponent::Destroy()
{
}

bool SphereColliderComponent::CheckCollisions()
{
	auto scene = m_Owner->GetScene();
	auto actors = scene->GetActors();
	for (auto actor : actors)
	{
		if (actor.get() == m_Owner)
			continue;

		if (auto collider = actor->GetComponent<SphereColliderComponent>())
		{
			if ((m_BoundingBox.Center.x - collider->GetBoundingBox().Center.x) < (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.x) &&
				(m_BoundingBox.Center.y - collider->GetBoundingBox().Center.y) < (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.y) &&
				(m_BoundingBox.Center.z - collider->GetBoundingBox().Center.z) < (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.z))
			{
				glm::vec3 d = m_BoundingBox.Center - collider->GetBoundingBox().Center;
				glm::vec3 v = glm::normalize(d) * (glm::length(collider->GetBoundingBox().Extents) + glm::length(m_BoundingBox.Extents) - glm::length(d)) * 0.5f;

				if (m_Owner->GetComponent<PlayerComponent>())
				{
					if (glm::abs(v.x) < glm::abs(v.y) && glm::abs(v.x) < glm::abs(v.z))
					{
						v.y = 0;
						v.z = 0;
					}
					else if (glm::abs(v.y) < glm::abs(v.x) && glm::abs(v.y) < glm::abs(v.z))
					{
						v.x = 0;
						v.z = 0;
					}
					else if (glm::abs(v.z) < glm::abs(v.x) && glm::abs(v.z) < glm::abs(v.y))
					{
						v.x = 0;
						v.y = 0;
					}

					m_Owner->GetTransform()->SetWorldPosition(m_Owner->GetTransform()->GetWorldPosition() + v);
				}
			}
		}
	}

	return false;
}
