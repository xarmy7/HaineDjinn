#pragma once
#include "ImGuiIncluder.h"
#include "Core/Scene.h"
#include "CameraFollow.h"
#include "Enemy.h"

struct GLFWwindow;

class PlayWindow
{
public:
	PlayWindow();
	static void Play(Scene* currentSceneDisplay);
	void RunPlayerWindow(ResourcesManager* resourceManager,Scene* scene, ModelLoader* modelLoader, MoveEnemy& moveEnemy);
	void SetPlayerScene(Scene& scene, GLFWwindow& _window, CameraFollow& camFollow);

	Scene* currentScene = {};
	Camera* oldCamera = {};
	GLFWwindow* window = {};

	bool saveOldCam = false;
};