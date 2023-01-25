#include <glad/gl.h>
#include <stb_image.h>

#include <IMGUI/imgui.h>
#include <IMGUI/imfilebrowser.h>
#include <IMGUI/imgui_impl_glfw.h>
#include <IMGUI/imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

//#include "XLSX/libxl.h"
//#include <windows.h>

#include <iostream>
#include <fstream>
#include <cmath>

#include "Core/Editor.h"
#include "Renderer/ResourcesManager.h"
#include "Renderer/Renderer.h"
#include "Core/Script.h"
#include "Core/App.h"

#include "Core/Transform.h"

#pragma warning (disable: 4996) 

std::deque<GameObject> Editor::preFabs;
std::string Editor::pathAssets;
bool Editor::isPlaying = false;

//using namespace libxl;

void SetDockSpace();


Editor::Editor()
{
	std::string pathUnclear = std::filesystem::current_path().string().c_str();
	for (int i = 0; i < (pathUnclear.size() - 6); i++)
		pathAssets.push_back(pathUnclear[i]);

	pathAssets += "Assets\\";

	std::string test = std::filesystem::relative(pathAssets, "HaineDjinn").string();

	MapManager::LoadMaps(pathAssets);
	//SceneObjManager::objectsName
}

void Editor::SetScene(Scene& scene)
{
	currentSceneDisplay = &scene;
	SceneObjManager::objectsName.clear();
	for (int i = 0; i < currentSceneDisplay->models.size(); i++)
		SceneObjManager::objectsName.push_back(currentSceneDisplay->models[i].modelName);
}


void Editor::SetImgui(GLFWwindow* _window)
{
	window = _window;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//ImGuizmo::BeginFrame();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows


	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");


	ImGuiFileBrowserFlags_CreateNewDir;
}

void Editor::RunEditor()
{

	App::InitImGuiAndDock();


	if (MapManager::aMapIsSelected)
	{
		//Object's Component window
		{
			ObjectCompManager::RunObjectCompManager(pathAssets, resourcesManager , currentSceneDisplay);
		}
	
		//Scene's Objects window
		{
			SceneObjManager::RunSceneObjManager(currentSceneDisplay, resourcesManager);
		}
	}
	
	//Play window
	{
		PlayWindow::Play(currentSceneDisplay);
	}

	//Scene window
	{
		SceneRenderer::RunScene(window, resourcesManager, currentSceneDisplay, modelLoader);
	}


	//Map Manager window
	{
		MapManager::RunMapManager(pathAssets, resourcesManager, currentSceneDisplay, modelLoader);
		if (MapManager::isCreatingAMapFile)
			MapManager::CreateNewMapFile(pathAssets, currentSceneDisplay->gDef);
	}

	//Asset Manager window
	{
		AssetsManager::Update();
	}

	ImGui::End();
}

void Editor::RenderImGui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Editor::ImGuiInit()
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


Editor::~Editor()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}