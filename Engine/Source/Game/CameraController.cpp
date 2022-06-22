#include "CameraController.h"

#include "Scene/Scene.h"
#include "Scene/Actor.h"
#include "Scene/Component/CameraComponent.h"
#include "Scene/Component/TransformComponent.h"
#include "Camera/CameraManager.h"
#include "Input/Input.h"
#include "Math/Math.h"
#include "Time/Time.h"

CameraController::CameraController(Actor* owner)
	: GameComponent(owner)
{
	m_CameraSpeed = 8.0f;
	m_FollowSpeed = 5.0f;
	m_LookUpLimit = 75.0f;
	m_LookDownLimit = -25.0f;
	m_GamepadRotationSensitivity = 20.0f;

	m_TurnRotation = 0.0f;
	m_LookUpRotation = 0.0f;
}

void CameraController::Start()
{
	auto input = Input::GetInstance();

	input->BindAxis("Player_Turn", &CameraController::Turn, this);
	input->BindAxis("Player_LookUp", &CameraController::LookUp, this);

	m_Target = m_Owner->GetScene()->GetComponent<TransformComponent>(m_TargetID);

	m_Camera = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraID);
	if (!m_Camera)
	{
		m_Owner->GetScene()->DestroyActor(m_Owner);
		return;
	}

	m_Pivot = (TransformComponent*)m_Camera->GetOwner()->GetTransform()->GetParent();
}

void CameraController::Update(float deltaTime)
{
	FollowTarget(m_FollowSpeed * Time::GetInstance()->GetFixedDeltaTime());

	// Turn Rotation
	glm::vec3 newRotation = m_Owner->GetTransform()->GetLocalRotation();
	newRotation.y -= m_TurnRotation * deltaTime;
	m_Owner->GetTransform()->SetLocalRotation(newRotation);

	// Look Up Rotation
	glm::vec3 newPivotRotation = m_Pivot->GetLocalRotation();
	newPivotRotation.x += m_LookUpRotation * deltaTime;
	newPivotRotation.x = glm::clamp(newPivotRotation.x, m_LookDownLimit, m_LookUpLimit);
	m_Pivot->SetLocalRotation(newPivotRotation);

	// Reset angles
	m_TurnRotation = 0.0f;
	m_LookUpRotation = 0.0f;
}

void CameraController::FixedUpdate()
{

}

void CameraController::FollowTarget(float speed)
{
	glm::vec3 currentPosition = m_Owner->GetTransform()->GetWorldPosition();
	glm::vec3 targetPosition = Math::Lerp(currentPosition, m_Target->GetWorldPosition(), speed);

	m_Owner->GetTransform()->SetWorldPosition(targetPosition);
}

void CameraController::Turn(float value)
{
	auto input = Input::GetInstance()->GetCurrentInputType();
	if (input == PlayerInputType::Gamepad)
		value *= m_GamepadRotationSensitivity;

	m_TurnRotation += value * m_CameraSpeed;
}

void CameraController::LookUp(float value)
{
	auto input = Input::GetInstance()->GetCurrentInputType();
	if (input == PlayerInputType::Gamepad)
		value *= m_GamepadRotationSensitivity;

	m_LookUpRotation += value * m_CameraSpeed;
}
