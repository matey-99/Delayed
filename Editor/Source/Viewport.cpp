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
#include "Renderer/RenderTools.h"
#include "Scene/Component/UI/RectTransformComponent.h"
#include "Scene/Component/Collider/BoxColliderComponent.h"
#include "Scene/Component/Collider/SphereColliderComponent.h"

Viewport::Viewport(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{
    RenderTarget::Config gizmosConfig;
    gizmosConfig.Attachment = RenderTarget::Attachment::Color;
    gizmosConfig.Type = RenderTarget::Type::Texture2D;
    gizmosConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;
    m_GizmosRenderTarget = RenderTarget::Create(gizmosConfig, 1920, 1080);

    RenderTarget::Config viewportConfig;
    viewportConfig.Attachment = RenderTarget::Attachment::Color;
    viewportConfig.Type = RenderTarget::Type::Texture2D;
    viewportConfig.ColorInternalFormat = RenderTarget::ColorInternalFormat::RGBA16F;
    m_ViewportRenderTarget = RenderTarget::Create(viewportConfig, 1920, 1080);


    m_DirectionArrowShader = CreateRef<Shader>("Gizmos",
                                               ContentHelper::GetAssetPath("Shaders/Editor/Gizmos.vert"),
                                               ContentHelper::GetAssetPath("Shaders/Editor/Gizmos.frag"));

    m_ColliderShader = CreateRef<Shader>("Collider",
                                               ContentHelper::GetAssetPath("Shaders/Editor/Collider.vert"),
                                               ContentHelper::GetAssetPath("Shaders/Editor/Collider.frag"));

    m_ViewportShader = CreateRef<Shader>("Viewport", 
                                         ContentHelper::GetAssetPath("Shaders/Editor/Viewport.vert"),
                                         ContentHelper::GetAssetPath("Shaders/Editor/Viewport.frag"));

    m_DirectionArrow = MeshImporter::GetInstance()->ImportMesh("Models/editor/DirectionArrow.fbx");
}

void Viewport::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Viewport");
    
    m_Hovered = ImGui::IsWindowHovered();

    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    if (m_Size != *((glm::vec2*)&viewportPanelSize))
    {
        m_Size = glm::vec2(viewportPanelSize.x, viewportPanelSize.y);
        Renderer::GetInstance()->ResizeWindow((unsigned int)m_Size.x, (unsigned int)m_Size.y);

        m_GizmosRenderTarget->Update((unsigned int)m_Size.x, (unsigned int)m_Size.y);
        m_ViewportRenderTarget->Update((unsigned int)m_Size.x, (unsigned int)m_Size.y);

        m_Editor->GetCamera()->AspectRactio = { m_Size.x, m_Size.y };

        m_Scene->GetUIRoot()->GetTransform()->CalculateWorldModelMatrix();
    }
    uint32_t viewportTexture = m_ViewportRenderTarget->GetTargets()[0];
    ImGui::Image((void*)viewportTexture, ImVec2(m_Size.x, m_Size.y), ImVec2(0, 1), ImVec2(1, 0));

    Ref<Actor> selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
    if (selectedActor)
    {
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

        Ref<EditorCamera> camera = m_Editor->GetCamera();

        auto transform = selectedActor->GetTransform();

        glm::mat4 model = transform->GetWorldModelMatrix();

        if (selectedActor->GetComponent<TransformComponent>())
        {
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = camera->GetProjectionMatrix();

            ImGuizmo::SetOrthographic(false);

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
        else if (auto rect = selectedActor->GetComponent<RectTransformComponent>())
        {
            glm::mat4 projection(1.0f);
            glm::mat4 view(1.0f);

            ImGuizmo::SetOrthographic(true);

            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(projection),
                m_Editor->GetGizmoOperation(), ImGuizmo::LOCAL, glm::value_ptr(model));

            if (ImGuizmo::IsUsing())
            {
                glm::vec3 position, rotation, scale;
                Math::DecomposeMatrix(model, position, rotation, scale);

                auto r = Renderer::GetInstance();
                position.x *= r->GetWindowWidth() + rect->GetAnchor().x * r->GetWindowWidth();
                position.y *= r->GetWindowHeight() + rect->GetAnchor().y * r->GetWindowHeight();

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
    }

    ImGui::End();
    ImGui::PopStyleVar();


}

void Viewport::RenderGizmos()
{
    // Gizmos
    m_GizmosRenderTarget->Bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    bool isGizmos = false;
    Ref<Actor> selectedActor = m_Editor->GetSceneHierarchyPanel()->GetSelectedActor();
    if (selectedActor)
    {
        if (selectedActor->GetComponent<DirectionalLight>() || selectedActor->GetComponent<SpotLight>())
        {
            isGizmos = true;

            glDisable(GL_DEPTH_TEST);

            glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(selectedActor->GetTransform()->GetWorldRotation())));
            glm::mat4 model = glm::translate(glm::mat4(1.0f), selectedActor->GetTransform()->GetWorldPosition()) * rotation * glm::scale(glm::mat4(1.0f), glm::vec3(0.05f, 0.05f, 0.2f));

            m_DirectionArrowShader->Use();
            m_DirectionArrowShader->SetMat4("u_Model", model);
            for (auto mesh : m_DirectionArrow)
            {
                mesh.Render();
            }

            glEnable(GL_DEPTH_TEST);
        }
        
        if (auto c = selectedActor->GetComponent<BoxColliderComponent>())
        {
            isGizmos = true;

            glDisable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            m_ColliderShader->Use();

            RenderTools::GetInstance()->RenderBoundingBox(c->GetBoundingBox());

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
        }

        if (auto c = selectedActor->GetComponent<SphereColliderComponent>())
        {
            isGizmos = true;

            glDisable(GL_DEPTH_TEST);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            m_ColliderShader->Use();

            RenderTools::GetInstance()->RenderBoundingSphere(c->GetBoundingSphere());

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glEnable(GL_DEPTH_TEST);
        }
    }
    m_GizmosRenderTarget->Unbind();


    // Merging output scene with gizmos
    m_ViewportRenderTarget->Bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Renderer::GetInstance()->GetOutput());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GizmosRenderTarget->GetTargets()[0]);

    m_ViewportShader->Use();
    m_ViewportShader->SetInt("u_Screen", 0);
    m_ViewportShader->SetInt("u_Gizmos", 1);

    RenderTools::GetInstance()->RenderQuad();

    glEnable(GL_DEPTH_TEST);

    m_ViewportRenderTarget->Unbind();

}
