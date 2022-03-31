#include "Input.h"

Ref<Input> Input::s_Instance{};
std::mutex Input::s_Mutex;

Input::Input()
{
	// Creating bindings should be done in some config file

	// Keyboard & Mouse
	std::unordered_map<int, float> moveForwardKeyboardBinding = { { GLFW_KEY_S, 1.0f }, { GLFW_KEY_W, -1.0f } };
	std::unordered_map<int, float> moveRightKeyboardBinding = { { GLFW_KEY_D, 1.0f }, { GLFW_KEY_A, -1.0f } };

	// Second fields are set as 0.0f, because value will be taken from mouse delta position
	std::unordered_map<int, float> turnBinding = { { MOUSE_X, 0.0f } };
	std::unordered_map<int, float> lookUpBinding = { { MOUSE_Y, 0.0f } };

	// Gamepad
	// Second fields are set as 0.0f, because value will be taken from gamepad
	std::unordered_map<int, float> moveForwardGamepadBinding = { { GLFW_GAMEPAD_AXIS_LEFT_Y, 0.0f } };
	std::unordered_map<int, float> moveRightGamepadBinding = { { GLFW_GAMEPAD_AXIS_LEFT_X, 0.0f } };
	std::unordered_map<int, float> turnRateBinding = { { GLFW_GAMEPAD_AXIS_RIGHT_X, 0.0f } };
	std::unordered_map<int, float> lookUpRateBinding = { { GLFW_GAMEPAD_AXIS_RIGHT_Y, 0.0f } };
	

	// Binding axis inputs
	// Keyboard & Mouse
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_MoveForward", InputType::Keyboard, moveForwardKeyboardBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_MoveRight", InputType::Keyboard, moveRightKeyboardBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_Turn", InputType::Mouse, turnBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_LookUp", InputType::Mouse, lookUpBinding));

	// Gamepad
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_MoveForward", InputType::Gamepad, moveForwardGamepadBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_MoveRight", InputType::Gamepad, moveRightGamepadBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_Turn", InputType::Gamepad, turnRateBinding));
	m_AxisInputBindings.push_back(AxisInputBinding::Create("Player_LookUp", InputType::Gamepad, lookUpRateBinding));

	// Binding action inputs
	// Keyboard & Mouse
	m_ActionInputBindings.push_back(ActionInputBinding::Create("Exit", InputType::Keyboard, GLFW_KEY_ESCAPE));

	// Gamepad
	m_ActionInputBindings.push_back(ActionInputBinding::Create("Exit", InputType::Gamepad, GLFW_GAMEPAD_BUTTON_BACK));

	// Fields initialization
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

	SetInputMode(InputMode::Player);
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
					case MOUSE_X:
						MakeAxis(axisInputBinding, mouseDeltaX);
						break;
					case MOUSE_Y:
						MakeAxis(axisInputBinding, mouseDeltaY);
						break;
					}
					break;
				case InputType::Gamepad:
					GLFWgamepadstate state;
					if (glfwGetGamepadState(GLFW_JOYSTICK_1, &state))
						MakeAxis(axisInputBinding, state.axes[input.first]);
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
