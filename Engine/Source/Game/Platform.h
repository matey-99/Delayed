#pragma once

#include "Core.h"

#include "GameComponent.h"

class MaterialInstance;
class AudioSourceComponent;
class Button;

class Platform : public GameComponent
{
public:
	Platform(Actor* owner);
	~Platform();

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void FixedUpdate() override;

	void AddButton(Button* button);

	inline void SetActive(bool active) { m_Active = active; }

private:
	bool ShouldBeActive();

private:
	bool m_Active;
	glm::vec3 m_DefaultPosition;

	Ref<MaterialInstance> m_Material;
	Ref<AudioSourceComponent> m_AudioSource;
	std::vector<Button*> m_Buttons;

#pragma region Serialization

	glm::vec3 m_Direction;
	float m_Distance;
	float m_Speed;

#pragma endregion

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};