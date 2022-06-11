#pragma once

#include "Core.h"

#include "GameComponent.h"

class Moving : public GameComponent
{
public:
	Moving(Actor* owner);
	~Moving();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	glm::vec3 m_DefaultPosition;

	glm::vec3 m_Direction;
	float m_Distance;
	float m_Speed;

	bool m_IsReturning;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};