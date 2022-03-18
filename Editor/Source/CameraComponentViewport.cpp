#include "CameraComponentViewport.h"

#include <glm/gtc/type_ptr.hpp>
#include <ImGuizmo.h>

#include "Editor.h"
#include "Math/Math.h"
#include "Renderer/RenderTarget.h"

CameraComponentViewport::CameraComponentViewport(Ref<Editor> editor, Ref<Scene> scene)
	: m_Editor(editor), m_Scene(scene)
{

}

void CameraComponentViewport::Render(Ref<CameraComponent> camera)
{
	/*m_Scene->PreRender();

	glClearColor(m_Scene->GetBackgroundColor()->x, m_Scene->GetBackgroundColor()->y, m_Scene->GetBackgroundColor()->z, m_Scene->GetBackgroundColor()->w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderer = Renderer::GetInstance();

	renderer->GetCameraVertexUniformBuffer()->Bind();
	renderer->GetCameraVertexUniformBuffer()->SetUniform(0, sizeof(glm::mat4), glm::value_ptr(camera->GetViewProjectionMatrix()));
	renderer->GetCameraVertexUniformBuffer()->SetUniform(64, sizeof(glm::mat4), glm::value_ptr(camera->GetViewMatrix()));
	renderer->GetCameraVertexUniformBuffer()->SetUniform(64 * 2, sizeof(glm::mat4), glm::value_ptr(camera->GetProjectionMatrix()));
	renderer->GetCameraVertexUniformBuffer()->Unbind();

	renderer->GetCameraFragmentUniformBuffer()->Bind();
	renderer->GetCameraFragmentUniformBuffer()->SetUniform(0, sizeof(glm::vec3), glm::value_ptr(camera->GetOwner()->GetTransform()->GetWorldPosition()));
	renderer->GetCameraFragmentUniformBuffer()->Unbind();

	m_Scene->Render();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Camera Viewport");

    uint32_t viewportTexture = m_ViewportFramebuffer->GetColorAttachments().at(0);
    ImGui::Image((void*)viewportTexture, ImVec2(480, 270), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
    ImGui::PopStyleVar();*/


}
