#include "EditorWindow/ObjectCompWindow.h"
#include "EditorWindow/Editorfunction.h"
#include "Core/Editor.h"
#include "../../../Renderer/include/Renderer/ResourcesManager.h"
#include "Core/Scene.h"
#include "Core/Transform.h"
#include "Core/Component.h"
#include "Core/Script.h"
#include "Physics/CubeCollider.h"
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/Rigidbody.h"
#include "PhysTestComp.h"

char ObjectCompManager::buf[255]{};
bool ObjectCompManager::isSelectingATexture = false;
bool ObjectCompManager::isSelectingAClassicScript = false;
bool ObjectCompManager::isCreatingANewComponent = false;
bool ObjectCompManager::isDeletingAComponent = false;


void ObjectCompManager::RunObjectCompManager(const std::string& pathAssets, ResourcesManager* resourceManager, Scene* currentSceneDisplay)
{
	ImGui::Begin("Object's Component");
	if (currentSceneDisplay->anObjectIsSelected)
	{

		if (ImGui::Button("Save as a Prefab"))
			//Editor::preFabs.push_back(*currentSceneDisplay->currentObjectDisplay);


		EditFileName(currentSceneDisplay->currentObjectDisplay->name, buf, IM_ARRAYSIZE(buf));

		ImGui::Checkbox("is Static", &currentSceneDisplay->currentObjectDisplay->isStatic);

		ModelInstance* instance = static_cast<ModelInstance*>(currentSceneDisplay->currentObjectDisplay->GetComponent(ComponentType::MODELINSTANCE));
		if (instance != nullptr)
		{
			//const aiScene* scene = instance->model->GetAssimpScene();
			//if (scene->HasAnimations())// instance->model->extension == ".FBX" || instance->model->extension == ".fbx")
			//{
			//	if (ImGui::Button("ADD Animator"))
			//	{
			//		Animator* animator = new Animator(currentSceneDisplay->currentObjectDisplay);
			//		currentSceneDisplay->currentObjectDisplay->components.push_back(animator);
			//	}
			//}

			if (!isSelectingATexture)
			{
				if (ImGui::Button("Select Texture"))
					isSelectingATexture = true;
			}
			else
				RunMaterialSelector(pathAssets, resourceManager, currentSceneDisplay->currentObjectDisplay);
		}

		for (Component* component : currentSceneDisplay->currentObjectDisplay->components)
		{
			component->ComponentEditor();
		}

		if (ImGui::Button("Add Component"))
			isCreatingANewComponent = true;

		if (ImGui::Button("Delete Component"))
			isDeletingAComponent = true;

		if(isCreatingANewComponent)
			AddNewComponent(currentSceneDisplay);

		if (isDeletingAComponent)
			DeleteComponent(currentSceneDisplay);

		
	}
	else if(currentSceneDisplay->aLightIsSelected)
	{
		ImGui::Checkbox("Is Enabled", &currentSceneDisplay->currentLight->lightEnabled);
		ImGui::DragFloat4("Position", currentSceneDisplay->currentLight->position.e);
		ImGui::DragFloat4("Ambient", currentSceneDisplay->currentLight->ambient.e);
		ImGui::DragFloat4("Diffuse", currentSceneDisplay->currentLight->diffuse.e);
		ImGui::DragFloat4("Specular", currentSceneDisplay->currentLight->specular.e);
		ImGui::DragFloat4("Attenuation", currentSceneDisplay->currentLight->attenuation.e);
	}
	ImGui::End();
}

void ObjectCompManager::RunMaterialSelector(const std::string& pathAssets, ResourcesManager* resourceManager, GameObject* currentObjectDisplay)
{
	ModelInstance* instance = static_cast<ModelInstance*>(currentObjectDisplay->GetComponent(ComponentType::MODELINSTANCE));
	ImGui::Begin("Materiel Selection Window");
	std::string pathModel = pathAssets + "Models";
	if (std::filesystem::exists(pathModel))
	{
		for (const auto& entry : std::filesystem::directory_iterator(pathModel))
		{
			if (entry.path().extension() == ".png" || entry.path().extension() == ".jpg" || entry.path().extension() == ".bmp" || entry.path().extension() == ".jpeg" || entry.path().extension() == ".PNG" || entry.path().extension() == ".JPG" || entry.path().extension() == ".BMP" || entry.path().extension() == ".JPEG")
			{
				if (ImGui::Button(entry.path().stem().string().c_str()))
				{
					int index = resourceManager->SearchResource(entry.path().stem().string(), ResourceType::TEXTURE);
					if (index < 0)
					{
						std::shared_ptr<Texture> tex = *static_cast<std::shared_ptr<Texture>*>(resourceManager->AddToLoad(std::filesystem::relative(entry.path()).string().c_str(), ResourceType::TEXTURE));
						instance->mat.SetDiffuseTexture(tex);
						instance->mat.m_name = tex->m_name;
					}
					else
					{
						instance->mat = *resourceManager->GetMaterials()[index];
					}
					isSelectingATexture = false;
				}
			}
		}
	}

	if (ImGui::Button("Back"))
		isSelectingATexture = false;
	ImGui::End();
}


void ObjectCompManager::AddNewComponent(Scene* currentSceneDisplay)
{
	if (ImGui::TreeNodeEx("New Component", ImGuiTreeNodeFlags_Framed))
	{
		ModelInstance* instance = static_cast<ModelInstance*>(currentSceneDisplay->currentObjectDisplay->GetComponent(ComponentType::MODELINSTANCE));
		if (instance != nullptr)
		{
			const aiScene* scene = instance->model->GetAssimpScene();
			Animator* anim = static_cast<Animator*>(currentSceneDisplay->currentObjectDisplay->GetComponent(ComponentType::ANIMATOR));
			if (scene->HasAnimations() && anim == nullptr)// instance->model->extension == ".FBX" || instance->model->extension == ".fbx")
			{
				if (ImGui::Button("Add Animator"))
				{
					Animator* animator = new Animator(currentSceneDisplay->currentObjectDisplay);
					currentSceneDisplay->currentObjectDisplay->components.push_back(animator);
					isCreatingANewComponent = false;
				}
			}
		}

		Rigidbody* tempRigidBody = static_cast<Rigidbody*>(currentSceneDisplay->currentObjectDisplay->GetComponent(ComponentType::RIGIDBODY));
		if (tempRigidBody == nullptr)
		{
			if (ImGui::Button("Add Rigidbody"))
			{
				Rigidbody* rg = new Rigidbody(currentSceneDisplay->currentObjectDisplay);
				currentSceneDisplay->currentObjectDisplay->components.push_back(rg);
				isCreatingANewComponent = false;
			}

		}
		else
		{
			int nbcube = 0;
			int nbsphere = 0;
			int nbcapsule = 0;
			for (Component* comp : currentSceneDisplay->currentObjectDisplay->components)
			{
				if (comp->type == ComponentType::CUBECOLLIDER)
					nbcube++;
				if (comp->type == ComponentType::SPHERECOLLIDER)
					nbsphere++;
				if (comp->type == ComponentType::CAPSULECOLLIDER)
					nbcapsule++;
			}


			if (ImGui::Button("Add CubeCollider"))
			{
				currentSceneDisplay->currentObjectDisplay->components.push_back(new CubeCollider(currentSceneDisplay->currentObjectDisplay, tempRigidBody, { 0.5f, 0.5f, 0.5f }));
				std::deque<Component*> NoCast = currentSceneDisplay->currentObjectDisplay->GetComponents(ComponentType::CUBECOLLIDER);
				CubeCollider* cubeC = static_cast<CubeCollider*>(NoCast.back());
				cubeC->name = "CubeCollider(" + std::to_string(nbcube) + ")";
				isCreatingANewComponent = false;
			}

			if (ImGui::Button("Add SphereCollider"))
			{
				currentSceneDisplay->currentObjectDisplay->components.push_back(new SphereCollider(currentSceneDisplay->currentObjectDisplay, tempRigidBody, 1.f));
				std::deque<Component*> NoCast = currentSceneDisplay->currentObjectDisplay->GetComponents(ComponentType::SPHERECOLLIDER);
				SphereCollider* sphereC = static_cast<SphereCollider*>(NoCast.back());
				sphereC->name = "SphereCollider(" + std::to_string(nbsphere) + ")";
				isCreatingANewComponent = false;
			}

			if (ImGui::Button("Add CapsuleCollider"))
			{
				currentSceneDisplay->currentObjectDisplay->components.push_back(new CapsuleCollider(currentSceneDisplay->currentObjectDisplay, tempRigidBody, 0.5f, 1.f));
				std::deque<Component*> NoCast = currentSceneDisplay->currentObjectDisplay->GetComponents(ComponentType::CAPSULECOLLIDER);
				CapsuleCollider* capsuleC = static_cast<CapsuleCollider*>(NoCast.back());
				capsuleC->nameComp = "CAPSULECOLLIDER";
				capsuleC->name = "CapsuleCollider(" + std::to_string(nbcapsule) + ")";
				isCreatingANewComponent = false;
			}

			PhysTestComp* tempPTC = static_cast<PhysTestComp*>(currentSceneDisplay->currentObjectDisplay->GetComponent(ComponentType::PHYS_TEST));
			if (tempPTC == nullptr)
			{
				if (ImGui::Button("Add PhysTestComp"))
				{
					PhysTestComp* ptc = new PhysTestComp(currentSceneDisplay->currentObjectDisplay);
					currentSceneDisplay->currentObjectDisplay->components.push_back(ptc);
					isCreatingANewComponent = false;
				}

			}
		}
		
		if (ImGui::Button("Back"))
			isCreatingANewComponent = false;
		ImGui::TreePop();
	}
}

void Deleting(Scene* currentSceneDisplay, int i)
{
	if (currentSceneDisplay->currentObjectDisplay->components[i]->type == ComponentType::RIGIDBODY)
	{
		currentSceneDisplay->currentObjectDisplay->DeleteComponent(ComponentType::CUBECOLLIDER);
		currentSceneDisplay->currentObjectDisplay->DeleteComponent(ComponentType::SPHERECOLLIDER);
		currentSceneDisplay->currentObjectDisplay->DeleteComponent(ComponentType::CAPSULECOLLIDER);
	}

	delete currentSceneDisplay->currentObjectDisplay->components[i];
	currentSceneDisplay->currentObjectDisplay->components.erase(currentSceneDisplay->currentObjectDisplay->components.begin() + i);
}

void ObjectCompManager::DeleteComponent(Scene* currentSceneDisplay)
{
	if (ImGui::TreeNodeEx("Delete Component", ImGuiTreeNodeFlags_Framed))
	{
		for (int i = 0; i < currentSceneDisplay->currentObjectDisplay->components.size(); i++)
		{
			if (currentSceneDisplay->currentObjectDisplay->components[i]->type == ComponentType::CUBECOLLIDER)
			{
				CubeCollider* cubeColl = static_cast<CubeCollider*>(currentSceneDisplay->currentObjectDisplay->components[i]);
				if (ImGui::Button(cubeColl->name.c_str()))
				{
					Deleting(currentSceneDisplay, i);
				}

			}
			else if (currentSceneDisplay->currentObjectDisplay->components[i]->type == ComponentType::SPHERECOLLIDER)
			{
				SphereCollider* sphereColl = static_cast<SphereCollider*>(currentSceneDisplay->currentObjectDisplay->components[i]);
				if (ImGui::Button(sphereColl->name.c_str()))
				{
					Deleting(currentSceneDisplay, i);
				}

			}
			else if (currentSceneDisplay->currentObjectDisplay->components[i]->type == ComponentType::CAPSULECOLLIDER)
			{
				CapsuleCollider* CapsuleColl = static_cast<CapsuleCollider*>(currentSceneDisplay->currentObjectDisplay->components[i]);
				if (ImGui::Button(CapsuleColl->name.c_str()))
				{
					Deleting(currentSceneDisplay, i);
				}

			}
			else if (currentSceneDisplay->currentObjectDisplay->components[i]->type != ComponentType::TRANSFORM)
			{
				if (ImGui::Button(currentSceneDisplay->currentObjectDisplay->components[i]->nameComp.c_str()))
				{
					Deleting(currentSceneDisplay, i);
					isDeletingAComponent = false;
				}
			}
		}


		if (ImGui::Button("Back"))
			isDeletingAComponent = false;
		ImGui::TreePop();
	}
}