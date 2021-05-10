#pragma once
#include <string>
#include <unordered_map>
#include "glm/glm.hpp"

#include "IGeneratableAsset.h"

struct ShaderProgramSources {
	std::string vertexSource;
	std::string fragmentSource;
};


class Shader : public GeneratableAsset<Shader>
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	//Caching for uniforms
	std::unordered_map<std::string, int> m_UniformLocations;
public:
	Shader();
	~Shader();
	
	inline unsigned int GetShaderID() const 
	{
		return m_RendererID;
	}

	Shader& Generate(const std::string& filepath) override;
	void RemoveShader();

	void Bind() const;
	void Unbind() const;

	//Set uniforms 
	void SetUniform1f(const std::string& name, float x);
	void SetUniform2f(const std::string& name, float x, float y);
	void SetUniform3f(const std::string& name, float x, float y, float z);
	void SetUniform4f(const std::string& name, float x, float y, float z, float w);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
	void SetUniform1i(const std::string& name, int x);
private:
	ShaderProgramSources ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSource);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};

