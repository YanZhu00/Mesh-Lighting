#pragma once

#include "hzpch.h"

namespace Hazel {

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};

	class Shader
	{
	private:
		std::string m_FilePath;

		// caching for uniforms
		std::unordered_map<std::string, int> m_UniformLocationCache;
	public:
		unsigned int m_RendererID;
		Shader(const std::string& filepath);
		~Shader();

		void Bind() const;
		void Unbind() const;

		// Set uniforms
		void SetUniform1i(const std::string& name, int value);
		void SetUniform3f(const std::string& name, float v0, float v1, float v2);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
		void SetUniformMatrix4fv(const std::string& name, float* value);

	private:
		ShaderProgramSource ParseShader(const std::string& filepath);
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(unsigned int type, const std::string& source);
		unsigned int GetUniformLocation(const std::string& name);
	};

}