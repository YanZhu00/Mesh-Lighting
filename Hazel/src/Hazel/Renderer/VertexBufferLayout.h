#pragma once

namespace Hazel {

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;

		static unsigned int GetSizeOfType(unsigned int type);
	};

	class VertexBufferLayout
	{
	private:
		std::vector<VertexBufferElement> m_Element;
		unsigned int m_Stride;
	public:
		VertexBufferLayout()
			: m_Stride(0) {};

		template<typename T>
		void Push(unsigned int count)
		{
			static_assert(false);
		}

		template<>
		void Push<float>(unsigned int count)
		{
			push_float(count);
		}

		template<>
		void Push<unsigned int>(unsigned int count)
		{
			push_int(count);
		}

		template<>
		void Push<unsigned char>(unsigned int count)
		{
			push_char(count);
		}

		inline const std::vector<VertexBufferElement> GetElement() const { return m_Element; }
		inline unsigned int GerStride() const { return m_Stride; }

		void push_float(unsigned int count);
		void push_int(unsigned int count);
		void push_char(unsigned int count);
	};

}