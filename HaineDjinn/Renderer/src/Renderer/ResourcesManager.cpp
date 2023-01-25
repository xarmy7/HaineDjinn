
#include "Renderer/ResourcesManager.h"
#include "Renderer/Mesh.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/Animation.h"
#include "Renderer/Animator.h"

#include "Renderer/Material.h"

#include <GLFW/glfw3.h>

#include <iostream>

#include <thread>
#include <functional>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked

#include <cassert>

ResourcesManager::ResourcesManager()
{
	//testDeque = &cppDeque;

	if (THREADING_TYPE == 1)
		runThreads = false;

	//for (int i = 0; i < 4; i++)
	//	threads[i] = std::thread(std::bind(&ResourcesManager::ThreadFunction, this, i));
}

#pragma region THREADING FUNCTIONS
// Adds a resource to the queue if not already in it
void* ResourcesManager::AddToLoad(const std::string filePath, const ResourceType type)
{
	for (Resource* resource : toLoad)
	{
		if (resource->m_type == type && resource->m_name == filePath)
			return resource->resourcePtr;
	}

	void* resourcePointer = nullptr;
	std::vector<std::shared_ptr<Animation>> anims;

	switch (type)
	{
	case ResourceType::MESH:
		for (std::shared_ptr<Mesh>& mesh : meshes)
			if (mesh->m_type == type && mesh->m_name == filePath)
				return &mesh;

		meshes.push_back(std::make_unique<Mesh>(filePath));
		resourcePointer = &meshes.back();
		break;
	case ResourceType::SHADER:
		for (std::shared_ptr<Shader>& shader : shaders)
			if (shader->m_type == type && shader->m_name == filePath)
				return &shader;

		shaders.push_back(std::make_unique<Shader>(filePath));
		resourcePointer = &shaders.back();
		break;
	case ResourceType::TEXTURE:
		for (std::shared_ptr<Texture>& texture : textures)
			if (texture->m_type == type && texture->m_name == filePath)
				return &texture;

		textures.push_back(std::make_unique<Texture>(filePath));
		resourcePointer = &textures.back();
		break;
	default:
		return nullptr;
	}

	toLoad.push_back(new Resource(filePath, type));
	toLoad.back()->resourcePtr = resourcePointer;

	return resourcePointer;
}

void* ResourcesManager::AddToLoad(const std::string name, const std::string diffTexPath, const std::string normTexPath, Vector4 ambientColor, Vector4 diffuseColor, Vector4 specularColor)
{
	for (Resource* resource : toLoad)
	{
		if (resource->m_type == ResourceType::MATERIAL && resource->m_name == name)
			return resource->resourcePtr;
	}

	void* resourcePointer = nullptr;

	for (std::shared_ptr<Material>& material : materials)
		if (material->m_type == ResourceType::MATERIAL && material->m_name == name)
			return &material;

	materials.push_back(std::make_unique<Material>(name, diffTexPath, normTexPath));
	materials.back()->ambientColor = ambientColor;
	materials.back()->diffuseColor = diffuseColor;
	materials.back()->specularColor = specularColor;
	resourcePointer = &materials.back();

	toLoad.push_back(new Resource(name, ResourceType::MATERIAL));
	toLoad.back()->resourcePtr = resourcePointer;

	return resourcePointer;
}

void ResourcesManager::ThreadFunction(int threadID)
{
	if (THREADING_TYPE == 1) return;

	while (runThreads)
	{
		// spinlocks if nothing to load/other thread using toLoad

		for (int i = 0; i < toLoad.size(); i++)
		{
			while ((toLoad.size() == 0 && runThreads) || flag.test_and_set()) { std::this_thread::yield(); }
	
			if (i < toLoad.size() 
				&& toLoad[i] != nullptr
				&& toLoad[i]->inProcess.load() == false
				&& toLoad[i]->m_type.load() != ResourceType::SHADER)
			{
				toLoad[i]->inProcess.store(true);
				Resource resource{ toLoad[i]->m_name, toLoad[i]->m_type.load() };
				flag.clear();

				LoadResource(resource.m_name, resource.m_type.load());

				for (int j = 0; j < toLoad.size(); j++)
				{
					if (toLoad[j]->m_type == resource.m_type && toLoad[j]->m_name == resource.m_name)
					{
						while (flag.test_and_set()) { std::this_thread::yield(); }

						if (j < toLoad.size())
							toLoad[j]->toDestroy.store(true);

						j = (int)(toLoad.size());
						flag.clear();
					}
				}
			}
			else
				flag.clear();
		}
	}
}
#pragma endregion

#pragma region RESOURCES CREATION/DESTRUCTION
// Resets the toLoad vector
void ResourcesManager::EraseQueue()
{
	while (flag.test_and_set()) { std::this_thread::yield(); }
	for (Resource* resource : toLoad)
		delete resource;

	toLoad.clear();
	flag.clear();
}

void ResourcesManager::CleanQueue()
{
	while (flag.test_and_set()) { std::this_thread::yield(); }

	for (int i = (int)toLoad.size() - 1; i >= 0; i--)
	{
		if (toLoad[i]->toDestroy)
		{
			delete toLoad[i];
			toLoad.erase(toLoad.begin() + i);
		}
	}
	flag.clear();
}

// Copy of searchResource, used in threads
void ResourcesManager::LoadResource(const std::string filePath, const ResourceType type)
{
	switch (type)
	{
	case ResourceType::MESH:
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i]->m_name == filePath && meshes[i]->m_type == type)
				return;

		CreateResource(filePath, type);
		return;
	case ResourceType::SHADER:
		for (int i = 0; i < shaders.size(); i++)
			if (shaders[i]->m_name == filePath && shaders[i]->m_type == type)
				return;

		CreateResource(filePath, type);
		return;
	case ResourceType::TEXTURE:
		for (int i = 0; i < textures.size(); i++)
			if (textures[i]->m_name == filePath && textures[i]->m_type == type)
				return;

		CreateResource(filePath, type);
		return;
	case ResourceType::MATERIAL:
		for (int i = 0; i < materials.size(); i++)
			if (materials[i]->m_name == filePath && materials[i]->m_type == type)
				return;
	/*
	case ResourceType::ANIMATION:
		for (int i = 0; i < animations.size(); i++)
			if (animations[i]->m_nameFile == filePath && animations[i]->m_type == type)
				return;
	*/
		CreateResource(filePath, type);
		return;
	default:
		return;
	}
}




// Generates a resource of a given name and a given type
void ResourcesManager::CreateResource(const std::string filePath, const ResourceType type)
{
	switch (type)
	{
	case ResourceType::MESH:
		//meshes.push_back(std::make_unique<Mesh>(filePath, 1.f));
		break;
	case ResourceType::SHADER:
		shaders.push_back(std::make_unique<Shader>(filePath.c_str()));
		break;
	case ResourceType::TEXTURE:
		textures.push_back(std::make_unique<Texture>(filePath));
		break;
	case ResourceType::MATERIAL:
		materials.push_back(std::make_unique<Material>(filePath, "", ""));
		break;
	/*
	case ResourceType::ANIMATION:
	{
		std::vector<std::shared_ptr<Animation>> anims = LoadFBXAnimFile(filePath);
		for (int i = 0; i < anims.size(); i++)
			animations.push_back(anims[i]);
		break;
	}
	*/
	default:
		break;
	}
}

void ResourcesManager::DestroyResource(const int index, const ResourceType type)
{
	if (index < 0) return;
	
	switch (type)
	{
	case ResourceType::MESH:
		meshes.erase(meshes.begin() + index);
		break;
	case ResourceType::SHADER:
		shaders.erase(shaders.begin() + index);
		break;
	case ResourceType::TEXTURE:
		textures.erase(textures.begin() + index);
		break;
	case ResourceType::MATERIAL:
		materials.erase(materials.begin() + index);
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region OGL USAGE
// Searches for a resource with a given name and a given type
int ResourcesManager::SearchResource(const std::string filePath, const ResourceType type)
{
#if THREADING_TYPE == 0
	for (Resource* resource : toLoad)
	{
		if (resource->m_name == filePath && resource->m_type == type && !resource->toDestroy)
			return CUT_RENDERING_CODE;
	}
#elif THREADING_TYPE == 1
	for (Resource* resource : toLoad)
	{
		if (resource->m_name == filePath && resource->m_type == type)
		{
			createResource(filePath, type);

			for (int i = 0; i < toLoad.size(); i++)
				if (toLoad[i] == resource)
				{
					delete toLoad[i];
					toLoad.erase(toLoad.begin() + i);
					i = (int)toLoad.size();
				}

			switch (type)
			{
			case ResourceType::MESH:
				return (int)(meshes.size() - 1);
			case ResourceType::SHADER:
				return (int)(shaders.size() - 1);
			case ResourceType::TEXTURE:
				return (int)(textures.size() - 1);
			case ResourceType::MATERIAL:
				return (int)(materials.size() - 1);
			default:
				return CUT_RENDERING_CODE;
			}
		}
	}
#endif

	while (flag.test_and_set()) { std::this_thread::yield(); }

	switch (type)
	{
	case ResourceType::MESH:
		for (int i = 0; i < meshes.size(); i++)
			if (meshes[i]->m_name == filePath && meshes[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		break;
	case ResourceType::SHADER:
		for (int i = 0; i < shaders.size(); i++)
			if (shaders[i]->m_name == filePath && shaders[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		flag.clear();
		CreateResource(filePath, type);
		return (int)(shaders.size() - 1);
	case ResourceType::TEXTURE:
		for (int i = 0; i < textures.size(); i++)
			if (textures[i]->m_name == filePath && textures[i]->m_type == type)
			{
				flag.clear();
				return i;
			}
		
		break;
	case ResourceType::MATERIAL:
		for (int i = 0; i < materials.size(); i++)
			if (materials[i]->m_name == filePath && materials[i]->m_type == type)
			{
				flag.clear();
				return i;
			}

		break;
	default:
		return CUT_RENDERING_CODE;
	}
	flag.clear();
	//AddToLoad(filePath, type);

	return CUT_RENDERING_CODE;
}


void ResourcesManager::BindResource(int index, const ResourceType type, bool bindDefault)
{
	if (index < 0) return;

	switch (type)
	{
	case ResourceType::MESH:
		meshes[index]->BindResource();
		break;
	case ResourceType::SHADER:
		shaders[index]->BindResource();
		break;
	case ResourceType::TEXTURE:
		textures[index]->BindResource(bindDefault);
		break;
	default:
		break;
	}
}

void ResourcesManager::CheckResourcesState(const ResourceType type)
{
	switch (type)
	{
	case ResourceType::MESH:
		for (std::shared_ptr<Mesh> mesh : meshes)
			if (!mesh->glReady) mesh->GenerateOpenGL();

		break;
	case ResourceType::TEXTURE:
		for (std::shared_ptr<Texture> texture : textures)
			if (!texture->glReady) texture->GenerateOpenGL();
		
		break;
	default:
		break;
	}
}
#pragma endregion

#pragma region GETTERS

int ResourcesManager::GetShaderProgram(const std::string& name)
{
	int index = SearchResource(name, ResourceType::SHADER);

	if (index < 0) 
		return 0;

	return shaders[index]->GetData();
}

int ResourcesManager::GetBufferSize(const std::string& name)
{
	int index = SearchResource(name, ResourceType::MESH);
	
	if (index < 0) 
		return 0;

	return (int)(meshes[index]->vertices.size());
}
#pragma endregion
