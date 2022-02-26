#include "Input.h"

Ref<Input> Input::s_Instance{};
std::mutex Input::s_Mutex;

Input::Input()
{
	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_MoveLeft", GLFW_KEY_A));
	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_MoveRight", GLFW_KEY_D));
	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_MoveForward", GLFW_KEY_W));
	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_MoveBackward", GLFW_KEY_S));

	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_RotateHeadLeft", GLFW_KEY_Q));
	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Player0_RotateHeadRight", GLFW_KEY_E));

	m_KeyboardActionMappings.push_back(CreateRef<KeyboardActionMapping>("Exit", GLFW_KEY_ESCAPE));

}

Input::~Input()
{
}

Ref<Input> Input::GetInstance()
{
	std::lock_guard<std::mutex> lock(s_Mutex);
	if (!s_Instance)
		s_Instance = CreateRef<Input>();

	return s_Instance;
}

void Input::ProcessKeyboardInput(GLFWwindow* window)
{
	for (auto actionMapping : m_KeyboardActionMappings)
	{
		if (glfwGetKey(window, actionMapping->m_Key) == GLFW_PRESS)
			MakeAction(actionMapping);
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

void Input::MakeAction(Ref<KeyboardActionMapping> ActionMapping)
{
	for (auto action : ActionMapping->m_Actions)
		action();
}

void Input::BindInput(std::string actionName, std::function<void()> action)
{
	auto actionMapping = FindActionMapping(actionName);

	if (actionMapping)
		actionMapping->m_Actions.push_back(action);
}

void Input::ClearBinding(std::string actionName)
{
	auto actionMapping = FindActionMapping(actionName);

	if (actionMapping)
		actionMapping->m_Actions.clear();
}
