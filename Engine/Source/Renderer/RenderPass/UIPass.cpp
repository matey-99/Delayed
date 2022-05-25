#include "UIPass.h"

#include "Scene/Component/UI/UIComponent.h"
#include "Renderer/Renderer.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"
#include "Math/Transform.h"

UIPass::UIPass()
{
	RenderTarget::Config uiConfig;
	uiConfig.Attachment = RenderTarget::Attachment::Color;
	uiConfig.Type = RenderTarget::Type::Texture2D;
	uiConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;

	m_RenderTarget = RenderTarget::Create(uiConfig, 1920, 1080);
}

UIPass::~UIPass()
{
}

void UIPass::Render(Ref<Scene> scene, uint32_t input)
{
	// Render UI elements
	m_RenderTarget->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	Transform transform;
	auto uiShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::UI, "UI");
	uiShader->Use();
	uiShader->SetMat4("u_Model", transform.GetModelMatrix());
	uiShader->SetInt("u_Image", 0);
	uiShader->SetVec4("u_Color", glm::vec4(1.0f));
	uiShader->SetBool("u_IsGammaCorrection", false);

	RenderTools::GetInstance()->RenderQuad();

	auto uiComponents = scene->GetComponents<UIComponent>();
	for (auto comp : uiComponents)
	{
		if (comp->GetOwner()->IsEnabled())
			comp->Render();
	}

	glDisable(GL_BLEND);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_RenderTarget->Unbind();
}

void UIPass::UpdateRenderTargets(uint32_t width, uint32_t height)
{
	m_RenderTarget->Update(width, height);
}