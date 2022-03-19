#include "ShadowsPass.h"

#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/StaticMeshComponent.h"

ShadowsPass::ShadowsPass()
{
	RenderTarget::Config directionalLightConfig;
	directionalLightConfig.Attachment = RenderTarget::Attachment::Depth;
	directionalLightConfig.Type = RenderTarget::Type::Texture2DArray;
	directionalLightConfig.DepthInternalFormat = RenderTarget::DepthInternalFormat::Depth32F;

	m_DirectionalLightRenderTarget = RenderTarget::Create(directionalLightConfig, 4096, 4096);
}

ShadowsPass::~ShadowsPass()
{
}

void ShadowsPass::Render(Ref<Scene> scene)
{
	// Directional Light
	if (scene->FindComponent<DirectionalLight>())
	{
		m_DirectionalLightRenderTarget->Bind();

		auto depthShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::Calculations, "SceneDepth");
		depthShader->Use();

		glClear(GL_DEPTH_BUFFER_BIT);
		glCullFace(GL_BACK);

		for (auto a : scene->GetActors())
		{
			if (auto smc = a->GetComponent<StaticMeshComponent>())
			{
				depthShader->SetMat4("u_Model", a->GetTransform()->GetWorldModelMatrix());

				for (auto mesh : smc->GetMeshes())
					mesh.Render();
			}
		}

		glCullFace(GL_BACK);

		m_DirectionalLightRenderTarget->Unbind();
	}
}
