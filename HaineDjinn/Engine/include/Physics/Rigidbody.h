#pragma once

#include "Core/Component.h"
#include "Core/Debug.h"
#include "Core/Maths.h"
#include "Core/EventSubscriber.h"

namespace reactphysics3d
{
	class RigidBody;
	class Collider;
	class CollisionShape;
	class Transform;
}

enum class BodyType { STATIC, KINEMATIC, DYNAMIC };



class Rigidbody : public Component
{
public:
	Rigidbody(GameObject* inGameObject);
	~Rigidbody();

	void SetBodyType(const BodyType inType = BodyType::DYNAMIC);
	void SetGravity(const bool inGravity = true);
	void SetStatic(const bool inStatic = true);

	void AddForce(const Vector3 force);
	void AddTorque(const Vector3 torque);
	reactphysics3d::Collider* AddCollider(reactphysics3d::CollisionShape* shape, const reactphysics3d::Transform& transform);
	void SetMass(const float inMass);

	void OnCollision(void* parameters) override;
	void OnTrigger(void* parameters) override;

	void FixedUpdate() override;
	void ComponentEditor() override;

	float mass = 1.f;
	bool useGravity = false;
	bool isStatic = false;
	const BodyType type = BodyType::DYNAMIC;
	int lockMovement[3] = { 1, 1, 1 };
	int lockRotation[3] = { 1, 1, 1 };
private:
	void OnTransformMove();

	EventSubscriber eventTransformMove = {};
	reactphysics3d::RigidBody* rigidbody;
};