#include <stdio.h>
#include <iostream>

#include <stb_image.h>

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
#include "Scene/SceneManager.h"
#include "Content/ContentHelper.h"
#include "Material/Material.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Light/Light.h"
#include "Input/Input.h"
#include "Camera/CameraManager.h"
#include "Renderer/RenderTools.h"

#define FPS 60.0f
#define MS_PER_UPDATE 1 / FPS

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
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "PBL", NULL, NULL);
    if (window == NULL)
        return 1;

    glfwMaximizeWindow(window);
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

    // Initialize FMOD
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

    // RENDERER
    auto renderer = Renderer::GetInstance();
    renderer->Initialize();

    // SCENE
    auto sceneManager = SceneManager::GetInstance();
    auto scene = sceneManager->LoadScene(ContentHelper::GetAssetPath("Scenes/MainMenu.scene"));

    // CAMERA
    auto cameraManager = CameraManager::GetInstance();
    cameraManager->SetMainCamera(scene->GetCurrentCamera());

    // INPUT
    auto input = Input::GetInstance();
    input->Initialize(window);

    // SHADER
    auto screenShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::PostProcessing, "Screen");

    // TIME
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
    float lag = 0.0f;

    // START
    scene->Start();

    lastFrame = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        // INPUTS
        glfwPollEvents();
        input->Process();

        // UPDATE
        scene = sceneManager->GetCurrentScene();
        scene->Update(deltaTime);

        // Check if window has changed
        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        if (windowWidth != renderer->GetWindowWidth() || windowHeight != renderer->GetWindowHeight())
        {
            renderer->ResizeWindow(windowWidth, windowHeight);
            cameraManager->GetMainCamera()->SetAspectRatio(glm::vec2(windowWidth, windowHeight));
            scene->GetUIRoot()->GetTransform()->CalculateWorldModelMatrix();
        }

        // RENDER
        renderer->Render(scene, cameraManager->GetMainCamera());

        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        screenShader->Use();
        screenShader->SetInt("u_Screen", 0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderer->GetOutput());

        glDisable(GL_DEPTH_TEST);

        RenderTools::GetInstance()->RenderQuad();

        glEnable(GL_DEPTH_TEST);

        glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}