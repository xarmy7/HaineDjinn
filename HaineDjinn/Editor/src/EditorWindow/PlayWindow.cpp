#include "EditorWindow/PlayWindow.h"
#include "Core/Editor.h"
#include "Game/InputSystem.h"
#include "Core/App.h"

PlayWindow::PlayWindow()
{
	
}

void PlayWindow::Play(Scene* currentSceneDisplay)
{
	ImGui::Begin("Play");

	if (ImGui::IsWindowFocused() && InputSystem::GetMouseButton(1))
	{
		std::deque<GameObject* > empty;
		currentSceneDisplay->gDef.Save(empty, currentSceneDisplay);
		Editor::isPlaying = true;
	}

	ImGui::End();
}

void PlayWindow::SetPlayerScene(Scene& scene, GLFWwindow& _window, CameraFollow& camFollow)
{
	if (!saveOldCam)
	{
		oldCamera = scene.currentCamera;
		saveOldCam = true;
	}

	scene.currentCamera = &camFollow.camera;
	currentScene = &scene;
	window = &_window;
}

void PlayWindow::RunPlayerWindow(ResourcesManager* resourceManager,Scene* scene, ModelLoader* modelLoader, MoveEnemy& moveEnemy)
{
	App::InitImGuiAndDock();

	ImGui::Begin("PlayWindow");

	/*if (!MapManager::aChunkIsSelected)
		ImGui::Text("You need to select a chunk first!");*/

	ImVec2 wsize = ImGui::GetWindowSize();
	wsize.y -= 35.f;
	currentScene->vpSize = { wsize.x, wsize.y };

	ImGui::Image((ImTextureID)(size_t)currentScene->renderer->cbo, wsize, ImVec2(0, 1), ImVec2(1, 0));

	if (InputSystem::GetInputDown("Escape"))
	{
		scene->currentCamera = oldCamera;
		Editor::isPlaying = false;
		saveOldCam = false;


		scene->gameObjects.clear();
		scene->cameras.clear();
		scene->lights.clear();
		scene->GetPhysicsCommon()->destroyPhysicsWorld(scene->GetPhysicsWorld());
		scene->SetPhysics(scene->GetPhysicsCommon());
		std::deque<GameObject*> empty;

		scene->gDef.Load(resourceManager, empty, scene, modelLoader);
		moveEnemy = {};
	}

	ImGui::End();
	ImGui::End();
}