#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Platform.h"
#include "Patterns/Singleton.h"

class Material;

class Button : public GameComponent
{
public:
	Button(Actor* owner);
	~Button();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void OnTriggerEnter(Ref<ColliderComponent> other) override;
	virtual void OnTriggerExit(Ref<ColliderComponent> other) override;

private:
	void Handle();
	
	void Press();
	void Release();

private:
	Ref<Platform> m_Platform;
	std::vector<Ref<Button>> m_ConnectedButtons;
	int m_TriggeringActorsCount;

	Ref<Material> m_NormalMaterial;
	Ref<Material> m_PressedMaterial;

	bool m_IsPressed;

#pragma region Serialization

	uint64_t m_PlatformID;
	std::vector<uint64_t> m_ConnectedButtonsIDs;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};