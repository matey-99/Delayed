#include "LightingPass.h"

#include "GBufferPass.h"
#include "ShadowsPass.h"
#include "SSAOPass.h"
#include "Renderer/RenderTools.h"
#include "Scene/Component/Light/SkyLight.h"

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

void LightingPass::Render(Ref<Scene> scene)
{
	m_RenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	auto renderer = Renderer::GetInstance();

	auto g = renderer->m_GBufferPass;

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

	auto s = renderer->m_ShadowsPass;

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D_ARRAY, s->GetDirectionalLightRenderTarget()->GetTargets()[0]);

	if (renderer->GetSettings().SSAOEnabled)
	{
		auto& ssao = Renderer::GetInstance()->m_SSAOPass;

		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, ssao->GetFinalRenderTarget()->GetTargets()[0]);
	}


	auto shader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Calculations, "Lighting");
	shader->Use();
	shader->SetInt("u_GBufferPosition", 0);
	shader->SetInt("u_GBufferNormal", 1);
	shader->SetInt("u_GBufferColorAO", 2);
	shader->SetInt("u_GBufferEmissive", 3);
	shader->SetInt("u_GBufferMetallicRoughness", 4);
	shader->SetInt("u_DirectionalLightShadowMaps", 5);
	shader->SetInt("u_SSAO", 6);
	shader->SetBool("u_SSAOEnabled", renderer->GetSettings().SSAOEnabled);

	if (auto skyLight = scene->FindComponent<SkyLight>())
	{
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyLight->GetIrradianceMap());

		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyLight->GetPrefilterMap());

		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, skyLight->GetBRDF());

		shader->SetInt("u_IrradianceMap", 7);
		shader->SetInt("u_PrefilterMap", 8);
		shader->SetInt("u_BRDF", 9);

		shader->SetVec3("u_SkyLightColor", skyLight->GetColor());
		shader->SetFloat("u_SkyLightIntensity", skyLight->GetIntensity());
		shader->SetFloat("u_SkyLightWeight", skyLight->GetWeight());

		shader->SetBool("u_SkyLightEnabled", skyLight->GetOwner()->IsEnabled());
	}
	else
	{
		shader->SetVec3("u_SkyLightColor", glm::vec3(1.0f));
		shader->SetFloat("u_SkyLightIntensity", 0.03f);
		shader->SetFloat("u_SkyLightWeight", 0.0f);

		shader->SetBool("u_SkyLightEnabled", false);
	}

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_RenderTarget->Unbind();
}
