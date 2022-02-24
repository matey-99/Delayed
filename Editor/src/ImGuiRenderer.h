#pragma once

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "ImGuizmo.h"

#include "Editor.h"
#include "Scene/Scene.h"
#include "Renderer/Framebuffer.h"

class ImGuiRenderer
{
private:
	ImGuiIO* m_IO;

	Ref<Editor> m_Editor;
	Ref<Scene> m_Scene;

	glm::vec2 m_ViewportSize;

public:
	ImGuiRenderer();

	void Setup(GLFWwindow* window, const char* glsl_version, Ref<Scene> scene);
	void Render();
	void EndFrame();
	void CleanUp();

	void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

	inline Ref<Editor> GetEditor() const { return m_Editor; }

	friend class Editor;
};