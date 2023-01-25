
#include <stb_image.h>
#include <glad/gl.h>


#include <IMGUI/imgui.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>
#include <IMGUI/imfilebrowser.h>

#include <reactphysics3d/reactphysics3d.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "EditorWindow/SceneObjWindow.h"
#include "Renderer/ResourcesManager.h"
#include "Core/Constants.h"
#include "Core/Maths.h"
#include "Core/App.h"
#include "Core/Debug.h"
#include "Core/Time.h"
#include "Game/InputSystem.h"
#include "Game/SoundSystem.h"
#include "Core/Editor.h"

#include "CameraFollow.h"
#include "Enemy.h"

#pragma warning (disable: 4996) 

#pragma region INITIALIZATION
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, (GLsizei)Scene::vpSize.x, (GLsizei)Scene::vpSize.y);
}

void OpenGLErrorCallback(GLenum Source, GLenum Type, GLuint Id, GLenum Severity, GLsizei Length, const GLchar* Message, const void* UserParam)
{
	if (Id == 1234) // TODO: Remove magic number
		return; // Wireframe::flush
	fprintf(stderr, "OpenGL log (0x%x): %s\n", Id, Message);
}

GLFWwindow* initOpenGL()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "HaineDjinn", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return nullptr;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	if (gladLoaderLoadGL() == 0)
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return nullptr;
	}

	glDebugMessageCallback(OpenGLErrorCallback, nullptr);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
	glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);

	return window;
}

App::App(GLFWwindow* inWindow)
{
	Debug::InitDebugging();
	InputSystem::SaveInputs("Inputs/testLoad.inputs");
	InputSystem::LoadInputs("Inputs/freeCam.inputs");
	modelLoader.SetResourcesManager(&resourcesManager);
	modelLoader.SetImporter(&importer);
	SceneObjManager::SetLoader(&modelLoader);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	if (inWindow == nullptr) return;

	window = inWindow;

	InputSystem::SetWindow(inWindow);

	renderer = new Renderer();

	scene.SetRenderer(renderer);
	scene.SetPhysics(&physicsCommon);
	scene.CreateScene();

	// Load & set window icon
	Texture appIconTex("HaineDjinn.png", false);
	appIconTex.ParseTexture();

	appIcon = new GLFWimage();
	appIcon->width = appIconTex.texValues.e[0];
	appIcon->height = appIconTex.texValues.e[1];
	appIcon->pixels = static_cast<unsigned char*>(appIconTex.data);
	glfwSetWindowIcon(window, 1, appIcon);

	currScene = 0;

	//SetImGui();je fais model.
	editorClass.SetImgui(window);
	editorClass.SetModelLoader(&modelLoader);
	editorClass.resourcesManager = &resourcesManager;
	

	time = (float)glfwGetTime();

	double mPosX, mPosY;
	glfwGetCursorPos(window, &mPosX, &mPosY);
	mousePos = { (float)mPosX, (float)mPosY };
}
#pragma endregion

void App::run()
{
	HD_LOG("Entering run function", LogSeverity::LOG_WARNING);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, (GLsizei)Scene::vpSize.x, (GLsizei)Scene::vpSize.y);
	
	CameraFollow camFollow = {};
	MoveEnemy moveEnemy = {};
	
	SoundSystem::Init();

	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Time::Update();
		InputSystem::UpdateInputSys();

		deltaTime = UpdateDeltaTime(time);
		processInput();
		
		//RunImGui();

		resourcesManager.CleanQueue();
		
		//scenes[currScene].update(deltaTime, window, mousePos, currScene);
		//scenes[currScene].render();

		scene.UpdateAndRender(editorClass.isPlaying);

		if (!Editor::isPlaying)
		{
			editorClass.SetScene(scene);
			editorClass.RunEditor();
		}
		else
		{
			playClass.SetPlayerScene(scene, *window, camFollow);
			playClass.RunPlayerWindow(&resourcesManager, editorClass.currentSceneDisplay, &modelLoader, moveEnemy);

			if (scene.gameObjects.size() != 0 && editorClass.currentSceneDisplay->sceneName == "MapSceneMinimal" && Editor::isPlaying)
			{
				camFollow.SetPlayer(scene.gameObjects[316]);
				camFollow.Update(scene.gameObjects[316], deltaTime);
				moveEnemy.SetEnemiesScene(scene);
				moveEnemy.Update(deltaTime, scene.gameObjects[316], scene);

			}
		}

		editorClass.RenderImGui();

		glfwSwapBuffers(window);
	}

	HD_LOG("Exiting run function", LogSeverity::LOG_WARNING);
}

void App::App::processInput()
{
	/*if (InputSystem::GetInputDown("Spell_1"))
	{
		if (isCursorHidden)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		isCursorHidden = !isCursorHidden;
	}*/
}

App::~App()
{
	modelLoader.Clear();
	delete renderer;
	//glfwDestroyWindow(window);
	Debug::EndDebugging();
	glfwTerminate();

	importer.FreeScene();
	delete appIcon->pixels;
	delete appIcon;
	//delete renderer;
}

float App::UpdateDeltaTime(float oldTime)
{
	time = (float)glfwGetTime();
	if (time - oldTime < 1.f / 15.f)
		return (time - oldTime);
	else
		return 1.f / 60.f;
}

void App::InitImGuiAndDock()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool dockOpen = true;
	static bool opt_fullscreen = true;
	static bool opt_padding = false;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else
	{
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	if (!opt_padding)
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &dockOpen, window_flags);
	if (!opt_padding)
		ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Submit the DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	}
}
