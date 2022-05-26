#include "TextComponent.h"

#include "Scene/Actor.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"
#include "Renderer/RenderPass/UIPass.h"
#include "Assets/AssetManager.h"
#include "ButtonComponent.h"

TextComponent::TextComponent(Actor* owner)
	: UIComponent(owner)
{
	m_Font = AssetManager::LoadFont("Fonts/arial.ttf");
	m_FontSize = 5.0f;

	m_Text = "New Text";
	m_NormalColor = glm::vec4(1.0f);
	m_HoveredColor = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
	m_PressedColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DisabledColor = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextComponent::Start()
{
	m_ParentButton = m_Owner->GetTransform()->GetParent()->GetOwner()->GetComponent<ButtonComponent>();
}

void TextComponent::Update(float deltaTime)
{
}

void TextComponent::Render()
{
	glm::vec4 color = m_NormalColor;
	if (m_ParentButton)
	{
		switch (m_ParentButton->GetCurrentState())
		{
		case ButtonState::Normal:
			color = m_NormalColor;
			break;
		case ButtonState::Hovered:
			color = m_HoveredColor;
			break;
		case ButtonState::Pressed:
			color = m_PressedColor;
			break;
		case ButtonState::Disabled:
			color = m_DisabledColor;
			break;
		}
	}

	auto shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::UI, "Text");
	shader->Use();
	shader->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());
	shader->SetVec4("u_Color", color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	float x = 0.0f;
	float y = 0.0f;
	for (std::string::const_iterator c = m_Text.begin(); c != m_Text.end(); c++)
	{
		Font::Character ch = m_Font->GetCharacter(*c);

		float fontSize = m_FontSize * 2;
		float xPos = x + ch.Bearing.x * fontSize / FONT_SIZE;
		float yPos = y - (ch.Size.y - ch.Bearing.y) * fontSize / FONT_SIZE;

		float w = ch.Size.x * fontSize / FONT_SIZE;
		float h = ch.Size.y * fontSize / FONT_SIZE;

		float vertices[] =
		{
			xPos,	   yPos + h,	0.0f, 0.0f,
			xPos,	   yPos,		0.0f, 1.0f,
			xPos + w, yPos,			1.0f, 1.0f,

			xPos,	   yPos + h,	0.0f, 0.0f,
			xPos + w,  yPos,		1.0f, 1.0f,
			xPos + w,  yPos + h,	1.0f, 0.0f
		};

		glBindTexture(GL_TEXTURE_2D, ch.ID);
		
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += (ch.Advance >> 6) * fontSize / FONT_SIZE;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextComponent::Destroy()
{
}

void TextComponent::ChangeFont(std::string path)
{
	m_Font = AssetManager::LoadFont(path);
}