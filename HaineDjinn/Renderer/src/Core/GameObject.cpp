
#include "Core/GameObject.h"
#include "Core/Component.h"
#include "Core/Transform.h"
#include "Core/Debug.h"
#include "../../Engine/include/Physics/Collider.h"

#include <sstream>
#include <IMGUI/imgui.h> 

GameObject::GameObject()
{
	transform = new Transform(this);
	collisionSubscriber.SetFunction([this](void* parameters) {OnCollisionEnter(parameters); });
	triggerSubscriber.SetFunction([this](void* parameters) {OnTriggerEnter(parameters); });
	float* q = &transform->rotation.y;
	//collisionSubscriber.SetFunction(std::bind<void(void*)>(&GameObject::OnCollisionEnter, this, nullptr));
}

GameObject::GameObject(const std::string& name)
	: name(name)
{
	transform = new Transform(this);
	collisionSubscriber.SetFunction([this](void* parameters) {OnCollisionEnter(parameters); });
	triggerSubscriber.SetFunction([this](void* parameters) {OnTriggerEnter(parameters); });
	//Log("Creating a GameObject named " + name);
}

GameObject::~GameObject()
{
	// TODO: Actually delete components
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] != nullptr)
			delete components[i];
	}
	components.clear();
}

void GameObject::AwakeComponents()
{
	componentsAwoken = true;

	// Call the awake function for all the components
	for (Component* component : components)
		component->Awake();
}

void GameObject::UpdateComponents()
{
	// Call the update function for all the components
	for (Component* component : components)
	{
		if (!component->IsAwake() || !component->IsActive())
			continue;

		if (!component->hasStarted)
		{
			component->Start();
			component->hasStarted = true;
		}

		component->Update();
	}
}

void GameObject::FixedUpdateComponents()
{
	for (Component* component : components)
	{
		if (!component->IsAwake())
			component->Awake();

		if (component->IsAwake() && component->IsActive())
			component->FixedUpdate();
	}
}

void GameObject::LateUpdateComponents()
{
	for (Component* component : components)
	{
		if (component->IsAwake() && component->IsActive())
			component->LateUpdate();
	}
}

void GameObject::OnDestroy()
{
	//Core::Engine::Graph::deleteGameObject(m_name);
}

void GameObject::OnCollisionEnter(void* parameters)
{
	if (parameters == nullptr)
	{
		HD_LOG("No parameters, ignoring rest of OnCollisionEnter", LogSeverity::LOG_WARNING);
		return;
	}

	CollisionInfo* collision = static_cast<CollisionInfo*>(parameters);

	if (collision == nullptr)
		return;

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnCollision(collision);
	}
}

void GameObject::OnTriggerEnter(void* parameters)
{
	if (parameters == nullptr)
	{
		HD_LOG("No parameters, ignoring rest of OnTriggerEnter", LogSeverity::LOG_WARNING);
		return;
	}

	TriggerInfo* trigger = static_cast<TriggerInfo*>(parameters);

	if (trigger == nullptr)
		return;

	for (int i = 0; i < components.size(); i++)
	{
		components[i]->OnTrigger(trigger);
	}
}

Component* GameObject::GetComponent(ComponentType type)
{
	for (Component* comp : components)
		if (comp->type == type)
			return comp;

	return nullptr;
}

std::deque<Component*> GameObject::GetComponents(ComponentType type)
{
	std::deque<Component*> comps;
	for (Component* comp : components)
		if (comp->type == type)
			comps.push_back(comp);

	return comps;
}

void GameObject::DeleteComponent(ComponentType type)
{
	for (int i = 0; i < components.size(); i++)
		if (components[i]->type == type)
		{
			delete components[i];
			components.erase(components.begin() + i);
			i--;
		}

}
