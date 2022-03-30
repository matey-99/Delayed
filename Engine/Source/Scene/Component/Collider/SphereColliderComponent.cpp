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
	m_Size = 1.0f;
}

void SphereColliderComponent::Start()
{
	if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		m_BoundingSphere = BoundingSphere(staticMesh->GetBoundingSphere().Center, staticMesh->GetBoundingSphere().Radius * m_Size);
	}
	else
	{
        m_BoundingSphere = BoundingSphere(m_Center, m_Size);
	}

	m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void SphereColliderComponent::Update(float deltaTime)
{
	glm::vec3 deltaPosition = m_Owner->GetTransform()->GetWorldPosition() - m_OwnerLastPosition;
	m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();

	m_BoundingSphere = BoundingSphere(m_BoundingSphere.Center + deltaPosition, m_BoundingSphere.Radius);

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
            glm::vec3 d = m_BoundingSphere.Center - collider->GetBoundingSphere().Center;
			if (glm::length(d) < (collider->GetBoundingSphere().Radius + m_BoundingSphere.Radius))
			{
				glm::vec3 v = glm::normalize(d) * (collider->GetBoundingSphere().Radius + m_BoundingSphere.Radius - glm::length(d));

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
