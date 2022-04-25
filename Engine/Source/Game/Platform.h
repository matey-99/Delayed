#pragma once

#include "Core.h"

#include "GameComponent.h"

class Platform : public GameComponent
{
public:
	Platform(Actor* owner);
	~Platform();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;

	inline void SetActive(bool active) { m_Active = active; }

private:
	bool m_Active;
	glm::vec3 m_DefaultPosition;

#pragma region Serialization

	glm::vec3 m_Direction;
	float m_Distance;
	float m_Speed;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};