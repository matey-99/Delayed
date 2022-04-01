#include "Input.h"

#include "InputSerializer.h"

Ref<Input> Input::s_Instance{};
std::mutex Input::s_Mutex;

Input::Input()
{
	m_Window = nullptr;
	m_Mode = InputMode::Player;
	m_LastMousePositionX = 0.0f;
	m_LastMousePositionY = 0.0f;
}

Input::~Input()
{
}

void Input::Initialize(GLFWwindow* window)
{
	m_Window = window;

	InputSerializer::Deserialize("../../../Config/Input.yaml");

	SetInputMode(InputMode::PlayerAndUI);
}

void Input::Process()
{
	// Mouse
	double mousePosX, mousePosY;
	glfwGetCursorPos(m_Window, &mousePosX, &mousePosY);

	float mouseDeltaX = mousePosX - m_LastMousePositionX;
	float mouseDeltaY = mousePosY - m_LastMousePositionY;

	m_LastMousePositionX = mousePosX;
	m_LastMousePositionY = mousePosY;

	// Bindings
	if (m_Mode == InputMode::Player || m_Mode == InputMode::PlayerAndUI)
	{
		// Actions
		for (auto actionInputBinding : m_ActionInputBindings)
		{
			switch (actionInputBinding->InputType)
			{
			case InputType::Keyboard:
				if (glfwGetKey(m_Window, actionInputBinding->Input) == GLFW_PRESS)
					MakeAction(actionInputBinding);
				break;
			case InputType::Mouse:
				if (glfwGetMouseButton(m_Window, actionInputBinding->Input) == GLFW_PRESS)
					MakeAction(actionInputBinding);
				break;
			case InputType::Gamepad:
				GLFWgamepadstate state;
				if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
				{
					if (state.buttons[actionInputBinding->Input])
						MakeAction(actionInputBinding);
				}
				break;
			}
		}

		// Axes
		for (auto axisInputBinding : m_AxisInputBindings)
		{
			for (auto input : axisInputBinding->Inputs)
			{
				switch (axisInputBinding->InputType)
				{
				case InputType::Keyboard:
					if (glfwGetKey(m_Window, input.first) == GLFW_PRESS)
						MakeAxis(axisInputBinding, input.second);
					break;
				case InputType::Mouse:
					switch (input.first)
					{
					case 0:
						MakeAxis(axisInputBinding, mouseDeltaX * input.second);
						break;
					case 1:
						MakeAxis(axisInputBinding, mouseDeltaY * input.second);
						break;
					}
					break;
				case InputType::Gamepad:
					GLFWgamepadstate state;
					if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
						MakeAxis(axisInputBinding, state.axes[input.first] * input.second);
					break;
				}
			}
		}
	}
}

std::vector<Ref<ActionInputBinding>> Input::FindActionInputBindings(std::string name)
{
	std::vector<Ref<ActionInputBinding>> result;
	for (auto action : m_ActionInputBindings)
	{
		if (action->Name == name)
			result.push_back(action);
	}

	return result;
}

void Input::MakeAction(Ref<ActionInputBinding> actionInputBinding)
{
	for (auto function : actionInputBinding->Functions)
		function();
}

void Input::BindAction(std::string actionName, std::function<void()> function)
{
	auto actionInputBindings = FindActionInputBindings(actionName);

	for (auto actionInputBinding : actionInputBindings)
		actionInputBinding->Functions.push_back(function);
}

void Input::ClearAction(std::string actionName)
{
	auto actionInputBindings = FindAxisInputBindings(actionName);

	for (auto actionInputBinding : actionInputBindings)
		actionInputBinding->Functions.clear();
}


std::vector<Ref<AxisInputBinding>> Input::FindAxisInputBindings(std::string name)
{
	std::vector<Ref<AxisInputBinding>> result;
	for (auto axis : m_AxisInputBindings)
	{
		if (axis->Name == name)
			result.push_back(axis);
	}

	return result;
}

void Input::MakeAxis(Ref<AxisInputBinding> axisInputBinding, float value)
{
	if (glm::abs(value) < 0.1f)
		value = 0.0f;

	for (auto function : axisInputBinding->Functions)
		function(value);
}


void Input::BindAxis(std::string axisName, std::function<void(float)> function)
{
	auto axisInputBindings = FindAxisInputBindings(axisName);

	for (auto axisInputBinding : axisInputBindings)
		axisInputBinding->Functions.push_back(function);
}

void Input::ClearAxis(std::string axisName)
{
	auto axisInputBindings = FindAxisInputBindings(axisName);

	for (auto axisInputBinding : axisInputBindings)
		axisInputBinding->Functions.clear();
}

void Input::SetInputMode(InputMode mode)
{
	m_Mode = mode;

	switch (mode)
	{
	case InputMode::Player:
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	case InputMode::UI:
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case InputMode::PlayerAndUI:
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	}
}
