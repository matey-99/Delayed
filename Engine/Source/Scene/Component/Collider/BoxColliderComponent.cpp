#include "BoxColliderComponent.h"

#include "Scene/Actor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Scene.h"
#include "Scene/Component/PlayerComponent.h"
#include "Scene/Component/TransformComponent.h"


BoxColliderComponent::BoxColliderComponent(Actor* owner)
	: ColliderComponent(owner)
{
	m_Center = glm::vec3(0.0f);
	m_Size = glm::vec3(1.0f);
}

void BoxColliderComponent::Start()
{
	m_Owner->GetTransform()->OnTransformChanged.Add(&BoxColliderComponent::UpdateBoundingBox, this);

	UpdateBoundingBox();

	m_OwnerLastPosition = m_Owner->GetTransform()->GetWorldPosition();
}

void BoxColliderComponent::Update(float deltaTime)
{
	CheckCollisions();
}

void BoxColliderComponent::Destroy()
{
}

void BoxColliderComponent::UpdateBoundingBox()
{
	if (auto staticMesh = m_Owner->GetComponent<StaticMeshComponent>())
	{
		m_BoundingBox = BoundingBox(staticMesh->GetBoundingBox().Min * m_Size, staticMesh->GetBoundingBox().Max * m_Size);
	}
	else
	{
		auto temp = BoundingBox(-m_Size + m_Center, m_Size + m_Center);
		auto points = temp.GetPoints();

		for (auto& point : points)
			point = m_Owner->GetTransform()->GetWorldModelMatrix() * glm::vec4(point, 1.0f);

		m_BoundingBox = BoundingBox(points);
	}
}

bool BoxColliderComponent::CheckCollisions()
{
	auto scene = m_Owner->GetScene();
	auto actors = scene->GetActors();
	for (auto actor : actors)
	{
		if (actor.get() == m_Owner)
			continue;

		if (auto collider = actor->GetComponent<BoxColliderComponent>())
		{
			if ((glm::abs(m_BoundingBox.Center.x - collider->GetBoundingBox().Center.x) < (m_BoundingBox.Extents.x + collider->GetBoundingBox().Extents.x)) &&
				(glm::abs(m_BoundingBox.Center.y - collider->GetBoundingBox().Center.y) < (m_BoundingBox.Extents.y + collider->GetBoundingBox().Extents.y)) &&
				(glm::abs(m_BoundingBox.Center.z - collider->GetBoundingBox().Center.z) < (m_BoundingBox.Extents.z + collider->GetBoundingBox().Extents.z)))
			{
				float left = m_BoundingBox.Max.x - collider->GetBoundingBox().Min.x;
				float right = collider->GetBoundingBox().Max.x - m_BoundingBox.Min.x;
				float bottom = m_BoundingBox.Max.y - collider->GetBoundingBox().Min.y;
				float top = collider->GetBoundingBox().Max.y - m_BoundingBox.Min.y;
				float backward = m_BoundingBox.Max.z - collider->GetBoundingBox().Min.z;
				float forward = collider->GetBoundingBox().Max.z - m_BoundingBox.Min.z;

				glm::vec3 v;
				v.x = left < right ? -left : right;
				v.y = bottom < top ? -bottom : top;
				v.z = backward < forward ? -backward : forward;


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
