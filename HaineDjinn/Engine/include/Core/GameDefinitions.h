#pragma once
#include <string>
#include <deque>
#include "TinyXML/tinyxml2.h"

#include "../../../Editor/include/EditorWindow/MapManagerWindow.h"

using namespace tinyxml2;

class ModelLoader;
class ResourcesManager;
struct Camera;

class GameDefinitions
{
private:
	//XMLElement* pElement;
	//XMLError eResult;

	int nbCubeCollider = 0;
	int nbSphereCollider = 0;
	int nbCapsuleCollider = 0;

	std::string typeComponent;

	//float ToFloat(const char* value);
public:
	GameDefinitions() = default;
	int index = 0;
	XMLError CreateMap(MapFile& map);
	void FillDataObject(XMLElement* pListElement, XMLElement* pListObjects, const std::deque<GameObject*>& objects, XMLDocument* xmlDoc);
	void FillDataCamera(XMLElement* pListElement, XMLElement* pListCameras, const std::deque<Camera*>& cameras, XMLDocument* xmlDoc);
	void ExtracInfoObjects(ResourcesManager* resourceManager, XMLElement* pListElement, std::deque<GameObject*>& objects, ModelLoader* modelLoader);
	void ExtracInfoCamera(ResourcesManager* resourceManager, XMLElement* pListElement, std::deque<Camera*>& cameras);
	XMLError Save(std::deque<GameObject*> preFabs, Scene* currentSceneDisplay);
	XMLError Load(ResourcesManager* resourceManager, std::deque<GameObject*>& preFabs, Scene* currentSceneDisplay, ModelLoader* modelLoader);
};