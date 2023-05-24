#include "AppLayer.h"


namespace Hazel {
	AppLayer::AppLayer()
	{
	}

	void AppLayer::OnAttach()
	{
		m_Scene = std::make_shared<Scene>();
		m_ModelingCamera = std::make_shared<ModelingCamera>();
		m_Viewports = std::vector<ViewportData>(3);

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = Framebuffer::Create(fbSpec);

		m_Selected = 0;
	}

	void AppLayer::OnDetach()
	{
	}

	void AppLayer::OnUpdate(float timestep)
	{
		m_ModelingCamera->OnUpdate(timestep, m_Viewports[0].width, m_Viewports[0].height);
		

	}

	void AppLayer::OnImGuiRender()
	{
		
		// Create the docking environment
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
		ImGui::PopStyleVar(3);
		ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
		ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		if (ImGui::BeginMenuBar())
		{
			ImGui::EndMenuBar();
		}

		ImGui::End();


		bool b_GlobalWinDebug = true;
		bool b_WinDebug = true;
		ImGuiWindowClass window_class;

	

		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = 0 | ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("TOOLBAR3");
		ImGui::Dummy(ImVec2(0.0f, 15.0f));

		DrawColorEditor();
		DrawLight();
		
		ImGui::End();
		

		b_WinDebug = true;
		if (b_WinDebug && b_GlobalWinDebug)
		{
			window_class.DockNodeFlagsOverrideSet = 0 | ImGuiDockNodeFlags_NoDockingOverMe | ImGuiDockNodeFlags_NoDockingOverOther | ImGuiDockNodeFlags_NoDockingSplitOther | ImGuiDockNodeFlags_NoTabBar;
			ImGui::SetNextWindowClass(&window_class);
		}
		ImGui::Begin("Main Viewport");
		UpdateViewport(0);
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_Viewports[0].width > 0.0f && m_Viewports[0].height > 0.0f && 
			(spec.Width != m_Viewports[0].width || spec.Height != m_Viewports[0].height))
		{
			m_Framebuffer->Resize((uint32_t)m_Viewports[0].width, (uint32_t)m_Viewports[0].height);
		}
		m_Framebuffer->Bind();
		m_Scene->OnUpdate(m_ModelingCamera->CameraData(), true);
		m_Scene->Draw();
		m_Framebuffer->Unbind();
		int64_t textureID = m_Framebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_Viewports[0].width, m_Viewports[0].height }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		bool m_ViewportFocused = ImGui::IsWindowFocused();
		bool m_ViewportHovered = ImGui::IsWindowHovered();

		ImGui::End();


		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowMinSize.x = 200.0f;
		style.WindowMinSize.y = 200.0f;
	
	}

	void AppLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(AppLayer::OnMouseScrolled));
		dispatcher.Dispatch<DragFileEvent>(HZ_BIND_EVENT_FN(AppLayer::OnFileDraged));
	}

	bool AppLayer::OnFileDraged(Hazel::DragFileEvent& event)
	{
		m_ModelingCamera->Reset();
		
		std::string filename = event.GetFileName();
		m_Scene->m_Object->ReadFile(filename);
		m_Scene->m_Object->ResetColor();

		return false;
	}

	bool AppLayer::OnMouseScrolled(Hazel::MouseScrolledEvent& event)
	{
		float offset = event.GetYOffset();
		auto pos = ImGui::GetMousePos();
		if (pos.x > m_Viewports[0].x1 && pos.x < m_Viewports[0].x2 && pos.y > m_Viewports[0].y1 && pos.y < m_Viewports[0].y2)
		{
			m_ModelingCamera->Scroll(offset);
		}

		return false;
	}

	bool AppLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& event)
	{
		if (event.GetMouseButton() == (int)Hazel::Mouse::ButtonLeft)
		{
			auto pos = ImGui::GetMousePos();
			if (pos.x > m_Viewports[0].x1 && pos.x < m_Viewports[0].x2 && pos.y > m_Viewports[0].y1 && pos.y < m_Viewports[0].y2)
			{
				m_ModelingCamera->SetSelected(true);
			}
		}

		return false;
	}

	bool AppLayer::OnMouseButtonReleased(Hazel::MouseButtonReleasedEvent& event)
	{
		if (event.GetMouseButton() == (int)Hazel::Mouse::ButtonLeft)
		{
			m_ModelingCamera->SetSelected(false);
		}

		return false;
	}

	inline void AppLayer::UpdateViewport(int index)
	{
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ImGuiWindow* ViewportWindow = GImGui->CurrentWindow;
		ImVec2 max = ViewportWindow->ContentRegionRect.Max;
		m_Viewports[index].x1 = max.x - viewportPanelSize.x;
		m_Viewports[index].y1 = max.y - viewportPanelSize.y;
		m_Viewports[index].x2 = max.x;
		m_Viewports[index].y2 = max.y;
		m_Viewports[index].width = viewportPanelSize.x;
		m_Viewports[index].height = viewportPanelSize.y;
	}
	
	inline void AppLayer::DrawColorEditor()
	{
		ImGui::Dummy(ImVec2(0.0f, 15.0f));
		if (ImGui::CollapsingHeader("Object Color", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::ColorPicker3("Color", &(m_Scene->m_Object->m_Color[0]), ImGuiColorEditFlags_DefaultOptions_);	
		}
	}
	
	inline void AppLayer::DrawLight()
	{
		ImGui::Dummy(ImVec2(0.0f, 60.0f));
		if (ImGui::CollapsingHeader("Light Position", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("\t\tLocation\t");
			ImGui::SameLine();
			if (ImGui::Button("reset", ImVec2(40, 25))) m_Scene->m_LightSource->ResetLoc();
			ImGui::Text("\t\t\t\tX");
			ImGui::SameLine();
			ImGui::DragFloat("\t", &m_Scene->m_LightSource->LightPos[0], 0.01f, 0, 0, "%.3f m");
			ImGui::Text("\t\t\t\tY");
			ImGui::SameLine();
			ImGui::DragFloat("\t\t", &m_Scene->m_LightSource->LightPos[1], 0.01f, 0, 0, "%.3f m");
			ImGui::Text("\t\t\t\tZ");
			ImGui::SameLine();
			ImGui::DragFloat("\t\t\t", &m_Scene->m_LightSource->LightPos[2], 0.01f, 0, 0, "%.3f m");
			ImGui::SameLine();
			ImGui::Dummy(ImVec2(0.0f, 20.0f));
		}
	}
	
}