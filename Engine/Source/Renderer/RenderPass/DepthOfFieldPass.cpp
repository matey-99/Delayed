#include "DepthOfFieldPass.h"

#include "GBufferPass.h"
#include "Math/Math.h"
#include "Renderer/RenderTools.h"

DepthOfFieldPass::DepthOfFieldPass()
{
	RenderTarget::Config blurConfig;
	blurConfig.Attachment = RenderTarget::Attachment::Color;
	blurConfig.Type = RenderTarget::Type::Texture2D;
	blurConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;
	m_BlurRenderTarget = RenderTarget::Create(blurConfig, 1920, 1080);

	RenderTarget::Config depthOfFieldConfig;
	depthOfFieldConfig.Attachment = RenderTarget::Attachment::Color;
	depthOfFieldConfig.Type = RenderTarget::Type::Texture2D;
	depthOfFieldConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;
	m_FinalRenderTarget = RenderTarget::Create(depthOfFieldConfig, 1920, 1080);
}

DepthOfFieldPass::~DepthOfFieldPass()
{
}

void DepthOfFieldPass::Render(uint32_t input)
{
	// Blur
	m_BlurRenderTarget->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	auto blurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Blur");
	blurShader->Use();
	blurShader->SetInt("u_Input", 0);
	blurShader->SetInt("u_Size", m_Settings.Size);


	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_BlurRenderTarget->Unbind();

	// Depth Of Field
	m_FinalRenderTarget->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->m_GBufferPass->GetRenderTarget()->GetTargets()[5]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, input);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, m_BlurRenderTarget->GetTargets()[0]);

	auto depthOfFieldShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "DepthOfField");
	depthOfFieldShader->Use();
	depthOfFieldShader->SetInt("u_GBufferViewPosition", 0);
	depthOfFieldShader->SetInt("u_Input", 1);
	depthOfFieldShader->SetInt("u_BlurredInput", 2);
	depthOfFieldShader->SetFloat("u_MinDistance", m_Settings.MinDistance);
	depthOfFieldShader->SetFloat("u_MaxDistance", m_Settings.MaxDistance);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_FinalRenderTarget->Unbind();
}

void DepthOfFieldPass::UpdateRenderTargets(uint32_t width, uint32_t height)
{
	m_BlurRenderTarget->Update(width, height);
	m_FinalRenderTarget->Update(width, height);
}
