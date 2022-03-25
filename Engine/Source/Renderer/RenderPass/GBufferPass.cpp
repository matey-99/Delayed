#include "GBufferPass.h"

GBufferPass::GBufferPass()
{
	// Position
	RenderTarget::Config gBuffer0Config;
	gBuffer0Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer0Config.Type = RenderTarget::Type::Texture2D;
	gBuffer0Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;

	// Normal
	RenderTarget::Config gBuffer1Config;
	gBuffer1Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer1Config.Type = RenderTarget::Type::Texture2D;
	gBuffer1Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;

	// Color: RGB, AO: A
	RenderTarget::Config gBuffer2Config;
	gBuffer2Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer2Config.Type = RenderTarget::Type::Texture2D;
	gBuffer2Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;

	// Emissive: RGB
	RenderTarget::Config gBuffer3Config;
	gBuffer3Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer3Config.Type = RenderTarget::Type::Texture2D;
	gBuffer3Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;

	// Roughness: R, Metallic: G
	RenderTarget::Config gBuffer4Config;
	gBuffer4Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer4Config.Type = RenderTarget::Type::Texture2D;
	gBuffer4Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA32F;

	// View-Space Position
	RenderTarget::Config gBuffer5Config;
	gBuffer5Config.Attachment = RenderTarget::Attachment::Color;
	gBuffer5Config.Type = RenderTarget::Type::Texture2D;
	gBuffer5Config.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;

	std::vector<RenderTarget::Config> gBufferConfigs = { gBuffer0Config, gBuffer1Config, gBuffer2Config, gBuffer3Config, gBuffer4Config, gBuffer5Config };

	m_RenderTarget = RenderTarget::CreateMRT(gBufferConfigs, 1920, 1080);
}

GBufferPass::~GBufferPass()
{
}

void GBufferPass::Render(Ref<Scene> scene)
{
	m_RenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->Render();

	m_RenderTarget->Unbind();
}
