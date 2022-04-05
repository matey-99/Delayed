#include "ButtonComponent.h"

#include "Scene/Actor.h"
#include "Input/Input.h"
#include "RectTransformComponent.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"
#include "Renderer/RenderPass/UIPass.h"

ButtonComponent::ButtonComponent(Actor* owner)
	: UIComponent(owner)
{
	m_Enabled = true;

	m_CurrentState = ButtonState::Normal;

	m_Image = Texture::Create("Textures/Default/UI.png");
	m_NormalColor = glm::vec4(1.0f);
	m_HoveredColor = glm::vec4(0.75f, 0.75f, 0.75f, 1.0f);
	m_PressedColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
	m_DisabledColor = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
}

void ButtonComponent::Start()
{
	Input::GetInstance()->OnLeftMouseButtonPressed.Add(&ButtonComponent::Press, this);
	Input::GetInstance()->OnLeftMouseButtonReleased.Add(&ButtonComponent::Release, this);
}

void ButtonComponent::Update(float deltaTime)
{
	if (m_Enabled)
	{
		if (m_CurrentState != ButtonState::Pressed)
		{
			m_CurrentState = ButtonState::Normal;

			// Check if button is hovered
			glm::vec2 mousePosition = Input::GetInstance()->GetMousePosition();

			auto rectTransform = Cast<RectTransformComponent>(m_Owner->GetTransform());
			glm::vec3 position = rectTransform->GetLocalPositionUI();
			glm::vec3 scale = rectTransform->GetLocalScaleUI();

			auto renderer = Renderer::GetInstance();

			float left = (position.x / 2) - (scale.x / 2) + (renderer->GetWindowWidth() / 2);
			float right = (position.x / 2) + (scale.x / 2) + (renderer->GetWindowWidth() / 2);
			float top = (-position.y / 2) - (scale.y / 2) + (renderer->GetWindowHeight() / 2);
			float bottom = (-position.y / 2) + (scale.y / 2) + (renderer->GetWindowHeight() / 2);

			if (mousePosition.x > left && mousePosition.x < right && mousePosition.y > top && mousePosition.y < bottom)
			{
				m_CurrentState = ButtonState::Hovered;
			}
		}
		
	}
	else
	{
		m_CurrentState = ButtonState::Disabled;
	}
}

void ButtonComponent::Render()
{
	m_Image->Bind(0);

	glm::vec4 color;
	switch (m_CurrentState)
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

	auto shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::UI, "UI");
	shader->Use();
	shader->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());
	shader->SetInt("u_Image", 0);
	shader->SetVec4("u_Color", color);
	shader->SetBool("u_IsGammaCorrection", true);
	shader->SetFloat("u_Gamma", Renderer::GetInstance()->m_UIPass->GetSettings().Gamma);
	shader->SetFloat("u_Exposure", Renderer::GetInstance()->m_UIPass->GetSettings().Exposure);

	RenderTools::GetInstance()->RenderQuad();

	m_Image->Unbind();
}

void ButtonComponent::Destroy()
{
}

void ButtonComponent::Press()
{
	if (m_CurrentState == ButtonState::Hovered)
	{
		m_CurrentState = ButtonState::Pressed;

		OnPressed.Broadcast();
	}
}

void ButtonComponent::Release()
{
	if (m_CurrentState == ButtonState::Pressed)
	{
		m_CurrentState = ButtonState::Normal;

		OnReleased.Broadcast();
	}
}

void ButtonComponent::ChangeImage(std::string path)
{
	m_Image = Texture::Create(path);
}
