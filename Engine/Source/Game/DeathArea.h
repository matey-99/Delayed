#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"

class DeathArea : public GameComponent
{
public:
	DeathArea(Actor* owner);
	~DeathArea();

	virtual void Start() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

private:

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};