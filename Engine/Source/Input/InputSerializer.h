#pragma once

#include "Input.h"

class InputSerializer
{
public:
	static void Deserialize(std::string path);

private:
	static std::unordered_map<std::string, InputType> s_InputTypes;

	static std::unordered_map<std::string, Key> s_Keys;

	static std::unordered_map<std::string, MouseButton> s_MouseButtons;
	static std::unordered_map<std::string, MouseAxis> s_MouseAxes;

	static std::unordered_map<std::string, GamepadButton> s_GamepadButtons;
	static std::unordered_map<std::string, GamepadAxis> s_GamepadAxes;
};