#pragma once

#include "Hazel.h"
#include "Scene/Scene.h"
#include "Camera/ModelingCamera.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>


namespace Hazel {

	struct ViewportData
	{
		float x1;
		float y1;
		float x2;
		float y2;
		float width;
		float height;
	};

	class AppLayer : public Hazel::Layer
	{
	public:
		AppLayer();
		virtual ~AppLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(float timestep) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& event) override;

		std::shared_ptr<Scene> m_Scene;

		std::shared_ptr<ModelingCamera> m_ModelingCamera;

		std::vector<ViewportData> m_Viewports;

		std::shared_ptr<Framebuffer> m_Framebuffer;

		inline void UpdateViewport(int index);

		int m_Selected;

	private:
		bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleased(MouseButtonReleasedEvent& event);
		bool OnMouseScrolled(Hazel::MouseScrolledEvent& event);

		inline void DrawColorEditor();
		inline void DrawLight();
	};

}
