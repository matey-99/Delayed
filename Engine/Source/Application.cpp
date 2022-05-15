#include "Application.h"

#include <glad/glad.h>

#include <fmod.hpp>
#include <fmod_errors.h>

#include "Input/Input.h"
#include "Time/Time.h"
#include "Renderer/Renderer.h"
#include "Renderer/RenderTools.h"
#include "Scene/SceneManager.h"
#include "Camera/CameraManager.h"
#include "Analysis/Profiler.h"
#include "Audio/AudioSystem.h"

Ref<Application> Application::s_Instance{};

Application::Application()
{
}

Application::~Application()
{
}

Ref<Application> Application::Create(std::string name)
{
    if (Application::s_Instance)
    {
        std::cout << "Application already exists!" << std::endl;
        return Application::s_Instance;
    }

    Ref<Application> app = CreateRef<Application>();

    // Setup window
    glfwSetErrorCallback(WindowErrorCallback);
    if (!glfwInit())
        return Ref<Application>();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    app->m_Window = glfwCreateWindow(1920, 1080, name.c_str(), NULL, NULL);
    if (app->m_Window == NULL)
        return Ref<Application>();

    glfwMaximizeWindow(app->m_Window);
    glfwMakeContextCurrent(app->m_Window);
    glfwSwapInterval(1); // Enable vsync

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(app->m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (err)
    {
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return Ref<Application>();
    }

    // Initialize FMOD
    FMOD::System* system = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) $s\n", result, FMOD_ErrorString(result));
        return Ref<Application>();
    }

    result = system->init(512, FMOD_INIT_NORMAL, 0);
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) $s\n", result, FMOD_ErrorString(result));
        return Ref<Application>();
    }

    s_Instance = app;
    return app;
}

void Application::Run()
{
    // TIME
    auto time = Time::GetInstance();

    // ANALYSIS
    auto profiler = Profiler::GetInstance();
    profiler->SetEnabled(true);

    // INPUT
    auto input = Input::GetInstance();
    input->Initialize(m_Window);

    // RENDERER
    auto renderer = Renderer::GetInstance();
    renderer->Initialize();

    // SCENE
    auto sceneManager = SceneManager::GetInstance();
    auto scene = sceneManager->LoadScene("Scenes/MainMenu.scene");

    // CAMERA
    auto cameraManager = CameraManager::GetInstance();

    // AUDIO
    auto audioSystem = AudioSystem::GetInstance();
    audioSystem->Initialize();

    time->SetLastFrameTime(glfwGetTime());
    while (!glfwWindowShouldClose(m_Window))
    {
        PROFILER_START("Frame Time");

        // INPUTS
        glfwPollEvents();
        input->Process();

        // FIXED UPDATES & UPDATE
        time->SetCurrentFrameTime(glfwGetTime());
        scene = sceneManager->GetCurrentScene();
        time->Tick(scene);

        //AUDIO
        audioSystem->Update(time->GetDeltaTime());

        // Check if window has changed
        int windowWidth, windowHeight;
        glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);
        if (windowWidth != renderer->GetWindowWidth() || windowHeight != renderer->GetWindowHeight())
        {
            renderer->ResizeWindow(windowWidth, windowHeight);
            cameraManager->GetMainCamera()->SetAspectRatio(glm::vec2(windowWidth, windowHeight));
            scene->GetUIRoot()->GetTransform()->CalculateWorldModelMatrix();
        }

        // RENDER
        renderer->Render(scene, cameraManager->GetMainCamera());
        renderer->Display();

        PROFILER_STOP();

        // ANALYSIS
        profiler->Update();

        glfwSwapBuffers(m_Window);
    }

    audioSystem->Shutdown();

    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void Application::Exit()
{
    s_Instance->ExitApp();
}

void Application::ExitApp()
{
    glfwSetWindowShouldClose(m_Window, GLFW_TRUE);
}

void Application::WindowErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}
