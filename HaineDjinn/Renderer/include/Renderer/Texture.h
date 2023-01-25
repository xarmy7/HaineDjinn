#pragma once

#include <glad/gl.h>
#include <string>

#include "Renderer/Resource.h"

typedef int GLint;

struct GPUTexture
{
	unsigned int texture = 0;
};

class Texture
{
public:
	Texture(const std::string& filePath, const bool toFlip = false);
	Texture() = default;
	~Texture();
	 
	void ParseTexture();
	void BindResource(bool drawDefault = false);
	void GenerateOpenGL(GLint wrapping = GL_REPEAT, GLint filtering = GL_LINEAR);

	std::string m_name;
	static const ResourceType m_type = ResourceType::TEXTURE;
	//std::string m_textureType = "";

	GPUTexture* gpu = nullptr;


	bool isParsed = false;
	bool glReady = false;
	bool dataFreed = false;
	bool flip = false;
	bool compressed = true;

	Maths::int3 texValues{};
	void* data;
};
