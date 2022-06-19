#pragma once

#include "Core.h"
#include "InputEnums.h"
#include "Patterns/Singleton.h"
#include "Patterns/Event.h"
#include "Patterns/Delegate.h"

#define GAMEPAD_AXIS_ERROR 0.1f

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
	Event OnActionInputPress;
	Event OnActionInputRelease;
	Event OnActionInputRepeat;
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
	DelegateOneParam<float> OnAxisInput;
};

enum class InputMode
{
	Player, UI, PlayerAndUI
};

enum class PlayerInputType
{
	KeyboardAndMouse, Gamepad
};

class Input : public Singleton<Input>
{
public:
	Input();
	~Input();

	void Initialize(GLFWwindow* window);

	void Process();

	// Actions
	std::vector<Ref<ActionInputBinding>> FindActionInputBindings(std::string name);
	void ClearAction(std::string actionName);

	// Axes
	std::vector<Ref<AxisInputBinding>> FindAxisInputBindings(std::string name);
	void ClearAxis(std::string axisName);

	// Bindings
	template <typename T>
	void BindAction(std::string actionName, InputEvent inputEvent, void (T::* func)(), T* object)
	{
		auto actionInputBindings = FindActionInputBindings(actionName);
		for (auto actionInputBinding : actionInputBindings)
		{
			switch (inputEvent)
			{
			case InputEvent::Press:
				actionInputBinding->OnActionInputPress.Add(func, object);
				break;
			case InputEvent::Release:
				actionInputBinding->OnActionInputRelease.Add(func, object);
				break;
			case InputEvent::Repeat:
				actionInputBinding->OnActionInputRepeat.Add(func, object);
				break;
			}
		}
	}

	template <typename T>
	void BindAxis(std::string axisName, void (T::* func)(float), T* object)
	{
		auto axisInputBindings = FindAxisInputBindings(axisName);
		for (auto axisInputBinding : axisInputBindings)
		{
			axisInputBinding->OnAxisInput.Add(func, object);
		}
	}

	// Other
	void SetInputMode(InputMode mode);
	glm::vec2 GetMousePosition();

	inline PlayerInputType GetCurrentInputType() const { return m_CurrentInputType; }

public:
	Event OnLeftMouseButtonPressed;
	Event OnLeftMouseButtonReleased;

private:
	GLFWwindow* m_Window;

	std::vector<Ref<ActionInputBinding>> m_ActionInputBindings;
	std::vector<Ref<AxisInputBinding>> m_AxisInputBindings;

	InputMode m_Mode;
	PlayerInputType m_CurrentInputType;

	float m_LastMousePositionX;
	float m_LastMousePositionY;

	friend class InputSerializer;
};