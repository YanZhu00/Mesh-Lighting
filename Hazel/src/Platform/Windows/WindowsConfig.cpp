#include "hzpch.h"
#include "Hazel/Utils/Config.h"

#include "wtypes.h"

namespace Hazel {

	Config::Config()
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();
		GetWindowRect(hDesktop, &desktop);
		unsigned int horizontal = desktop.right;
		unsigned int vertical = desktop.bottom;

		if (horizontal > 1920)
		{
			m_Is4k = true;
			m_ImGuiFile = "imgui4k.ini";
		}
		else
		{
			m_Is4k = false;
			m_ImGuiFile = "imgui.ini";
		}

		std::ifstream imFile(m_ImGuiFile);
		std::string source;
		for (int i = 0; i < 2; ++i) {
			std::getline(imFile, source);
		}
		std::getline(imFile, source);
		size_t pos1 = source.find_first_of("=", 0);
		size_t pos2 = source.find_first_of(",", 0);
		std::string s_width = source.substr(pos1+1, pos2-pos1-1);
		std::string s_height = source.substr(pos2+1);
		m_x = stoi(s_width);
		m_y = stoi(s_height);

	}

}