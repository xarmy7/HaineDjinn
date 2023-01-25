
#include "PhysTestComp.h"
#include "Game/InputSystem.h"
#include "Physics/Rigidbody.h"

#include <IMGUI/imgui.h>

PhysTestComp::PhysTestComp(GameObject* inGameObject) : Component(inGameObject, ComponentType::PHYS_TEST, nullptr)
{
	initialPos = inGameObject->transform->position;
	initialRot = inGameObject->transform->rotation;

	forceCooldown.hasEnded = true;
	torqueCooldown.hasEnded = true;

	rb = inGameObject->GetComponent<Rigidbody>(ComponentType::RIGIDBODY);
}

void PhysTestComp::Update()
{
	forceCooldown.Update();
	torqueCooldown.Update();
}

void PhysTestComp::FixedUpdate()
{
	if (InputSystem::GetInput("B"))
	{
		rb->SetGravity(false);
		rb->SetStatic(true);
	}
	if (InputSystem::GetInput("N"))
	{
		rb->SetGravity(true);
		rb->SetStatic(false);
	}
	if (InputSystem::GetInput("R"))
	{
		gameObject->transform->position = initialPos;
		gameObject->transform->rotation = initialRot;
		rb->SetGravity(false);
		rb->SetStatic(true);
		gameObject->transform->onMoveEvent.Broadcast(nullptr);
	}
	if (InputSystem::GetInput("T"))
	{
		gameObject->transform->position = initialPos;
		gameObject->transform->onMoveEvent.Broadcast(nullptr);
	}
	if (InputSystem::GetInput("Y"))
	{
		gameObject->transform->position = initialPos;
		gameObject->transform->rotation = initialRot;
		gameObject->transform->onMoveEvent.Broadcast(nullptr);
	}
	if (InputSystem::GetInput("F"))
	{
		if (forceCooldown.hasEnded)
		{
			forceCooldown.Restart(1.f);
			rb->AddForce({(float)(rand() % 290) - 145.f, (float)(rand() % 290) - 145.f, (float)(rand() % 290) - 145.f });
		}
	}
	if (InputSystem::GetInput("G"))
	{
		if (torqueCooldown.hasEnded)
		{
			torqueCooldown.Restart(1.f);
			rb->AddTorque({ (float)(rand() % 290) - 145.f, (float)(rand() % 290) - 145.f, (float)(rand() % 290) - 145.f });
		}
	}
}

void PhysTestComp::ComponentEditor()
{
	if (ImGui::TreeNodeEx("Physics Test Component", ImGuiTreeNodeFlags_Framed))
	{
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Stored pos:\n {%.3f, %.3f, %.3f}", initialPos.x, initialPos.y, initialPos.z);
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Stored rot:\n {%.3f, %.3f, %.3f}", initialRot.x, initialRot.y, initialRot.z);

		ImGui::TextColored({ 1.0f, 0.0f, 0.9f, 1.f }, "Controls:\n - B to lock all PTC\n linked rigidbodies\n - N to unlock all PTC\n linked rigidbodies\n - R to reset transform\n and lock\n - T to reset position\n only\n - Y to reset position\n and rotation\n - F to apply a random\n force (magnitude\n between 1 and ~500)\n - G to apply a random\n torque (between 1 and ~500)");

		ImGui::TreePop();
	}
}
