#include "GhostPathElement.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Game/Ghost.h"
#include "Scene/Component/StaticMeshComponent.h"

GhostPathElement::GhostPathElement(Actor* owner)
	: GameComponent(owner)
{
	m_LifeTime = GHOST_POSITIONS_COUNT / 60.0f;
	m_LifeTimer = 0.0f;

	auto staticMesh = m_Owner->AddComponent<StaticMeshComponent>();
	staticMesh->ChangeMesh("Models/defaults/default_sphere.obj");
	staticMesh->ChangeMaterial(0, "Materials/M_Ghost.mat");
}

void GhostPathElement::Start()
{
}

void GhostPathElement::Update(float deltaTime)
{
	if (m_LifeTimer >= m_LifeTime)
	{
		m_Owner->GetScene()->DestroyActor(m_Owner);
		return;
	}
	else
		m_LifeTimer += deltaTime;
}
