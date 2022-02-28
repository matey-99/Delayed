#include "DebugPanel.h"

#include "Editor.h"
#include "Scene/Component/StaticMeshComponent.h"

DebugPanel::DebugPanel(Ref<Editor> editor) 
	: m_Editor(editor)
{
}

void DebugPanel::Render()
{
	ImGui::Begin("Debug");

	ImGui::Text("Framerate: %.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000 / ImGui::GetIO().Framerate);

	auto selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
	if (selectedActor)
	{
		ImGui::Text("Selected actor: ");
		ImGui::SameLine();
		ImGui::Text(selectedActor->GetName().c_str());

		if (selectedActor->GetComponent<StaticMeshComponent>())
			ImGui::Text("Vertices: %i", selectedActor->GetComponent<StaticMeshComponent>()->GetRenderedVerticesCount());
	}

	ImGui::End();
}
