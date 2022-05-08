#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Platform.h"
#include "Patterns/Singleton.h"

class Block : public GameComponent
{
public:
	Block(Actor* owner);
	~Block();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) override;
	virtual void OnTriggerExit(Ref<ColliderComponent> other) override;

private:
	std::vector<Ref<Block>> m_ConnectedButtons;
	bool m_IsIn;

#pragma region Serialization


#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};