#include <stdio.h>
#include <iostream>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <stb_image.h>

#include "ImGuiRenderer.h"

#include <glad/glad.h>  // Initialize with gladLoadGL()

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fmod.hpp>
#include <fmod_errors.h>

#include "typedefs.h"
#include "Scene/Scene.h"
#include "Scene/SceneSerializer.h"
#include "Editor.h"
#include "Material/Material.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Light/Light.h"
#include "Input/Input.h"
#include "Scene/SceneManager.h"
#include "Content/ContentHelper.h"
#include "Camera/CameraManager.h"

#define FPS 60.0f
#define MS_PER_UPDATE 1 / FPS

Ref<Scene> scene = Ref<Scene>();
Ref<Editor> editor = Ref<Editor>();

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lag = 0.0f;

bool rotateCamera = false;
bool moveCamera = false;

bool isPlayMode = false;
bool isViewportHovered = false;

const float mouseSensitivity = 0.1f;
const float scrollSensitivity = 0.1f;
float lastMouseX = 400, lastMouseY = 300;

bool leftCtrlClicked = false;
bool keyDClicked = false;

void ProcessKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        editor->GetCamera()->Move(CameraMovement::Forward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        editor->GetCamera()->Move(CameraMovement::Backward, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        editor->GetCamera()->Move(CameraMovement::Left, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)// || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        editor->GetCamera()->Move(CameraMovement::Right, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::ROTATE);
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        Editor::GetInstance()->SetGizmoOperation(ImGuizmo::OPERATION::SCALE);
}

void ProcessMouseInput(GLFWwindow* window)
{
    if (isViewportHovered)
    {
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, xpos, ypos);
            moveCamera = true;
        }
        else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPos(window, xpos, ypos);

            rotateCamera = true;
            moveCamera = false;
        }
    }

    if (moveCamera && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        moveCamera = false;
    }
    else if (rotateCamera && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        rotateCamera = false;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_LEFT_CONTROL)
    {
        if (action == GLFW_PRESS)
            leftCtrlClicked = true;
        else if (action == GLFW_RELEASE)
            leftCtrlClicked = false;
    }

    if (key == GLFW_KEY_D)
    {
        if (action == GLFW_PRESS)
            keyDClicked = true;
        else if (action == GLFW_RELEASE)
            keyDClicked = false;
    }

    if (leftCtrlClicked && keyDClicked)
        Editor::GetInstance()->GetSceneHierarchyPanel()->DuplicateSelectedActor();

    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    float xoffset = xpos - lastMouseX;
    float yoffset = lastMouseY - ypos;
    lastMouseX = xpos;
    lastMouseY = ypos;

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    if (rotateCamera)
    {
        editor->GetCamera()->Rotate(xoffset, yoffset, deltaTime);
    }

    if (moveCamera)
    {
        editor->GetCamera()->Move(xoffset, yoffset, deltaTime);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (isViewportHovered && !isPlayMode)
        editor->GetCamera()->Move(yoffset, deltaTime);

    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 450";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1440, 900, "PBL", NULL, NULL);
    if (window == NULL)
        return 1;

    //glfwMaximizeWindow(window);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    // Initialize OpenGL loader
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    FMOD::System* system = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) $s\n", result, FMOD_ErrorString(result));
        return 1;
    }

    result = system->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) $s\n", result, FMOD_ErrorString(result));
        return 1;
    }

    Renderer::GetInstance()->Initialize();

    auto sceneManager = SceneManager::GetInstance();
    sceneManager->LoadScene(ContentHelper::GetAssetPath("Scenes/MainMenu.scene"));

    scene = sceneManager->GetCurrentScene();

    ImGuiRenderer imGuiRenderer = ImGuiRenderer();
    imGuiRenderer.Setup(window, glsl_version, scene);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    editor = Editor::GetInstance();
    auto cameraManager = CameraManager::GetInstance();
    cameraManager->GetInstance()->SetMainCamera(editor->GetCamera());
    auto input = Input::GetInstance();
    input->Initialize(window);

    editor->Start();

    lastFrame = glfwGetTime();

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        scene = sceneManager->GetCurrentScene();

        isViewportHovered = imGuiRenderer.GetEditor()->GetViewport()->IsHovered();

        if (isViewportHovered)
            ProcessKeyboardInput(window);

        ProcessMouseInput(window);

        glfwPollEvents();

        editor->Update(deltaTime);

        editor->RenderScene();
        editor->GetViewport()->RenderGizmos();

        imGuiRenderer.Render();
        imGuiRenderer.EndFrame();

        glfwSwapBuffers(window);
    }

    imGuiRenderer.CleanUp();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}