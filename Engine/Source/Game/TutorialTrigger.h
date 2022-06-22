#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Saveable.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "TutorialManager.h"

class TutorialTrigger : public GameComponent, public Saveable
{
public:
	TutorialTrigger(Actor* owner);
	~TutorialTrigger();

	virtual void Start() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

private:
	TutorialType m_Type;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};