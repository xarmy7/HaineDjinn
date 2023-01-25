#pragma once

#include "Core/Maths.h"
#include "Core/Constants.h"
#include "Renderer/Resource.h"

#include <thread>
#include <atomic>

#include <deque>
#include <memory>
#include <string>
#include <glad/gl.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked

class Material;
class Texture;
class Mesh;
class Animation;
struct Shader;

class ResourcesManager
{
private:
	//std::thread threads[THREADS_NUMBER];

	// Using Deques so the resources' location won't change
	std::deque<Resource*> toLoad;
	std::deque<std::shared_ptr<Material>> materials;
	std::deque<std::shared_ptr<Texture>> textures;
	std::deque<std::shared_ptr<Mesh>> meshes;
	std::deque<std::shared_ptr<Shader>> shaders;

	bool runThreads = false;

	void ThreadFunction(int threadID);
public:
	// flag used to synchronize threads
	std::atomic_flag flag = ATOMIC_FLAG_INIT;
	std::deque<std::shared_ptr<Animation>> animations;
	
	ResourcesManager();
	~ResourcesManager() 
	{
		runThreads = false;

		//while (flag.test_and_set()) { std::this_thread::yield(); }
		
		for (Resource* resource : toLoad)
			delete resource;
		
		//flag.clear();

		//for (int i = 0; i < THREADS_NUMBER; i++)
		//	threads[i].join();
	};

	int SearchResource(const std::string filePath, const ResourceType type);
	void BindResource(int index, const ResourceType type, bool bindDefault = false);
	void CheckResourcesState(const ResourceType type);

	void ResetResources()
	{	
		//while (flag.test_and_set()) { std::this_thread::yield(); }
		
		textures.clear();
		shaders.clear();
		meshes.clear();
		animations.clear();

		//flag.clear();
		EraseQueue();
	}
	void EraseQueue();
	void CleanQueue();

	// Mesh/Texture/Shader overload
	void* AddToLoad(const std::string filePath, const ResourceType type);
	// Material overload
	void* AddToLoad(const std::string name, const std::string diffTexPath, const std::string normTexPath, Vector4 ambientColor = { 1.f, 1.f, 1.f, 1.f }, Vector4 diffuseColor = { 1.f, 1.f, 1.f, 1.f }, Vector4 specularColor = { 1.f, 1.f, 1.f, 1.f });
	
	void LoadResource(const std::string filePath, const ResourceType type);
	void CreateResource(const std::string filePath, const ResourceType type);
	void DestroyResource(const int index, const ResourceType type);

	size_t GetArraySize(ResourceType type) 
	{
		switch (type)
		{
		case ResourceType::MESH:
			return meshes.size();
		case ResourceType::SHADER:
			return shaders.size();
		case ResourceType::TEXTURE:
			return textures.size();
		case ResourceType::MATERIAL:
			return materials.size();
		case ResourceType::ANIMATION:
			return animations.size();
		default:
			return toLoad.size();
		}
	};
	const std::deque<std::shared_ptr<Material>>& GetMaterials() { return materials; };
	const std::deque<std::shared_ptr<Texture>>& GetTextures() { return textures; };

	int GetShaderProgram(const std::string& name);
	int GetBufferSize(const std::string& name);
};
