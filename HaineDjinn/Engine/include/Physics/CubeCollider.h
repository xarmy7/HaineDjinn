#pragma once

#include "Physics/Collider.h"

#pragma warning(disable : 26812)

class CubeCollider : public Collider
{
public:
	CubeCollider(GameObject* inGameObject, Rigidbody* rb, const Vector3 halfSizes);
		
	void ComponentEditor() override;
	Vector3 GetHalfSize() { return halfSize; }

private:
	Vector3 halfSize = { 1.f, 1.f , 1.f };

	reactphysics3d::BoxShape* boxShape; 
};
