#pragma once

#include "Physics/Collider.h"

#pragma warning(disable : 26812)

class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(GameObject* inGameObject, Rigidbody* rb, const float inRadius = 1.f, const float inHeight = 1.f);

	void ComponentEditor() override;
	float GetRadius() { return radius; }
	float GetHeight() { return height; }
	std::string name;

private:
	float radius = 1.f;
	float height = 1.f;

	reactphysics3d::CapsuleShape* capsuleShape;
};
