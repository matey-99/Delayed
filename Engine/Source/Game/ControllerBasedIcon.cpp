#include "ControllerBasedIcon.h"

#include "Input/Input.h"
#include "Scene/Actor.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Assets/AssetManager.h"

ControllerBasedIcon::ControllerBasedIcon(Actor* owner)
	: GameComponent(owner)
{
	m_KeyboardIcon = AssetManager::LoadTexture("Textures/Default/UI.png");
	m_GamepadIcon = AssetManager::LoadTexture("Textures/Default/UI_Black.png");
}

ControllerBasedIcon::~ControllerBasedIcon()
{
}

void ControllerBasedIcon::Start()
{
	m_IconComponent = m_Owner->GetComponent<ImageComponent>();

	Input::GetInstance()->OnPlayerInputTypeChanged.Add(&ControllerBasedIcon::ChangeIcon, this);
}

void ControllerBasedIcon::ChangeIcon()
{
	switch (Input::GetInstance()->GetCurrentInputType())
	{
	case PlayerInputType::KeyboardAndMouse:
		m_IconComponent->SetImage(m_KeyboardIcon);
		break;
	case PlayerInputType::Gamepad:
		m_IconComponent->SetImage(m_GamepadIcon);
		break;
	}
}
