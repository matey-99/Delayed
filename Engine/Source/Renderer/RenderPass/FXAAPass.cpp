#include "FXAAPass.h"

#include "Renderer/Renderer.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderTools.h"

FXAAPass::FXAAPass()
{
	RenderTarget::Config fxaaConfig;
	fxaaConfig.Attachment = RenderTarget::Attachment::Color;
	fxaaConfig.Type = RenderTarget::Type::Texture2D;
	fxaaConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	fxaaConfig.Filter = RenderTarget::Filter::Linear;
	m_RenderTarget = RenderTarget::Create(fxaaConfig, 1920, 1080);
}

FXAAPass::~FXAAPass()
{
}

void FXAAPass::Render(uint32_t input)
{
	m_RenderTarget->Bind();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	auto fxaaShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "FXAA");
	fxaaShader->Use();
	fxaaShader->SetInt("u_SourceTexture", 0);
	fxaaShader->SetVec2("u_InverseScreenSize", glm::vec2(1.0f / m_RenderTarget->GetWidth(), 1.0f / m_RenderTarget->GetHeight()));

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_RenderTarget->Unbind();
}

void FXAAPass::UpdateRenderTarget(uint32_t width, uint32_t height)
{
	m_RenderTarget->Update(width, height);
}