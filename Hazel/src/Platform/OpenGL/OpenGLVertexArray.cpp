#include "hzpch.h"

#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/VertexBufferLayout.h"
#include "Hazel/Renderer/Renderer.h"
#include <glad/glad.h>

namespace Hazel {

	VertexArray::VertexArray()
	{
		GLCall(glGenVertexArrays(1, &m_RendererID));
		n_layout = 0;
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererID));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();
		const auto& elements = layout.GetElement();
		unsigned int offset = 0;
		for (unsigned int i = 0; i < elements.size(); i++)
		{
			const auto& element = elements[i];
			GLCall(glEnableVertexAttribArray(n_layout));
			GLCall(glVertexAttribPointer(n_layout, element.count, element.type,
				element.normalized, layout.GerStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
			n_layout++;
		}
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererID));
	}

	void VertexArray::Unbind() const
	{
		GLCall(glBindVertexArray(0));
	}

}