#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Hazel {

	class VertexBufferLayout;

	class VertexArray
	{
	private:
		unsigned int m_RendererID;
		unsigned int n_layout;
	public:
		VertexArray();
		~VertexArray();

		void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

		void Bind() const;
		void Unbind() const;
	};

}