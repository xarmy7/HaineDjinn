#pragma once
#include "ImGuiIncluder.h"
#include "../../../Engine/include/Core/Scene.h"
#include "MapManagerWindow.h"

class ModelLoader;

struct GLFWwindow;

class SceneRenderer
{
	static bool camLock;
public:
	static bool closing;

	static void RunScene(GLFWwindow* window, ResourcesManager* resourceManager, Scene* currentSceneDisplay, ModelLoader* modelLoader);

	static void DrawGuizmo(Scene* currentSceneDisplay);
};
