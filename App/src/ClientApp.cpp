#include <Hazel.h>
#include "Hazel/Core/EntryPoint.h"

#include "AppLayer.h"

namespace Hazel {

	class ClientApp : public Hazel::Application
	{
	public:
		ClientApp()
		{
			PushOverlay(new AppLayer());
		}

		~ClientApp()
		{

		}

	};

	Hazel::Application* Hazel::CreateApplication()
	{
		return new ClientApp();
	}

}