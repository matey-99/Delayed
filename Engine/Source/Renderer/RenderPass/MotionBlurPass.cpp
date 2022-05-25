#include "MotionBlurPass.h"

#include "Renderer/Renderer.h"
#include "Material/ShaderLibrary.h"
#include "Renderer/RenderTools.h"
#include "Analysis/Profiler.h"
#include "Renderer/RenderTools.h"
#include "GBufferPass.h"

MotionBlurPass::MotionBlurPass()
{
	RenderTarget::Config motionBlurConfig;
	motionBlurConfig.Attachment = RenderTarget::Attachment::Color;
	motionBlurConfig.Type = RenderTarget::Type::Texture2D;
	motionBlurConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	motionBlurConfig.Filter = RenderTarget::Filter::Linear;
	m_RenderTarget = RenderTarget::Create(motionBlurConfig, 1920, 1080);

}

MotionBlurPass::~MotionBlurPass()
{
}

void MotionBlurPass::Render(uint32_t input)
{
	// Output
	m_RenderTarget->Bind();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	auto g = Renderer::GetInstance()->m_GBufferPass;

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[5]);

	auto motionBlurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "MotionBlur");
	motionBlurShader->Use();
	motionBlurShader->SetInt("u_Screen", 0);
	motionBlurShader->SetInt("u_GBufferViewPosition", 1);
	motionBlurShader->SetInt("u_Size", m_Settings.Size);
	motionBlurShader->SetFloat("u_Separation", m_Settings.Separation);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
	m_RenderTarget->Unbind();
}

void MotionBlurPass::UpdateRenderTarget(uint32_t width, uint32_t height)
{
	m_RenderTarget->Update(width, height);
}
