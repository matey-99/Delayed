#include "Viewport.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Importer/MeshImporter.h"
#include "Editor.h"
#include "Math/Math.h"
#include "Content/ContentHelper.h"
#include "Scene/Component/Light/DirectionalLight.h"
#include "Scene/Component/Light/SpotLight.h"

Viewport::Viewport(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{
    m_DirectionArrowShader = CreateRef<Shader>("Gizmos",
                                               ContentHelper::GetAssetPath("Shaders/Editor/Gizmos.vert"),
                                               ContentHelper::GetAssetPath("Shaders/Editor/Gizmos.frag"));
    m_DirectionArrow = MeshImporter::GetInstance()->ImportMesh("Models/editor/DirectionArrow.fbx");
}

void Viewport::Render(uint32_t bufferTexture)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    



    m_Hovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_Size != *((glm::vec2*)&viewportPanelSize))
    {
        m_Size = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        //framebuffer->Resize((unsigned int)m_Size.x, (unsigned int)m_Size.y);

        m_Editor->GetCamera()->AspectRactio = { m_Size.x, m_Size.y };
    }
    uint32_t viewportTexture = bufferTexture;
    ImGui::Image((void*)viewportTexture, ImVec2(m_Size.x, m_Size.y), ImVec2(0, 1), ImVec2(1, 0));

    Ref<Actor> selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
    if (selectedActor)
    {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        Ref<EditorCamera> camera = m_Editor->GetCamera();

        auto transform = selectedActor->GetTransform();

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix();
        glm::mat4 model = transform->GetWorldModelMatrix();

        ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
            m_Editor->GetGizmoOperation(), ImGuizmo::LOCAL, glm::value_ptr(model));

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 position, rotation, scale;
            Math::DecomposeMatrix(model, position, rotation, scale);

            switch (m_Editor->GetGizmoOperation())
            {
            case ImGuizmo::OPERATION::TRANSLATE:
                selectedActor->GetTransform()->SetWorldPosition(position);
                break;
            case ImGuizmo::OPERATION::ROTATE:
                selectedActor->GetTransform()->SetLocalRotation(glm::degrees(rotation));
                break;
            case ImGuizmo::OPERATION::SCALE:
                selectedActor->GetTransform()->SetLocalScale(scale);
                break;
            }
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();


}

void Viewport::RenderGizmos()
{
    //Ref<Actor> selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
    //if (selectedActor)
    //{
    //    if (selectedActor->GetComponent<DirectionalLight>() || selectedActor->GetComponent<SpotLight>())
    //    {
    //        auto renderer = Renderer::GetInstance();
    //        renderer->GetMainSceneFramebuffer()->Bind();

    //        glDisable(GL_DEPTH_TEST);

    //        glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(selectedActor->GetTransform()->GetWorldRotation())));
    //        glm::mat4 model = glm::translate(glm::mat4(1.0f), selectedActor->GetTransform()->GetWorldPosition()) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.2f));

    //        m_DirectionArrowShader->Use();
    //        m_DirectionArrowShader->SetMat4("u_Model", model);
    //        for (auto mesh : m_DirectionArrow)
    //        {
    //            mesh.Render();
    //        }

    //        glEnable(GL_DEPTH_TEST);

    //        renderer->GetMainSceneFramebuffer()->Unbind();
    //    }
    //}
}
