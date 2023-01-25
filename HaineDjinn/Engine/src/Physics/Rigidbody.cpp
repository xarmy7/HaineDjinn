
#include <random>
#include "Physics/Rigidbody.h"
#include "Physics/Collider.h"
#include "Core/Transform.h"
#include "Core/Scene.h"


#include <IMGUI/imgui.h>
#include <reactphysics3d/reactphysics3d.h>

#include "../../../Editor/include/CameraFollow.h";
#include "../../../Editor/include/Enemy.h";

#include "Game/InputSystem.h"


Rigidbody::Rigidbody(GameObject* inGameObject): Component(inGameObject, ComponentType::RIGIDBODY, nullptr) 
{
	Transform goTransform = *inGameObject->transform;
	rigidbody = Scene::GetPhysicsWorld()->createRigidBody(static_cast<reactphysics3d::Transform>(goTransform));
	nameComp = "RIGIDBODY";

	SetMass(1.f);
	SetGravity(false);

	eventTransformMove.SetFunction(std::bind(&Rigidbody::OnTransformMove, this));
	eventTransformMove.Subscribe(&inGameObject->transform->onMoveEvent);

	hasUpdate = false;
	hasLateUpdate = false;
}

Rigidbody::~Rigidbody()
{
	//Scene::GetPhysicsWorld()->destroyCollisionBody(rigidbody);
	Scene::GetPhysicsWorld()->destroyRigidBody(rigidbody);
}

void Rigidbody::FixedUpdate()
{
	reactphysics3d::Transform tr = rigidbody->getTransform();

	gameObject->transform->position = { tr.getPosition().x, tr.getPosition().y, tr.getPosition().z };
	Quaternion q = { tr.getOrientation().x, tr.getOrientation().y, tr.getOrientation().z, tr.getOrientation().w };
	gameObject->transform->rotation = q;
}

void Rigidbody::SetBodyType(const BodyType type)
{
	rigidbody->setType(static_cast<reactphysics3d::BodyType>(type));
}

void Rigidbody::SetGravity(const bool inGravity) {
	useGravity = inGravity;
	rigidbody->enableGravity(useGravity);
	rigidbody->setIsSleeping(false);
	HD_LOG("Setting gravity to: " + std::to_string(useGravity), LogSeverity::LOG_MESSAGE);
}

void Rigidbody::SetStatic(const bool inStatic) {
	isStatic = inStatic;

	if (inStatic)
		rigidbody->setType(reactphysics3d::BodyType::STATIC);
	else
		rigidbody->setType(reactphysics3d::BodyType::DYNAMIC);

	HD_LOG("Setting gravity to: " + std::to_string(isStatic), LogSeverity::LOG_MESSAGE);
}

void Rigidbody::SetMass(const float inMass) {
	mass = inMass;
	rigidbody->setMass(inMass);

	HD_LOG("Setting mass to: " + std::to_string(mass), LogSeverity::LOG_MESSAGE);

}

void Rigidbody::AddForce(const Vector3 force)
{
	rigidbody->applyWorldForceAtCenterOfMass({ force.x, force.y, force.z });
}
void Rigidbody::AddTorque(const Vector3 torque)
{
	rigidbody->applyLocalTorque({ torque.x, torque.y, torque.z });
}

reactphysics3d::Collider* Rigidbody::AddCollider(reactphysics3d::CollisionShape* shape, const reactphysics3d::Transform& transform)
{
	if (shape == nullptr) 
		return nullptr;

	return rigidbody->addCollider(shape, transform);
}

void Rigidbody::OnCollision(void* parameters)
{

}
void Rigidbody::OnTrigger(void* parameters)
{
	TriggerInfo* trigger = static_cast<TriggerInfo*>(parameters);

	Collider* self = nullptr;
	
	Enemy enemy;


	if (trigger->collider1->GetHost()->entityType == "Player" && trigger->collider2->GetHost()->entityType == "Enemy")
	{
		CameraFollow::player.gameObject = trigger->collider1->GetHost();
		enemy.gameObject = trigger->collider2->GetHost();

		if (CameraFollow::player.isAttacking)
		{
			enemy.gameObject->transform->position.x = rand() % 1000 + 2000;
			enemy.gameObject->transform->position.y = -100000.f;
			enemy.gameObject->transform->position.z = rand() % 1000 + 2000;
			enemy.gameObject->transform->onMoveEvent.Broadcast(nullptr);

		}
	}

	if (trigger->collider2->GetHost()->entityType == "Player" && trigger->collider1->GetHost()->entityType == "Enemy")
	{
		CameraFollow::player.gameObject = trigger->collider2->GetHost();
		enemy.gameObject = trigger->collider1->GetHost();

		if (CameraFollow::player.isAttacking)
		{
			enemy.gameObject->transform->position.x = rand() % 1000 + 2000;
			enemy.gameObject->transform->position.y = -100000.f;
			enemy.gameObject->transform->position.z = rand() % 1000 + 2000;
			enemy.gameObject->transform->onMoveEvent.Broadcast(nullptr);
		}
	}

	if (self == nullptr)
	{
		HD_LOG("Self is null, return", LogSeverity::LOG_ERROR);
		return;
	}

	HD_LOG("trigger: Self = " + self->GetHost()->name, LogSeverity::LOG_MESSAGE);

}

void Rigidbody::ComponentEditor()
{
	if (ImGui::TreeNodeEx("Rigidbody", ImGuiTreeNodeFlags_Framed))
	{
		reactphysics3d::Vector3 rbPos = rigidbody->getTransform().getPosition();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Position:\n {%.3f, %.3f, %.3f}", rbPos.x, rbPos.y, rbPos.z);
		reactphysics3d::Vector3 rbVel = rigidbody->getLinearVelocity();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Velocity:\n {%.3f, %.3f, %.3f}", rbVel.x, rbVel.y, rbVel.z);
		reactphysics3d::Vector3 rbAVel = rigidbody->getAngularVelocity();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Angular Velocity:\n {%.3f, %.3f, %.3f}", rbAVel.x, rbAVel.y, rbAVel.z);

		if (ImGui::Button("Toggle Gravity"))
		{
			SetGravity(!useGravity);
		}
		ImGui::SameLine();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Curr. %s", useGravity ? "true" : "false");

		if (ImGui::Button("Is Static"))
		{
			SetStatic(!isStatic);
		}
		ImGui::SameLine();
		ImGui::TextColored({ 0.8f, 0.8f, 0.8f, 1.f }, "Curr. %s", isStatic ? "true" : "false");

		if (ImGui::DragFloat("Mass", &mass, 0.05f, 0.0001f))
		{
			if (mass <= 0)
				mass = 0.0001f;

			SetMass(mass);
		}

		if (ImGui::SliderInt3("Movement locks", lockMovement, 0, 1))
		{
			rigidbody->setLinearLockAxisFactor(reactphysics3d::Vector3(lockMovement[0], lockMovement[1], lockMovement[2]));
		}

		if (ImGui::SliderInt3("Rotation locks", lockRotation, 0, 1))
		{
			rigidbody->setAngularLockAxisFactor(reactphysics3d::Vector3(lockRotation[0], lockRotation[1], lockRotation[2]));
		}


		ImGui::TreePop();
	}
}

void Rigidbody::OnTransformMove()
{
	rigidbody->setTransform(static_cast<reactphysics3d::Transform>(*gameObject->transform));
}
