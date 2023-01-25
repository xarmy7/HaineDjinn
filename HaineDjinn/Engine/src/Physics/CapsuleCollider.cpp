
#include "Physics/CapsuleCollider.h"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Physics/Rigidbody.h"
 
#include <reactphysics3d/reactphysics3d.h>
#include <IMGUI/imgui.h>

CapsuleCollider::CapsuleCollider(GameObject* inGameObject, Rigidbody* rb, const float inRadius, const float inHeight) : Collider(inGameObject, rb, ComponentType::CAPSULECOLLIDER)
{
	radius = inRadius;
	height = inHeight;
	nameComp = "CAPSULECOLLIDER";

	capsuleShape = Scene::GetPhysicsCommon()->createCapsuleShape(inRadius, inHeight);

	collider = rigidbody->AddCollider(capsuleShape, reactphysics3d::Transform::identity());
}

void CapsuleCollider::ComponentEditor()
{
	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
	{
		Collider::ComponentEditor();

		if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.001f))
		{
			if (radius <= 0.f)
				radius = 0.0001f;

			capsuleShape->setRadius(radius);
		}

		if (ImGui::DragFloat("Height", &height, 0.1f, 0.001f))
		{
			if (height <= 0.f)
				height = 0.0001f;

			capsuleShape->setHeight(height);
		}

		ImGui::TreePop();
	}
}
