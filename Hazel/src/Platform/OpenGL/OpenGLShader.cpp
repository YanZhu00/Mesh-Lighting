#include "hzpch.h"
#include "Hazel/Renderer/Shader.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	Shader::Shader(const std::string& filepath)
		:m_FilePath(filepath), m_RendererID(0)
	{
		ShaderProgramSource source = ParseShader(filepath);
		m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
		Bind();
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_RendererID));
	}


	ShaderProgramSource Shader::ParseShader(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		if (!stream)
		{
			std::cout << "shader file open error!" << std::endl;
		}
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1
		};

		std::string line;
		std::stringstream ss[2];
		ShaderType type = ShaderType::NONE;
		//getline(stream, line);
		while (getline(stream, line))
		{
			if (line.find("#shader") != std::string::npos) //  
			{
				if (line.find("vertex") != std::string::npos)
				{
					type = ShaderType::VERTEX;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					type = ShaderType::FRAGMENT;
				}
			}
			else
			{
				ss[(int)type] << line << '\n';
			}
		}

		return { ss[0].str(), ss[1].str() };
	}

	unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		GLCall(unsigned int program = glCreateProgram());
		unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
		unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

		GLCall(glAttachShader(program, vs));
		GLCall(glAttachShader(program, fs));
		GLCall(glLinkProgram(program));
		GLCall(glValidateProgram(program));

		GLCall(glDeleteShader(vs));
		GLCall(glDeleteShader(fs));

		return program;
	}

	unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
	{
		GLCall(unsigned int id = glCreateShader(type));
		const char* src = source.c_str();
		GLCall(glShaderSource(id, 1, &src, nullptr));
		GLCall(glCompileShader(id));

		//Error handling
		int result;
		GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
		if (result == GL_FALSE)
		{
			int length;
			GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
			char* message = (char*)alloca(length * sizeof(char));
			GLCall(glGetShaderInfoLog(id, length, &length, message));
			std::cout << "Failde to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
				<< "shader!" << std::endl;
			std::cout << message << std::endl;
			GLCall(glDeleteShader(id));
			return 0;
		}

		return id;
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_RendererID));
	}

	void Shader::Unbind() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		// call glUseProgram(shader) before passing uniform
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
	{
		// call glUseProgram(shader) before passing uniform
		GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		// call glUseProgram(shader) before passing uniform
		GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
	}
	
	void Shader::SetUniformMatrix4fv(const std::string& name, float* value)
	{
		// call glUseProgram(shader) before passing uniform
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value));
	}

	unsigned int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
			return m_UniformLocationCache[name];
		GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
		if (location == -1) // -1 means it couldn't find uniform
		{
			std::cout << "Warning: uniform '" << name << "' doesn't exist" << std::endl;
			//ASSERT(false);
		}
		m_UniformLocationCache[name] = location;
		return location;
	}

}
