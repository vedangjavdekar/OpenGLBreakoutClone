#pragma once
#include <string>
#include <map>

#include "shaders/Shader.h"
#include "textures/Texture.h"

class ResourceManager
{
public:
	static std::map<std::string, Shader>    Shaders;
	static std::map<std::string, Texture> Textures;
	static Shader LoadShader(std::string name, const std::string& shaderSource);
	static Shader& GetShader(std::string name);
	static Texture LoadTexture(std::string name, const std::string& filepath);
	static Texture& GetTexture(std::string name);
	static void Clear();
private:
	ResourceManager() { }
	template<typename T>
	static T LoadAssetFromFile(const std::string& filepath)
	{
		T asset;
		asset.Generate(filepath);
		return asset;
	}

};

