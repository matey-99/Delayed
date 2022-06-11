#include "ShadowsPass.h"

#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"

ShadowsPass::ShadowsPass()
{
	RenderTarget::Config directionalLightConfig;
	directionalLightConfig.Attachment = RenderTarget::Attachment::Depth;
	directionalLightConfig.Type = RenderTarget::Type::Texture2DArray;
	directionalLightConfig.DepthInternalFormat = RenderTarget::DepthInternalFormat::Depth32F;

	m_DirectionalLightRenderTarget = RenderTarget::Create(directionalLightConfig, 2048, 2048);
}

ShadowsPass::~ShadowsPass()
{
}

void ShadowsPass::Render(Ref<Scene> scene)
{
	// Directional Light
	if (auto light = scene->FindComponent<DirectionalLight>())
	{
		m_DirectionalLightRenderTarget->Bind();

		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_FRONT);

		if (light->IsCastingShadows())
		{
			auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Calculations, "SceneDepth");
			scene->Render(depthShader);
		}

		glCullFace(GL_BACK);

		m_DirectionalLightRenderTarget->Unbind();
	}
}
