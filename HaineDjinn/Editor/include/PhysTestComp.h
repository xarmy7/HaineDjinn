#pragma once

#include "Core/Component.h"
#include "Core/Transform.h"
#include "Core/Time.h"

class Rigidbody;

class PhysTestComp : public Component
{
public:
	PhysTestComp(GameObject* inGameObject);
	~PhysTestComp() {};

	void FixedUpdate() override;
	void Update() override;
	void ComponentEditor() override;
private:
	Rigidbody* rb;

	Timer forceCooldown;
	Timer torqueCooldown;

	Vector3 initialPos;
	Quaternion initialRot;
};
