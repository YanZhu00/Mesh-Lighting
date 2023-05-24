#pragma once

#include "hzpch.h"
#include "Hazel/Renderer/VertexBufferLayout.h"
#include "Hazel/Renderer/Renderer.h"
#include "glad/glad.h"

namespace Hazel {

	unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:			return 4;
		case GL_UNSIGNED_INT:	return 4;
		case GL_UNSIGNED_BYTE:  return 1;
		}
		ASSERT(false);
		return 0;
	}

	void VertexBufferLayout::push_float(unsigned int count)
	{
		m_Element.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	void VertexBufferLayout::push_int(unsigned int count)
	{
		m_Element.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	void VertexBufferLayout::push_char(unsigned int count)
	{
		m_Element.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

}