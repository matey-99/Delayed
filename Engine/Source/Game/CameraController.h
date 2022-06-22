#pragma once

#include "GameComponent.h"

class CameraComponent;
class TransformComponent;

class CameraController : public GameComponent
{
public:
	CameraController(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;

private:
	void FollowTarget(float speed);

	void Turn(float value);
	void LookUp(float value);

private:
	Ref<CameraComponent> m_Camera;
	TransformComponent* m_Pivot;

	Ref<TransformComponent> m_Target;

	float m_CameraSpeed;
	float m_FollowSpeed;
	float m_LookUpLimit;
    float m_LookDownLimit;
	float m_GamepadRotationSensitivity;

	float m_LookUpRotation;
	float m_TurnRotation;

#pragma region Serialization

	uint64_t m_TargetID;
	uint64_t m_CameraID;

#pragma endregion

	friend class SceneSerializer;
};