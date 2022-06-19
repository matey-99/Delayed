#pragma once

#include "Core.h"

#include "GameComponent.h"

class TransformComponent;

class Moving : public GameComponent
{
public:
	Moving(Actor* owner);
	~Moving();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	glm::vec3 m_DefaultPosition;
	glm::vec3 m_TargetPosition;

	glm::vec3 m_Direction;
	float m_Distance;
	float m_Smoothness;
	float m_ErrorInReachingTarget;

	bool m_IsReturning;

	glm::vec3 m_Velocity;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};