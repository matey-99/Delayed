#include "PostProcessingPass.h"

#include "LightingPass.h"
#include "Renderer/RenderTools.h"
#include "Analysis/Profiler.h"

PostProcessingPass::PostProcessingPass()
{
	RenderTarget::Config postProcessingConfig;
	postProcessingConfig.Attachment = RenderTarget::Attachment::Color;
	postProcessingConfig.Type = RenderTarget::Type::Texture2D;
	postProcessingConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	postProcessingConfig.Filter = RenderTarget::Filter::Linear;
	m_MainRenderTarget = RenderTarget::Create(postProcessingConfig, 1920, 1080);

	RenderTarget::Config thresholdConfig;
	thresholdConfig.Attachment = RenderTarget::Attachment::Color;
	thresholdConfig.Type = RenderTarget::Type::Texture2D;
	thresholdConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
	thresholdConfig.Filter = RenderTarget::Filter::Linear;
	m_ThresholdRenderTarget = RenderTarget::Create(thresholdConfig, m_MainRenderTarget->GetWidth() / 2, m_MainRenderTarget->GetHeight() / 2);

	for (int i = 0; i < 2; i++)
	{
		RenderTarget::Config downscaleConfig;
		downscaleConfig.Attachment = RenderTarget::Attachment::Color;
		downscaleConfig.Type = RenderTarget::Type::Texture2D;
		downscaleConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
		downscaleConfig.Filter = RenderTarget::Filter::Linear;
		m_DownscaleRenderTargets[i] = RenderTarget::Create(downscaleConfig, m_MainRenderTarget->GetWidth() / (4 * (i + 1)), m_MainRenderTarget->GetHeight() / (4 * (i + 1)));

		RenderTarget::Config blurConfig;
		blurConfig.Attachment = RenderTarget::Attachment::Color;
		blurConfig.Type = RenderTarget::Type::Texture2D;
		blurConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
		blurConfig.Filter = RenderTarget::Filter::Linear;
		m_BlurRenderTargets[i] = RenderTarget::Create(blurConfig, m_MainRenderTarget->GetWidth() / 8, m_MainRenderTarget->GetHeight() / 8);

		RenderTarget::Config upscaleConfig;
		upscaleConfig.Attachment = RenderTarget::Attachment::Color;
		upscaleConfig.Type = RenderTarget::Type::Texture2D;
		upscaleConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;
		upscaleConfig.Filter = RenderTarget::Filter::Linear;
		m_UpscaleRenderTargets[i] = RenderTarget::Create(upscaleConfig, m_MainRenderTarget->GetWidth() / (4 / (i + 1)), m_MainRenderTarget->GetHeight() / (4 / (i + 1)));
	}


}

PostProcessingPass::~PostProcessingPass()
{
}

void PostProcessingPass::Render(uint32_t input)
{
	// Bloom
	if (m_Settings.BloomEnabled)
	{
		// Threshold with downscale 1/2
		m_ThresholdRenderTarget->Bind();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, input);

		auto thresholdShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Threshold");
		thresholdShader->Use();
		thresholdShader->SetInt("u_Screen", 0);
		thresholdShader->SetFloat("u_Threshold", m_Settings.BloomThreshold);
		thresholdShader->SetFloat("u_Limit", m_Settings.BloomLimit);

		RenderTools::GetInstance()->RenderQuad();

		m_ThresholdRenderTarget->Unbind();

		// Downscale 1/4
		m_DownscaleRenderTargets[0]->Bind();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ThresholdRenderTarget->GetTargets()[0]);

		auto scaleShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Scale");
		scaleShader->Use();
		scaleShader->SetInt("u_SourceTexture", 0);

		RenderTools::GetInstance()->RenderQuad();

		m_DownscaleRenderTargets[0]->Unbind();

		// Downscale 1/8
		m_DownscaleRenderTargets[1]->Bind();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_DownscaleRenderTargets[0]->GetTargets()[0]);

		scaleShader->Use();
		scaleShader->SetInt("u_SourceTexture", 0);

		RenderTools::GetInstance()->RenderQuad();

		m_DownscaleRenderTargets[1]->Unbind();

		// Gaussian Blur
		ComputeGaussianBlurKernel(m_Settings.BloomBlurSigma, m_DownscaleRenderTargets[1]->GetWidth(), m_DownscaleRenderTargets[1]->GetHeight());
		int blurStages = 2;
		for (int i = 0; i < blurStages; i++)
		{
			// Horizontal
			m_BlurRenderTargets[0]->Bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, (i == 0) ? m_DownscaleRenderTargets[1]->GetTargets()[0] : m_BlurRenderTargets[1]->GetTargets()[0]);

			auto blurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "GaussianBlurHorizontal");
			blurShader->Use();
			blurShader->SetInt("u_SourceTexture", 0);
			for (int i = 0; i < GAUSSIAN_BLUR_KERNEL_SIZE; i++)
				blurShader->SetVec2("u_GaussianBlurCache[" + std::to_string(i) + "]", m_GaussianBlurHorizontalCache[i]);

			RenderTools::GetInstance()->RenderQuad();

			m_BlurRenderTargets[0]->Unbind();

			// Vertical
			m_BlurRenderTargets[1]->Bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_BlurRenderTargets[0]->GetTargets()[0]);

			blurShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "GaussianBlurVertical");
			blurShader->Use();
			blurShader->SetInt("u_SourceTexture", 0);
			for (int i = 0; i < GAUSSIAN_BLUR_KERNEL_SIZE; i++)
				blurShader->SetVec2("u_GaussianBlurCache[" + std::to_string(i) + "]", m_GaussianBlurVerticalCache[i]);

			RenderTools::GetInstance()->RenderQuad();

			m_BlurRenderTargets[1]->Unbind();
		}

		// Upscale 1/4
		m_UpscaleRenderTargets[0]->Bind();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_BlurRenderTargets[1]->GetTargets()[0]);

		scaleShader->Use();
		scaleShader->SetInt("u_SourceTexture", 0);

		RenderTools::GetInstance()->RenderQuad();

		m_UpscaleRenderTargets[0]->Unbind();

		// Upscale 1/2
		m_UpscaleRenderTargets[1]->Bind();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_UpscaleRenderTargets[0]->GetTargets()[0]);

		scaleShader->Use();
		scaleShader->SetInt("u_SourceTexture", 0);

		RenderTools::GetInstance()->RenderQuad();

		m_UpscaleRenderTargets[1]->Unbind();
	}

	// Output
	m_MainRenderTarget->Bind();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, input);

	auto postProcessingShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "PostProcessing");
	postProcessingShader->Use();
	postProcessingShader->SetInt("u_Screen", 0);
	postProcessingShader->SetBool("u_IsBloom", m_Settings.BloomEnabled);
	if (m_Settings.BloomEnabled)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_UpscaleRenderTargets[1]->GetTargets()[0]);

		postProcessingShader->SetInt("u_BloomTexture", 1);
		postProcessingShader->SetFloat("u_BloomIntensity", m_Settings.BloomIntensity);
	}

	postProcessingShader->SetFloat("u_Gamma", m_Settings.Gamma);
	postProcessingShader->SetFloat("u_Exposure", m_Settings.Exposure);
	postProcessingShader->SetFloat("u_Saturation", m_Settings.Saturation);
	postProcessingShader->SetFloat("u_Temperature", m_Settings.Temperature);
	postProcessingShader->SetFloat("u_Hue", m_Settings.Hue);
	postProcessingShader->SetFloat("u_Gain", m_Settings.Gain);
	postProcessingShader->SetFloat("u_Lift", m_Settings.Lift);
	postProcessingShader->SetFloat("u_Offset", m_Settings.Offset);
	postProcessingShader->SetFloat("u_Contrast", m_Settings.Contrast);
	postProcessingShader->SetFloat("u_ContrastPivot", m_Settings.ContrastPivot);
    postProcessingShader->SetBool("u_IsAberration", m_Settings.AberrationEnabled);
    postProcessingShader->SetVec3("u_AberrationShift", m_Settings.AberrationShift);
    postProcessingShader->SetBool("u_IsFisheye", m_Settings.FisheyeEnabled);
    postProcessingShader->SetFloat("u_Scale", m_Settings.Scale);
    postProcessingShader->SetBool("u_IsVignette", m_Settings.VignetteEnabled);
    postProcessingShader->SetVec3("u_VignetteColor", m_Settings.VignetteColor);
    postProcessingShader->SetFloat("u_VignetteIntensity", m_Settings.VignetteIntensity);
    postProcessingShader->SetFloat("u_VignetteSize", m_Settings.VignetteSize);


	RenderTools::GetInstance()->RenderQuad();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	 
	m_MainRenderTarget->Unbind();
}

void PostProcessingPass::UpdateRenderTargets(uint32_t width, uint32_t height)
{
	m_MainRenderTarget->Update(width, height);
	m_ThresholdRenderTarget->Update(width / 2, height / 2);
	m_DownscaleRenderTargets[0]->Update(width / 4, height / 4);
	m_DownscaleRenderTargets[1]->Update(width / 8, height / 8);
	m_BlurRenderTargets[0]->Update(width / 8, height / 8);
	m_BlurRenderTargets[1]->Update(width / 8, height / 8);
	m_UpscaleRenderTargets[0]->Update(width / 4, height / 4);
	m_UpscaleRenderTargets[1]->Update(width / 2, height / 2);
}

void PostProcessingPass::ComputeGaussianBlurKernel(float sigma, float width, float height)
{
	float total = 0.0f;
	float xOffset = 1.0f / width;
	float yOffset = 1.0f / height;

	for (int i = -GAUSSIAN_BLUR_RADIUS; i <= GAUSSIAN_BLUR_RADIUS; i++)
	{
		float distance = (float)(i * i);
		float weight = (1 / (2 * glm::pi<float>() * sigma * sigma)) * glm::exp(-(distance / (2 * sigma * sigma)));

		m_GaussianBlurHorizontalCache[i + GAUSSIAN_BLUR_RADIUS] = glm::vec2(weight, i * xOffset);
		m_GaussianBlurVerticalCache[i + GAUSSIAN_BLUR_RADIUS] = glm::vec2(weight, i * yOffset);

		total += weight;
	}

	for (int i = 0; i < GAUSSIAN_BLUR_KERNEL_SIZE; i++)
	{
		m_GaussianBlurHorizontalCache[i].x /= total;
		m_GaussianBlurVerticalCache[i].x /= total;
	}
}