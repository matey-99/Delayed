#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Platform.h"

class Block;

class BlockTrigger : public GameComponent
{
public:
	BlockTrigger(Actor* owner);
	~BlockTrigger();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) override;
	virtual void OnTriggerExit(Ref<ColliderComponent> other) override;

private:
	Ref<ColliderComponent> m_ConnectedBlockCollider;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};