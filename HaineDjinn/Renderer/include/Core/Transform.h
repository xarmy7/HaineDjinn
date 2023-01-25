#pragma once

#include "Core/Maths.h"
#include "Core/Event.h"
#include "Core/Component.h"

#include <reactphysics3d/reactphysics3d.h>

#pragma once

class Event;

class Transform : public Component
{
private:
	std::shared_ptr<Transform> parent = nullptr;
	std::deque<Transform*> children;

	bool hasBeenUpdated = false;
	mat4x4 model = Maths::mat4::Identity();

	void OnDestroy() override;
	void DeleteChildFromTransform(Transform* transform);

public:
	Event onMoveEvent;

	Transform(GameObject* gameObject);
	Transform(const Transform& transform) = default;
	void  Awake() override 
	{ 
		isAwake = true; 
		rotation = QuaternionFromAxisAngle({ 0, 0, 1 }, euler.z) * QuaternionFromAxisAngle({ 1, 0, 0 }, euler.x) * QuaternionFromAxisAngle({ 0, 1, 0 }, euler.y) * QuaternionIdentity();
		rotation = QuaternionNormalize(rotation);
		onMoveEvent.Broadcast(nullptr);
	}

	mat4x4 GetTransformMatrix(Transform transfom);

	Vector3 position = Vector3{ 0.f, 0.f, 0.f };
	Quaternion rotation = {};
	Vector3 euler = Vector3{ 0.f, 0.f, 0.f };
	Vector3 scale = Vector3{1.f, 1.f, 1.f};

	GameObject* GetGOParent();
	std::shared_ptr<Transform> GetParent();
	Transform* GetChild(int childIndex);
	GameObject* GetGOChild(int childIndex);
	int GetChildrenCount();
	mat4x4 GetModel();
	mat4x4 GetGlobalModel();
	mat4x4 GetParentModel() const;


	Vector3 GetGlobalRotation() const;
	Vector3 GetGlobalPosition() const;
	Vector3 GetParentRotation() const;
	Vector3 GetParentPosition() const;

	Vector3 GetForward();

	bool HasParent();
	bool HasChild();
	void SetParent(std::shared_ptr<Transform> _parent);
	void SetParent(GameObject* gameObject);
	void SetChild(Transform* child);
	void SetChild(GameObject* gameObject);

	void Update() override;

	void ComponentEditor() override;

	static void ParseComponent(GameObject* gameObject, std::istringstream& iss, std::string& parentName);

	explicit operator reactphysics3d::Transform() const 
	{
		reactphysics3d::Transform transform;
		transform.setPosition({ position.x, position.y, position.z });
		transform.setOrientation({rotation.x, rotation.y, rotation.z, rotation.w});
		
		return transform;
	}
};
