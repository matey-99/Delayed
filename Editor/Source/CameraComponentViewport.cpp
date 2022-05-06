#include "CameraComponentViewport.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Editor.h"
#include "Math/Math.h"
#include "Renderer/RenderTarget.h"

CameraComponentViewport::CameraComponentViewport(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{
    m_Locked = false;
}

void CameraComponentViewport::Render(Ref<CameraComponent> camera)
{
    if (camera)
        m_CameraComponent = camera;

    auto renderer = Renderer::GetInstance();
    renderer->Render(m_Scene, m_CameraComponent, 1);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Camera Viewport");

    ImGui::Checkbox("Lock", &m_Locked);

    uint32_t viewportTexture = renderer->GetOutput(1);
    ImGui::Image((void*)viewportTexture, ImVec2(480, 270), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
    ImGui::PopStyleVar();
}
