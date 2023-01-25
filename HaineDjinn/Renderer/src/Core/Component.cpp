
#include "Core/Component.h"

#include <IMGUI/imgui.h>

#include <algorithm>
#include <memory>
#include <vector>

Component::Component(GameObject* inGameObject, ComponentType inType, Component* childPtr)
	: type(inType)
{
	this->gameObject = inGameObject;
	if (childPtr != nullptr)
		gameObject->components.push_back(childPtr);
	
	hasGameObject = true;
}

void Component::SetActive(bool value)
{
	// Call the correct function when state changed
	if (IsActive() != value)
		value ? OnEnable() : OnDisable();

	// Activate or deactivate the component
	Object::SetActive(value);
}

void Component::OnDestroy()
{
	if (gameObject->components.size() == 0)
		return;

	int index = -1;
	for (int i = 0; i < gameObject->components.size(); i++)
	{
		if (gameObject->components[i] == this)
		{
			index = i;
			break;
		}
	}

	gameObject->components.erase(gameObject->components.begin() + index);
}

bool Component::IsActive()
{
	return isAwake;
}

GameObject* Component::GetHost()
{
	return gameObject;
}

void Component::ComponentEditor()
{
	if (ImGui::TreeNodeEx("Blank Component", ImGuiTreeNodeFlags_Framed))
	{
		ImGui::TreePop();
	}
}
