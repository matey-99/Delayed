#pragma once

#include "Core.h"

#include "GameComponent.h"

class ImageComponent;
class TextComponent;
class Interactable;

class InteractionPanel : public GameComponent
{
public:
	InteractionPanel(Actor* owner);
	~InteractionPanel();

	virtual void Start() override;

	void UpdatePanel(Ref<Interactable> interactable);

private:
	Ref<ImageComponent> m_InteractionIcon;
	Ref<TextComponent> m_InteractionText;

	friend class SceneSerializer;
};