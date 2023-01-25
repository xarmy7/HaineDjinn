
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>

#include "Game/InputSystem.h"
#include "Core/Debug.h"

namespace InputSystem
{
	// Anonymous namespace, InputSystem's private members/variables
	namespace
	{
		enum class ParserState
		{
			KEYS,
			KAXIS,
		};

		GLFWwindow* window = nullptr;

		// std::map<keycode, isDown>; Filled by GetInputDown
		std::map<int, bool> keystates = {
		};
		std::map<int, bool> mouseButtonStates = {
		};

		double lastPosX, lastPosY;
		double currPosX, currPosY; 
		double deltaX, deltaY;
	}

#pragma region INPUT SYSTEM FUNCTIONS
	void SetWindow(GLFWwindow* inWindow)
	{
		window = inWindow;
	}

	void UpdateInputSys()
	{
		// Mouse position update
		glfwGetCursorPos(window, &currPosX, &currPosY);

		deltaX = currPosX - lastPosX;
		deltaY = currPosY - lastPosY;

		lastPosX = currPosX;
		lastPosY = currPosY;
		glfwPollEvents();
	}
#pragma endregion

#pragma region INPUTS PARSER & READER
	std::string IntToHex(int i)
	{
		std::stringstream stream;
		stream << "0x"
			<< std::setfill('0') << std::setw(sizeof(int) * 2)
			<< std::hex << i;
		return stream.str();
	}

	int HexToInt(std::string& hexa)
	{
		std::istringstream stream(hexa);
		std::string section;
		std::string hexStr;
		
		std::getline(stream, section, 'x');

		stream >> hexStr;

		return std::stoi(hexStr, nullptr, 16);
	}

	void ParseKeybind(const std::string& str)
	{
		std::istringstream stream(str);
		std::string name;
		std::string hexaInt;

		std::getline(stream, name, ':');
		std::getline(stream, hexaInt);
		
		int keybind = HexToInt(hexaInt);

		Keyboard::keybinds.insert(std::pair<std::string, int>(name, keybind));
	}

	void ParseKeyboardAxis(const std::string& str)
	{
		std::istringstream stream(str);
		std::string name;
		std::string hexaInt1;
		std::string hexaInt2;

		std::getline(stream, name, ':');
		std::getline(stream, hexaInt1, ',');
		std::getline(stream, hexaInt2);

		std::pair<int, int> keypair;

		keypair.first = HexToInt(hexaInt1);
		keypair.second = HexToInt(hexaInt2);

		Keyboard::axes.insert(std::pair<std::string, std::pair<int, int>>(name, keypair));
	}

	void LoadInputs(const std::string& fileName)
	{
		HD_LOG("Loading inputs from file " + fileName, LogSeverity::LOG_MESSAGE);

		std::ifstream file;
		file.open(fileName);

		if (!file.is_open())
		{
			HD_LOG("[ERROR] Inputs file " + fileName + "can't be opened", LogSeverity::LOG_ERROR);
			return;
		}

		Keyboard::axes.clear();
		Keyboard::keybinds.clear();

		ParserState state = ParserState::KEYS;

		while (!file.eof())
		{
			std::string strLine;
			std::getline(file, strLine);

			if (strLine == "\0" || strLine == "\n")
				continue;
			else if (strLine == "keys")
			{
				state = ParserState::KEYS;
				continue;
			}
			else if (strLine == "kaxis")
			{
				state = ParserState::KAXIS;
				continue;
			}

			switch (state)
			{
			case ParserState::KEYS:
				ParseKeybind(strLine);
				break;
			case ParserState::KAXIS:
				ParseKeyboardAxis(strLine);
				break;
			default:
				break;
			}
		}

		file.close();

		HD_LOG("Inputs from " + fileName + " loaded.", LogSeverity::LOG_MESSAGE);

	}

	void SaveInputs(const std::string& fileName)
	{
		HD_LOG("Saving inputs in file " + fileName, LogSeverity::LOG_MESSAGE);

		std::ofstream file;
		file.open(fileName);

		if (!file.is_open())
		{
			HD_LOG("Inputs file " + fileName + "can't be opened", LogSeverity::LOG_ERROR);
			return;
		}

		file << "keys\n";
		for (auto const& [key, value] : Keyboard::keybinds)
		{
			file << key << ':' << IntToHex(value) << '\n';
		}

		file << "kaxis\n";
		for (auto const& [key, value] : Keyboard::axes)
		{
			file << key << ':' << IntToHex(value.first) << ',' << IntToHex(value.second) << '\n';
		}

		file.close();

		HD_LOG("Inputs " + fileName + " saved.", LogSeverity::LOG_MESSAGE);
	}
#pragma endregion

#pragma region KEYBOARD INPUTS
	bool GetInput(const std::string& input)
	{
		// TODO: if keyboard, else if controller
		auto iterator = Keyboard::keybinds.find(input);

		bool keyPressed = false;
		
		if (iterator != Keyboard::keybinds.end())
			keyPressed = glfwGetKey(window, iterator->second);
		else
			HD_LOG("Keybind " + input + " not found.", LogSeverity::LOG_ERROR);

		return keyPressed;
	}

	bool GetInputDown(const std::string& input)
	{
		// TODO: if keyboard, else if controller
		auto iterator = Keyboard::keybinds.find(input);

		bool keyPressed = false;

		if (iterator != Keyboard::keybinds.end())
		{
			keyPressed = glfwGetKey(window, iterator->second);

			auto lastFrame = keystates.find(iterator->second);

			if (lastFrame == keystates.end())
			{
				keystates.insert(std::pair<int, bool>(iterator->second, false));
				lastFrame = keystates.find(iterator->second);
			}

			if (keyPressed)
			{

				if (!lastFrame->second)
				{
					lastFrame->second = true;
					return true;
				}
				else
					return false;
			}
			else
			{
				lastFrame->second = false;
				return false;
			}
		}
		else
			HD_LOG("Keybind " + input + " not found.", LogSeverity::LOG_ERROR);

		return false;
	}

	float GetAxis(const std::string& axis)
	{
		// TODO: if keyboard, else if controller
		auto iterator = Keyboard::axes.find(axis);
		
		float value = 0;

		if (iterator != Keyboard::axes.end())
		{
			if (glfwGetKey(window, iterator->second.first))
				value -= 1.0f;

			if (glfwGetKey(window, iterator->second.second))
				value += 1.0f;
		}
		else			
			HD_LOG("Axis " + axis + " not found.", LogSeverity::LOG_ERROR);
	
		return value;
	}
#pragma endregion

#pragma region MOUSE
	bool GetMouseButton(const unsigned int button)
	{
		if (button == 0 || button > 8) 
			return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1);

		return glfwGetMouseButton(window, Mouse::mouseButtons[button]);
	}
	 
	bool GetMouseButtonDown(unsigned int button)
	{
		if (button == 0 || button > 8) button = 1;
	
		bool buttonPressed = false;

		buttonPressed = glfwGetMouseButton(window, Mouse::mouseButtons[button]);

		auto lastFrame = keystates.find(button);

		if (lastFrame == keystates.end())
		{
			keystates.insert(std::pair<int, bool>(button, false));
			lastFrame = keystates.find(button);
		}

		if (buttonPressed)
		{

			if (!lastFrame->second)
			{
				lastFrame->second = true;
				return true;
			}
			else
				return false;
		}

		lastFrame->second = false;
		return false;
	}

	Vector2 GetMousePos()
	{
		return { (float)currPosX, (float)currPosY };
	}

	Vector2 GetMouseDelta()
	{
		return { (float)deltaX, (float)deltaY };
	}
}
#pragma endregion
