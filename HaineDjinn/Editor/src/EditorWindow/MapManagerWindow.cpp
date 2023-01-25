#include "EditorWindow/MapManagerWindow.h"
#include "EditorWindow/EditorFunction.h"
#include "EditorWindow/SceneWindow.h"
#include "Renderer/ModelLoader.h"
#include "Core/Editor.h"
#include "Game/SoundSystem.h"
#include <IrrKlang/irrKlang.h>
#include <sstream>
#include <istream>

bool				 MapManager::canOpenMapFolder = false;
bool				 MapManager::isGenerated = false;
char				 MapManager::buf[255]{};
bool				 MapManager::fileAlreadyExist = false;
std::vector<MapFile> MapManager::mapFiles;
MapFile				 MapManager::newMapFile;
bool			     MapManager::canEdit = false;
bool			     MapManager::keepASave = false;
std::string			 MapManager::oldNameSave;
MapFile				 MapManager::editedMap;
bool				 MapManager::isCreatingAMapFile = false;
bool				 MapManager::aMapIsSelected = false;
MapFile				 MapManager::selectedMap;
bool				 MapManager::isSaved = false;



MapFile::MapFile(std::filesystem::path mapFileName)
{
	name = mapFileName.stem().string();
}

void MapFile::Clear()
{
	name.clear();
}

void MapManager::LoadMaps(const std::string& pathAssets)
{
	std::string pathMap = pathAssets + "Map";
	if (std::filesystem::exists(pathMap))
	{
		for (const auto& entry : std::filesystem::directory_iterator(pathMap))
			mapFiles.push_back(MapFile(entry.path().string()));
	}
}


void MapManager::RunMapManager(const std::string& pathAssets, ResourcesManager* resourceManager, Scene* currentSceneDisplay, ModelLoader* modelLoader)
{
	ImGui::Begin("Map Manager");
	int countReset = 1;
	if (!aMapIsSelected)
	{
		if (ImGui::Button("ADD NEW MAP") && canOpenMapFolder)
			isCreatingAMapFile = true;

		ImGui::Text("Current Map availaible:");
		std::string pathMap = pathAssets + "Map";
		if (std::filesystem::exists(pathMap))
		{
			canOpenMapFolder = true;
			for (MapFile mapEntry : MapManager::mapFiles)
			{
				ImGui::SameLine(0.f, 10.f);
				if (ImGui::Button(mapEntry.name.c_str()))
				{
					aMapIsSelected = true;
					selectedMap = mapEntry;
					currentSceneDisplay->sceneName = selectedMap.name;
					std::deque<GameObject* > empty;
					currentSceneDisplay->gDef.Load(resourceManager, empty, currentSceneDisplay, modelLoader);

					if (currentSceneDisplay->cameras.size() == 0)
					{
						currentSceneDisplay->cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
						currentSceneDisplay->currentCamera = currentSceneDisplay->cameras.back();
						currentSceneDisplay->currentCamera->isMainCamera = true;
						currentSceneDisplay->currentCamera->name = "TempName";
					}
				}
			}
		}
		else
		{
			if (std::filesystem::create_directory(pathMap))
				canOpenMapFolder = true;
			else
			{
				canOpenMapFolder = false;
				std::cout << "create_directory() failed, Map folder wasn't created" << std::endl;
			}
		}
	}
	else
	{
		if (ImGui::Button("Back"))
		{
			aMapIsSelected = false;
			selectedMap.Clear();
			currentSceneDisplay->gameObjects.clear();
			currentSceneDisplay->cameras.clear();
		}

		ImGui::SameLine(0.f, 10.f);

		if (ImGui::Button("Save"))
		{
			SoundSystem::PlaySound2D("Sounds/snd_fragment_retrievewav-14728.mp3");

			// start the sound engine with default parameters
			irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

			// play some sound stream, looped
			if(engine != nullptr)
				engine->play2D("Sounds/snd_fragment_retrievewav-14728.mp3", false);
			std::deque<GameObject* > empty;

			currentSceneDisplay->gDef.Save(empty, currentSceneDisplay);
			isSaved = true;
		}
	}
	ImGui::End();
}

void MapManager::SaveMap(const std::string& pathAssets, MapFile& mapFile)
{
	if (!keepASave)
	{
		std::string fileToRemove = pathAssets + "Map/" + oldNameSave + ".txt";
		int status = remove(fileToRemove.c_str());

		for (int i = 0; i < mapFiles.size(); i++)
		{
			if (oldNameSave == mapFiles[i].name)
				mapFiles.erase(mapFiles.begin() + i);
		}
	}

	std::string	fileName = pathAssets + "Map/" + mapFile.name + ".txt";

	std::ofstream MyFile(pathAssets + "Map/" + mapFile.name + ".txt");

	MyFile.close();
}

void MapManager::CreateNewMapFile(const std::string& pathAssets, GameDefinitions& gDef)
{
	ImGui::Begin("Creating New Map");
	ImGui::InputText("Map Name", buf, sizeof(buf));
	{
		newMapFile.name = buf;
		std::string pathMap = pathAssets + "Map";
		bool sameName = false;
		for (const auto& entry : std::filesystem::directory_iterator(pathMap))
			if (entry.path().stem().string() == newMapFile.name)
				sameName = true;

		if (sameName)
			fileAlreadyExist = true;
		else
			fileAlreadyExist = false;

		if (fileAlreadyExist)
			ImGui::Text("File Already exist, find another name");

		if (newMapFile.name != "" && !fileAlreadyExist)
		{
			if (ImGui::Button("Validate"))
			{
				gDef.CreateMap(newMapFile);

				SaveMap(pathAssets, newMapFile);
				mapFiles.push_back(newMapFile);
				newMapFile.Clear();
				isGenerated = false;
				isCreatingAMapFile = false;
			}
		}

		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Abort"))
		{
			newMapFile.Clear();
			isGenerated = false;
			fileAlreadyExist = false;
			isCreatingAMapFile = false;
		}
	}


	ImGui::End();
}
