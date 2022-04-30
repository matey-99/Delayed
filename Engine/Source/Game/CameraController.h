#pragma once

#include "GameComponent.h"

class CameraComponent;

class CameraController : public GameComponent
{
public:
	CameraController(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	Ref<CameraComponent> m_Camera;
	Ref<Actor> m_Pivot;

#pragma region Serialization

	uint64_t m_CameraID;
	uint64_t m_PivotID;

#pragma endregion

	friend class SceneSerializer;
};