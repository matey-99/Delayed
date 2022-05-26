#pragma once

#include "Scene/Component/Component.h"

#include "UIComponent.h"
#include "Assets/Font.h"

class ButtonComponent;

class TextComponent : public UIComponent
{
public:
	TextComponent(Actor* owner);

	virtual void Start() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Destroy() override;

	void ChangeFont(std::string path);

	inline void SetNormalColor(const glm::vec4& color) { m_NormalColor = color; }

private:
	Ref<Font> m_Font;
	float m_FontSize;

	std::string m_Text;
	glm::vec4 m_NormalColor;
	glm::vec4 m_HoveredColor;
	glm::vec4 m_PressedColor;
	glm::vec4 m_DisabledColor;

	Ref<ButtonComponent> m_ParentButton;

	uint32_t m_VAO;
	uint32_t m_VBO;

	friend class SceneSerializer;
	friend class ActorDetailsPanel;
};