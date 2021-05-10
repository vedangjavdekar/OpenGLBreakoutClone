#include "Shader.h"
#include "renderer/Renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>

Shader::Shader()
	:m_FilePath(""), m_RendererID(0)
{
}

Shader::~Shader()
{
	
}

Shader& Shader::Generate(const std::string& filepath)
{
	ShaderProgramSources shaderSources = ParseShader(filepath);
	m_RendererID = CreateShader(shaderSources.vertexSource, shaderSources.fragmentSource);
	return *this;
}

void Shader::Bind() const
{
	//std::cout << "Shader ID: " << m_RendererID << std::endl;
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

ShaderProgramSources Shader::ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType {
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	ShaderType currentShaderType = ShaderType::NONE;
	std::stringstream ss[2];
	std::string line;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				currentShaderType = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				currentShaderType = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)currentShaderType] << line << '\n';
		}
	}
	return { ss[0].str(),ss[1].str() };
}


unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderSource)
{
	unsigned int id = glCreateShader(shaderType);
	const char* src = shaderSource.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int len;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
		char* message = (char*)_malloca(len * sizeof(char));
		GLCall(glGetShaderInfoLog(id, len, &len, message));

		std::cout << (shaderType == GL_VERTEX_SHADER ? "Vertex " : "Fragment ")
			<< "Shader Compilation Failed: \n"
			<< message
			<< std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vertexID = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fragmentID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vertexID));
	GLCall(glAttachShader(program, fragmentID));

	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vertexID));
	GLCall(glDeleteShader(fragmentID));

	return program;
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
	Bind();
	GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	Bind();
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));
}


void Shader::SetUniform3f(const std::string& name, float x, float y, float z)
{
	Bind();
	GLCall(glUniform3f(GetUniformLocation(name), x, y, z));
}

void Shader::SetUniform2f(const std::string& name, float x, float y)
{
	Bind();
	GLCall(glUniform2f(GetUniformLocation(name), x, y));
}

void Shader::SetUniform1f(const std::string& name, float x)
{
	Bind();
	GLCall(glUniform1f(GetUniformLocation(name), x));
}

void Shader::SetUniform1i(const std::string& name, int x)
{
	Bind();
	GLCall(glUniform1i(GetUniformLocation(name), x));
}

void Shader::RemoveShader()
{
	GLCall(glDeleteProgram(m_RendererID));
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocations.find(name) != m_UniformLocations.end())
	{
		return m_UniformLocations[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform '" << name << "' doens't exist." << std::endl;
	
	m_UniformLocations[name] = location;
	return location;
}
