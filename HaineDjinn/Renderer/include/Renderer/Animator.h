#pragma once
#include "Animation.h"

class Animator : public Component
{
public:
    Animator(GameObject* gameObject);
    ~Animator();

    void UpdateAnimation(double dt);
    void ComponentEditor() override;

    void SetAnimation(int animationId);
    const Animation* GetAnimation() const { return currentAnimation; }
    bool HasAnimation() const { return currentAnimation != nullptr; }

    const std::vector<mat4x4>& GetFinalBoneMatrices() const { return finalBoneMatrices; }
    
    Animation* currentAnimation = nullptr;

private:
    void PlayAnimation(Animation* pAnimation);
    void CalculateBoneTransform(const AssimpNodeData& node, mat4x4 parentTransform);

    //std::deque<std::shared_ptr<Animation>>* animations;


    std::vector<mat4x4> finalBoneMatrices = {};

    double currentTime = 0.f;
    double deltaTime = 0.f;

    bool isSelectingAAnimation = false;
    bool isSelectingAClassicAnim = false;
    bool isSelectingAFBXAnim = false;
    bool isSelectingAWrongAnim = false;
    std::filesystem::directory_entry entryTemp = {};
    const aiScene* animScene = {};
    Assimp::Importer importer = {};
};
