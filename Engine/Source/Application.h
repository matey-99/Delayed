#pragma once

#include <GLFW/glfw3.h>

#include "typedefs.h"

class Application
{
public:
	Application();
	~Application();

	static Ref<Application> Create(std::string name);

	void Run();
	void RunEditor();

	static void Exit();
	void ExitApp();

	static void WindowErrorCallback(int error, const char* description);

private:
	static Ref<Application> s_Instance;

	GLFWwindow* m_Window;
};