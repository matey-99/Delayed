#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Platform.h"
#include "Patterns/Singleton.h"

class Checkpoint : public GameComponent
{
public:
	Checkpoint(Actor* owner);
	~Checkpoint();

	virtual void Start() override;

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) override;

#pragma region Serialization

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};