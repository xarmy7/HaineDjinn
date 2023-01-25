
#include "Core/GameDefinitions.h"
#include "Core/Debug.h"
#include "Renderer/ModelLoader.h"
#include "Core/Component.h"
#include "../../../Renderer/include/Renderer/ResourcesManager.h"
#include "Core/Transform.h"
#include "Core/Scene.h"
#include "Physics/CubeCollider.h"
#include "Physics/SphereCollider.h"
#include "Physics/CapsuleCollider.h"
#include "Physics/Rigidbody.h"
#include "../../../Editor/include/PhysTestComp.h"


#include <cassert>
#include <sstream>
#include <vector>

#ifndef XMLCheckResult
#define XMLCheckResult(a_eResult) if (a_eResult != XML_SUCCESS) { printf("Error: %i\n", a_eResult); return a_eResult; }
#endif

using namespace tinyxml2;

float ToFloat(const char* value)
{
	return std::stof((std::string)value);
}

XMLError GameDefinitions::CreateMap(MapFile& map)
{
	//Creation of the current Map part in the XML file
	std::string nodeName = "PreFab";
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile("SavedData.xml");
	XMLNode* pGameDef = xmlDoc.FirstChild();
	if (pGameDef == nullptr)
	{
		pGameDef = xmlDoc.NewElement("GameDefinitions");
		xmlDoc.InsertFirstChild(pGameDef);
	}

	//Creation of the ObjectsNode in the XML
	XMLElement* mapNode = xmlDoc.NewElement(map.name.c_str());
	pGameDef->InsertEndChild(mapNode);

	//saving XMLDoc to XML file
	eResult = xmlDoc.SaveFile("SavedData.xml");
	XMLCheckResult(eResult);

	return XML_SUCCESS;
}


void GameDefinitions::FillDataObject(XMLElement* pListElement, XMLElement* pListObjects,const std::deque<GameObject*>& objects, XMLDocument* xmlDoc)
{
	for(int i = 0; i < objects.size(); i++)
	{
		GameObject& object = *objects[i];
		XMLElement* pListElement = xmlDoc->NewElement("GameObject");
		pListElement->SetAttribute("name", object.name.c_str());

		//CREATION DES NODES COMPONENTS
		XMLElement* pListComp;
		if (pListElement->FirstChildElement("ComponentList") == nullptr)
			pListComp = xmlDoc->NewElement("ComponentList");
		else
		{
			pListComp = pListElement->FirstChildElement("ComponentList");
			pListComp->DeleteChildren();
		}

		std::string cubeColliderSizeSTR;
		std::string capsuleColliderSizeSTR;
		std::string lockMovement; 
		std::string lockRotation;
		ModelInstance* instance;
		Animator* animator;
		Rigidbody* rigidbody;
		Collider* cubeColl;
		CubeCollider* cubeC;
		Collider* sphereColl;
		SphereCollider* sphereC;
		Collider* capsuleColl;
		CapsuleCollider* capsuleC;
		for (Component* comp : object.components)
		{
			XMLElement* pListElementComp = xmlDoc->NewElement("Component");

			switch (comp->type)
			{
			case ComponentType::TRANSFORM: //FILL OF TRANSFORM'S DATA
				pListElementComp->SetAttribute("ComponentType", "Transform");
				pListElementComp->SetAttribute("scaleX", object.transform->scale.x);
				pListElementComp->SetAttribute("scaleY", object.transform->scale.y);
				pListElementComp->SetAttribute("scaleZ", object.transform->scale.z);
				pListElementComp->SetAttribute("posX", object.transform->position.x);
				pListElementComp->SetAttribute("posY", object.transform->position.y);
				pListElementComp->SetAttribute("posZ", object.transform->position.z);
				pListElementComp->SetAttribute("angleX", object.transform->euler.x);
				pListElementComp->SetAttribute("angleY", object.transform->euler.y);
				pListElementComp->SetAttribute("angleZ", object.transform->euler.z);
				break;
			case ComponentType::MODELINSTANCE: //FILL OF MODELINSTANCE'S DATA
				pListElementComp->SetAttribute("ComponentType", "ModelInstance");
				instance = static_cast<ModelInstance*>(object.GetComponent(ComponentType::MODELINSTANCE));
				pListElementComp->SetAttribute("isStatic", instance->isStatic);
				pListElementComp->SetAttribute("filePath", std::filesystem::relative(instance->model->filePath).string().c_str());
				pListElementComp->SetAttribute("material", instance->mat.m_name.c_str());
				break;
			case ComponentType::ANIMATOR: //FILL OF ANIMATOR'S DATA
				animator = static_cast<Animator*>(object.GetComponent(ComponentType::ANIMATOR));
				pListElementComp->SetAttribute("ComponentType", "Animator");
				if(animator->currentAnimation != nullptr)
					pListElementComp->SetAttribute("animation", animator->GetAnimation()->animation->mName.C_Str());
				break;
			case ComponentType::RIGIDBODY: //FILL OF RIGIDBODY'S DATA
				rigidbody = static_cast<Rigidbody*>(object.GetComponent(ComponentType::RIGIDBODY));
				pListElementComp->SetAttribute("ComponentType", "Rigidbody");
				pListElementComp->SetAttribute("Mass", std::to_string(rigidbody->mass).c_str());
				pListElementComp->SetAttribute("IsGravitating", std::to_string(rigidbody->useGravity).c_str());
				pListElementComp->SetAttribute("IsStatic",  std::to_string(rigidbody->isStatic).c_str());
				
				lockMovement = " " + std::to_string(rigidbody->lockMovement[0]) + " " + std::to_string(rigidbody->lockMovement[1]) + " " + std::to_string(rigidbody->lockMovement[2]) + " ";
				pListElementComp->SetAttribute("lockMovement", lockMovement.c_str());

				lockRotation = " " + std::to_string(rigidbody->lockRotation[0]) + " " + std::to_string(rigidbody->lockRotation[1]) + " " + std::to_string(rigidbody->lockRotation[2]) + " ";
				pListElementComp->SetAttribute("lockRotation", lockRotation.c_str());


				switch (rigidbody->type)
				{
					case BodyType::DYNAMIC:
						pListElementComp->SetAttribute("Type", "DYNAMIC");
						break;
					case BodyType::KINEMATIC:
						pListElementComp->SetAttribute("Type", "KINEMATIC");
						break;
					case BodyType::STATIC:
						pListElementComp->SetAttribute("Type", "STATIC");
						break;
					default:
						break;
				}
				break;
			case ComponentType::CUBECOLLIDER: //FILL OF CUBECOLLIDER'S DATA
				pListElementComp->SetAttribute("ComponentType", "CubeCollider");
				cubeColl = static_cast<Collider*>(object.GetComponents(ComponentType::CUBECOLLIDER)[nbCubeCollider]);
				cubeC = static_cast<CubeCollider*>(cubeColl);
				cubeColliderSizeSTR = "x " + std::to_string(cubeC->GetHalfSize().x) + " y " + std::to_string(cubeC->GetHalfSize().y) + " z " + std::to_string(cubeC->GetHalfSize().z);
				pListElementComp->SetAttribute("cubeColliderSize", cubeColliderSizeSTR.c_str());
				pListElementComp->SetAttribute("cubeColliderName", cubeC->name.c_str());
				pListElementComp->SetAttribute("colliderIsTrigger", std::to_string(cubeC->isTrigger).c_str());
				pListElementComp->SetAttribute("cubeColliderPosX", std::to_string(cubeC->relativePos.x).c_str());
				pListElementComp->SetAttribute("cubeColliderPosY", std::to_string(cubeC->relativePos.y).c_str());
				pListElementComp->SetAttribute("cubeColliderPosZ", std::to_string(cubeC->relativePos.z).c_str());
				pListElementComp->SetAttribute("cubeColliderRotX", std::to_string(cubeC->relativeRot.x).c_str());
				pListElementComp->SetAttribute("cubeColliderRotY", std::to_string(cubeC->relativeRot.y).c_str());
				pListElementComp->SetAttribute("cubeColliderRotZ", std::to_string(cubeC->relativeRot.z).c_str());
				nbCubeCollider++;
				break;
			case ComponentType::SPHERECOLLIDER: //FILL OF SPHERECOLLIDER'S DATA
				pListElementComp->SetAttribute("ComponentType", "SphereCollider");
				sphereColl = static_cast<Collider*>(object.GetComponents(ComponentType::SPHERECOLLIDER)[nbSphereCollider]);
				sphereC = static_cast<SphereCollider*>(sphereColl);
				pListElementComp->SetAttribute("sphereColliderRadius", sphereC->GetRadius());
				pListElementComp->SetAttribute("sphereColliderName", sphereC->name.c_str());
				pListElementComp->SetAttribute("colliderIsTrigger", std::to_string(sphereC->isTrigger).c_str());

				pListElementComp->SetAttribute("sphereColliderPosX", std::to_string(sphereC->relativePos.x).c_str());
				pListElementComp->SetAttribute("sphereColliderPosY", std::to_string(sphereC->relativePos.y).c_str());
				pListElementComp->SetAttribute("sphereColliderPosZ", std::to_string(sphereC->relativePos.z).c_str());
				nbSphereCollider++;
				break;
			case ComponentType::CAPSULECOLLIDER:
				pListElementComp->SetAttribute("ComponentType", "CapsuleCollider");
				capsuleColl = static_cast<Collider*>(object.GetComponents(ComponentType::CAPSULECOLLIDER)[nbCapsuleCollider]);
				capsuleC = static_cast<CapsuleCollider*>(capsuleColl);
				capsuleColliderSizeSTR = "r " + std::to_string(capsuleC->GetRadius()) + " h " + std::to_string(capsuleC->GetHeight());
				pListElementComp->SetAttribute("capsuleColliderSize", capsuleColliderSizeSTR.c_str());
				pListElementComp->SetAttribute("capsuleColliderName", capsuleC->name.c_str());
				pListElementComp->SetAttribute("colliderIsTrigger", std::to_string(capsuleC->isTrigger).c_str());
				pListElementComp->SetAttribute("capsuleColliderPosX", std::to_string(capsuleC->relativePos.x).c_str());
				pListElementComp->SetAttribute("capsuleColliderPosY", std::to_string(capsuleC->relativePos.y).c_str());
				pListElementComp->SetAttribute("capsuleColliderPosZ", std::to_string(capsuleC->relativePos.z).c_str());
				pListElementComp->SetAttribute("capsuleColliderrotX", std::to_string(capsuleC->relativeRot.x).c_str());
				pListElementComp->SetAttribute("capsuleColliderRotY", std::to_string(capsuleC->relativeRot.y).c_str());
				pListElementComp->SetAttribute("capsuleColliderRotZ", std::to_string(capsuleC->relativeRot.z).c_str());
				nbCapsuleCollider++;
				break;
			case ComponentType::PHYS_TEST:
				pListElementComp->SetAttribute("ComponentType", "PhysTestComp");
				break;
			default:
				break;
			}

			pListComp->InsertEndChild(pListElementComp);
		}

		nbCubeCollider = 0;
		nbSphereCollider = 0;
		nbCapsuleCollider = 0;

		pListComp->SetAttribute("ComponentCount", object.components.size());
		pListElement->InsertEndChild(pListComp);

		pListObjects->InsertEndChild(pListElement);
	}
}

void GameDefinitions::FillDataCamera(XMLElement* pListElement, XMLElement* pListCameras, const std::deque<Camera*>& cameras, XMLDocument* xmlDoc)
{
	for (Camera* cam : cameras)
	{
		XMLElement* pListElement = xmlDoc->NewElement("Camera");
		pListElement->SetAttribute("name", cam->name.c_str());
		pListElement->SetAttribute("is_blocked", cam->blocked);
		pListElement->SetAttribute("is_mainCamera", cam->isMainCamera);
		pListElement->SetAttribute("isPerpective", cam->isPerspective);
		pListElement->SetAttribute("yaw", cam->yaw);
		pListElement->SetAttribute("pitch", cam->pitch);
		pListElement->SetAttribute("aspect", cam->aspect);
		pListElement->SetAttribute("fovY", cam->fovY);
		pListElement->SetAttribute("nearVal", cam->nearVal);
		pListElement->SetAttribute("farVal", cam->farVal);
		pListElement->SetAttribute("posX", cam->position.x);
		pListElement->SetAttribute("posY", cam->position.y);
		pListElement->SetAttribute("posZ", cam->position.z);
		pListElement->SetAttribute("posW", cam->position.w);
		pListElement->SetAttribute("offsetFromTargetX", cam->offsetFromTarget.x);
		pListElement->SetAttribute("offsetFromTargetY", cam->offsetFromTarget.y);
		pListElement->SetAttribute("offsetFromTargetZ", cam->offsetFromTarget.z);


		pListCameras->InsertEndChild(pListElement);
	}
}


XMLError GameDefinitions::Save(std::deque<GameObject*> preFabs, Scene* currentSceneDisplay)
{
	XMLDocument xmlDoc;

	//OPENING OF THE XML FILE AND THE CURRENT MAP SECTION
	XMLError eResult = xmlDoc.LoadFile("SavedData.xml");
	XMLNode* pGameDefinitions = xmlDoc.FirstChild();
	if (pGameDefinitions == nullptr)
		return XML_ERROR_FILE_READ_ERROR;
	XMLElement* pElementMap = pGameDefinitions->FirstChildElement(currentSceneDisplay->sceneName.c_str());
	if (pElementMap == nullptr)
		return XML_ERROR_PARSING_ELEMENT;

	//OPENING OF THE OBJECT SECTION
	XMLElement* pListObject;
	if (pElementMap->FirstChildElement("ObjectsList") == nullptr)
		pListObject = xmlDoc.NewElement("ObjectsList");
	else
	{
		pListObject = pElementMap->FirstChildElement("ObjectsList");
		pListObject->DeleteChildren();
	}
	XMLElement* pListElementObjects = xmlDoc.NewElement("GameObject");

	//FILL OF THE OBJECT SECTION
	FillDataObject(pListElementObjects, pListObject, currentSceneDisplay->gameObjects, &xmlDoc);
	pListObject->SetAttribute("itemCount", currentSceneDisplay->gameObjects.size());
	pElementMap->InsertEndChild(pListObject);

	//OPENING OF THE CAMERA SECTION
	XMLElement* pListCameras;
	if (pElementMap->FirstChildElement("CamerasList") == nullptr)
		pListCameras = xmlDoc.NewElement("CamerasList");
	else
	{
		pListCameras = pElementMap->FirstChildElement("CamerasList");
		pListCameras->DeleteChildren();
	}
	XMLElement* pListElementCamera = xmlDoc.NewElement("Camera");

	//FILL OF THE CAMERA SECTION
	FillDataCamera(pListElementCamera, pListCameras, currentSceneDisplay->cameras, &xmlDoc);
	pListCameras->SetAttribute("itemCount", currentSceneDisplay->cameras.size());
	pElementMap->InsertEndChild(pListCameras);


	//saving XMLDoc to XML file
	eResult = xmlDoc.SaveFile("SavedData.xml");
	XMLCheckResult(eResult);

	return XML_SUCCESS;
}

void GameDefinitions::ExtracInfoObjects(ResourcesManager* resourceManager, XMLElement* pListElement, std::deque<GameObject*>& objects, ModelLoader* modelLoader)
{
	std::string tempNameObject;
	std::string tempNameComponent;
	for (; pListElement != NULL; pListElement = pListElement->NextSiblingElement())
	{
		const XMLAttribute* attributeOfGameobject = pListElement->FirstAttribute();
		objects.emplace_back(new GameObject);
		GameObject* object = objects.back();

		for (; attributeOfGameobject != NULL; attributeOfGameobject = attributeOfGameobject->Next())
		{
			tempNameObject = attributeOfGameobject->Name();

			if (tempNameObject == "name")
				object->name = attributeOfGameobject->Value();

			XMLElement* pListComponents = pListElement->FirstChildElement("ComponentList");
			XMLElement* pListElementComponent = pListComponents->FirstChildElement("Component");

		
			for (; pListElementComponent != NULL; pListElementComponent = pListElementComponent->NextSiblingElement())
			{
				const XMLAttribute* attributeOfComponent = pListElementComponent->FirstAttribute();
				for (; attributeOfComponent != NULL; attributeOfComponent = attributeOfComponent->Next())
				{
					tempNameComponent = attributeOfComponent->Name();

					if (tempNameComponent == "ComponentType")
					{
						typeComponent = attributeOfComponent->Value();
						if (typeComponent == "Transform")
							object->transform->nameComp = "TRANSFORM";
						if (typeComponent == "ModelInstance")
						{
							ModelInstance* instance = new ModelInstance(object);
							instance->nameComp = "MODELINSTANCE";
							object->components.push_back(instance);
						}
						if (typeComponent == "Animator")
						{
							Animator* animator = new Animator(object);
							animator->nameComp = "ANIMATOR";
							object->components.push_back(animator);
						}
						if (typeComponent == "Rigidbody")
						{
							Rigidbody* rg = new Rigidbody(object);
							rg->nameComp = "RIGIDBODY";
							object->components.push_back(rg);
						}
						if (typeComponent == "PhysTestComp")
						{
							PhysTestComp* ptc = new PhysTestComp(object);
							ptc->nameComp = "PHYS_TEST";
							object->components.push_back(ptc);
						}
					}

					if (typeComponent == "Transform")
					{
						if (tempNameComponent == "scaleX")
							object->transform->scale.x = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "scaleY")
							object->transform->scale.y = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "scaleZ")
							object->transform->scale.z = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "posX")
							object->transform->position.x = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "posY")
							object->transform->position.y = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "posZ")
							object->transform->position.z = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "angleX")
							object->transform->euler.x = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "angleY")
							object->transform->euler.y = ToFloat(attributeOfComponent->Value());
						else if (tempNameComponent == "angleZ")
							object->transform->euler.z = ToFloat(attributeOfComponent->Value());
					}
					else if (typeComponent == "ModelInstance")
					{
						if (tempNameComponent == "isStatic")
						{
							ModelInstance* instance = static_cast<ModelInstance*>(object->GetComponent(ComponentType::MODELINSTANCE));
							instance->isStatic = attributeOfComponent->Value();
						}
						else if (tempNameComponent == "filePath")
						{
							ModelInstance* instance = static_cast<ModelInstance*>(object->GetComponent(ComponentType::MODELINSTANCE));
							instance->model = &modelLoader->FindModel(attributeOfComponent->Value(), instance);
						}
						else if (tempNameComponent == "material")
						{
							std::shared_ptr<Texture> tex = *static_cast<std::shared_ptr<Texture>*>(resourceManager->AddToLoad(attributeOfComponent->Value(), ResourceType::TEXTURE));
							ModelInstance* instance = static_cast<ModelInstance*>(object->GetComponent(ComponentType::MODELINSTANCE));
							instance->mat.SetDiffuseTexture(tex);
							instance->mat.m_name = tex->m_name;
						}
					}
					else if (typeComponent == "Animator")
					{
						if (tempNameComponent == "animation")
						{
							ModelInstance* instance = static_cast<ModelInstance*>(object->GetComponent(ComponentType::MODELINSTANCE));
							Animator* animator = static_cast<Animator*>(object->GetComponent(ComponentType::ANIMATOR));
							if (instance != nullptr && instance->model != nullptr)
							{
								const aiScene* scene = instance->model->GetAssimpScene();
								for (int i = 0; i < scene->mNumAnimations; ++i)
								{
									const aiAnimation* anim = scene->mAnimations[i];
									if (strcmp(anim->mName.C_Str(), attributeOfComponent->Value()) == 0)
									{
										animator->SetAnimation(i);
										break;
									}
								}
							}
							else
							{
								if (instance == nullptr)
									HD_LOG("MODELINSTANCE COMPONENT IS NULL", LogSeverity::LOG_ERROR);
								else if(instance->model == nullptr)
									HD_LOG("MODELINSTANCE'S MODEL IS NULL", LogSeverity::LOG_ERROR);

								break;
							}
						}
					}
					else if (typeComponent == "Rigidbody")
					{
						if (tempNameComponent == "Mass")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							rb->SetMass(ToFloat(attributeOfComponent->Value()));
						}
						if (tempNameComponent == "IsGravitating")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							if (ToFloat(attributeOfComponent->Value()) == 0)
								rb->SetGravity(false);
							else
								rb->SetGravity(true);
						}
						if (tempNameComponent == "IsStatic")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							if (ToFloat(attributeOfComponent->Value()) == 0)
								rb->SetStatic(false);
							else
								rb->SetStatic(true);
						}
						if (tempNameComponent == "Type")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							if (attributeOfComponent->Value() == "DYNAMIC")
								rb->SetBodyType(BodyType::DYNAMIC);
							if (attributeOfComponent->Value() == "KINEMATIC")
								rb->SetBodyType(BodyType::KINEMATIC);
							if (attributeOfComponent->Value() == "STATIC")
								rb->SetBodyType(BodyType::STATIC);
						}

						if (tempNameComponent == "lockMovement")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							
							int temp[3];
							std::istringstream line(attributeOfComponent->Value());

							line >> temp[0];
							rb->lockMovement[0] = temp[0];
							

							line >> temp[1];
							rb->lockMovement[1] = temp[1];
							

							line >> temp[2];
							rb->lockMovement[2] = temp[2];
							

						}

						if (tempNameComponent == "lockRotation")
						{
							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));

							int temp[3];
							std::istringstream line(attributeOfComponent->Value());

							line >> temp[0];
							rb->lockRotation[0] = temp[0];


							line >> temp[1];
							rb->lockRotation[1] = temp[1];


							line >> temp[2];
							rb->lockRotation[2] = temp[2];

						}

					}
					else if (typeComponent == "CubeCollider")
					{
						if (tempNameComponent == "cubeColliderSize")
						{
							Vector3 halfSize;
							std::string firstWord;
							std::string skip;
							std::istringstream line(attributeOfComponent->Value());

							line >> firstWord;

							line >> halfSize.x;
							line >> skip;

							line >> halfSize.y;
							line >> skip;

							line >> halfSize.z;
							line >> skip;

							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							CubeCollider* cc = new CubeCollider(object, rb, halfSize);
							cc->nameComp = "CUBECOLLIDER";
							object->components.push_back(cc);
						}
						else if (tempNameComponent == "colliderIsTrigger")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							if (ToFloat(attributeOfComponent->Value()) == 0)
								cubeC->SetTrigger(false);
							else
								cubeC->SetTrigger(true);
						}
						else if (tempNameComponent == "cubeColliderName")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->name = attributeOfComponent->Value();
						}
						else if (tempNameComponent == "cubeColliderPosX")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativePos.x = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "cubeColliderPosY")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativePos.y = ToFloat(attributeOfComponent->Value());

						}
						else if (tempNameComponent == "cubeColliderPosZ")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativePos.z = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "cubeColliderRotX")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativeRot.x = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "cubeColliderRotY")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativeRot.y = ToFloat(attributeOfComponent->Value());

						}
						else if (tempNameComponent == "cubeColliderRotZ")
						{
							CubeCollider* cubeC = static_cast<CubeCollider*>(object->components.back());
							cubeC->relativeRot.z = ToFloat(attributeOfComponent->Value());
							cubeC->SetPosAndRot();
						}
					}
					else if (typeComponent == "SphereCollider")
					{
						if (tempNameComponent == "sphereColliderRadius")
						{
							float radius = ToFloat(attributeOfComponent->Value());

							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							SphereCollider* sc = new SphereCollider(object, rb, radius);
							sc->nameComp = "SPHERECOLLIDER";
							object->components.push_back(sc);
						}
						else if (tempNameComponent == "sphereColliderName")
						{
							SphereCollider* sphereC = static_cast<SphereCollider*>(object->components.back());
							sphereC->name = attributeOfComponent->Value();
						}
						else if (tempNameComponent == "colliderIsTrigger")
						{
							SphereCollider* sphereC = static_cast<SphereCollider*>(object->components.back());
							if (ToFloat(attributeOfComponent->Value()) == 0)
								sphereC->SetTrigger(false);
							else
								sphereC->SetTrigger(true);
						}
						else if (tempNameComponent == "sphereColliderPosX")
						{
							SphereCollider* sphereC = static_cast<SphereCollider*>(object->components.back());
							sphereC->relativePos.x = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "sphereColliderPosY")
						{
							SphereCollider* sphereC = static_cast<SphereCollider*>(object->components.back());
							sphereC->relativePos.y = ToFloat(attributeOfComponent->Value());

						}
						else if (tempNameComponent == "sphereColliderPosZ")
						{
							SphereCollider* sphereC = static_cast<SphereCollider*>(object->components.back());
							sphereC->relativePos.z = ToFloat(attributeOfComponent->Value());
							sphereC->SetPosAndRot();

						}
					}
					else if (typeComponent == "CapsuleCollider")
					{
						if (tempNameComponent == "capsuleColliderSize")
						{
							float radius;
							float height;
							std::string firstWord;
							std::string skip;
							std::istringstream line(attributeOfComponent->Value());

							line >> firstWord;

							line >> radius;
							line >> skip;

							line >> height;
							line >> skip;

							Rigidbody* rb = static_cast<Rigidbody*>(object->GetComponent(ComponentType::RIGIDBODY));
							CapsuleCollider* cc = new CapsuleCollider(object, rb, radius, height);
							cc->nameComp = "CAPSULECOLLIDER";
							object->components.push_back(cc);
						}
						else if (tempNameComponent == "colliderIsTrigger")
						{
							CapsuleCollider* capsuleC = static_cast<CapsuleCollider*>(object->components.back());
							if(ToFloat(attributeOfComponent->Value()) == 0)
								capsuleC->SetTrigger(false);
							else
								capsuleC->SetTrigger(true);

						}
						else if (tempNameComponent == "capsuleColliderName")
						{
							CapsuleCollider* capsuleC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleC->name = attributeOfComponent->Value();
						}
						else if (tempNameComponent == "capsuleColliderPosX")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativePos.x = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "capsuleColliderPosY")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativePos.y = ToFloat(attributeOfComponent->Value());

						}
						else if (tempNameComponent == "capsuleColliderPosZ")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativePos.z = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "capsuleColliderRotX")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativeRot.x = ToFloat(attributeOfComponent->Value());
						}
						else if (tempNameComponent == "capsuleColliderRotY")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativeRot.y = ToFloat(attributeOfComponent->Value());

						}
						else if (tempNameComponent == "capsuleColliderRotZ")
						{
							CapsuleCollider* capsuleCC = static_cast<CapsuleCollider*>(object->components.back());
							capsuleCC->relativeRot.z = ToFloat(attributeOfComponent->Value());

							capsuleCC->SetPosAndRot();
						}
					}
					else if (typeComponent == "PhysTestComp")
					{

					}
				}
				attributeOfComponent = nullptr;
			}

			pListComponents =nullptr;
			pListElementComponent = nullptr;
		}
		attributeOfGameobject = nullptr;
		// Refresh rotation for rigidbody
		object->transform->onMoveEvent.Broadcast(nullptr);
	}
}
void GameDefinitions::ExtracInfoCamera(ResourcesManager* resourceManager, XMLElement* pListElement, std::deque<Camera*>& cameras)
{
	for (; pListElement != NULL; pListElement = pListElement->NextSiblingElement())
	{
		const XMLAttribute* attributeOfGameobject = pListElement->FirstAttribute();
		Camera* camera;

		if (cameras.size() == 0)
		{
			cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
			camera = cameras.back();
			camera->isMainCamera = true;
			camera->name = "Camera(1)";
		}
		else
		{
			if (cameras.size() == 1 && cameras[0]->name == "TempName")
			{
				camera = cameras[0];
				camera->name = "Camera(1)";
			}
			else
			{
				cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
				camera = cameras.back();
			}

			for (; attributeOfGameobject != NULL; attributeOfGameobject = attributeOfGameobject->Next())
			{
				std::string tempName = attributeOfGameobject->Name();

				if (tempName == "name" && cameras.size() != 1 && cameras[0]->name != "TempName")
					camera->name = attributeOfGameobject->Value();
				else if (tempName == "is_blocked")
					camera->blocked = attributeOfGameobject->Value();
				else if (tempName == "is_mainCamera")
					camera->isMainCamera = attributeOfGameobject->Value();
				else if (tempName == "isPerpective")
					camera->isPerspective = attributeOfGameobject->Value();
				else if (tempName == "yaw")
					camera->yaw = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "pitch")
					camera->pitch = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "aspect")
					camera->aspect = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "fovY")
					camera->fovY = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "nearVal")
					camera->nearVal = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "farVal")
					camera->farVal = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "posX")
					camera->position.x = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "posY")
					camera->position.y = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "posZ")
					camera->position.z = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "posW")
					camera->position.w = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "offsetFromTargetX")
					camera->offsetFromTarget.x = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "offsetFromTargetY")
					camera->offsetFromTarget.y = ToFloat(attributeOfGameobject->Value());
				else if (tempName == "offsetFromTargetZ")
					camera->offsetFromTarget.z = ToFloat(attributeOfGameobject->Value());

			}
		}
	}
}

XMLError GameDefinitions::Load(ResourcesManager* resourceManager, std::deque<GameObject*>& preFabs, Scene* currentSceneDisplay, ModelLoader* modelLoader)
{
	//OPENING OF THE XML FILE
	XMLDocument xmlDoc;
	XMLError eResult = xmlDoc.LoadFile("SavedData.xml");
	XMLCheckResult(eResult);
	XMLNode* pGameDefinitions = xmlDoc.FirstChild();
	if (pGameDefinitions == nullptr)
		return XML_ERROR_FILE_READ_ERROR;

	//OPENING OF THE CURRENT MAP SECTION
	XMLElement* mapElement = pGameDefinitions->FirstChildElement(currentSceneDisplay->sceneName.c_str());
	if (mapElement == nullptr)
		return XML_ERROR_PARSING_ELEMENT;

	//OPENING AND READING OF THE OBJECTS INFO -> FILL INTO THE SCENE OBJECTS'S DEQUE
	XMLElement* pListObjects = mapElement->FirstChildElement("ObjectsList");
	if (pListObjects == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	XMLElement* pListElementObject = pListObjects->FirstChildElement("GameObject");
	ExtracInfoObjects(resourceManager, pListElementObject, currentSceneDisplay->gameObjects, modelLoader);

	//OPENING AND READING OF THE CAMERAS INFO -> FILL INTO THE SCENE CAMERAS'S DEQUE
	XMLElement* pListCameras = mapElement->FirstChildElement("CamerasList");
	if (pListCameras == nullptr)
		return XML_ERROR_PARSING_ELEMENT;
	XMLElement* pListElementCamera = pListCameras->FirstChildElement("Camera");
	ExtracInfoCamera(resourceManager, pListElementCamera, currentSceneDisplay->cameras);

	return XML_SUCCESS;
}
