#pragma once

#include "EditorWindow/ImGuiIncluder.h"
#include "EditorWindow/EditorFunction.h"
#include "EditorWindow/AssetManagerWindow.h"
#include "EditorWindow/PlayWindow.h"
#include "EditorWindow/SceneWindow.h"
#include "EditorWindow/FileManagerWindow.h"
#include "EditorWindow/MapManagerWindow.h"
#include "EditorWindow/ObjectCompWindow.h"
#include "EditorWindow/SceneObjWindow.h"
#include "../../../Renderer/include/Renderer/ResourcesManager.h"

#include "Script.h"
struct GLFWwindow;

GLFWwindow* initOpenGL();

class Editor
{
public:
	Editor();
	~Editor();
	void SetModelLoader(ModelLoader* inLoader) { modelLoader = inLoader; };
	void SetImgui(GLFWwindow* window);
	void SetScene(Scene& scene);
	void ImGuiInit();
	void RunEditor();
	void RenderImGui();

	Scene* currentSceneDisplay = nullptr;
	ResourcesManager* resourcesManager = nullptr;
	static std::deque<GameObject> preFabs;
	static bool isPlaying;
	static std::string pathAssets;

private:
	ModelLoader* modelLoader = nullptr;
	bool fileAlreadyExist = false;

	bool isCreatingAEntityFile = false;
	bool isCreatingAMapFile = false;

	bool isEditingAMapFile = false;

	bool isCreatingAFile = false;
	//ImGui::FileBrowser fileDialog;

	GLFWwindow* window = nullptr;
	
};

