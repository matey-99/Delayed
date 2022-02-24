#include "WorldSettingsPanel.h"

#include "Scene/SceneSerializer.h"

WorldSettingsPanel::WorldSettingsPanel(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{

}

void WorldSettingsPanel::Render()
{
    ImGui::Begin("World Settings");

    auto bg = &m_Scene->m_BackgroundColor;
    ImGui::ColorEdit3("Background color", (float*)bg);

    if (ImGui::Button("Save scene"))
        SceneSerializer::Serialize(m_Scene);

    ImGui::End();
}
