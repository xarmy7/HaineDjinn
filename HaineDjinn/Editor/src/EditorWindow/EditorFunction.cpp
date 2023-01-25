#include <IMGUI/imgui.h>

#include "Editorwindow/EditorFunction.h"

#pragma warning (disable: 4996) 


void EditFileName(std::string& objName, char* buf, int bufSize)
{
	ImGui::InputText("Name", buf, bufSize);
	if (buf[0] != '\0')
	{
		if (ImGui::Button("validate"))
			objName = buf;
	}
}