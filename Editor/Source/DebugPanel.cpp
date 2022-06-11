#include "DebugPanel.h"

#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Input/Input.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderPass/ShadowsPass.h"

DebugPanel::DebugPanel(Ref<Editor> editor) 
	: m_Editor(editor)
{
	auto r = Renderer::GetInstance();
	uint32_t dirLightShadowMaps = r->m_ShadowsPass->GetDirectionalLightRenderTarget()->GetTargets()[0];
	glGenTextures(4, m_ShadowMaps);
	for (int i = 0; i < 4; i++)
	{
		glTextureView(m_ShadowMaps[i], GL_TEXTURE_2D, dirLightShadowMaps, GL_DEPTH_COMPONENT32F, 0, 1, i, 1);
	}

	m_DisplayShadowMaps = false;
}

void DebugPanel::Render()
{
	ImGui::Begin("Debug");

	ImGui::Text("Framerate: %.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	auto renderer = Renderer::GetInstance();
	ImGui::Text("Window Size: x = %d, y = %d", renderer->GetWindowWidth(), renderer->GetWindowHeight());
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	glm::vec2 mousePos = Input::GetInstance()->GetMousePosition();
	ImGui::Text("Mouse Position: x = %.1f, y = %.1f", mousePos.x, mousePos.y);
	ImGui::Dummy(ImVec2(0.0f, 10.0f));

	auto selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
	if (selectedActor)
	{
		ImGui::Text("Selected actor: ");
		ImGui::SameLine();
		ImGui::Text(selectedActor->GetName().c_str());

		if (selectedActor->GetComponent<StaticMeshComponent>())
			ImGui::Text("Vertices: %i", selectedActor->GetComponent<StaticMeshComponent>()->GetRenderedVerticesCount());
	}

	ImGui::Checkbox("Display Shadow Maps", &m_DisplayShadowMaps);
	if (m_DisplayShadowMaps)
	{
		for (int i = 0; i < 4; i++)
		{
			ImGui::Image((void*)m_ShadowMaps[i], ImVec2(128, 128));
			if (i % 2 == 0)
				ImGui::SameLine();
		}
	}


	ImGui::End();

	



}
