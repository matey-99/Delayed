#pragma once

#include "Core.h"

#include "GameComponent.h"

class Material;
class MaterialInstance;
class AudioSourceComponent;

class Clouds : public GameComponent
{
public:
	Clouds(Actor* owner);
	~Clouds();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;

private:
	Ref<MaterialInstance> m_MaterialInstance;

	glm::vec3 m_MoveDirection;
	float m_MoveSpeed;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};