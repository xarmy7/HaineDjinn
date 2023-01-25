#pragma once
#include <string>

#include "ImGuiIncluder.h"

namespace ImGui
{
	class FileBrowser;
}

class FileManager
{
public:
	static std::string fileName;
	static bool isNamingAfile;
	static bool itsAFolder;
	static bool itsAHeader;
	static bool itsASource;
	static char buf[255];


	static void RunFileManager(ImGui::FileBrowser& fileDialog, bool& isCreatingAFile);
	static void CreateNewFile(const std::string& pathAssets, bool& isCreatingAFile);
};