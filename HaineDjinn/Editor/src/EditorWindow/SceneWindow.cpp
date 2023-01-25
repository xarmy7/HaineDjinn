
#include <thread>
#include <chrono>
#include <IrrKlang/irrKlang.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imGuizmo.h>

#include "EditorWindow/SceneWindow.h"
#include "EditorWindow/SceneObjWindow.h"
#include "EditorWindow/MapManagerWindow.h"
#include "Renderer/ModelLoader.h"
#include "Game/InputSystem.h"
#include "Game/SoundSystem.h"
#include "Editorwindow/editorfunction.h"
#include "Core/Editor.h"
#include "Core/Transform.h"



bool SceneRenderer::camLock = false;
bool SceneRenderer::closing = false;


//bool SceneRenderer::aMapIsSelected = false;
//bool SceneRenderer::aChunkIsSelected = false;
//Chunk SceneRenderer::selectedChunk;
//MapFile SceneRenderer::selectedMap;


void SceneRenderer::DrawGuizmo(Scene* currentSceneDisplay)
{
	if (currentSceneDisplay->anObjectIsSelected)
	{
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

		Camera* sceneCamera = currentSceneDisplay->currentCamera;
		mat4x4 cameraProj = sceneCamera->GetProjectionMatrix();
		mat4x4 cameraView = sceneCamera->GetViewMatrix();

		Transform* tr = currentSceneDisplay->currentObjectDisplay->transform;
		mat4x4 trMat = mat4::Identity();
		ImGuizmo::RecomposeMatrixFromComponents(tr->position.e, tr->euler.e, tr->scale.e, trMat.e);

		if(InputSystem::GetInput("LControl") || InputSystem::GetInput("RControl"))
			ImGuizmo::Manipulate(cameraView.e, cameraProj.e,ImGuizmo::OPERATION::SCALE, ImGuizmo::LOCAL, trMat.e);
		else
			ImGuizmo::Manipulate(cameraView.e, cameraProj.e, ImGuizmo::OPERATION::TRANSLATE, ImGuizmo::LOCAL, trMat.e);
			
			if (ImGuizmo::IsUsing())
			{
				ImGuizmo::DecomposeMatrixToComponents(trMat.e, tr->position.e, tr->euler.e, tr->scale.e);
				tr->rotation = QuaternionFromEuler(tr->euler);
				tr->onMoveEvent.Broadcast(nullptr);
			}
	}
}

void SceneRenderer::RunScene(GLFWwindow* window, ResourcesManager* resourceManager, Scene* currentSceneDisplay, ModelLoader* modelLoader)
{
	ImGui::Begin("Scene");
	ImGuiIO io = ImGui::GetIO();

	if (!MapManager::aMapIsSelected)
	{
		ImGui::Text("No Map Selected in the Map Manager");
	}//DisplayMaps(resourceManager, currentSceneDisplay, modelLoader);
	else
	{
		if (ImGui::IsWindowFocused())
			Editor::isPlaying = false;

		ImVec2 wsize = ImGui::GetWindowSize();
		wsize.y -= 35.f;
		currentSceneDisplay->vpSize = { wsize.x, wsize.y };

		ImGui::Image((ImTextureID)(size_t)currentSceneDisplay->renderer->cbo, wsize, ImVec2(0, 1), ImVec2(1, 0));

		/*ImGui::Begin("Debug");
		ImGui::Image((ImTextureID)(size_t)currentSceneDisplay->renderer->pickingCBO, wsize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();*/


		DrawGuizmo(currentSceneDisplay);
		
		if (InputSystem::GetMouseButton(3) || InputSystem::GetMouseButton(4) || InputSystem::GetMouseButton(5))
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			currentSceneDisplay->currentCamera->blocked = false;
			camLock = true;
		}
		else
		{
			camLock = false;
			currentSceneDisplay->currentCamera->blocked = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (InputSystem::GetInputDown("Escape"))
			closing = true;

		if(closing)
		{
			if(!MapManager::isSaved)
			{
				ImGui::Begin("WARNING");
				ImGui::Text("You didnt saved the current scene!");
				if (ImGui::Button("Save and Quit"))
				{
					MapManager::aMapIsSelected = false;
					MapManager::isSaved = false;
					closing = false;
					std::deque<GameObject* > empty;
					currentSceneDisplay->gDef.Save(empty , currentSceneDisplay);
					currentSceneDisplay->anObjectIsSelected = false;
					for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
						delete currentSceneDisplay->gameObjects[i];
					currentSceneDisplay->gameObjects.clear();
					currentSceneDisplay->cameras.clear();
					//MapManager::selectedMap.objects.clear();
					ImGui::SameLine(0.f, 10.f);
				}
				ImGui::SameLine(0.f, 10.f);
				if (ImGui::Button("Save"))
				{
					SoundSystem::PlaySound2D("Sounds/snd_fragment_retrievewav-14728.mp3");

					MapManager::isSaved = false;
					std::deque<GameObject* > empty;
					currentSceneDisplay->gDef.Save(empty, currentSceneDisplay);
					closing = false;
					ImGui::SameLine(0.f, 10.f);
				}
				ImGui::SameLine(0.f, 10.f);
				if (ImGui::Button("Quit"))
				{
					MapManager::aMapIsSelected = false;
					MapManager::isSaved = false;
					currentSceneDisplay->anObjectIsSelected = false;
					for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
						delete currentSceneDisplay->gameObjects[i];
					currentSceneDisplay->gameObjects.clear();
					currentSceneDisplay->cameras.clear();
					closing = false;
				}
				ImGui::SameLine(0.f, 10.f);
				if (ImGui::Button("X"))
					closing = false;
		
				ImGui::End();
			}
			else
			{
				MapManager::aMapIsSelected = false;
				closing = false;
			}
		}
	}
	ImGui::End();
}
