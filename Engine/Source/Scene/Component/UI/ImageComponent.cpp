#include "ImageComponent.h"

#include "Scene/Actor.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"
#include "Renderer/RenderPass/UIPass.h"
#include "Assets/AssetManager.h"

ImageComponent::ImageComponent(Actor* owner)
	: UIComponent(owner)
{
	m_Image = AssetManager::LoadTexture("Textures/Default/UI.png");
	m_Color = glm::vec4(1.0f);
}

void ImageComponent::Start()
{
}

void ImageComponent::Update(float deltaTime)
{
}

void ImageComponent::Render()
{
	m_Image->Bind(0);

	auto shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::UI, "UI");
	shader->Use();
	shader->SetMat4("u_Model", m_Owner->GetTransform()->GetWorldModelMatrix());
	shader->SetInt("u_Image", 0);
	shader->SetVec4("u_Color", m_Color);
	shader->SetBool("u_IsGammaCorrection", true);
	shader->SetFloat("u_Gamma", Renderer::GetInstance()->m_UIPass->GetSettings().Gamma);
	shader->SetFloat("u_Exposure", Renderer::GetInstance()->m_UIPass->GetSettings().Exposure);

	RenderTools::GetInstance()->RenderQuad();

	m_Image->Unbind();
}

void ImageComponent::Destroy()
{
}

void ImageComponent::ChangeImage(std::string path)
{
	m_Image = AssetManager::LoadTexture(path);
}
