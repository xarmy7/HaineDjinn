
#include "Renderer/Animator.h"
#include "../../../Editor/include/EditorWindow/ImGuiIncluder.h"
#include "Core/Debug.h"

Animator::Animator(GameObject* gameObject) : Component(gameObject, ComponentType::ANIMATOR, {})
{
	currentTime = 0.0;
	nameComp = "ANIMATOR";

	finalBoneMatrices.reserve(100);
	for (int i = 0; i < 100; i++)
		finalBoneMatrices.push_back(mat4::Identity());
}

Animator::~Animator()
{
	delete currentAnimation;
}

void Animator::UpdateAnimation(double dt)
{
    deltaTime = dt;
     if (currentAnimation)
    {
        currentTime += currentAnimation->GetTicksPerSecond() * dt;
        currentTime = fmod(currentTime, currentAnimation->GetDuration());
        CalculateBoneTransform(currentAnimation->GetRootNode(), mat4::Identity());
    }
}

void Animator::PlayAnimation(Animation* pAnimation)
{
    currentAnimation = pAnimation;
    currentTime = 0.0f;
}

void Animator::CalculateBoneTransform(const AssimpNodeData& node, mat4x4 parentTransform)
{
    mat4x4 nodeTransform = node.transformation;

    Bone* Bone = currentAnimation->FindBone(node.name);

	if (Bone)
	{
		Bone->Update(currentTime);
		nodeTransform = Bone->GetLocalTransform();
	}

	mat4x4 globalTransformation = parentTransform * nodeTransform;

	const std::unordered_map<std::string, BoneInfo>& boneInfoMap = currentAnimation->GetBoneIDMap();

	if (boneInfoMap.find(node.name) != boneInfoMap.end())
	{
		int index = boneInfoMap.find(node.name)->second.id;
		mat4x4 offset = boneInfoMap.find(node.name)->second.offset;

        finalBoneMatrices[index] = globalTransformation * offset;
    }

    for (unsigned int i = 0; i < node.children.size(); i++)
        CalculateBoneTransform(node.children[i], globalTransformation);
}

void Animator::ComponentEditor()
{
	ModelInstance* instance = static_cast<ModelInstance*>(gameObject->GetComponent(ComponentType::MODELINSTANCE));
	if (ImGui::TreeNodeEx("Animator", ImGuiTreeNodeFlags_Framed))
	{
		if (ImGui::BeginCombo("anim", currentAnimation == nullptr ? "(none)" : currentAnimation->animation->mName.C_Str()))
		{
			const aiScene* scene = instance->model->GetAssimpScene();
			for (unsigned int i = 0; i < scene->mNumAnimations; i++)
			{
				const aiAnimation* anim = scene->mAnimations[i];
				if (ImGui::Selectable(anim->mName.C_Str()))
					SetAnimation(i);
			}

			ImGui::EndCombo();
		}

		ImGui::TreePop();
	}

	//ImGui::End();
}

void Animator::SetAnimation(int animationId)
{
	ModelInstance* instance = static_cast<ModelInstance*>(gameObject->GetComponent(ComponentType::MODELINSTANCE));
	const aiScene* scene = instance->model->GetAssimpScene();
	//assert(animationId >= 0 && animationId < scene->mNumAnimations);
	HD_ASSERT(animationId >= 0 && animationId < (int)(scene->mNumAnimations), "Animator::SetAnimation assert");

	delete currentAnimation;
	currentAnimation = new Animation(instance->model->GetAssimpScene(), scene->mAnimations[animationId], gameObject);
}

