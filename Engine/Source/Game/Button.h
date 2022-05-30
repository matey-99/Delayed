#pragma once

#include "Core.h"

#include "GameComponent.h"
#include "Scene/Component/Collider/ColliderComponent.h"
#include "Platform.h"

class MaterialInstance;
class AudioSourceComponent;

class Button : public GameComponent
{
public:
	Button(Actor* owner);
	~Button();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Destroy() override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;
	virtual void OnTriggerExit(ColliderComponent* other) override;

	inline bool IsPressed() const { return m_IsPressed; }

private:
	void Handle();
	
	void Press();
	void Release();

private:
	Ref<Platform> m_Platform;
	std::vector<Ref<Button>> m_ConnectedButtons;
	int m_TriggeringActorsCount;

	float m_EmissionStrength;
	float m_EmissionTime;
	float m_EmissionTimer;

	Ref<MaterialInstance> m_Material;
	Ref<AudioSourceComponent> m_AudioSource;

	bool m_IsPressed;

#pragma region Serialization

	uint64_t m_PlatformID;
	std::vector<uint64_t> m_ConnectedButtonsIDs;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};