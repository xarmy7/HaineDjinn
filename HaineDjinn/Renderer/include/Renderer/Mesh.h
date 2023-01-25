#pragma once

#include <string>
#include <vector>

#include "Core/Maths.h"
#include "Renderer/Resource.h"
#include "Renderer/Texture.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> // needs to be defined in every file that needs to be tracked

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	Vector3 position;
	Vector3 normal;
	Vector2 uv;

	int boneID[MAX_BONE_INFLUENCE];
	float weight[MAX_BONE_INFLUENCE]; //Weight from each bone
};

struct GPUMesh
{
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};

class Mesh
{
private:
public:
	Mesh() {};
	Mesh(const std::string& filePath) { m_name = filePath; };
	Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
	//Mesh(std::string filePath, float scale);
	~Mesh();

	void GenerateOpenGL();
	void BindResource();

	ResourceType m_type = ResourceType::MESH;

	bool glReady = false;
	bool gamma = false;

	GPUMesh* gpu = nullptr;

	std::string m_name = "blank.obj";
	std::string directory = "";

	std::vector<Vertex>				vertices;
	std::vector<unsigned int>		indices;
};
