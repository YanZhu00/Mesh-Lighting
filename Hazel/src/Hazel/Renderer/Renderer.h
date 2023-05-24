#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

namespace Hazel {

#define ASSERT(x) if (!(x)) __debugbreak();
#ifdef DEBUG
#define Call(x) ClearError();\
x;\
ASSERT(LogCall(#x, __FILE__, __LINE__))
#else
#define GLCall(x) x
#endif

	void ClearError();
	bool LogCall(const char* function, const char* file, int line);


	class Renderer
	{
	public:
		static void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader);
		static void DrawLine(const VertexArray& va, const IndexBuffer& ib, const Shader& shader, float width=1);
	};

}