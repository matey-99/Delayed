#include "VignettePass.h"

#include "Renderer/Renderer.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderPass/PostProcessingPass.h"
#include "Renderer/RenderTools.h"

VignettePass::VignettePass()
{
	RenderTarget::Config vignetteConfig;
	vignetteConfig.Attachment = RenderTarget::Attachment::Color;
	vignetteConfig.Type = RenderTarget::Type::Texture2D;
	vignetteConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	vignetteConfig.Filter = RenderTarget::Filter::Linear;
	m_RenderTarget = RenderTarget::Create(vignetteConfig, 1920, 1080);
}

VignettePass::~VignettePass()
{
}

void VignettePass::Render(uint32_t input)
{
	m_RenderTarget->Bind();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	auto vignetteShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Vignette");
	vignetteShader->Use();
	vignetteShader->SetInt("u_SourceTexture", 0);
	vignetteShader->SetFloat("u_Intensity", m_Settings.Intensity);
	vignetteShader->SetFloat("u_Size", m_Settings.Size);
	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_RenderTarget->Unbind();
}

void VignettePass::UpdateRenderTarget(uint32_t width, uint32_t height)
{
	m_RenderTarget->Update(width, height);
}