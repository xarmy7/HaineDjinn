#include "EditorWindow/AssetManagerwindow.h"
#include "Core/Editor.h"

namespace AssetsManager
{
	void Init() {}
	void Update()
	{
		ImGui::Begin("Asset");
		//for (const auto& entry : std::filesystem::directory_iterator(Editor::pathAssets))
		//	ImGui::Text(entry.path().string().c_str());
		ImGui::End();
	}
}