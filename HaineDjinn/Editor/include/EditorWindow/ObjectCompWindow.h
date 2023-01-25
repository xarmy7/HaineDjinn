#pragma once
#include <string>

class ResourcesManager;
class Scene;
class PreFab;
class GameObject;

class ObjectCompManager
{
	// TODO: Remove unused variables
	static bool isSelectingATexture;
	static bool isSelectingAScript;
	static bool isSelectingAClassicScript;
	static bool isCreatingANewComponent;
	static bool isDeletingAComponent;
	
	static bool isCreatingAPersonnalModel;
	static bool isCreatingAEngineModel;
	


public:
	static char buf[255];
	static void RunObjectCompManager(const std::string& pathAssets, ResourcesManager* resourceManager, Scene* currentSceneDisplay);
	static void RunMaterialSelector(const std::string& pathAssets, ResourcesManager* resourceManager, GameObject* currentObjectDisplay);




	static void AddNewComponent(Scene* currentSceneDisplay);
	static void DeleteComponent(Scene* currentSceneDisplay);
};
