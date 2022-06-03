#pragma once

#include "Core.h"

#include "Game/GameComponent.h"
#include "Game/Saveable.h"
#include "Scene/Component/Collider/ColliderComponent.h"

class TransformComponent;
class ParticleSystemComponent;
class ImageComponent;
class Player;

enum class ObeliskEffect
{
	Corrupt, Heal, GiveTeleportSkill
};

class Obelisk : public GameComponent, public Saveable
{
public:
	Obelisk(Actor* owner);
	~Obelisk();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

private:
	void HandleParticles();
	void HandlePostFX();
	void GetEffect();

private:
	Ref<ParticleSystemComponent> m_ParticleSystem;
	Ref<TransformComponent> m_PlayerTransform;
	Ref<ImageComponent> m_PostFX;
	bool m_Used;
	ObeliskEffect m_Effect;
	float m_TimeToGetEffect;
	TimerHandle m_EffectTimerHandle;

	Ref<Player> m_Player;

#pragma region Serialization

	uint64_t m_PostFXID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};