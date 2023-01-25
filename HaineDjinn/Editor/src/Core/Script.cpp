/*
#include "Core/Script.h"
#include "Renderer/ResourcesManager.h"
#include "Renderer/ModelInstance.h"

#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/imfilebrowser.h>

#include <iostream>
#include <fstream>

ScriptFile::ScriptFile(GameObject* gameObject) 
	: Component(gameObject, ComponentType::SCRIPT, {})
{
	
}

ScriptFile::ScriptFile(std::filesystem::path scriptFileName, GameObject* gameObject) 
	: Component(gameObject, ComponentType::SCRIPT, {})
{
	name = scriptFileName.stem().string();
}

void ScriptFile::ComponentEditor()
{
	//ModelInstance* instance = static_cast<ModelInstance*>(gameObject->GetComponent(ComponentType::MODELINSTANCE));

	if (ImGui::TreeNodeEx("Script", ImGuiTreeNodeFlags_Framed))
	{
		ImGui::TextColored({ 1.0f, 0.0f, 0.9f, 1.f }, "Choose your script");

		if (ImGui::BeginCombo("script", currentScript == nullptr ? "(none)" : currentScript->name.c_str()))
		{
			for (unsigned int i = 0; i < allScript.size() - 1; i++)
			{
				//const aiAnimation* anim = scene->mAnimations[i];

				if (ImGui::Selectable(GetName(i).c_str()))
					SetScript(i);
			}
			ImGui::EndCombo();
		}
		ImGui::TreePop();
	}
}

void ScriptSystem::SetScript(int index)
{
	delete currentScript;
	currentScript = new ScriptFile(gameObject);
}

void ScriptSystem::LoadScript(const std::string& pathAssets)
{
	std::string pathScript = pathAssets + "Scripts";
	if (std::filesystem::exists(pathScript))
	{
		for (const auto& entry : std::filesystem::directory_iterator(pathScript))
		{
			allScript.push_back(entry.path().stem().string());

			if (IsTheFileEmpty(entry.path().string()))
				BeginOfScript(entry.path().string());
		}
	}
}

bool ScriptSystem::IsTheFileEmpty(const std::string& file)
{
	std::string line;
	std::ifstream myfile(file);
	if (myfile.is_open())
	{
		if (getline(myfile, line))
		{
			myfile.close();
			return false;
		}
		else
		{
			std::cout << "Nothing in the file" << std::endl;
			myfile.close();
			return true;
		}
	}
}

void ScriptSystem::BeginOfScript(const std::string& file)
{
	std::ofstream myfile;
	myfile.open(file);
	myfile << "void Init()\n";
	myfile << "{\n";
	myfile << "\n";
	myfile << "}\n";

	myfile << "void Update(float deltatime)\n";
	myfile << "{\n";
	myfile << "\n";
	myfile << "}\n";
	myfile.close();
}

void ScriptSystem::CreateScript()
{
	
}

void ScriptSystem::LaunchScript()
{

}

void ScriptSystem::PlayScript()
{
	/*if (isPlaying)
}

std::string ScriptSystem::GetName(int index)
{
	for (int j = allScript[index].size(); j >= 0; j--)
	{
		if (allScript[index][j] == '\\')
		{
			index = j;
			break;
		}
	}

	return allScript[index].substr(index);
}*/