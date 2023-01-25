#include <iostream>
#include <fstream>

#include <IMGUI/imgui.h>
#include <IMGUI/imfilebrowser.h>

#include "EditorWindow/FileManagerWindow.h"

std::string FileManager::fileName = "";
bool FileManager::isNamingAfile = false;
bool FileManager::itsAFolder = false;
bool FileManager::itsAHeader = false;
bool FileManager::itsASource = false;
char FileManager::buf[255]{};


void FileManager::RunFileManager(ImGui::FileBrowser& fileDialog, bool& isCreatingAFile)
{
	ImGui::Begin("Folder Manager");
	if (ImGui::Button("open new"))
		fileDialog.Open();
	if (ImGui::Button("create new") && !isCreatingAFile)
		isCreatingAFile = true;

	fileDialog.Display();

	if (fileDialog.HasSelected())
	{
		if (fileDialog.GetSelected().extension() == ".txt")
		{
			std::string FileApp_Name = "notepad.exe " + fileDialog.GetSelected().string();
			system(FileApp_Name.c_str());
		}

		std::cout << "Selected filename" << fileDialog.GetSelected().string() << std::endl;
		fileDialog.ClearSelected();
	}
	ImGui::End();
}

void FileManager::CreateNewFile(const std::string& pathAssets, bool& isCreatingAFile)
{
	std::string folder = "Scripts\\";
	std::string pathFolder = pathAssets + folder;

	ImGui::Begin("File type");
	ImGui::InputText("Name", buf, sizeof(buf));
	{
		fileName = buf;
		if (fileName != "")
		{
			/*ImGui::Checkbox("Folder", &itsAFolder);
			ImGui::Checkbox("Source (.cpp)", &itsASource);
			ImGui::Checkbox("Header (.h)", &itsAHeader);*/

			if (ImGui::Checkbox("Folder", &itsAFolder))
			{
				itsASource = false;
				itsAHeader = false;
			}
			if (ImGui::Checkbox("Source (.cpp)", &itsASource))
			{
				itsAFolder = false;
				itsAHeader = false;
			}
			if (ImGui::Checkbox("Header (.h)", &itsAHeader))
			{
				itsAFolder = false;
				itsASource = false;
			}

			if (ImGui::Button("Validate"))
			{
				if (itsAFolder)
				{
					std::string filePath = pathFolder + fileName;
					if (std::filesystem::create_directory(filePath))
						std::cout << "created directory - " << filePath << std::endl;
					else
						std::cout << "create_directory() failed" << std::endl;
					isCreatingAFile = false;
				}
				else if (itsASource)
				{
					std::ofstream MyFile(pathFolder + fileName + ".cpp");
					MyFile.close();
					isCreatingAFile = false;
				}
				else if (itsAHeader)
				{
					std::ofstream MyFile(pathFolder + fileName + ".h");
					MyFile.close();
					isCreatingAFile = false;
				}
			}

		}
	}
	ImGui::End();
}
