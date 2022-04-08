#include "ForwardPass.h"

ForwardPass::ForwardPass()
{
}

ForwardPass::~ForwardPass()
{
}

void ForwardPass::Render(Ref<Scene> scene)
{
	m_RenderTarget->Bind();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene->Render();

	m_RenderTarget->Unbind();
}
