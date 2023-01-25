#pragma once

#include <string>
#include <memory>
#include <vector>

#include "Renderer/Texture.h"

struct GPUMaterial
{
	// TODO: Add GPU material (UBO?)
};

class Material
{
public:
	Material();
	Material(const std::string name, const std::string dTexturePath, const std::string nTexturePath);
	~Material();

	Material(const Material& copy);
	Material& operator=(const Material& other);

	void BindResource();

	void SetDiffuseTexture(std::shared_ptr<Texture> texture) { hasDiffuse = true; diffuseTexture = texture; };
	void SetNormalTexture(std::shared_ptr<Texture> texture) { hasNormal = true; normalTexture = texture; };

	Vector4 ambientColor = { 1.f, 1.f, 1.f, 1.f };
	Vector4 diffuseColor = { 1.f, 1.f, 1.f, 1.f };
	Vector4 specularColor = { 1.f, 1.f, 1.f, 1.f };

	//std::vector<Texture> theTexture;


	//std::vector<Texture> textures_loaded; //stores all the textures, optimization textures aren't loaded more than once.

	bool hasDiffuse = false;
	bool hasNormal = false;

	GPUMaterial* gpu = nullptr;

	std::string m_name;
	static const ResourceType m_type = ResourceType::MATERIAL;

private:
	std::shared_ptr<Texture> diffuseTexture = {};
	std::shared_ptr<Texture> normalTexture = {};

};