
#include <glad/gl.h>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <fstream>


#include "Core/Maths.h"
#include "Core/Debug.h"

#include "Renderer/Resource.h"
#include "Renderer/Mesh.h"


#ifndef OFFSETOF
#define OFFSETOF(TYPE, MEMBER) __builtin_offsetof(TYPE, MEMBER)
#endif

#pragma region POST PARSING OPERATIONS
void scalePositions(std::vector<Maths::Vector3>& positions, float scale)
{
	for (int i = 0; i < positions.size(); i++)
		positions[i] = positions[i] * scale;
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
{
    this->vertices = vertices;
    this->indices = indices;

    gpu = new GPUMesh();

    // now that we have all the required data, set the vertex buffer and its attribute pointers.
    GenerateOpenGL();
}

//unsigned int& VAO, unsigned int& VBO, std::vector<float>& buffer
void Mesh::GenerateOpenGL()
{
    //HD_LOG("Started initiating GPU mesh: " + m_name, LogSeverity::LOG_MESSAGE);

    if (gpu == nullptr)
    {
        gpu = new GPUMesh();
    }

	glGenBuffers(1, &gpu->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, gpu->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &gpu->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &gpu->VAO);
	glBindVertexArray(gpu->VAO);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3); //Id
    glEnableVertexAttribArray(4); // Weight from bone

    glBindBuffer(GL_ARRAY_BUFFER, gpu->VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, uv));
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex), (void*)OFFSETOF(Vertex, boneID));
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)OFFSETOF(Vertex, weight));
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu->EBO);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//vertices.clear();
	glReady = true;
}
#pragma endregion

Mesh::~Mesh()
{
	glDeleteBuffers(1, &gpu->VBO);
	glDeleteVertexArrays(1, &gpu->VAO);

    if (gpu != nullptr)
        delete gpu;
}

void Mesh::BindResource()
{
	if (!glReady) GenerateOpenGL();

	glBindVertexArray(gpu->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gpu->EBO);
}
