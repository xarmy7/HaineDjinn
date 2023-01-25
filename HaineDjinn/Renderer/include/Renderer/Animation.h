#pragma once
#include "Core/Maths.h"
#include "Renderer/Bone.h"
#include "Renderer/ModelInstance.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

struct AssimpNodeData
{
    mat4x4 transformation = {};
    std::string name = "";
    int childrenCount = 0;
    std::vector<AssimpNodeData> children = {};
};

struct aiAnimation;

class Animation
{
public:
    Animation() = default;
   /* static std::shared_ptr<Animation> LoadClassicAnimFile() {};
    static std::vector<std::shared_ptr<Animation>> LoadFBXAnimFile() {};*/

    Animation(const aiScene* scene, const aiAnimation* animation, GameObject* gameObject);
    //~Animation() { }

    Bone* FindBone(const std::string& name);
    
    double GetTicksPerSecond();
    double GetDuration();
    inline const std::unordered_map<std::string, BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap;}
    const AssimpNodeData& GetRootNode() const { return m_RootNode; }

    const aiAnimation* animation;

private:
    //aiScene sceneAnim;
    void ReadMissingBones(const aiAnimation* animation, Model* model);
    void ReadHierarchyData(AssimpNodeData& dest, aiNode* src);

    static const ResourceType m_type = ResourceType::ANIMATION;
    //std::string m_textureType = "";

    std::vector<Bone> m_Bones = {};
    AssimpNodeData m_RootNode = {};
    std::unordered_map<std::string, BoneInfo> m_BoneInfoMap = {}; // Add m_Bone index
};
