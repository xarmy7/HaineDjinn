#pragma once

#include "Physics/Collider.h"

#pragma warning(disable : 26812)

class SphereCollider : public Collider
{
public:
	SphereCollider(GameObject* inGameObject, Rigidbody* rb, const float inRadius = 1.f);

	void ComponentEditor() override;
	float GetRadius() { return radius; }

private:
	float radius = 1.f;

	reactphysics3d::SphereShape* sphereShape;
};
