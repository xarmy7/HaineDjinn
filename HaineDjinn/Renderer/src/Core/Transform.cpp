
#include "Core/Transform.h"
#include "Core/Event.h"
#include "Core/Debug.h"

#include <IMGUI/imgui.h>
#include <istream>

//#include "utils.hpp"

Transform::Transform(GameObject* gameObject) : Component(gameObject, ComponentType::TRANSFORM, this)
{
	nameComp = "TRANSFORM";
	rotation = QuaternionFromAxisAngle({ 0, 0, 1 }, euler.z) * QuaternionFromAxisAngle({ 1, 0, 0 }, euler.x) * QuaternionFromAxisAngle({ 0, 1, 0 }, euler.y) * QuaternionIdentity();
	rotation = QuaternionNormalize(rotation);
	HD_LOG("constructor q = {" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z) + ", " + std::to_string(rotation.w) + "}", LogSeverity::LOG_MESSAGE);
	onMoveEvent = {};
}

void Transform::OnDestroy()
{
	if (HasParent())
		GetParent()->DeleteChildFromTransform(this);
}

bool Transform::HasParent()
{
	return parent != nullptr;
}

bool Transform::HasChild()
{
	return children.size() > 0;
}

Transform* Transform::GetChild(int childIndex)
{
	return children[childIndex];
}

GameObject* Transform::GetGOChild(int childIndex)
{
	return children[childIndex]->GetHost();
}

int Transform::GetChildrenCount()
{
	return (int)(children.size());
}

GameObject* Transform::GetGOParent()
{
	return parent->GetHost();
}

std::shared_ptr<Transform> Transform::GetParent()
{
	return parent;
}

mat4x4 Transform::GetModel()
{
	return	Maths::mat4::Translate(position) *
			mat4::RotateQuat(rotation) * 
			Maths::mat4::Scale(scale);
}

mat4x4 Transform::GetGlobalModel()
{
	if (parent)
		return GetParentModel() * GetModel();

	return GetModel();
}

mat4x4 Transform::GetParentModel() const
{
	if (parent)
		return parent->GetGlobalModel();

	return Maths::mat4::Identity();
}

void Transform::DeleteChildFromTransform(Transform* transform)
{
	for (size_t i = 0; i < children.size(); i++)
	{
		if (children[i] == transform)
		{
			children[i] = children.back();
			children.pop_back();

			return;
		}
	}
}

Vector3 Transform::GetGlobalRotation() const
{
	return euler + GetParentRotation();
}

Vector3 Transform::GetGlobalPosition() const
{
	return position + GetParentPosition();
}

Vector3 Transform::GetParentRotation() const
{
	if (parent)
		return parent->GetGlobalRotation();

	return Vector3{ 0.f, 0.f, 0.f };
}

Vector3 Transform::GetParentPosition() const
{
	if (parent)
		return parent->GetGlobalPosition();

	return Vector3{ 0.f, 0.f, 0.f };
}

Vector3 Transform::GetForward()
{
	mat4x4 model = GetGlobalModel();

	return -Vector3{ model.e[2], model.e[6], model.e[10] };
}

void Transform::SetParent(std::shared_ptr<Transform> _parent)
{
	parent = _parent;
}

void Transform::SetParent(GameObject* gameObject)
{

}

void Transform::SetChild(Transform* child)
{
	children.push_back(child);
}

void Transform::SetChild(GameObject* gameObject)
{

}

void Transform::Update()
{
	hasBeenUpdated = true;
	//euler = mat4::RotationMatToEuler(mat4::RotateQuat(rotation));
	//rotation = ClampLoop(rotation, 0.f, Maths::TAU);
}

void Transform::ParseComponent(GameObject* gameObject, std::istringstream& iss, std::string& parentName)
{
	
}

mat4x4 Transform::GetTransformMatrix(Transform transform)
{
	mat4x4 returnMat = mat4::Identity();
	returnMat = returnMat * mat4::Translate(position) * mat4::RotateQuat(rotation) * mat4::Scale(scale);

	return returnMat;
}

void Transform::ComponentEditor()
{
	if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed))
	{
		Vector3 eulerDeg = euler;
		eulerDeg.x = euler.x * 180 / M_PI;
		eulerDeg.y = euler.y * 180 / M_PI;
		eulerDeg.z = euler.z * 180 / M_PI;

		//HD_LOG("eulerDeg = {" + std::to_string(euler.x) + ", " + std::to_string(euler.y) + ", " + std::to_string(euler.z) + "}", LogSeverity::LOG_MESSAGE);
		//HD_LOG("q = {" + std::to_string(rotation.x) + ", " + std::to_string(rotation.y) + ", " + std::to_string(rotation.z) + ", " + std::to_string(rotation.w) + "}", LogSeverity::LOG_MESSAGE);

		if (ImGui::DragFloat3("Position", position.e, 0.05f) ||
			ImGui::DragFloat3("Rotation", eulerDeg.e, 0.5f))
		{
			euler.x = eulerDeg.x * M_PI / 180;
			euler.y = eulerDeg.y * M_PI / 180;
			euler.z = eulerDeg.z * M_PI / 180;
			rotation = QuaternionFromAxisAngle({ 0, 0, 1 }, euler.z) * QuaternionFromAxisAngle({ 1, 0, 0 }, euler.x) * QuaternionFromAxisAngle({ 0, 1, 0 }, euler.y) * QuaternionIdentity();
			rotation = QuaternionNormalize(rotation);

			onMoveEvent.Broadcast(nullptr);
		}
		ImGui::DragFloat3("Scale", scale.e, 0.05f, 0.f, 1000.f, "%f");

		ImGui::TreePop();
	}
}
