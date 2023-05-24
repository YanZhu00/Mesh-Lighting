#include "hzpch.h"
#include "Hazel/Renderer/Framebuffer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Hazel {

	std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification& spec)
	{
		 return std::make_shared<OpenGLFramebuffer>(spec);
	}

}