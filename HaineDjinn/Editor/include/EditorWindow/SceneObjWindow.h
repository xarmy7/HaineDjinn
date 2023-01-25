#pragma once

#include <iostream>

#include "ImGuiIncluder.h"
#include "EditorWindow/SceneWindow.h"


#include "../../../Engine/include/Core/GameDefinitions.h"

class ModelLoader;

class SceneObjManager
{
private:
	static ModelLoader* modelLoader;
	static bool isCreatingAnPersonnalModel;
	static bool isCreatingAnEngineModel;
	static bool isCreatingACamera;
	
public:

	static void SetLoader(ModelLoader* inLoader) { modelLoader = inLoader; };

	static char buf[255];
	static bool isSelectingAnObject;
	static std::vector<std::string> objectsName;
	static void RunSceneObjManager(Scene* currentSceneDisplay, ResourcesManager* resourcesManager);
	static void DisplayElement(Scene* currentSceneDisplay);
	static void ModelSelector(Scene* currentSceneDisplay, ResourcesManager* resourcesManager);
	static void CameraCreator(Scene* currentSceneDisplay);
	static void ObjectSelector();
};
