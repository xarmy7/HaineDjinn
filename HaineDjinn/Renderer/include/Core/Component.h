#pragma once

#include "../../../Engine/include/Core/Object.h"
#include "Core/GameObject.h"

enum class ComponentType
{
	MODELINSTANCE,
	TRANSFORM,
	RIGIDBODY,
	CUBECOLLIDER,
	SPHERECOLLIDER,
	CAPSULECOLLIDER,
	ANIMATION,
	ANIMATOR,
	SCRIPT,
	UNKNOWN,

	// USER COMPONENTS
	ENEMY,
	PHYS_TEST,
};

class Player;

class Component : public Object
{
private:

	bool hasGameObject = false;

protected:
	void OnDestroy() override;
	GameObject* gameObject;

	bool isAwake = false;

public:
	Component(GameObject* inGameObject, ComponentType inType, Component* childPtr);
	virtual ~Component() {};
	const ComponentType type = ComponentType::UNKNOWN;
	std::string nameComp = "UNKNOWN";

	bool hasStarted = false;

	void SetActive(bool value) override;

	void virtual draw() const { }

	void virtual Awake() { isAwake = true; }
	void virtual Start() { }
	void virtual Update() { }
	void virtual FixedUpdate() { }
	void virtual LateFixedUpdate() { }
	void virtual LateUpdate() { }
	void virtual OnEnable() { }
	void virtual OnDisable() { } 
	void virtual OnCollision(void* parameters) {}
	void virtual OnTrigger(void* parameters) {}

	bool hasFixedUpdate = true;
	bool hasLateFixedUpdate = true;
	bool hasUpdate = true;
	bool hasLateUpdate = true;

	bool IsActive() override;
	bool IsAwake() { return isAwake; }
	//void Destroy() override;

	GameObject* GetHost();

	virtual std::string ToString() const { return ""; }
	
	virtual void ComponentEditor();
};
