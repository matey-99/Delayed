#pragma once

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <GLFW/glfw3.h>

#include "Patterns/Singleton.h"
#include "typedefs.h"

struct KeyboardActionMapping
{
public:
	KeyboardActionMapping(std::string name, int key)
		: m_Name(name), m_Key(key)
	{}

	std::string m_Name;
	int m_Key;
	std::vector<std::function<void()>> m_Functions;
};

struct KeyboardAxisMapping
{
public:
	KeyboardAxisMapping(std::string name, std::unordered_map<int, float> keys)
		: m_Name(name), m_Keys(keys)
	{}

	std::string m_Name;
	std::unordered_map<int, float> m_Keys;
	std::vector<std::function<void(float)>> m_Functions;
};

class Input : public Singleton<Input>
{
public:
	Input();
	~Input();

	void ProcessKeyboardInput(GLFWwindow* window);

	// Keyboard Actions
	Ref<KeyboardActionMapping> FindActionMapping(std::string name);
	void MakeAction(Ref<KeyboardActionMapping> actionMapping);
	void BindAction(std::string actionName, std::function<void()> function);
	void ClearAction(std::string actionName);

	// Keyboard Axes
	Ref<KeyboardAxisMapping> FindAxisMapping(std::string name);
	void MakeAxis(Ref<KeyboardAxisMapping> axisMapping, float value);
	void BindAxis(std::string axisName, std::function<void(float)> function);
	void ClearAxis(std::string axisName);

private:
	std::vector<Ref<KeyboardActionMapping>> m_KeyboardActionMappings;
	std::vector<Ref<KeyboardAxisMapping>> m_KeyboardAxisMappings;
};