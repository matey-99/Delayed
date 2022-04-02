#pragma once

#include "UIComponent.h"
#include "Renderer/Texture.h"
#include "Patterns/Event.h"

class ButtonComponent : public UIComponent
{
private:
	enum class ButtonState
	{
		Normal, Hovered, Pressed, Disabled
	};

public:
	ButtonComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Destroy() override;

	void Press();
	void Release();

	void ChangeImage(std::string path);

	inline bool IsEnabled() const { return m_Enabled; }
	inline ButtonState GetCurrentState() const { return m_CurrentState; }

public:
	Event OnPressed;
	Event OnReleased;

private:
	bool m_Enabled;

	ButtonState m_CurrentState;

	Ref<Texture> m_Image;
	glm::vec4 m_NormalColor;
	glm::vec4 m_HoveredColor;
	glm::vec4 m_PressedColor;
	glm::vec4 m_DisabledColor;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};