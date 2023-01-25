
#include "EditorWindow/SceneObjWindow.h"
#include "Core/Editor.h"
#include "Renderer/ModelLoader.h"
#include "Core/Debug.h"
#include "Core/Transform.h"
#include "Physics/CubeCollider.h"
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/Rigidbody.h"
#pragma warning (disable: 4996) 


char SceneObjManager::buf[255]{};
bool SceneObjManager::isSelectingAnObject = false;
std::vector<std::string> SceneObjManager::objectsName;

ModelLoader* SceneObjManager::modelLoader = nullptr;
bool SceneObjManager::isCreatingAnPersonnalModel = false;
bool SceneObjManager::isCreatingAnEngineModel = false;
bool SceneObjManager::isCreatingACamera = false;


void DrawObjectsButton(Scene* currentSceneDisplay, char buf[255])
{
	for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
	{
		if (ImGui::Button(currentSceneDisplay->gameObjects[i]->name.c_str()))
		{
			currentSceneDisplay->currentObjectDisplay = currentSceneDisplay->gameObjects[i];
			currentSceneDisplay->currentObjectDisplay->isSelected = true;
			currentSceneDisplay->anObjectIsSelected = true;
			std::strncpy(buf, currentSceneDisplay->currentObjectDisplay->name.c_str(), currentSceneDisplay->currentObjectDisplay->name.size());
		}

		std::string buttonName = "Delete " + currentSceneDisplay->gameObjects[i]->name;
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button(buttonName.c_str()))
		{
			currentSceneDisplay->anObjectIsSelected = false;
			currentSceneDisplay->currentObjectDisplay = nullptr;
			delete currentSceneDisplay->gameObjects[i];
			currentSceneDisplay->gameObjects.erase(currentSceneDisplay->gameObjects.begin() + i);
		}
	}
}
void DrawCamerasButton(Scene* currentSceneDisplay)
{
	for (int i = 0; i < currentSceneDisplay->cameras.size(); i++)
	{
		if (ImGui::Button(currentSceneDisplay->cameras[i]->name.c_str()))
		{
			currentSceneDisplay->currentCamera = currentSceneDisplay->cameras[i];
			currentSceneDisplay->currentCamera->isMainCamera = true;
		}

		if (currentSceneDisplay->cameras[i] != currentSceneDisplay->currentCamera)
		{
			std::string buttonName = "Delete " + currentSceneDisplay->cameras[i]->name;
			ImGui::SameLine(0.f, 10.f);
			if (ImGui::Button(buttonName.c_str()))
			{
				for (int y = (int)currentSceneDisplay->cameras.size() - 1; y >= 0; y--)
				{
					if (y == i)
						currentSceneDisplay->cameras.erase(currentSceneDisplay->cameras.begin() + y);
				}
			}
		}
	}
}


void SceneObjManager::DisplayElement(Scene* currentSceneDisplay)
{
	ImGui::Text("Map's Objects:");
	if (ImGui::TreeNodeEx("Personnal Model", ImGuiTreeNodeFlags_Framed))
	{
		DrawObjectsButton(currentSceneDisplay, buf);
		ImGui::TreePop();
	}
	if (ImGui::TreeNodeEx("Camera(s)", ImGuiTreeNodeFlags_Framed))
	{
		DrawCamerasButton(currentSceneDisplay);
		ImGui::TreePop();
	}
}

void CreateEmptyObject(Scene* currentSceneDisplay, bool& isSelectingAnObject, bool& isCreatingAnEngineModel)
{
	currentSceneDisplay->anObjectIsSelected = false;
	currentSceneDisplay->gameObjects.emplace_back(new GameObject);
	GameObject* object = currentSceneDisplay->gameObjects.back();

	bool isNamed = false;
	for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
	{
		if (currentSceneDisplay->gameObjects[i]->name == "EmptyObject(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")")
		{
			currentSceneDisplay->gameObjects[i]->name = "EmptyObject(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")" + "[2]";
			isNamed = true;
		}
	}
	if (!isNamed)
		object->name = "EmptyObject(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")";

	currentSceneDisplay->currentObjectDisplay = object;
	currentSceneDisplay->anObjectIsSelected = true;

	isSelectingAnObject = false;
	isCreatingAnEngineModel = false;
}
void CreateCubeCollider(Scene* currentSceneDisplay, bool& isSelectingAnObject, bool& isCreatingAnEngineModel)
{
	currentSceneDisplay->anObjectIsSelected = false;
	currentSceneDisplay->gameObjects.emplace_back(new GameObject);
	GameObject* object = currentSceneDisplay->gameObjects.back();

	bool isNamed = false;
	for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
	{
		if (currentSceneDisplay->gameObjects[i]->name == "EmptyCube(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")")
		{
			object->name = "EmptyCube(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")" + "[2]";
			isNamed = true;
		}
	}
	if (!isNamed)
		object->name = "EmptyCube(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")";

	Rigidbody* rg = new Rigidbody(object);
	rg->nameComp = "RIGIDBODY";
	object->components.push_back(rg);

	CubeCollider* cc = new CubeCollider(object, rg, { 0.5f,0.5f,0.5f });
	cc->nameComp = "CUBECOLLIDER";
	cc->name = "CubeCollider(0)";
	object->components.push_back(cc);

	currentSceneDisplay->currentObjectDisplay = object;
	currentSceneDisplay->anObjectIsSelected = true;

	isSelectingAnObject = false;
	isCreatingAnEngineModel = false;
}
void CreateSphereCollider(Scene* currentSceneDisplay, bool& isSelectingAnObject, bool& isCreatingAnEngineModel)
{
	currentSceneDisplay->anObjectIsSelected = false;
	currentSceneDisplay->gameObjects.emplace_back(new GameObject);
	GameObject* object = currentSceneDisplay->gameObjects.back();

	bool isNamed = false;
	for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
	{
		if (currentSceneDisplay->gameObjects[i]->name == "EmptySphere(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")")
		{
			object->name = "EmptySphere(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")" + "[2]";
			isNamed = true;
		}
	}
	if (!isNamed)
		object->name = "EmptySphere(" + std::to_string(currentSceneDisplay->gameObjects.size()) + ")";

	Rigidbody* rg = new Rigidbody(object);
	rg->nameComp = "RIGIDBODY";
	object->components.push_back(rg);

	SphereCollider* sc = new SphereCollider(object, rg, 0.5f);
	sc->nameComp = "SPHERECOLLIDER";
	sc->name = "SphereCollider(0)";
	object->components.push_back(sc);

	currentSceneDisplay->currentObjectDisplay = object;
	currentSceneDisplay->anObjectIsSelected = true;

	isSelectingAnObject = false;
	isCreatingAnEngineModel = false;
}



void SceneObjManager::ModelSelector(Scene* currentSceneDisplay, ResourcesManager* resourcesManager)
{
	if (ImGui::Button("<---"))
	{
		isCreatingAnEngineModel = false;
		isCreatingAnPersonnalModel = false;
	}

	ImGui::Text("|");
	ImGui::Text("|");
	ImGui::Text("|");

	if (isCreatingAnEngineModel)
	{
		if (ImGui::Button("Create empty Object"))
			CreateEmptyObject(currentSceneDisplay, isSelectingAnObject, isCreatingAnEngineModel);
		
		if (ImGui::Button("Create empty CubeCollider"))
			CreateCubeCollider(currentSceneDisplay, isSelectingAnObject, isCreatingAnEngineModel);

		if (ImGui::Button("Create empty SphereCollider"))
			CreateSphereCollider(currentSceneDisplay, isSelectingAnObject, isCreatingAnEngineModel);

	}
	else if (isCreatingAnPersonnalModel)
	{
		ImGui::Text("objects in the \"Models\" folder:");
		std::string path = Editor::pathAssets + "Models";
		if (std::filesystem::exists(path))
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
			{
				if (entry.path().extension() == ".obj" || entry.path().extension() == ".fbx" || entry.path().extension() == ".OBJ" || entry.path().extension() == ".FBX")
				{
					if (ImGui::Button(entry.path().stem().string().c_str()))
					{
						currentSceneDisplay->anObjectIsSelected = false;
						currentSceneDisplay->gameObjects.emplace_back(new GameObject);
						GameObject* object = currentSceneDisplay->gameObjects.back();
						ModelInstance* instance = new ModelInstance(object);
						Model* model = &modelLoader->FindModel(std::filesystem::relative(entry.path()).string().c_str(), instance);

						object->components.push_back(instance);
						HD_LOG(model->modelName, LogSeverity::LOG_MESSAGE);

						instance->model = model;
						instance->model->filePath = path + "\\" + instance->model->modelName + entry.path().extension().string();

						bool isNamed = false;
						for (int i = 0; i < currentSceneDisplay->gameObjects.size(); i++)
						{
							if (currentSceneDisplay->gameObjects[i]->name == model->modelName + '(' + std::to_string(currentSceneDisplay->gameObjects.size()) + ')')
							{
								object->name = model->modelName + '(' + std::to_string(currentSceneDisplay->gameObjects.size()) + ')' + "[2]";
								isNamed = true;
							}
						}
						if (!isNamed)
							object->name = model->modelName + '(' + std::to_string(currentSceneDisplay->gameObjects.size()) + ')';


						instance->isStatic = true;
						instance->scale = 0.025f;
						instance->position = { 0.f,1.f,0.f };
						instance->eulerAngles = { (float)(rand() % 10560), (float)(rand() % 10980), (float)(rand() % 1010) };
						instance->mat = *model->parts.back().material;

						if (entry.path().extension() == ".FBX" || entry.path().extension() == ".fbx")
							resourcesManager->AddToLoad(entry.path().string(), ResourceType::ANIMATION);
						isSelectingAnObject = false;
						isCreatingAnPersonnalModel = false;
					}
				}
			}
		}
	}
}

void SceneObjManager::CameraCreator(Scene* currentSceneDisplay)
{
	currentSceneDisplay->cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
	bool nameAlreadyExist = false;
	for (int i = 0; i < currentSceneDisplay->cameras.size(); i++)
	{
		if (currentSceneDisplay->cameras[i]->name == ("Camera(" + std::to_string(currentSceneDisplay->cameras.size()) + ")"))
			nameAlreadyExist = true;
	}

	if (nameAlreadyExist)
		currentSceneDisplay->cameras.back()->name = "Camera(" + std::to_string(currentSceneDisplay->cameras.size() + 1) + ")";
	else
		currentSceneDisplay->cameras.back()->name = "Camera(" + std::to_string(currentSceneDisplay->cameras.size()) + ")";

	isCreatingACamera = false;
	isSelectingAnObject = false;
}

void SceneObjManager::ObjectSelector()
{
	ImGui::Text("Select Object Type:");
	if (ImGui::Button("Personnal Model"))
		isCreatingAnPersonnalModel = true;
	if (ImGui::Button("Engine Model"))
		isCreatingAnEngineModel = true;
	if (ImGui::Button("Camera"))
		isCreatingACamera = true;

}

void SceneObjManager::RunSceneObjManager(Scene* currentSceneDisplay, ResourcesManager* resourcesManager)
{
	ImGui::Begin("Scene's Component");

	if (!isSelectingAnObject)
	{
		if (ImGui::Button("Add New Object"))
			isSelectingAnObject = true;

		float* pos[3];
		pos[0] = &currentSceneDisplay->currentCamera->position.x;
		pos[1] = &currentSceneDisplay->currentCamera->position.y;
		pos[2] = &currentSceneDisplay->currentCamera->position.z;
		ImGui::DragFloat3("Cam Position", *pos);

		float* angle[2];
		angle[0] = &currentSceneDisplay->currentCamera->yaw;
		angle[1] = &currentSceneDisplay->currentCamera->pitch;
		ImGui::DragFloat2("Cam Angle", *angle);

		DisplayElement(currentSceneDisplay);
	}
	else if (isCreatingAnPersonnalModel || isCreatingAnEngineModel)
		ModelSelector(currentSceneDisplay, resourcesManager);
	else if (isCreatingACamera)
		CameraCreator(currentSceneDisplay);
	else
	{
		ObjectSelector();
		
		if (ImGui::Button("Back"))
			isSelectingAnObject = false;
	}
	ImGui::End();
}
