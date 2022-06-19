#include "InteractionPanel.h"

#include "Scene/Actor.h"
#include "Scene/Scene.h"
#include "Scene/Component/UI/TextComponent.h"
#include "Scene/Component/UI/ImageComponent.h"
#include "Spaceship.h"
#include "PickableSpaceshipPart.h"

InteractionPanel::InteractionPanel(Actor* owner)
	: GameComponent(owner)
{
}

InteractionPanel::~InteractionPanel()
{
}

void InteractionPanel::Start()
{
	m_InteractionText = m_Owner->GetTransform()->GetChildren()[0]->GetOwner()->GetComponent<TextComponent>();
	m_InteractionIcon = m_Owner->GetTransform()->GetChildren()[1]->GetOwner()->GetComponent<ImageComponent>();
}

void InteractionPanel::UpdatePanel(Ref<Interactable> interactable)
{
	if (auto spaceship = Cast<Spaceship>(interactable))
	{
		m_InteractionText->SetText("Fix spaceship parts");
	}
	else if (auto pickableSpaceshipPart = Cast<PickableSpaceshipPart>(interactable))
	{
		m_InteractionText->SetText("Pick up spaceship part");
	}
}
