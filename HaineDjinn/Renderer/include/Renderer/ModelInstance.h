#pragma once

#include "Core/Maths.h"
#include "Renderer/Mesh.h"
#include "Core/Component.h"
#include "Renderer/Material.h"
#include "Renderer/Shader.h"

#include <cassert>
#include <vector>
#include <map>
#include <unordered_map>

struct Part
{
    Mesh* mesh;
    Material* material;

    mat4x4 localMatrix;
};

struct BoneInfo
{
    int id; //Index in finalBoneMatrices
    mat4x4 offset; //matrix transform vertex from model space to bone space
};

namespace Assimp
{
    class Importer;
}
struct aiScene;

class Model
{
public:
    const aiScene* scene;
    std::unordered_map<std::string, BoneInfo> boneInfoMap;
    int boneCounter = 0;

    std::unordered_map<std::string, BoneInfo>& GetBoneInfoMap() { return boneInfoMap; };
    int& GetBoneCount() { return boneCounter; };

    const aiScene* GetAssimpScene() const;

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

    mat4x4 AiMatrixToMat4x4(const aiMatrix4x4& p_matrix);

    Shader* shaderLoaded = nullptr;

    std::string modelName;

    std::string extension;
    std::string filePath;

    std::vector<Part> parts;

    Mesh* GetLastMesh() { return parts.back().mesh; }
    Material* GetLastMaterial() { return parts.back().material; }
    void SetLastMaterial(const Material& inMaterial) { parts.back().material = new Material(inMaterial); }

    Model(std::string filePath, float scale = 1.f) { assert(false); }
    Model() {};
    ~Model();
};

class ModelInstance : public Component
{
public:
    ModelInstance(GameObject* gameObject) : Component(gameObject, ComponentType::MODELINSTANCE, {})
    {
        nameComp = "MODELINSTANCE";
    }
    Material mat;

    Model* model = nullptr;

    Vector3 position = {};
    Vector3 eulerAngles = {}; // Vous pouvez utiliser un quaternion aussi
    float scale = 1.f;        // Uniform scale pour simplifier

    std::string objectName = "";
    bool isStatic = false;
    bool isAnEntity = false;

    bool dirtyFlag = true; // true pour signifier qu'il faut recalculer la worldMatrix de toutes ses parts
    std::vector<mat4x4> worldMatricesPerParts = {};

    void ComponentEditor() override;

    ModelInstance& operator=(const ModelInstance& other) { 
        
        model = other.model;
        position = other.position;
        eulerAngles = other.eulerAngles;
        scale = other.scale;
        objectName= other.objectName;
        isStatic = other.isStatic;
        isAnEntity = other.isAnEntity;
        dirtyFlag = other.dirtyFlag;
        worldMatricesPerParts = other.worldMatricesPerParts;

        return *this;
    };
};
