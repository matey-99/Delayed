#pragma once

#include "Core.h"

#include "GameComponent.h"

class ImageComponent;
class TextComponent;
class Interactable;
class Texture;

class ControllerBasedIcon : public GameComponent
{
public:
	ControllerBasedIcon(Actor* owner);
	~ControllerBasedIcon();

	virtual void Start() override;

	void ChangeIcon();

private:
	Ref<ImageComponent> m_IconComponent;

	Ref<Texture> m_KeyboardIcon;
	Ref<Texture> m_GamepadIcon;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};