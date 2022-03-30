#include "Input.h"

Ref<Input> Input::s_Instance{};
std::mutex Input::s_Mutex;

Input::Input()
{
	// This should be done in some config file
	std::unordered_map<int, float> moveForwardBinding;
	moveForwardBinding.insert({ GLFW_KEY_W, 1.0f });
	moveForwardBinding.insert({ GLFW_KEY_S, -1.0f });

	std::unordered_map<int, float> moveRightBinding;
	moveRightBinding.insert({ GLFW_KEY_D, 1.0f });
	moveRightBinding.insert({ GLFW_KEY_A, -1.0f });

	m_KeyboardAxisMappings.push_back(CreateRef<KeyboardAxisMapping>("Player_MoveForward", moveForwardBinding));
	m_KeyboardAxisMappings.push_back(CreateRef<KeyboardAxisMapping>("Player_MoveRight", moveRightBinding));

	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Exit", GLFW_KEY_ESCAPE));
}

Input::~Input()
{
}

void Input::ProcessKeyboardInput(GLFWwindow* window)
{
	// Actions
	for (auto actionMapping : m_KeyboardActionMappings)
	{
		if (glfwGetKey(window, actionMapping->m_Key) == GLFW_PRESS)
			MakeAction(actionMapping);
	}

	// Axes
	for (auto axisMapping : m_KeyboardAxisMappings)
	{
		for (auto func : axisMapping->m_Functions)
		{
			for (auto key : axisMapping->m_Keys)
			{
				if (glfwGetKey(window, key.first) == GLFW_PRESS)
					func(key.second);
			}
		}
	}
}

Ref<KeyboardActionMapping> Input::FindActionMapping(std::string name)
{
	for (auto action : m_KeyboardActionMappings)
	{
		if (action->m_Name == name)
			return action;
	}

	return Ref<KeyboardActionMapping>();
}

void Input::MakeAction(Ref<KeyboardActionMapping> actionMapping)
{
	for (auto function : actionMapping->m_Functions)
		function();
}

void Input::BindAction(std::string actionName, std::function<void()> function)
{
	auto actionMapping = FindActionMapping(actionName);

	if (actionMapping)
		actionMapping->m_Functions.push_back(function);
}

void Input::ClearAction(std::string actionName)
{
	auto actionMapping = FindAxisMapping(actionName);

	if (actionMapping)
		actionMapping->m_Functions.clear();
}


Ref<KeyboardAxisMapping> Input::FindAxisMapping(std::string name)
{
	for (auto axis : m_KeyboardAxisMappings)
	{
		if (axis->m_Name == name)
			return axis;
	}

	return Ref<KeyboardAxisMapping>();
}

void Input::MakeAxis(Ref<KeyboardAxisMapping> axisMapping, float value)
{
	for (auto function : axisMapping->m_Functions)
		function(value);
}


void Input::BindAxis(std::string axisName, std::function<void(float)> function)
{
	auto axisMapping = FindAxisMapping(axisName);

	if (axisMapping)
		axisMapping->m_Functions.push_back(function);
}

void Input::ClearAxis(std::string axisName)
{
	auto axisMapping = FindAxisMapping(axisName);

	if (axisMapping)
		axisMapping->m_Functions.clear();
}
