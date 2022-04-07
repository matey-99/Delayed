#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Patterns/Singleton.h"

class Button : public GameComponent
{
public:
	Button(Actor* owner);
	~Button();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) override;
	virtual void OnTriggerStay(Ref<ColliderComponent> other) override;
	virtual void OnTriggerExit(Ref<ColliderComponent> other) override;

private:
	Ref<Actor> m_Platform;
	std::vector<Ref<Button>> m_ConnectedButtons;

	bool m_IsPressed;

#pragma region Serialization

	glm::vec3 m_PlatformRelativeStartPosition;
	glm::vec3 m_PlatformRelativeEndPosition;

	float m_Speed;

	uint64_t m_PlatformID;
	std::vector<uint64_t> m_ConnectedButtonsIDs;

#pragma endregion

	friend class SceneSerializer;
};