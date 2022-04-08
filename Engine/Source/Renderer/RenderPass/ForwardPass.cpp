#include "ForwardPass.h"

#include "Renderer/RenderPass/GBufferPass.h"
#include "Renderer/RenderPass/LightingPass.h"
#include "Renderer/RenderPass/ShadowsPass.h"

ForwardPass::ForwardPass()
{
}

ForwardPass::~ForwardPass()
{
}

void ForwardPass::Render(Ref<Scene> scene)
{
	auto gBufferMRT = Renderer::GetInstance()->m_GBufferPass->GetRenderTarget();
	auto lightingRT  = Renderer::GetInstance()->m_LightingPass->GetRenderTarget();

	RenderTarget::Copy(gBufferMRT, lightingRT, RenderTarget::Buffer::Depth, RenderTarget::Filter::Nearest);

	lightingRT->Bind();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	scene->Render(Material::BlendMode::Transparent);

	glDisable(GL_BLEND);

	lightingRT->Unbind();
}