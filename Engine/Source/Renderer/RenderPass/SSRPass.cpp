#include "SSRPass.h"

#include "GBufferPass.h"
#include "Math/Math.h"
#include "Renderer/RenderTools.h"


SSRPass::SSRPass()
{
	RenderTarget::Config ssrConfig;
	ssrConfig.Attachment = RenderTarget::Attachment::Color;
	ssrConfig.Type = RenderTarget::Type::Texture2D;
	ssrConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	ssrConfig.Filter = RenderTarget::Filter::Linear;  // Czy dodaæ?
	m_RenderTarget = RenderTarget::Create(ssrConfig, 1920, 1080);
}

SSRPass::~SSRPass()
{
}

void SSRPass::Render(uint32_t input)
{
	m_RenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	auto g = Renderer::GetInstance()->m_GBufferPass;

	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, input);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[5]);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[4]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[1]);


	auto ssrShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "SSR");
	ssrShader->Use();
	ssrShader->SetInt("u_GBufferViewPosition", 0);
	ssrShader->SetInt("u_GBufferNormal", 1);
	ssrShader->SetInt("u_GBufferMetallicRoughness", 4);
	ssrShader->SetInt("u_NumBinarySearchSteps", m_Settings.NumBinarySearchSteps);
	ssrShader->SetFloat("u_RayStep", m_Settings.RayStep);
	ssrShader->SetFloat("u_MinRayStep", m_Settings.MinRayStep);
	ssrShader->SetInt("u_MaxSteps", m_Settings.MaxSteps);
	ssrShader->SetFloat("u_ReflectionSpecularFalloffExponent", m_Settings.ReflectionSpecularFalloffExponent);
	ssrShader->SetInt("u_Screen", 11);

	auto r = Renderer::GetInstance();
	ssrShader->SetVec2("u_ScreenSize", glm::vec2(r->GetWindowWidth(), r->GetWindowHeight()));

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	m_RenderTarget->Unbind();
}

void SSRPass::UpdateRenderTargets(uint32_t width, uint32_t height)
{
	m_RenderTarget->Update(width, height);
}
