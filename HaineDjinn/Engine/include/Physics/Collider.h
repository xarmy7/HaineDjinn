#pragma once

#include "Core/Component.h"
#include "Core/Maths.h"
#include "Core/Event.h"
//#include "Physics/PhysicsLayers.h"

#pragma warning(disable : 26812)

class Rigidbody;

namespace reactphysics3d
{
	class RigidBody;
	class Collider;
	class CollisionShape;
	class SphereShape;
	class BoxShape;
	class CapsuleShape;
	class Transform;
}

class Collider : public Component
{
public:
	Collider(GameObject* inGameObject, Rigidbody* rb, const ComponentType type);
	Collider(const Collider& collider) = default;

	~Collider();

	//void SetLayer(const CollisionLayers inLayer);
	void SetTrigger(const bool inTrigger = true);

	virtual void Awake() override;

	virtual void ComponentEditor() override;
	void SetPosAndRot();

	Event collisionEvent = {};
	Event triggerEvent = {};

	Vector3 relativePos = {};
	Vector3 relativeRot = {};
	bool isTrigger = false; 
	std::string name;
protected:
	Rigidbody* rigidbody = nullptr;

	//CollisionLayers layer = CollisionLayers::LAYER1;

	reactphysics3d::Collider* collider;

private:
};

enum class CollisionType
{
	ENTER,
	STAY,
	EXIT
};

struct CollisionInfo
{
	Collider* collider1 = nullptr;
	Collider* collider2 = nullptr;
	CollisionType collType = CollisionType::EXIT;
	Vector3 contactPoint = {};
};

struct TriggerInfo
{
	Collider* collider1 = nullptr;
	Collider* collider2 = nullptr;
	CollisionType collType = CollisionType::EXIT;
};
