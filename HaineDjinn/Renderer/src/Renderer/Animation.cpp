#include "Renderer/Animation.h"
#include "Renderer/animator.h"
//#include "../../../Editor/include/Core/Editor.h"
#include "../../../Editor/include/EditorWindow/ImGuiIncluder.h"
//#include "../../../Editor/include/EditorWindow/EditorFunction.h"

#include <assimp/scene.h>
#include <assimp/anim.h>

#include <cassert>
#include <unordered_map>
#include <filesystem>
#include <IMGUI/imgui.h>



Animation::Animation(const aiScene* scene, const aiAnimation* animation, GameObject* gameObject)
    : animation(animation)
{
    m_Bones.reserve(100);

    ReadHierarchyData(m_RootNode, scene->mRootNode);
    ModelInstance* instance = static_cast<ModelInstance*>(gameObject->GetComponent(ComponentType::MODELINSTANCE));
    ReadMissingBones(animation, instance->model);
}
/*
Animation::Animation(std::filesystem::path animationPath, GameObject* gameObject)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath.string(), aiProcess_Triangulate);
    assert(scene && scene->mRootNode);

    m_Bones.reserve(100);

    ReadHierarchyData(m_RootNode, scene->mRootNode);
    ModelInstance* instance = static_cast<ModelInstance*>(gameObject->GetComponent(ComponentType::MODELINSTANCE));
    ReadMissingBones(animation, instance->model);
}

Animation::Animation(std::filesystem::path animationPath)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(animationPath.string(), aiProcess_Triangulate);
    assert(scene && scene->mRootNode);

    aiAnimation* animation;
    if (scene->mAnimations == nullptr)
    {
        exist = false;
        return;
    }
    else
    {
        exist = true;
        animation = scene->mAnimations[0];
    }

    m_Bones.reserve(100);

    m_Duration = animation->mDuration;
    m_TicksPerSecond = animation->mTicksPerSecond;
    ReadHierarchyData(m_RootNode, scene->mRootNode);
    m_nameFile = animationPath.string();
}
*/
double Animation::GetDuration()
{
    return animation->mDuration;
}

double Animation::GetTicksPerSecond()
{
    return animation->mTicksPerSecond;
}

Bone* Animation::FindBone(const std::string& name)
{
    std::vector<Bone>::iterator iter = std::find_if(m_Bones.begin(), m_Bones.end(), [&](const Bone& Bone)
        {
            return Bone.GetBoneName() == name;
        }
    );
    if (iter == m_Bones.end()) return nullptr;
    else return &(*iter);
}

void Animation::ReadMissingBones(const aiAnimation* animation, Model* model)
{
    int size = animation->mNumChannels;

    std::unordered_map<std::string, BoneInfo>& boneInfoMap = model->GetBoneInfoMap();//getting m_BoneInfoMap from Model class
    int& boneCount = model->GetBoneCount(); //getting the m_BoneCounter from Model class

    //reading channels(bones engaged in an animation and their keyframes)
    for (int i = 0; i < size; i++)
    {
        aiNodeAnim* channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;

        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            boneInfoMap[boneName].id = boneCount;
            boneCount++;
        }
        m_Bones.push_back(Bone(channel->mNodeName.data,
            boneInfoMap[channel->mNodeName.data].id, channel));
    }

    m_BoneInfoMap = boneInfoMap;
}

void Animation::ReadHierarchyData(AssimpNodeData& dest, aiNode* src)
{
    assert(src);

    dest.name = src->mName.data;

    aiMatrix4x4 temp = src->mTransformation.Transpose();
    ai_real* pointer = &temp.a1;
    for (unsigned int i = 0; i < 16; i++)
    {
        dest.transformation.e[i] = *pointer;
        pointer++;
    }

    //dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
    dest.children.reserve(src->mNumChildren);

    for (unsigned int i = 0; i < src->mNumChildren; i++)
    {
        AssimpNodeData newData;
        ReadHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
    }
}

