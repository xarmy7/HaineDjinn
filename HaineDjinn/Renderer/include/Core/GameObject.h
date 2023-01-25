#pragma once

#include <deque>
#include <string>
#include <memory>
#include <istream>

#include "../../../Engine/include/Core/Object.h"
#include "Core/EventSubscriber.h"

class Component;
enum class ComponentType;
class Transform;


class GameObject : public Object
{
private:

public:
	bool isStatic = false;
	bool componentsAwoken = false;

	std::string name = "GameObject";
	std::string entityType = "null";
	int id = 0;


	Transform* transform;
	std::deque<Component*> components;
	bool isSelected = false;

	EventSubscriber collisionSubscriber = {};
	EventSubscriber triggerSubscriber = {};

	GameObject();
	GameObject(const std::string& name);
	GameObject(const GameObject& copy) = delete;
	void operator=(GameObject& cpy) = delete;
	virtual ~GameObject();

	void AwakeComponents();
	void UpdateComponents();
	void FixedUpdateComponents();
	void LateUpdateComponents();
	void DeleteComponent(ComponentType type);
	Component* GetComponent(ComponentType type);
	template <typename T>
	T* GetComponent(ComponentType type) { return static_cast<T*>(GetComponent(type)); }

	std::deque<Component*> GetComponents(ComponentType type);


	virtual void OnCollisionEnter(void* parameters);
	virtual void OnTriggerEnter(void* parameters);

	//void ParseComponents(std::istringstream& parseComponent, std::string& parentName);

	//void Destroy() override;
	void OnDestroy() override;
};

