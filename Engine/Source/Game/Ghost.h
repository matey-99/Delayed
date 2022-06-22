#pragma once

#include "GameComponent.h"
#include "Saveable.h"

#define GHOST_POSITIONS_COUNT 200

class MaterialInstance;
class Animator;

class Ghost : public GameComponent, public Saveable
{
public:
	Ghost(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	virtual const SaveData Save() override;
	virtual void Load(const SaveData& data) override;

	void Corrupt();
	void Heal();

	void UpdateAnimatorParams(bool grounded, bool landing, bool jumping, bool dashing, bool sprinting);

	inline void SetPlayerActor(Ref<Actor> player) { m_PlayerActor = player; }
	inline void SetCorrupted(bool corrupted) { m_IsCorrupted = corrupted; }

private:
	void HandleAnimator();

private:
	Ref<Actor> m_PlayerActor;
	Ref<MaterialInstance> m_Material;
	Ref<Animator> m_Animator;

	glm::vec3 m_PositionOffset;
	glm::vec3 m_NormalEmissiveColor;
	glm::vec3 m_CorruptedEmissiveColor;
	glm::vec3 m_DefaultVignetteColor;
	glm::vec3 m_CorruptedVignetteColor;

	glm::vec3 m_Positions[GHOST_POSITIONS_COUNT];
	float m_RotationsY[GHOST_POSITIONS_COUNT];
	float m_MovementSpeed[GHOST_POSITIONS_COUNT];
	bool m_IsGrounded[GHOST_POSITIONS_COUNT];
	bool m_IsLanding[GHOST_POSITIONS_COUNT];
	bool m_IsJumping[GHOST_POSITIONS_COUNT];
	bool m_IsDashing[GHOST_POSITIONS_COUNT];
	bool m_IsSprinting[GHOST_POSITIONS_COUNT];

	int m_CurrentPositionIndex;
	bool m_FollowPlayer;
	bool m_IsCorrupted;

#pragma region Serialization

	uint64_t m_PlayerID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};