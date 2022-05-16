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

	virtual void OnTriggerEnter(ColliderComponent* other) {};
	virtual void OnTriggerStay(ColliderComponent* other) {};
	virtual void OnTriggerExit(ColliderComponent* other) {};
};