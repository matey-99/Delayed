#include "Viewport.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Editor.h"
#include "Math/Math.h"

Viewport::Viewport(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{

}

void Viewport::Render(Ref<Framebuffer> framebuffer)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");

    m_Hovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_Size != *((glm::vec2*)&viewportPanelSize))
    {
        m_Size = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        framebuffer->Resize((unsigned int)m_Size.x, (unsigned int)m_Size.y);

        m_Scene->GetCamera()->AspectRactio = { m_Size.x, m_Size.y };
    }
    uint32_t viewportTexture = framebuffer->GetColorAttachment();
    ImGui::Image((void*)viewportTexture, ImVec2(m_Size.x, m_Size.y), ImVec2(0, 1), ImVec2(1, 0));

    Ref<Entity> selectedEntity = m_Editor->GetSceneHierarchyPanel()->GetSelectedEntity();
    if (selectedEntity)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        Ref<Camera> camera = m_Scene->GetCamera();

        Transform transform = selectedEntity->GetTransform();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 model = transform.ModelMatrix;

        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
            m_Editor->GetGizmoOperation(), ImGuizmo::LOCAL, glm::value_ptr(model));

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 position, rotation, scale;
            Math::DecomposeMatrix(model, position, rotation, scale);

            switch (m_Editor->GetGizmoOperation())
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                selectedEntity->SetWorldPosition(position);
                break;
            case ImGuizmo::OPERATION::ROTATE:
                selectedEntity->SetLocalRotation(glm::degrees(rotation));
                break;
            case ImGuizmo::OPERATION::SCALE:
                selectedEntity->SetLocalScale(scale);
                break;
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();


}
