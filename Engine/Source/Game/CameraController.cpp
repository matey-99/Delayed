#include "CameraController.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"

CameraController::CameraController(Actor* owner)
	: GameComponent(owner)
{
}

void CameraController::Start()
{
	m_Camera = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraID);
	m_Pivot = m_Owner->GetScene()->FindActor(m_PivotID);
}

void CameraController::Update(float deltaTime)
{
}
