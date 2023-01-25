
#include "Physics/CubeCollider.h"
#include "Core/Debug.h"
#include "Core/Scene.h"
#include "Physics/Rigidbody.h"

#include <reactphysics3d/reactphysics3d.h>
#include <IMGUI/imgui.h>

CubeCollider::CubeCollider(GameObject* inGameObject, Rigidbody* rb, const Vector3 halfSizes) : Collider(inGameObject, rb, ComponentType::CUBECOLLIDER)
{
	halfSize = halfSizes;
	nameComp = "CUBECOLLIDER";
	boxShape = Scene::GetPhysicsCommon()->createBoxShape({ halfSizes.x, halfSizes.y, halfSizes.z });

	collider = rigidbody->AddCollider(boxShape, reactphysics3d::Transform::identity());
}

void CubeCollider::ComponentEditor()
{
	if (ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_Framed))
	{
		Collider::ComponentEditor();

		if (ImGui::DragFloat3("Half size", halfSize.e, 0.1f, 0.001f))
		{
			boxShape->setHalfExtents({ halfSize.x, halfSize.y, halfSize.z });

			reactphysics3d::Vector3 collHalfSize = boxShape->getHalfExtents();
		}

		ImGui::TreePop();
	}
}
