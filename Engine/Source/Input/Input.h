#pragma once

#include <string>
#include <vector>
#include <functional>
#include <mutex>
#include <GLFW/glfw3.h>

#include "Patterns/Singleton.h"
#include "typedefs.h"

#define MOUSE_X 0
#define MOUSE_Y 1

enum class InputType
{
	Keyboard, Mouse, Gamepad
};

struct ActionInputBinding
{
public:
	ActionInputBinding(std::string name, InputType inputType, int input)
		: Name(name), InputType(inputType), Input(input)
	{}

	static Ref<ActionInputBinding> Create(std::string name, InputType inputType, int input)
	{
		return CreateRef<ActionInputBinding>(name, inputType, input);
	}

	std::string Name;
	InputType InputType;
	int Input;
	std::vector<std::function<void()>> Functions;
};

struct AxisInputBinding
{
public:
	AxisInputBinding(std::string name, InputType inputType, std::unordered_map<int, float> inputs)
		: Name(name), InputType(inputType), Inputs(inputs)
	{}

	static Ref<AxisInputBinding> Create(std::string name, InputType inputType, std::unordered_map<int, float> inputs)
	{
		return CreateRef<AxisInputBinding>(name, inputType, inputs);
	}

	std::string Name;
	InputType InputType;
	std::unordered_map<int, float> Inputs;
	std::vector<std::function<void(float)>> Functions;
};

class Input : public Singleton<Input>
{
private:
	enum class InputMode
	{
		Player, UI, PlayerAndUI
	};

public:
	Input();
	~Input();

	void Initialize(GLFWwindow* window);

	void Process();

	// Actions
	std::vector<Ref<ActionInputBinding>> FindActionInputBindings(std::string name);
	void MakeAction(Ref<ActionInputBinding> actionInputBinding);
	void BindAction(std::string actionName, std::function<void()> function);
	void ClearAction(std::string actionName);

	// Axes
	std::vector<Ref<AxisInputBinding>> FindAxisInputBindings(std::string name);
	void MakeAxis(Ref<AxisInputBinding> axisInputBinding, float value);
	void BindAxis(std::string axisName, std::function<void(float)> function);
	void ClearAxis(std::string axisName);

	void SetInputMode(InputMode mode);

private:
	GLFWwindow* m_Window;

	std::vector<Ref<ActionInputBinding>> m_ActionInputBindings;
	std::vector<Ref<AxisInputBinding>> m_AxisInputBindings;

	InputMode m_Mode;

	float m_LastMousePositionX;
	float m_LastMousePositionY;
};