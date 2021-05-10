#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#include "vendor/stb_image/stb_image.h"


// Instantiate static variables
std::map<std::string, Texture> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;


Shader ResourceManager::LoadShader(std::string name, const std::string& shaderSource)
{
	Shaders[name] = LoadAssetFromFile<Shader>(shaderSource);
	return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

Texture ResourceManager::LoadTexture(std::string name, const std::string& filepath)
{
	Textures[name] = LoadAssetFromFile<Texture>(filepath);
	return Textures[name];
}

Texture& ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}

void ResourceManager::Clear()
{
	for (auto& shader : Shaders)
	{
		shader.second.RemoveShader();
	}
	for (auto& texture : Textures)
	{
		texture.second.RemoveTexture();
	}

	Shaders.clear();
	Textures.clear();
}
