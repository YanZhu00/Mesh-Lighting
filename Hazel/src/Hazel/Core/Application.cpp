#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Log.h"

//#include <glad/glad.h>

#include "Input.h"
#include "Hazel/Utils/PlatformUtils.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application() 
	{
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;
		WindowProps windowprop("Mesh&Lighting", s_Config.xWin(), s_Config.yWin());
		m_Window = std::unique_ptr<Window>(Window::Create(windowprop));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			//std::cout << (*--it)->GetName() << std::endl;
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
		//HZ_CORE_TRACE("{0}", e);
	}

	void Application::UpdateFrame()
	{
	
		float time = Time::GetTime();
		float timestep = time - m_LastFrameTime;
		m_LastFrameTime = time;
		m_Window->Begin();

		if (!m_Minimized)
		{
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			{
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
			}
			m_ImGuiLayer->End();
		}

		m_Window->End();
	}

	void Application::Run()
	{
		while (m_Running)
		{
			UpdateFrame();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		UpdateFrame();
		return true;
	}

}