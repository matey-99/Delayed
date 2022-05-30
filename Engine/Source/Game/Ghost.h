#pragma once

#include "GameComponent.h"

#define GHOST_POSITIONS_COUNT 200

class Ghost : public GameComponent
{
public:
	Ghost(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

	virtual void OnTriggerEnter(ColliderComponent* other) override;

	inline void SetCorrupted(bool corrupted) { m_IsCorrupted = corrupted; }

private:
	Ref<Actor> m_PlayerActor;
	glm::vec3 m_Positions[GHOST_POSITIONS_COUNT];
	int m_CurrentPositionIndex;
	bool m_FollowPlayer;
	bool m_IsCorrupted;

#pragma region Serialization

	uint64_t m_PlayerID;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};