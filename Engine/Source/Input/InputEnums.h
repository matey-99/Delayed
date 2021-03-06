#pragma once

#include <GLFW/glfw3.h>

enum class InputType
{
	Keyboard		= 0,
	Mouse			= 1,
	Gamepad			= 2
};

enum class InputEvent
{
	Press			= GLFW_PRESS,
	Release			= GLFW_RELEASE,
	Repeat			= GLFW_REPEAT
};

enum class Key
{
	Space			= GLFW_KEY_SPACE,
	Apostrophe		= GLFW_KEY_APOSTROPHE,
	Comma			= GLFW_KEY_COMMA,
	Minus			= GLFW_KEY_MINUS,
	Period			= GLFW_KEY_PERIOD,
	Slash			= GLFW_KEY_SLASH,
	_0				= GLFW_KEY_0,
	_1				= GLFW_KEY_1,
	_2				= GLFW_KEY_2,
	_3				= GLFW_KEY_3,
	_4				= GLFW_KEY_4,
	_5				= GLFW_KEY_5,
	_6				= GLFW_KEY_6,
	_7				= GLFW_KEY_7,
	_8				= GLFW_KEY_8,
	_9				= GLFW_KEY_9,
	Semicolon		= GLFW_KEY_SEMICOLON,
	Equal			= GLFW_KEY_EQUAL,
	A				= GLFW_KEY_A,
	B				= GLFW_KEY_B,
	C				= GLFW_KEY_C,
	D				= GLFW_KEY_D,
	E				= GLFW_KEY_E,
	F				= GLFW_KEY_F,
	G				= GLFW_KEY_G,
	H				= GLFW_KEY_H,
	I				= GLFW_KEY_I,
	J				= GLFW_KEY_J,
	K				= GLFW_KEY_K,
	L				= GLFW_KEY_L,
	M				= GLFW_KEY_M,
	N				= GLFW_KEY_N,
	O				= GLFW_KEY_O,
	P				= GLFW_KEY_P,
	Q				= GLFW_KEY_Q,
	R				= GLFW_KEY_R,
	S				= GLFW_KEY_S,
	T				= GLFW_KEY_T,
	U				= GLFW_KEY_U,
	V				= GLFW_KEY_V,
	W				= GLFW_KEY_W,
	X				= GLFW_KEY_X,
	Y				= GLFW_KEY_Y,
	Z				= GLFW_KEY_Z,
	LeftBracket		= GLFW_KEY_LEFT_BRACKET,
	Backslash		= GLFW_KEY_BACKSLASH,
	RightBracket	= GLFW_KEY_RIGHT_BRACKET,
	GraveAccent		= GLFW_KEY_GRAVE_ACCENT,

	Escape			= GLFW_KEY_ESCAPE,
	Enter			= GLFW_KEY_ENTER,
	Tab				= GLFW_KEY_TAB,
	Backspace		= GLFW_KEY_BACKSPACE,
	Insert			= GLFW_KEY_INSERT,
	Delete			= GLFW_KEY_DELETE,
	Right			= GLFW_KEY_RIGHT,
	Left			= GLFW_KEY_LEFT,
	Down			= GLFW_KEY_DOWN,
	Up				= GLFW_KEY_UP,
	PageUp			= GLFW_KEY_PAGE_UP,
	PageDown		= GLFW_KEY_PAGE_DOWN,
	Home			= GLFW_KEY_HOME,
	End				= GLFW_KEY_END,
	CapsLock		= GLFW_KEY_CAPS_LOCK,
	ScrollLock		= GLFW_KEY_SCROLL_LOCK,
	NumLock			= GLFW_KEY_NUM_LOCK,
	PrintScreen		= GLFW_KEY_PRINT_SCREEN,
	Pause			= GLFW_KEY_PAUSE,
	F1				= GLFW_KEY_F1,
	F2				= GLFW_KEY_F2,
	F3				= GLFW_KEY_F3,
	F4				= GLFW_KEY_F4,
	F5				= GLFW_KEY_F5,
	F6				= GLFW_KEY_F6,
	F7				= GLFW_KEY_F7,
	F8				= GLFW_KEY_F8,
	F9				= GLFW_KEY_F9,
	F10				= GLFW_KEY_F10,
	F11				= GLFW_KEY_F11,
	F12				= GLFW_KEY_F12,
	KP0				= GLFW_KEY_KP_0,
	KP1				= GLFW_KEY_KP_1,
	KP2				= GLFW_KEY_KP_2,
	KP3				= GLFW_KEY_KP_3,
	KP4				= GLFW_KEY_KP_4,
	KP5				= GLFW_KEY_KP_5,
	KP6				= GLFW_KEY_KP_6,
	KP7				= GLFW_KEY_KP_7,
	KP8				= GLFW_KEY_KP_8,
	KP9				= GLFW_KEY_KP_9,
	KPDecimal		= GLFW_KEY_KP_DECIMAL,
	KPDivide		= GLFW_KEY_KP_DIVIDE,
	KPMultiply		= GLFW_KEY_KP_MULTIPLY,
	KPSubtract		= GLFW_KEY_KP_SUBTRACT,
	KPAdd			= GLFW_KEY_KP_ADD,
	KPEnter			= GLFW_KEY_KP_ENTER,
	KPEqual			= GLFW_KEY_KP_EQUAL,
	LeftShift		= GLFW_KEY_LEFT_SHIFT,
	LeftControl		= GLFW_KEY_LEFT_CONTROL,
	LeftAlt			= GLFW_KEY_LEFT_ALT,
	LeftSuper		= GLFW_KEY_LEFT_SUPER,
	RightShift		= GLFW_KEY_RIGHT_SHIFT,
	RightControl	= GLFW_KEY_RIGHT_CONTROL,
	RightAlt		= GLFW_KEY_RIGHT_ALT,
	RightSuper		= GLFW_KEY_RIGHT_SUPER,
	Menu			= GLFW_KEY_MENU
};

enum class MouseButton
{
	Left			= GLFW_MOUSE_BUTTON_1,
	Right			= GLFW_MOUSE_BUTTON_2,
	Middle			= GLFW_MOUSE_BUTTON_3
};

enum class MouseAxis
{
	X				= 0,
	Y				= 1
};

enum class GamepadButton
{
	A				= GLFW_GAMEPAD_BUTTON_A,
	B				= GLFW_GAMEPAD_BUTTON_B,
	X				= GLFW_GAMEPAD_BUTTON_X,
	Y				= GLFW_GAMEPAD_BUTTON_Y,
	LeftBumper		= GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
	RightBumper		= GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
	Back			= GLFW_GAMEPAD_BUTTON_BACK,
	Start			= GLFW_GAMEPAD_BUTTON_START,
	Guide			= GLFW_GAMEPAD_BUTTON_GUIDE,
	LeftThumb		= GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
	RightThumb		= GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
	DPadUp			= GLFW_GAMEPAD_BUTTON_DPAD_UP,
	DPadRight		= GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
	DPadDown		= GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
	DPadLeft		= GLFW_GAMEPAD_BUTTON_DPAD_LEFT
};

enum class GamepadAxis
{
	LeftX			= GLFW_GAMEPAD_AXIS_LEFT_X,
	LeftY			= GLFW_GAMEPAD_AXIS_LEFT_Y,
	RightX			= GLFW_GAMEPAD_AXIS_RIGHT_X,
	RightY			= GLFW_GAMEPAD_AXIS_RIGHT_Y,
	LeftTrigger		= GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
	RightTrigger	= GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER
};