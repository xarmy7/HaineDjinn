#pragma once

#include <vector>
#include <deque>
#include <unordered_map>

#include "Renderer/ModelInstance.h"
#include "Renderer/Camera.h"
#include "Renderer/Light.h"
#include "Core/Time.h"
#include "Physics/PhysicsEventListener.h"

#include "Core/GameDefinitions.h"

namespace reactphysics3d
{
	class PhysicsWorld;
	class PhysicsCommon;
}

class GameObject;
class Renderer;

class Scene
{
public:
	Scene();
	Scene(Renderer* inRenderer);
	~Scene();

	void UpdateAndRender(bool isPlaying);
	void CreateScene();

	GameObject& Instantiate(const std::string& gameObjectName);

	void SetRenderer(Renderer* inRenderer) { renderer = inRenderer; }
	void SetPhysics(reactphysics3d::PhysicsCommon* physCommon);

	Renderer* renderer = nullptr;
	//std::vector<ModelInstance> objects;

	std::deque<Model> models;
	std::deque<Light> lights;
	Light* currentLight;
	std::deque<GameObject*> gameObjects;
	GameObject* currentObjectDisplay;
	std::deque<Camera*> cameras;
	Camera* currentCamera;
	std::deque<GameObject>* prefabs;

	std::string sceneName = "";

	GameDefinitions gDef = {};

	size_t tempSize = 0;


	static Vector2 vpSize;
	bool anObjectIsSelected;
	bool aLightIsSelected;
	bool aCameraIsSelected;

	static reactphysics3d::PhysicsWorld* GetPhysicsWorld() { return physicsWorld; }
	static reactphysics3d::PhysicsCommon* GetPhysicsCommon() { return physicsCommon; }

private:
	Vector2 oldVpSize = {};
	static reactphysics3d::PhysicsWorld* physicsWorld;
	static reactphysics3d::PhysicsCommon* physicsCommon;

	PhysicsEventListener physEventsListener = {};

	Timer fixedUpdateTimer = {};

};
