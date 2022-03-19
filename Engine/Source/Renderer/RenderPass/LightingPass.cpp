#include "LightingPass.h"

#include "GBufferPass.h"
#include "ShadowsPass.h"
#include "Renderer/RenderTools.h"

LightingPass::LightingPass()
{
	RenderTarget::Config lightingConfig;
	lightingConfig.Attachment = RenderTarget::Attachment::Color;
	lightingConfig.Type = RenderTarget::Type::Texture2D;
	lightingConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;

	m_RenderTarget = RenderTarget::Create(lightingConfig, 1920, 1080);
}

LightingPass::~LightingPass()
{
}

void LightingPass::Render()
{
	m_RenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	auto g = Renderer::GetInstance()->m_GBufferPass;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[1]);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[2]);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[3]);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, g->GetRenderTarget()->GetTargets()[4]);

	auto s = Renderer::GetInstance()->m_ShadowsPass;

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D_ARRAY, s->GetDirectionalLightRenderTarget()->GetTargets()[0]);

	auto shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Material, "Lighting");
	shader->Use();
	shader->SetInt("u_GBufferPosition", 0);
	shader->SetInt("u_GBufferNormal", 1);
	shader->SetInt("u_GBufferColorAO", 2);
	shader->SetInt("u_GBufferEmissive", 3);
	shader->SetInt("u_GBufferMetallicRoughness", 4);
	shader->SetInt("u_DirectionalLightShadowMaps", 5);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_RenderTarget->Unbind();
}
