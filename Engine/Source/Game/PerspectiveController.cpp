#include "PerspectiveController.h"

#include "Input/Input.h"
#include "Time/Time.h"
#include "Scene/Actor.h"
#include "Ghost.h"
#include "Footsteps.h"
#include "Scene/Component/CameraComponent.h"
#include "Camera/CameraManager.h"
#include "Player.h"
#include "TPPPlayer.h"

PerspectiveController* PerspectiveController::s_Instance{};

PerspectiveController::PerspectiveController(Actor* owner)
	: GameComponent(owner)
{
	m_CanChangePerspective = true;
	m_CanChangePerspective_Gamepad = true;
	m_CurrentPerspective = PerspectiveType::FPP;
}

void PerspectiveController::Start()
{
	if (!s_Instance)
		s_Instance = this;

	auto input = Input::GetInstance();
	input->BindAction("Perspective", InputEvent::Press, &PerspectiveController::ChangePerspective, this);
	input->BindAction("Perspective", InputEvent::Release, &PerspectiveController::AllowChangePerspective, this);

	input->BindAction("Perspective_Gamepad", InputEvent::Press, &PerspectiveController::ChangePerspective_Gamepad, this);
	input->BindAction("Perspective_Gamepad", InputEvent::Release, &PerspectiveController::AllowChangePerspective_Gamepad, this);

	m_Ghost = m_Owner->GetScene()->FindComponent<Ghost>();

	m_PlayerFPP = m_Owner->GetScene()->FindActor(m_PlayerFPPID);
	m_CameraFPP = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraFPPID);

	m_PlayerTPP = m_Owner->GetScene()->FindActor(m_PlayerTPPID);
	m_CameraTPP = m_Owner->GetScene()->GetComponent<CameraComponent>(m_CameraTPPID);
	m_CameraController = m_Owner->GetScene()->FindActor(m_CameraControllerID);
}

void PerspectiveController::ChangePerspective()
{
	if (!m_CanChangePerspective)
		return;

	if (m_CurrentPerspective == PerspectiveType::FPP)
		SetTPP();
	else if (m_CurrentPerspective == PerspectiveType::TPP)
		SetFPP();

	m_CanChangePerspective = false;
}

void PerspectiveController::AllowChangePerspective()
{
	m_CanChangePerspective = true;
}

void PerspectiveController::ChangePerspective_Gamepad()
{
	if (!m_CanChangePerspective_Gamepad)
		return;

	if (m_CurrentPerspective == PerspectiveType::FPP)
		SetTPP();
	else if (m_CurrentPerspective == PerspectiveType::TPP)
		SetFPP();

	m_CanChangePerspective_Gamepad = false;
}

void PerspectiveController::AllowChangePerspective_Gamepad()
{
	m_CanChangePerspective_Gamepad = true;
}

void PerspectiveController::SetFPP()
{
	m_CurrentPerspective = PerspectiveType::FPP;

	m_PlayerTPP->SetEnabled(false);
	m_CameraController->SetEnabled(false);

	m_Ghost->SetPlayerActor(m_PlayerFPP);

	m_PlayerFPP->GetTransform()->SetWorldPosition(m_PlayerTPP->GetTransform()->GetWorldPosition());
	//m_PlayerFPP->GetTransform()->SetLocalRotation(m_PlayerTPP->GetTransform()->GetLocalRotation());

	CameraManager::GetInstance()->SetMainCamera(m_CameraFPP);

	m_PlayerFPP->SetEnabled(true);
}

void PerspectiveController::SetTPP()
{
	m_CurrentPerspective = PerspectiveType::TPP;

	m_PlayerFPP->SetEnabled(false);

	m_Ghost->SetPlayerActor(m_PlayerTPP);

	m_PlayerTPP->GetTransform()->SetWorldPosition(m_PlayerFPP->GetTransform()->GetWorldPosition());
	m_CameraController->GetTransform()->SetWorldPosition(m_PlayerFPP->GetTransform()->GetWorldPosition());
	//m_PlayerTPP->GetTransform()->SetLocalRotation(m_PlayerFPP->GetTransform()->GetLocalRotation());

	CameraManager::GetInstance()->SetMainCamera(m_CameraTPP);

	m_PlayerTPP->SetEnabled(true);
	m_CameraController->SetEnabled(true);
}
