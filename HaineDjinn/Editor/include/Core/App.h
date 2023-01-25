#pragma once

#include "Core/Maths.h"
#include "Core/Scene.h"
#include "Renderer/ResourcesManager.h"
#include "Renderer/Renderer.h"
#include "Core/Editor.h"
#include "Renderer/ModelLoader.h"

struct GLFWwindow;
struct GLFWimage;
namespace Assimp
{
	class Importer;
}

GLFWwindow* initOpenGL();

class App
{
public:
	App(GLFWwindow* inWindow);
	~App();

	//void processInput();

	void run();
	void processInput();

	float UpdateDeltaTime(float oldTime);
	static void InitImGuiAndDock();
private:
	reactphysics3d::PhysicsCommon physicsCommon;

	Scene scene = {};
	GLFWwindow* window = {};
	ResourcesManager resourcesManager = {};

	Renderer* renderer = {};
	Maths::Vector2 mousePos{};
	float time = 0.f;
	float deltaTime = 0.f;
	int currScene = 0;
	bool isCursorHidden = true;

	Editor editorClass = {};
	PlayWindow playClass = {};

	GLFWimage* appIcon = {};
	ModelLoader modelLoader = {};
	Assimp::Importer importer = {};
};
