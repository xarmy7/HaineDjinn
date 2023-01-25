#pragma once

//#include "Core/Maths.h"
#include "Renderer/ModelInstance.h"

#include <stdlib.h>
#include <deque>

class ResourcesManager;

class ModelLoader
{
public:

    ModelLoader();
    Model& CreateModel(const std::string& filePath, float scale, ModelInstance* instance);
    Model& FindModel(const std::string& filePath, ModelInstance* instance);

    void Clear() { models.clear(); }

    // Read only reference to models, used by SceneObjWindow.cpp
    const std::deque<Model>& GetModels() { return models; };

    void SetResourcesManager(ResourcesManager* inRM) { resourcesManager = inRM; };
    void SetImporter(Assimp::Importer* inImporter) { importer = inImporter; };
    void LoadMesh(Model& model, const std::string& filePath, float scale, ModelInstance* instance);
    //void LoadMaterialTextures(const aiScene* scene, const std::string& directory, ModelInstance* instance);

    const aiScene* scene = nullptr;

private:
     Assimp::Importer* importer = nullptr; // Assimp data is owned by importer
     ResourcesManager* resourcesManager = nullptr;

    std::deque<Model> models; // deque to prevent breaking pointers in ModelInstance

    void GenerateMaterials(const aiScene* scene, std::vector<Material>& materials, Model& model, const std::string& directory, ModelInstance* instance);
    void ProcessNode(Model& model, aiNode* node, const aiScene* scene, std::vector<Material>& materials);
    void ProcessMesh(Model& model, aiMesh* mesh, const aiScene* scene, std::vector<Material>& materials);
    void LoadPart(Model& model);

    std::vector<std::string> GetTextureNames(aiMaterial* mat, aiTextureType type, const std::string& directory);
};
