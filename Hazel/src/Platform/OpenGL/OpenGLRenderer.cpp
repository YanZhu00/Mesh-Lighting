#include "hzpch.h"
#include <glad/glad.h>
#include "Hazel/Renderer/Renderer.h"

namespace Hazel {

	void ClearError()
	{
		while (glGetError() != GL_NO_ERROR);
	}

	bool LogCall(const char* function, const char* file, int line)
	{
		while (GLenum error = glGetError())
		{
			std::cout << "[OpenGL Error] (" << error << ")" << function <<
				" " << file << ":" << line << std::endl;
			return false;
		}
		return true;
	}


	void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
	{
		shader.Bind();
		va.Bind();
		ib.Bind();
		GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

	void Renderer::DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, float width)
	{
		shader.Bind();
		va.Bind();
		ib.Bind();
		glLineWidth(width);
		GLCall(glDrawElements(GL_LINES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
	}

}