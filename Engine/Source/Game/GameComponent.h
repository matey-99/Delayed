#pragma once

#include "Scene/Component/Component.h"
#include "Scene/Component/Collider/ColliderComponent.h"

class GameComponent : public Component
{
public:
	GameComponent(Actor* owner) 
		: Component(owner) {};

	virtual void Start() override {};
	virtual void Update(float deltaTime) override {};
	virtual void FixedUpdate() override {};
	virtual void Destroy() override {};

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) {};
	virtual void OnTriggerExit(Ref<ColliderComponent> other) {};
};