#pragma once

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <GLFW/glfw3.h>

#include "typedefs.h"

struct KeyboardActionMapping
{
public:
	KeyboardActionMapping(std::string name, int key)
		: m_Name(name), m_Key(key)
	{}

	std::string m_Name;
	int m_Key;
	std::vector<std::function<void()>> m_Actions;
};

class Input
{
public:
	Input();
	~Input();

	Input(Input& other) = delete;
	void operator=(const Input&) = delete;

	static Ref<Input> GetInstance();

	void ProcessKeyboardInput(GLFWwindow* window);

	Ref<KeyboardActionMapping> FindActionMapping(std::string name);
	void MakeAction(Ref<KeyboardActionMapping> ActionMapping);

	void BindInput(std::string actionName, std::function<void()> action);
	void ClearBinding(std::string actionName);

private:
	static Ref<Input> s_Instance;
	static std::mutex s_Mutex;

	std::vector<Ref<KeyboardActionMapping>> m_KeyboardActionMappings;
};