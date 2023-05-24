#pragma once

#include "hzpch.h"

namespace Hazel {

	class Config
	{
	public:
		Config();
		std::string m_ImGuiFile;
		unsigned int xWin() { return m_x; }
		unsigned int yWin() { return m_y; }
		bool Is4k() { return m_Is4k; }
	private:
		unsigned int m_x;
		unsigned int m_y;
		bool m_Is4k;
	};

}