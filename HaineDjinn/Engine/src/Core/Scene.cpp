
#include "Core/Scene.h"
#include "Core/GameObject.h"
#include "Renderer/Renderer.h"
#include <reactphysics3d/reactphysics3d.h>

Vector2 Scene::vpSize = { 690.f, 523.f };
reactphysics3d::PhysicsWorld* Scene::physicsWorld = nullptr;
reactphysics3d::PhysicsCommon* Scene::physicsCommon = nullptr;

Scene::Scene()
{
	fixedUpdateTimer.hasEnded = true;
	cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
	currentCamera = cameras.back();
	currentCamera->isMainCamera = true;
	currentCamera->name = "TempName";
}

Scene::Scene(Renderer* inRenderer)
{
	renderer = inRenderer;
	fixedUpdateTimer.hasEnded = true;
	cameras.push_back(new Camera(SCR_WIDTH, SCR_HEIGHT));
	currentCamera = cameras.back();
	currentCamera->isMainCamera = true;
	currentCamera->name = "TempName";
}

void Scene::CreateScene()
{
	if (renderer == nullptr)
	{
		std::cout << "No renderer in Scene" << std::endl;
		return;
	}

	std::vector<ModelInstance> staticModels;

	Material mat("testMat", "Models/T_M_MED_Banana_Smooth_Body_D", "Models/T_M_MED_Banana_Smooth_Body_N.png");

	fixedUpdateTimer.Restart((float)(Time::FixedDeltaTime()), true);
	physicsWorld->update((reactphysics3d::decimal)Time::FixedDeltaTime());
}

void Scene::UpdateAndRender(bool isPlaying )
{
	if (vpSize.x != oldVpSize.x || vpSize.y != oldVpSize.y)
		glViewport(0, 0, (GLsizei)vpSize.x, (GLsizei)vpSize.y);
	
	oldVpSize = vpSize;

	fixedUpdateTimer.Update();

	// Fixed update
	if (fixedUpdateTimer.hasEnded)
	{
		// Update physics
		physicsWorld->update((reactphysics3d::decimal)Time::FixedDeltaTime());

		for (int i = 0;  i < gameObjects.size(); i++)
		{
			if (!gameObjects[i]->IsActive())
				continue;

			gameObjects[i]->FixedUpdateComponents();
		}
	}

	for (int i = 0; i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->componentsAwoken)
			gameObjects[i]->AwakeComponents();
	}

	// Update
	for (int i = 0;  i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->IsActive())
			continue;

		gameObjects[i]->UpdateComponents();
	}

	// Late update
	for (int i = 0;  i < gameObjects.size(); i++)
	{
		if (!gameObjects[i]->IsActive())
			continue;

		gameObjects[i]->LateUpdateComponents();
	}

	currentCamera->FreeMove(1.f / 60.f, {});
	currentCamera->aspect = vpSize.x / vpSize.y;

	renderer->Render(currentCamera->GetViewMatrix(), currentCamera->GetProjectionMatrix(), vpSize, gameObjects, lights, isPlaying);
}

GameObject& Scene::Instantiate(const std::string& gameObjectName)
{
	gameObjects.push_back(new GameObject(gameObjectName));

	return *gameObjects.back();
}

void Scene::SetPhysics(reactphysics3d::PhysicsCommon* physCommon)
{
	if (physCommon == nullptr)
		return;

	physicsCommon = physCommon;
	physicsWorld = physCommon->createPhysicsWorld();

	physicsWorld->setEventListener(&physEventsListener);

	// Enable debug rendering 
	physicsWorld->setIsDebugRenderingEnabled(true);

	// Get a reference to the debug renderer 
	reactphysics3d::DebugRenderer& debugRenderer = physicsWorld->getDebugRenderer();

	// Select element
	debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
	debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLIDER_AABB, true);
	debugRenderer.setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);

	renderer->SetPhysicsRenderer(&debugRenderer);
}

Scene::~Scene()
{

	for (int i = 0; i < cameras.size(); i++)
	{
		delete cameras[i];
	}
	cameras.clear();
}
