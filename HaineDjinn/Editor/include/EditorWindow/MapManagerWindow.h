#pragma once
//#include "ImGuiIncluder.h"
#include "Renderer/Renderer.h"
#include <filesystem>

class GameDefinitions;
class ResourcesManager;
class Scene;
class ModelLoader;

class MapFile
{
	public:
		MapFile() = default;
		MapFile(std::filesystem::path mapFileName);
		~MapFile() = default;
		void Clear();

		std::string name;
};



class MapManager
{
	static bool canOpenMapFolder;
	static bool isGenerated;
	static char buf[255];
	static bool fileAlreadyExist;

	static bool canEdit;
	static std::string oldNameSave;
	static bool keepASave;
	static MapFile editedMap;

	static MapFile newMapFile;

public:
	static MapFile selectedMap;
	static bool aMapIsSelected;
	static bool isSaved;

	static bool isCreatingAMapFile;
	static std::vector<MapFile> mapFiles;
	static void LoadMaps(const std::string& pathAssets);
	static void RunMapManager(const std::string& pathAssets, ResourcesManager* resourceManager, Scene* currentSceneDisplay, ModelLoader* modelLoader);
	static void CreateNewMapFile(const std::string& pathAssets, GameDefinitions& gDef);
	static void SaveMap(const std::string& pathAssets, MapFile& mapFile);
};