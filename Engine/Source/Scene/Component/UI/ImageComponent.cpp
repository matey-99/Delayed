#include "ImageComponent.h"

#include "Scene/Actor.h"
#include "Content/ContentHelper.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"

ImageComponent::ImageComponent(Actor* owner)
	: UIComponent(owner)
{
	m_Image = Texture::Create(ContentHelper::GetAssetPath("Textures/Default/UI.png"));
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

	RenderTools::GetInstance()->RenderQuad();

	m_Image->Unbind();
}

void ImageComponent::Destroy()
{
}

void ImageComponent::ChangeImage(std::string path)
{
	m_Image = Texture::Create(ContentHelper::GetAssetPath(path));
}
