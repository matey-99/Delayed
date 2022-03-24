#include "SSAOPass.h"

#include "GBufferPass.h"
#include "Math/Math.h"
#include "Renderer/RenderTools.h"

SSAOPass::SSAOPass()
{
	RenderTarget::Config ssaoConfig;
	ssaoConfig.Attachment = RenderTarget::Attachment::Color;
	ssaoConfig.Type = RenderTarget::Type::Texture2D;
	ssaoConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::R8;
	m_FirstRenderTarget = RenderTarget::Create(ssaoConfig, 1920, 1080);

	RenderTarget::Config ssaoBlurConfig;
	ssaoBlurConfig.Attachment = RenderTarget::Attachment::Color;
	ssaoBlurConfig.Type = RenderTarget::Type::Texture2D;
	ssaoBlurConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::R8;
	m_FinalRenderTarget = RenderTarget::Create(ssaoBlurConfig, 1920, 1080);

	std::uniform_real_distribution<float> random(0.0f, 1.0f);
	std::default_random_engine generator;
	for (uint32_t i = 0; i < 64; i++)
	{
		glm::vec3 sample(random(generator) * 2.0f - 1.0f, random(generator) * 2.0f - 1.0f, random(generator));
		sample = glm::normalize(sample);
		sample *= random(generator);

		float scale = (float)i / 64.0f;
		scale = Math::Lerp(0.1f, 1.0f, scale * scale);

		sample *= scale;
		m_SSAOKernel.push_back(sample);
	}

	for (uint32_t i = 0; i < 16; i++)
	{
		glm::vec3 noise(random(generator) * 2.0f - 1.0f, random(generator) * 2.0f - 1.0f, 0.0f);
		m_SSAONoise.push_back(noise);
	}

	glGenTextures(1, &m_NoiseTexture);
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 4, 4, 0, GL_RGB, GL_FLOAT, &m_SSAONoise[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindTexture(GL_TEXTURE_2D, 0);
}

SSAOPass::~SSAOPass()
{
}

void SSAOPass::Render()
{
	// SSAO
	m_FirstRenderTarget->Bind();

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
	glBindTexture(GL_TEXTURE_2D, m_NoiseTexture);

	auto ssaoShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "SSAO");
	ssaoShader->Use();
	ssaoShader->SetInt("u_GBufferPosition", 0);
	ssaoShader->SetInt("u_GBufferNormal", 1);
	ssaoShader->SetInt("u_NoiseTexture", 2);
	ssaoShader->SetInt("u_KernelSize", m_Settings.KernelSize);
	ssaoShader->SetFloat("u_Radius", m_Settings.Radius);
	ssaoShader->SetFloat("u_Bias", m_Settings.Bias);
	ssaoShader->SetFloat("u_Intensity", m_Settings.Intensity);

	auto r = Renderer::GetInstance();
	ssaoShader->SetVec2("u_ScreenSize", glm::vec2(r->GetWindowWidth(), r->GetWindowHeight()));

	for (uint32_t i = 0; i < 64; i++)
		ssaoShader->SetVec3("u_Kernel[" + std::to_string(i) + "]", m_SSAOKernel[i]);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_FirstRenderTarget->Unbind();

	// SSAO blur
	m_FinalRenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_FirstRenderTarget->GetTargets()[0]);

	auto ssaoBlurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "SSAOBlur");
	ssaoBlurShader->Use();
	ssaoBlurShader->SetInt("u_SSAO", 0);

	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	m_FinalRenderTarget->Unbind();
}

void SSAOPass::UpdateRenderTargets(uint32_t width, uint32_t height)
{
	m_FirstRenderTarget->Update(width, height);
	m_FinalRenderTarget->Update(width, height);
}
