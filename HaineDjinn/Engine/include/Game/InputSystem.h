#pragma once

#include <GLFW/glfw3.h>

#include <map>
#include <string>

#include "Core/Maths.h"

// Making namespaces to avoid static classes having no "real" private members
// Public members/variables
namespace InputSystem
{
	// might be useful for controller/mouse bindings on keybinds
	//enum class InputType {
	//	KEYBOARD,
	//	MOUSE,
	//	CONTROLLER,
	//};

	namespace Keyboard {
		static std::map<std::string, int> keybinds = {
			{"Spell_1", GLFW_KEY_Q },
			{"Spell_2", GLFW_KEY_W },
			{"Spell_3", GLFW_KEY_E },
			{"A", GLFW_KEY_A },
			{"B", GLFW_KEY_B },
			{"C", GLFW_KEY_C },
			{"D", GLFW_KEY_D },
			{"E", GLFW_KEY_E },
			{"F", GLFW_KEY_F },
			{"G", GLFW_KEY_G },
			{"H", GLFW_KEY_H },
			{"I", GLFW_KEY_I },
			{"J", GLFW_KEY_J },
			{"K", GLFW_KEY_K },
			{"L", GLFW_KEY_L },
			{"M", GLFW_KEY_M },
			{"N", GLFW_KEY_N },
			{"O", GLFW_KEY_O },
			{"P", GLFW_KEY_P },
			{"Q", GLFW_KEY_Q },
			{"R", GLFW_KEY_R },
			{"S", GLFW_KEY_S },
			{"T", GLFW_KEY_T },
			{"U", GLFW_KEY_U },
			{"V", GLFW_KEY_V },
			{"W", GLFW_KEY_W },
			{"X", GLFW_KEY_X },
			{"Y", GLFW_KEY_Y },
			{"Z", GLFW_KEY_Z },
			{"Escape", GLFW_KEY_ESCAPE },
			{"Space", GLFW_KEY_SPACE },
			{"LControl", GLFW_KEY_LEFT_CONTROL},
			{"RControl", GLFW_KEY_RIGHT_CONTROL},
		};

		static std::map<std::string, std::pair<int, int>> axes = {
			{"Horizontal", {GLFW_KEY_A, GLFW_KEY_D}},
			{"Vertical", {GLFW_KEY_S, GLFW_KEY_W}},
			{"Upwards", {GLFW_KEY_LEFT_SHIFT, GLFW_KEY_SPACE}},
		};
	}

	namespace Mouse {
		static const int mouseButtons[] =
		{
			0,
			GLFW_MOUSE_BUTTON_1,
			GLFW_MOUSE_BUTTON_2,
			GLFW_MOUSE_BUTTON_3,
			GLFW_MOUSE_BUTTON_4,
			GLFW_MOUSE_BUTTON_5,
			GLFW_MOUSE_BUTTON_6,
			GLFW_MOUSE_BUTTON_7,
			GLFW_MOUSE_BUTTON_8,
		};
	}

	void SetWindow(GLFWwindow* inWindow);
	void UpdateInputSys();

	// TODO: Save and load inputs in a file
	void LoadInputs(const std::string& fileName);
	void SaveInputs(const std::string& fileName);

	// Read value from `axis` in range (-1, 1)
	float GetAxis(const std::string& axis);
	// Get current `input` state
	bool GetInput(const std::string& input);
	// `true` if `input` pressed this frame, `false` if not
	bool GetInputDown(const std::string& input);

	// Get current `button` state
	bool GetMouseButton(const unsigned int button);
	// `true` if `button` pressed this frame, `false` if not
	bool GetMouseButtonDown(unsigned int button);
	// Get current mouse position
	Vector2 GetMousePos();
	// Get mouse translation last frame
	Vector2 GetMouseDelta();
}
