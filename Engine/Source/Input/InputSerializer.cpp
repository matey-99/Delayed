#include "InputSerializer.h"

#include "yaml/yaml.h"

void InputSerializer::Deserialize(std::string path)
{
	auto input = Input::GetInstance();

	std::ifstream file(path);
	std::stringstream ss;
	ss << file.rdbuf();

	YAML::Node data = YAML::Load(ss.str());
	YAML::Node actionInputBindings = data["ActionInputBindings"];
	YAML::Node axisInputBindings = data["AxisInputBindings"];
	if (!actionInputBindings || !axisInputBindings)
	{
		std::cout << "Cannot open Config/Input file!" << std::endl;
		return;
	}

	for (auto inputBinding : actionInputBindings)
	{
		std::string name = inputBinding["Name"].as<std::string>();
		InputType type = s_InputTypes.find(inputBinding["InputType"].as<std::string>())->second;
		std::string inputStr = inputBinding["Input"].as<std::string>();

		int i;
		switch (type)
		{
		case InputType::Keyboard:
			i = (int)s_Keys.find(inputStr)->second;
			break;
		case InputType::Mouse:
			i = (int)s_MouseButtons.find(inputStr)->second;
			break;
		case InputType::Gamepad:
			i = (int)s_GamepadButtons.find(inputStr)->second;
			break;
		default:
			std::cout << "Unexpected input in: " << name << "!" << std::endl;
			return;
		}

		input->m_ActionInputBindings.push_back(ActionInputBinding::Create(name, type, i));
	}

	for (auto inputBinding : axisInputBindings)
	{
		std::string name = inputBinding["Name"].as<std::string>();
		InputType type = s_InputTypes.find(inputBinding["InputType"].as<std::string>())->second;

		std::unordered_map<int, float> inputs;
		for (auto input : inputBinding["Inputs"])
		{
			std::string inputName = input["Input"].as<std::string>();
			float value = input["Value"].as<float>();

			int i;
			switch (type)
			{
			case InputType::Keyboard:
				i = (int)s_Keys.find(inputName)->second;
				break;
			case InputType::Mouse:
				i = (int)s_MouseAxes.find(inputName)->second;
				break;
			case InputType::Gamepad:
				i = (int)s_GamepadAxes.find(inputName)->second;
				break;
			default:
				std::cout << "Unexpected input in: " << name << "!" << std::endl;
				return;
			}

			inputs.insert({ i, value });
		}

		input->m_AxisInputBindings.push_back(AxisInputBinding::Create(name, type, inputs));
	}
}

std::unordered_map<std::string, InputType> InputSerializer::s_InputTypes =
{
	{ "Keyboard", InputType::Keyboard },
	{ "Mouse", InputType::Mouse },
	{ "Gamepad", InputType::Gamepad },
};

std::unordered_map<std::string, Key> InputSerializer::s_Keys =
{
	{ "Space", Key::Space },
	{ "Apostrophe", Key::Apostrophe },
	{ "Comma", Key::Comma },
	{ "Minus", Key::Minus },
	{ "Period", Key::Period },
	{ "Slash", Key::Slash },
	{ "0", Key::_0 },
	{ "1", Key::_1 },
	{ "2", Key::_2 },
	{ "3", Key::_3 },
	{ "4", Key::_4 },
	{ "5", Key::_5 },
	{ "6", Key::_6 },
	{ "7", Key::_7 },
	{ "8", Key::_8 },
	{ "9", Key::_9 },
	{ "Semicolon", Key::Semicolon },
	{ "Equal", Key::Equal },
	{ "A", Key::A },
	{ "B", Key::B },
	{ "C", Key::C },
	{ "D", Key::D },
	{ "E", Key::E },
	{ "F", Key::F },
	{ "G", Key::G },
	{ "H", Key::H },
	{ "I", Key::I },
	{ "J", Key::J },
	{ "K", Key::K },
	{ "L", Key::L },
	{ "M", Key::M },
	{ "N", Key::N },
	{ "O", Key::O },
	{ "P", Key::P },
	{ "Q", Key::Q },
	{ "R", Key::R },
	{ "S", Key::S },
	{ "T", Key::T },
	{ "U", Key::U },
	{ "V", Key::V },
	{ "W", Key::W },
	{ "X", Key::X },
	{ "Y", Key::Y },
	{ "Z", Key::Z },
	{ "LeftBracket", Key::LeftBracket },
	{ "Backslash", Key::Backslash },
	{ "RightBracket", Key::RightBracket},
	{ "GraveAccent", Key::GraveAccent },

	{ "Escape", Key::Escape },
	{ "Enter", Key::Enter },
	{ "Tab", Key::Tab },
	{ "Backspace", Key::Backspace },
	{ "Insert", Key::Insert },
	{ "Delete", Key::Delete },
	{ "Right", Key::Right },
	{ "Left", Key::Left },
	{ "Down", Key::Down },
	{ "Up", Key::Up },
	{ "PageUp", Key::PageUp },
	{ "PageDown", Key::PageDown },
	{ "Home", Key::Home },
	{ "End", Key::End },
	{ "CapsLock", Key::CapsLock },
	{ "ScrollLock", Key::ScrollLock },
	{ "NumLock", Key::NumLock },
	{ "PrintScreen", Key::PrintScreen },
	{ "Pause", Key::Pause },
	{ "F1", Key::F1 },
	{ "F2", Key::F2 },
	{ "F3", Key::F3 },
	{ "F4", Key::F4 },
	{ "F5", Key::F5 },
	{ "F6", Key::F6 },
	{ "F7", Key::F7 },
	{ "F8", Key::F8 },
	{ "F9", Key::F9 },
	{ "F10", Key::F10 },
	{ "F11", Key::F11 },
	{ "F12", Key::F12 },
	{ "KP0", Key::KP0 },
	{ "KP1", Key::KP1 },
	{ "KP2", Key::KP2 },
	{ "KP3", Key::KP3 },
	{ "KP4", Key::KP4 },
	{ "KP5", Key::KP5 },
	{ "KP6", Key::KP6 },
	{ "KP7", Key::KP7 },
	{ "KP8", Key::KP8 },
	{ "KP9", Key::KP9 },
	{ "KPDecimal", Key::KPDecimal },
	{ "KPDivide", Key::KPDivide },
	{ "KPMultiply", Key::KPMultiply },
	{ "KPSubtract", Key::KPSubtract },
	{ "KPAdd", Key::KPAdd },
	{ "KPEnter", Key::KPEnter },
	{ "KPEqual", Key::KPEqual },
	{ "LeftShift", Key::LeftShift },
	{ "LeftControl", Key::LeftControl },
	{ "LeftAlt", Key::LeftAlt },
	{ "LeftSuper", Key::LeftSuper },
	{ "RightShift", Key::RightShift },
	{ "RightControl", Key::RightControl },
	{ "RightAlt", Key::RightAlt },
	{ "RightSuper", Key::RightSuper },
	{ "Menu", Key::Menu }
};

std::unordered_map<std::string, MouseButton> InputSerializer::s_MouseButtons =
{
	{ "LeftMouseButton", MouseButton::Left },
	{ "RightMouseButton", MouseButton::Right },
	{ "MiddleMouseButton", MouseButton::Middle }
};
std::unordered_map<std::string, MouseAxis> InputSerializer::s_MouseAxes =
{
	{ "MouseX", MouseAxis::X },
	{ "MouseY", MouseAxis::Y }
};

std::unordered_map<std::string, GamepadButton> InputSerializer::s_GamepadButtons =
{
	{ "A", GamepadButton::A },
	{ "B", GamepadButton::B },
	{ "X", GamepadButton::X },
	{ "Y", GamepadButton::Y },
	{ "LeftBumper", GamepadButton::LeftBumper },
	{ "RightBumper", GamepadButton::RightBumper },
	{ "Back", GamepadButton::Back },
	{ "Start", GamepadButton::Start },
	{ "Guide", GamepadButton::Guide },
	{ "LeftThumb", GamepadButton::LeftThumb },
	{ "RightThumb", GamepadButton::RightThumb },
	{ "DPadUp", GamepadButton::DPadUp },
	{ "DPadRight", GamepadButton::DPadRight },
	{ "DPadDown", GamepadButton::DPadDown },
	{ "DPadLeft", GamepadButton::DPadLeft }
};
std::unordered_map<std::string, GamepadAxis> InputSerializer::s_GamepadAxes =
{
	{ "LeftX", GamepadAxis::LeftX },
	{ "LeftY", GamepadAxis::LeftY },
	{ "RightX", GamepadAxis::RightX },
	{ "RightY", GamepadAxis::RightY },
	{ "LeftTrigger", GamepadAxis::LeftTrigger },
	{ "RightTrigger", GamepadAxis::RightTrigger }
};
