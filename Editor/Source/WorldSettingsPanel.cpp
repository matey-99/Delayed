#include "WorldSettingsPanel.h"

#include "Scene/SceneSerializer.h"
#include "Scene/SceneManager.h"

WorldSettingsPanel::WorldSettingsPanel(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{

}

void WorldSettingsPanel::Render()
{
    ImGui::Begin("World Settings");

    size_t maxSize = 128;
    std::string cameraIDStr = std::to_string(m_Scene->m_CurrentCamera->GetOwner()->GetID());
    char* id = (char*)cameraIDStr.c_str();
    ImGui::InputText("Current Camera ID", id, maxSize);
    m_Scene->m_CurrentCamera->GetOwner()->SetID(std::atoll(id));

    if (ImGui::Button("Save scene"))
        SceneSerializer::Serialize(m_Scene, "../../../Content/" + SceneManager::GetInstance()->GetCurrentScenePath());

    ImGui::End();
}
