
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Physics/Collider.h"
#include "Physics/Rigidbody.h"
#include <IMGUI/imgui.h>
#include <cassert>


Collider::Collider(GameObject* inGameObject, Rigidbody* rb, const ComponentType type) : Component(inGameObject, type, nullptr)
{
	assert(rb != nullptr, "Can't create a collider without a rigidbody.");
	rigidbody = rb;
}

Collider::~Collider()
{

}

void Collider::Awake()
{
	collider->setUserData(static_cast<void*>(this));
	gameObject->collisionSubscriber.Subscribe(&collisionEvent);
	gameObject->triggerSubscriber.Subscribe(&triggerEvent);
	Component::Awake();
}

//void Collider::SetLayer(const CollisionLayers inLayer) 
//{ 
//	collider->setCollisionCategoryBits((int)(inLayer)); 
//	layer = inLayer;
//}

void Collider::SetTrigger(const bool inTrigger) 
{
	collider->setIsTrigger(inTrigger);
	isTrigger = inTrigger;
}

void Collider::SetPosAndRot()
{
	reactphysics3d::Transform transform;

	transform.setPosition({ relativePos.x, relativePos.y, relativePos.z });
	transform.setOrientation(reactphysics3d::Quaternion::fromEulerAngles({ relativeRot.x, relativeRot.y, relativeRot.z }));

	collider->setLocalToBodyTransform(transform);
}

void Collider::ComponentEditor()
{
	if (ImGui::TreeNodeEx("Collider Base Class", ImGuiTreeNodeFlags_Framed))
	{
		ImGui::TextColored({ 1.0f, 0.0f, 0.9f, 1.f }, "TODO:\n - Layer\n");

		if (ImGui::Button("Is Trigger"))
		{
			SetTrigger(!isTrigger);
		}
		ImGui::SameLine();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Curr. %s", isTrigger ? "true" : "false");

		if (ImGui::DragFloat3("Relative position", relativePos.e, 0.025f, 0.001f) ||
			ImGui::DragFloat3("Relative rotation", relativeRot.e, 0.025f, 0.001f))
		{
			reactphysics3d::Transform transform;

			transform.setPosition({ relativePos.x, relativePos.y, relativePos.z });
			transform.setOrientation(reactphysics3d::Quaternion::fromEulerAngles({ relativeRot.x, relativeRot.y, relativeRot.z }));

			collider->setLocalToBodyTransform(transform);
		}

		//if (ImGui::BeginCombo("Layer", GetLayerAsString(layer).c_str()))
		//{
		//	/*for (int i = CollisionLayers::LAYER1; i < (int)(CollisionLayers::LAYER3); i++)
		//	{
		//		if (ImGui::Selectable(GetLayerAsString(CollisionLayers::LAYER1 + i).c_str()))
		//			SetLayer(CollisionLayers::LAYER1);
		//	}*/
		//	if (ImGui::Selectable(GetLayerAsString(CollisionLayers::LAYER1).c_str()))
		//		SetLayer(CollisionLayers::LAYER1);
		//	if (ImGui::Selectable(GetLayerAsString(CollisionLayers::LAYER2).c_str()))
		//		SetLayer(CollisionLayers::LAYER2);
		//	if (ImGui::Selectable(GetLayerAsString(CollisionLayers::LAYER3).c_str()))
		//		SetLayer(CollisionLayers::LAYER3);

		//	ImGui::EndCombo();
		//}

		ImGui::TreePop();
	}
}
