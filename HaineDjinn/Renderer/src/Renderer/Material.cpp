
#include "Renderer/Material.h"

#include <stdlib.h>

Material::Material()
{
}

Material::Material(const std::string name, const std::string dTexturePath, const std::string nTexturePath)
{
	m_name = name;
	SetDiffuseTexture(std::make_shared<Texture>(dTexturePath));
	SetNormalTexture(std::make_shared<Texture>(nTexturePath));
	gpu = new GPUMaterial;
}

Material::~Material()
{
	if (gpu != nullptr) 
		delete gpu;
}

Material::Material(const Material& copy)
{
	*this = copy;
	//this->operator=(copy);
}

Material& Material::operator=(const Material& other)
{
	this->m_name = other.m_name;
	this->ambientColor = other.ambientColor;
	this->diffuseColor = other.diffuseColor;
	this->diffuseTexture = other.diffuseTexture;
	this->hasDiffuse = other.hasDiffuse;
	this->hasNormal = other.hasNormal;
	this->normalTexture = other.normalTexture;
	this->gpu = new GPUMaterial;
	return *this;
}

void Material::BindResource()
{
	if (diffuseTexture == nullptr)
		glBindTexture(GL_TEXTURE_2D, 0);

	if (hasDiffuse)
		diffuseTexture->BindResource();
	else
		diffuseTexture->BindResource(true); // Bind OpenGL default texture
}
