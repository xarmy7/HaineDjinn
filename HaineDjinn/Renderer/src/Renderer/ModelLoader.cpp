
#include "Renderer/ModelLoader.h"
#include "Renderer/ResourcesManager.h"
#include "Core/Debug.h"

#include <string>
#include <sstream>
#include <cassert>
#include <memory>
#include <filesystem>
#include <stb_image.h>
#include <glad/gl.h>

#include <memory>

#include <assimp/Importer.hpp>

ModelLoader::ModelLoader()
{

}

Model& ModelLoader::FindModel(const std::string& filePath, ModelInstance* instance)
{
    std::string modelName = std::filesystem::path(filePath).filename().string();
    std::istringstream fileNameStream(modelName);
    std::getline(fileNameStream, modelName, '.');

    for (int i = 0; i < models.size(); i++)
        if (models[i].modelName == modelName)
            return models[i];

    return CreateModel(filePath, 1.f, instance);
}

Model& ModelLoader::CreateModel(const std::string& filePath, float scale, ModelInstance* instance)
{
    std::string modelName = std::filesystem::path(filePath).filename().string();
    std::istringstream fileNameStream(modelName);
    std::getline(fileNameStream, modelName, '.');

    /*for (int i = 0; i < models.size(); i++)
        if (models[i].modelName == modelName)
            return models[i];*/

    /*for (Model model : models)
        if (model.modelName == filePath)
        {
            HD_LOG("Model already created", LogSeverity::LOG_WARNING);
            return;
        }*/

    HD_LOG("Starting model loading: " + filePath, LogSeverity::LOG_MESSAGE);

    models.push_back({});

    Model& currentModel = models.back();

    LoadMesh(currentModel, filePath, scale, instance);

    if (currentModel.parts.size() <= 0)
    {
        HD_LOG("No parts in model, sending empty model out of vector", LogSeverity::LOG_ERROR);
        return currentModel;
    }

    for (int i = 0; i < currentModel.parts.size(); i++)
    {
        currentModel.parts[i].mesh->m_name = modelName + std::to_string(i);
        currentModel.parts[i].material = &instance->mat;

        //currentModel.parts[i].mesh->gpu = new GPUMesh();
    }

    currentModel.modelName = modelName;
    currentModel.filePath = filePath;
    currentModel.extension = std::filesystem::path(filePath).extension().string();
    currentModel.filePath = filePath;
        
    HD_LOG("Finished loading model: " + currentModel.modelName + currentModel.extension, LogSeverity::LOG_MESSAGE);

    return currentModel;
}

void ModelLoader::GenerateMaterials(const aiScene* scene, std::vector<Material>& materials, Model& model, const std::string& directory, ModelInstance* instance)
{
    scene->mNumTextures;
    for (unsigned int i = 0; i < scene->mNumMaterials; i++)
    {
        aiMaterial* mat = scene->mMaterials[i];
        
        Material material;
        
        std::vector<std::string> diffuseMaps = GetTextureNames(mat, aiTextureType_DIFFUSE, directory);
        std::vector<std::string> specularMaps = GetTextureNames(mat, aiTextureType_SPECULAR, directory);
        std::vector<std::string> normalMaps = GetTextureNames(mat, aiTextureType_NORMALS, directory);
        std::vector<std::string> heightMaps = GetTextureNames(mat, aiTextureType_HEIGHT, directory);

        if (diffuseMaps.size() > 0)
        {
            std::shared_ptr<Texture> texPtr = *static_cast<std::shared_ptr<Texture>*>(resourcesManager->AddToLoad((diffuseMaps[0]), ResourceType::TEXTURE));
            instance->mat.SetDiffuseTexture(texPtr);
            instance->mat.m_name = texPtr->m_name;
        }

        if (normalMaps.size() > 0)
        {
            void* texPtr = resourcesManager->AddToLoad((normalMaps[0]), ResourceType::TEXTURE);
            std::shared_ptr<Texture>* sharedPtr = static_cast<std::shared_ptr<Texture>*>(texPtr);
            material.SetNormalTexture(*sharedPtr);
        }

        materials.push_back(material);
    }
}

void ModelLoader::LoadMesh(Model& model, const std::string& filePath, float scale, ModelInstance* instance)
{

    // read file via ASSIMP
    importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
    scene = importer->ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::string errorStr("ASSIMP ERROR: ");
        errorStr = errorStr + importer->GetErrorString();
        HD_LOG(errorStr , LogSeverity::LOG_ERROR);
        return;
    }
    std::string folderName = std::filesystem::path(filePath).parent_path().string();
    model.scene = scene;
    // Create all necessary materials
    std::vector<Material> materials;
    GenerateMaterials(scene, materials, model, folderName, instance);

    // process ASSIMP's root node recursively
    ProcessNode(model, scene->mRootNode, scene, materials);
}

void ModelLoader::ProcessNode(Model& model, aiNode* node, const aiScene* scene, std::vector<Material>& materials)
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(model, mesh, scene, materials);

        aiMatrix4x4 temp = node->mTransformation.Transpose();

        ai_real* pointer = &temp.a1;

        for (int i = 0; i < 16; i++)
        {
            model.parts.back().localMatrix.e[i] = *pointer;
            pointer++;
        }
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
        ProcessNode(model, node->mChildren[i], scene, materials);
}

// TODO: Move to Model.h?
Model::~Model()
{
    for (int i = 0; i < parts.size(); i++)
    {
//        delete parts[i].material;
    }

    parts.clear();
    delete shaderLoaded;
}

void Model::SetVertexBoneDataToDefault(Vertex& vertex)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        vertex.boneID[i] = -1;
        vertex.weight[i] = 0.0f;
    }
}

const aiScene* Model::GetAssimpScene() const
{
    assert(scene != nullptr);
    return scene;
}

void ModelLoader::ProcessMesh(Model& model, aiMesh* mesh, const aiScene* scene, std::vector<Material>& materials)
{
    LoadPart(model);

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vector3 vector; 

        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.normal = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            Vector2 vec;

            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.uv = vec;
            // tangent
            vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            //vertex.Tangent = vector;
            // bitangentw
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            //vertex.Bitangent = vector;
        }
        else
            vertex.uv = { 0.0f, 0.0f };

        model.GetLastMesh()->vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace& face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            model.GetLastMesh()->indices.push_back(face.mIndices[j]);
    }
    
    if (materials.size() > mesh->mMaterialIndex)
        model.SetLastMaterial(materials[mesh->mMaterialIndex]);

    //ProcessMaterial(model.GetLastMaterial(), mesh, scene);
    model.ExtractBoneWeightForVertices(model.GetLastMesh()->vertices, mesh, scene);

    return;
}

void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.boneID[i] < 0)
        {
            vertex.weight[i] = weight;
            vertex.boneID[i] = boneID;
            break;
        }
    }
}

mat4x4 Model::AiMatrixToMat4x4(const aiMatrix4x4& p_matrix)
{
    mat4x4 matrix{
    p_matrix.a1, p_matrix.a2, p_matrix.a3, p_matrix.a4,
    p_matrix.b1, p_matrix.b2, p_matrix.b3, p_matrix.b4,
    p_matrix.c1, p_matrix.c2, p_matrix.c3, p_matrix.c4,
    p_matrix.d1, p_matrix.d2, p_matrix.d3, p_matrix.d4
    };
    return matrix;
}

void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
{
    for (unsigned int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
    {
        int boneID = -1;
        std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
        if (boneInfoMap.find(boneName) == boneInfoMap.end())
        {
            BoneInfo newBoneInfo = {};
            newBoneInfo.id = boneCounter;

            aiMatrix4x4 temp = mesh->mBones[boneIndex]->mOffsetMatrix.Transpose();
            ai_real* pointer = &temp.a1;
            for (int i = 0; i < 16; i++)
            {
                newBoneInfo.offset.e[i] = *pointer;
                pointer++;
            }

            newBoneInfo.offset = AiMatrixToMat4x4(mesh->mBones[boneIndex]->mOffsetMatrix);
            boneInfoMap[boneName] = newBoneInfo;
            boneID = boneCounter;
            boneCounter++;
        }
        else
        {
            boneID = boneInfoMap[boneName].id;
        }
        assert(boneID != -1);
        aiVertexWeight* weights = mesh->mBones[boneIndex]->mWeights;
        int numWeights = mesh->mBones[boneIndex]->mNumWeights;

        for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
        {
            int vertexId = weights[weightIndex].mVertexId;
            float weight = weights[weightIndex].mWeight;
            assert(vertexId <= vertices.size());
            SetVertexBoneData(vertices[vertexId], boneID, weight);
        }
    }
}

void ModelLoader::LoadPart(Model& model)
{
    model.parts.push_back({});

    model.parts.back().mesh = new Mesh;
    model.parts.back().material = new Material;
}

//void ModelLoader::LoadMaterialTextures(const aiScene* scene, const std::string& directory, ModelInstance* instance)
//{
//    std::vector<Texture> textures;
//
//    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
//    {
//        aiString str;
//        mat->GetTexture(type, i, &str);
//        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
//        bool skip = false;
//        for (unsigned int j = 0; j < material->textures_loaded.size(); j++)
//        {
//            if (std::strcmp(material->textures_loaded[j].m_name.data(), str.C_Str()) == 0)
//            {
//                material->theTexture.push_back(material->textures_loaded[j]);
//                //skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
//                break;
//            }
//        }
//        if (!skip)
//        {
//            textures.emplace_back(directory + "/" + str.C_Str());
//            //texture.m_textureType = typeName;
//            //texture.m_name = str.C_Str();
//            //material->theTexture.push_back(texture);
//            //material->textures_loaded.push_back(texture);  // store it as texture loaded for entire model, we won't unnecesery load duplicate textures.
//        }
//    }
//
//    return textures;
//}

std::vector<std::string> ModelLoader::GetTextureNames(aiMaterial* mat, aiTextureType type, const std::string& directory)
{
    std::vector<std::string> names;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        names.emplace_back(directory + "/" + str.C_Str());
    }

    return names;
}
