#include "ImGuiRenderer.h"

#include "Scene/SceneSerializer.h"
#include "Scene/SceneManager.h"

ImGuiRenderer::ImGuiRenderer()
{
    m_IsCreateSceneWindowVisible = false;
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

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                m_IsCreateSceneWindowVisible = true;
            }

            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }

    if (m_IsCreateSceneWindowVisible)
    {
        ImGui::Begin("Create Scene");

        size_t maxSize = 128;
        char* name = (char*)m_CreateSceneName.c_str();
        ImGui::InputText("##Name", name, maxSize);
        m_CreateSceneName = name;
        ImGui::Dummy(ImVec2(0.0f, 10.0f));

        if (ImGui::Button("Save"))
        {
            m_IsCreateSceneWindowVisible = false;
            SceneManager::GetInstance()->CreateScene(m_CreateSceneName);
        }
        ImGui::End();
    }

    ImGui::Begin("Camera");
    ImGui::Text("Position: x = %f, y = %f, z = %f", m_Scene->GetCamera()->Position.x, m_Scene->GetCamera()->Position.y, m_Scene->GetCamera()->Position.z);
    ImGui::Text("Rotation: yaw = %f, pitch = %f", m_Scene->GetCamera()->Yaw, m_Scene->GetCamera()->Pitch);
    ImGui::DragFloat("Movement speed", &m_Scene->GetCamera()->m_MovementSpeed);
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
