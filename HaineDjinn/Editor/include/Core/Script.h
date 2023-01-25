#pragma once

#include "../../../Renderer/include/Core/Component.h"

#include <string>
#include <filesystem>

//class ScriptFile : Component
//{
//public:
//	ScriptFile() = default;
//	ScriptFile(GameObject* gameObject);
//	ScriptFile(std::filesystem::path scriptFileName, GameObject* gameObject);
//
//	~ScriptFile() {};
//	void Clear();
//	void ComponentEditor() override;
//	std::string name;
//};
//
//class ScriptSystem
//{
//private:
//
//public:
//	ScriptSystem() = default;
//	~ScriptSystem() = default;
//
//	ScriptFile* currentScript = nullptr;
//
//	std::vector<std::string> allScript;
//
//	void PlayScript();
//	void LoadScript(const std::string& pathAssets);
//	void CreateScript();
//	void LaunchScript();
//	void BeginOfScript(const std::string& file);
//	bool IsTheFileEmpty(const std::string& file);
//	void SetScript(int index);
//	std::string GetName(int index);
//};