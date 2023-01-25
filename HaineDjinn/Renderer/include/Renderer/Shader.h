#pragma once

#include <string>
#include "Renderer/Resource.h"

struct Shader
{
public:
	Shader() = default;
	Shader(const std::string& filePath);
	~Shader();

	void BindResource();
	int GetData();

	std::string m_name = "";
	ResourceType m_type = ResourceType::SHADER;
	bool glReady = false;
	int shaderProgram = 0;

	GLint modelMatLoc = 0;
};
