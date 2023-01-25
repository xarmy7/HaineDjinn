
#include "Physics/SphereCollider.h"
#include "Physics/Rigidbody.h"
#include "Core/Debug.h"
#include "Core/Scene.h"

#include <IMGUI/imgui.h>
#include <reactphysics3d/reactphysics3d.h>

#include "Core/Time.h"

SphereCollider::SphereCollider(GameObject* inGameObject, Rigidbody* rb, const float inRadius) : Collider(inGameObject, rb, ComponentType::SPHERECOLLIDER)
{
	radius = inRadius;
	nameComp = "SPHERECOLLIDER";
	sphereShape = Scene::GetPhysicsCommon()->createSphereShape(inRadius);
	
	collider = rigidbody->AddCollider(sphereShape, reactphysics3d::Transform::identity());
}

void SphereCollider::ComponentEditor()
{
	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
	{
		Collider::ComponentEditor();

		if (ImGui::DragFloat("Radius", &radius, 0.1f, 0.001f))
		{
			if (radius <= 0.f)
				radius = 0.0001f;

			sphereShape->setRadius(radius);
		}

		ImGui::TreePop();
	}
}
