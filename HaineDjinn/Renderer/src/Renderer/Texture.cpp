
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_DXT_IMPLEMENTATION
#include <stb_dxt.h>
#include <glad/gl.h>

#include <string>
#include <iostream>

#include "Core/Debug.h"
#include "Renderer/Resource.h"
#include "Renderer/Texture.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked

void Texture::GenerateOpenGL(GLint wrapping, GLint filtering)
{
	HD_LOG("Started initiating GPU texture: " + m_name, LogSeverity::LOG_MESSAGE);

	if (!isParsed)
	{
		HD_LOG("Texture isn't parsed yet", LogSeverity::LOG_WARNING);
		ParseTexture();
	}

	if (data == nullptr)
	{
		HD_LOG("No texture data, trying to load using texture name", LogSeverity::LOG_WARNING);

		if (m_name == "")
		{
			HD_LOG("No texture name, texture is now OpenGL base texture", LogSeverity::LOG_ERROR);
			glReady = true;
			gpu->texture = 0;
			dataFreed = true;
			return;
		}

		data = stbi_load(m_name.c_str(), &texValues.e[0], &texValues.e[1], &texValues.e[2], 0);
		if (data == nullptr)
		{
			HD_LOG("Couldn't parse texture " + m_name + " after final failsafe, binding to OGL default", LogSeverity::LOG_ERROR);
			glReady = true;
			gpu->texture = 0;
			dataFreed = true;
			return;
		}

		HD_LOG("Texture " + m_name + " successfully loaded.", LogSeverity::LOG_MESSAGE);
		dataFreed = false;
	}

	glGenTextures(1, &gpu->texture);
	glBindTexture(GL_TEXTURE_2D, gpu->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);

	GLint GLImageFormat[] =
	{
		-1, // 0 Channels, unused
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA
	};

	if (data != nullptr)
	{
		unsigned char* test = static_cast<unsigned char*>(data);
		std::string textAsStr = (char*)test;
		glTexImage2D(GL_TEXTURE_2D, 0, GLImageFormat[texValues.e[2]], texValues.e[0], texValues.e[1], 0, GLImageFormat[texValues.e[2]], GL_UNSIGNED_BYTE, data);
		//glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, texValues.e[0], texValues.e[1], 0, sizeof(unsigned char) * texValues.e[0] * texValues.e[1], data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glReady = true;
	}
	else
		HD_LOG("Couldn't init texture: " + m_name, LogSeverity::LOG_ERROR);

	stbi_image_free(data);
	dataFreed = true;
}

Texture::Texture(const std::string& filePath, const bool toFlip)
{
	HD_LOG("Texture " + filePath + " Constructor", LogSeverity::LOG_MESSAGE);
	m_name = filePath;
	flip = toFlip;
}

Texture::~Texture()
{
	if (glReady)
	{
		glDeleteTextures(1, &gpu->texture);
		glReady = false;
	}

	if (dataFreed == false)
	{
		//stbi_image_free(data);
		dataFreed = true;
	}

	// TODO: Safe delete
	if (gpu != nullptr)
		delete gpu;
}

void Texture::ParseTexture()
{
	HD_LOG("Started parsing texture: " + m_name, LogSeverity::LOG_MESSAGE);

	gpu = new GPUTexture();


	stbi_set_flip_vertically_on_load(false);
	data = stbi_load(m_name.c_str(), &texValues.e[0], &texValues.e[1], &texValues.e[2], 0);

	if (data == nullptr)
	{
		HD_LOG("Couldn't parse texture " + m_name, LogSeverity::LOG_ERROR);
		dataFreed = true;
		return;
	}

	isParsed = true;
	
	int i = 0;
	//for (int i = 0; i < texValues.e[0] * texValues.e[1]; i++)
		//stb_compress_dxt_block((unsigned char*)(data) + i * 16, (unsigned char*)(data) + i * 64, 1, 1);

	HD_LOG("Finished parsing texture: " + m_name, LogSeverity::LOG_MESSAGE);
}

void Texture::BindResource(bool drawDefault)
{
	if (drawDefault)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
	{
		if (!glReady) 
			GenerateOpenGL();
		
		glBindTexture(GL_TEXTURE_2D, gpu->texture);
	}
}
