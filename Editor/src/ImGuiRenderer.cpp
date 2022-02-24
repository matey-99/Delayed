#include "ImGuiRenderer.h"

#include "Scene/SceneSerializer.h"

ImGuiRenderer::ImGuiRenderer()
{
}

void ImGuiRenderer::Setup(GLFWwindow* window, const char* glsl_version, Ref<Scene> scene)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    m_IO = &ImGui::GetIO(); (void)m_IO;

    m_IO->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    m_IO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    m_IO->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    m_IO->WantCaptureMouse = true;
    m_IO->WantCaptureKeyboard = true;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    m_Scene = scene;

    m_Editor = Editor::GetInstance();
    m_Editor->Initialize(m_Scene);
}

void ImGuiRenderer::Render()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Camera");
    ImGui::Text("Position: x = %f, y = %f, z = %f", m_Scene->GetCamera()->Position.x, m_Scene->GetCamera()->Position.y, m_Scene->GetCamera()->Position.z);
    ImGui::Text("Rotation: yaw = %f, pitch = %f", m_Scene->GetCamera()->Yaw, m_Scene->GetCamera()->Pitch);
    ImGui::DragFloat("Movement speed", &m_Scene->GetCamera()->MovementSpeed);
    ImGui::End();

    ImGui::Begin("Toolbar");
    if (ImGui::Button("Translate"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
    ImGui::SameLine();
    if (ImGui::Button("Rotate"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
    ImGui::SameLine();
    if (ImGui::Button("Scale"))
        m_Editor->SetGizmoOperation(ImGuizmo::OPERATION::SCALE);

    std::string buttonText = m_Editor->IsPlayMode() ? "Exit" : "Play";
    ImGui::SameLine();
    if (ImGui::Button(buttonText.c_str()))
        m_Editor->ManagePlayMode();
    ImGui::End();

    m_Editor->Render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiRenderer::EndFrame()
{
    if (m_IO->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void ImGuiRenderer::CleanUp()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiRenderer::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}
