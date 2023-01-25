
#include <glad/gl.h>

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Core/Debug.h"
#include "Renderer/Resource.h"
#include "Renderer/Shader.h"

unsigned int LoadShader(const std::string& filePath)
{
	std::string fileStr(filePath);
	HD_LOG("Started parsing shader: " + fileStr, LogSeverity::LOG_MESSAGE);

	std::string fileFrag = filePath;
	fileFrag += ".frag";
	std::string fileVert = filePath;
	fileVert += ".vert";
	std::stringstream fragBuffer;
	std::stringstream vertBuffer;
	std::fstream file1;
	std::fstream file2;

	file1.open(fileFrag);
	fragBuffer << file1.rdbuf();
	std::string str = fragBuffer.str();
	const char* fragmentShaderSource = str.c_str();

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	file2.open(fileVert);
	vertBuffer << file2.rdbuf();
	str = vertBuffer.str();
	const char* vertexShaderSource = str.c_str();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	unsigned int shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	GLint success;
	GLchar infoLog[512];

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::string errorStr("VERTEX SHADER " + filePath + " COMPILATION_FAILED: ");
		errorStr = errorStr + infoLog;
		HD_LOG(errorStr, LogSeverity::LOG_ERROR);
	}

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::string errorStr("FRAGMENT SHADER " + filePath + " COMPILATION_FAILED: ");
		errorStr = errorStr + infoLog;
		HD_LOG(errorStr, LogSeverity::LOG_ERROR);
	}

	file1.close();
	file2.close();

	return shaderProgram;
}

Shader::Shader(const std::string& filePath)
{
	shaderProgram = LoadShader(filePath);
	glReady = true;
	m_name = filePath;
	modelMatLoc = glGetUniformLocation(shaderProgram, "uModelMat");

	std::string fileStr(filePath);
	HD_LOG("Finished parsing shader: " + fileStr, LogSeverity::LOG_MESSAGE);
}

Shader::~Shader()
{
	glDeleteProgram(shaderProgram);
}

void Shader::BindResource()
{
	glUseProgram(shaderProgram);
}

int Shader::GetData()
{
	return shaderProgram;
}