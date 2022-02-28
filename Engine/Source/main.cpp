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
#include "Material/Material.h"
#include "Scene/Component/StaticMeshComponent.h"
#include "Scene/Component/Light/Light.h"
#include "Renderer/Framebuffer.h"
#include "Input/Input.h"

#define FPS 60.0f
#define MS_PER_UPDATE 1 / FPS

Ref<Scene> scene = Ref<Scene>();

float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lag = 0.0f;

bool rotateCamera = false;
bool moveCamera = false;

const float mouseSensitivity = 0.1f;
const float scrollSensitivity = 0.1f;
float lastMouseX = 400, lastMouseY = 300;

void ProcessKeyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void ProcessMouseInput(GLFWwindow* window)
{
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

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

    FMOD_RESULT result;
    FMOD::System* system = nullptr;

    result = FMOD::System_Create(&system);
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

    Renderer::GetInstance()->InitializeMainSceneFramebuffer();
    Renderer::GetInstance()->InitializePostProcessingFramebuffer();
    Renderer::GetInstance()->InitializeShadowMapFramebuffers();

    Renderer::GetInstance()->InitializePostProcessing();

    auto sceneManager = SceneManager::GetInstance();
    sceneManager->LoadScene("../../../Assets/Scenes/Untitled2.scene");

    scene = sceneManager->GetCurrentScene();

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    auto renderer = Renderer::GetInstance();
    auto input = Input::GetInstance();
    auto screenShader = ShaderLibrary::GetInstance()->GetShader(ShaderType::POST_PROCESSING, "Screen");


    bool shouldRender = false;
    lastFrame = glfwGetTime();
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        lag += deltaTime;

        ProcessKeyboardInput(window);
        ProcessMouseInput(window);

        glfwPollEvents();

        scene = sceneManager->GetCurrentScene();

        while (lag >= MS_PER_UPDATE)
        {
            scene->Update();
            scene->Tick(deltaTime);

            shouldRender = true;
            lag -= MS_PER_UPDATE;
        }

        if (shouldRender)
        {
            renderer->RenderScene(scene);

            if (renderer->IsPostProcessing())
                renderer->AddPostProcessingEffects();

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            screenShader->Use();
            screenShader->SetInt("u_Screen", 0);
            glBindVertexArray(renderer->GetPostProcessingVAO());
            glDisable(GL_DEPTH_TEST);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, renderer->IsPostProcessing() ? renderer->GetPostProcessingFramebuffer()->GetColorAttachment() :
                renderer->GetMainSceneFramebuffer()->GetColorAttachment());
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glBindVertexArray(0);
            glEnable(GL_DEPTH_TEST);

            glfwSwapBuffers(window);

            shouldRender = false;
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}