#include "mxpch.hpp"
#include "Mixture/Core/Window.hpp"

#include "Mixture/Core/Base.hpp"

#include "Mixture/Events/ApplicationEvent.hpp"
#include "Mixture/Events/KeyEvent.hpp"
#include "Mixture/Events/MouseEvent.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace Mixture
{
    static void GLFWErrorCallback(int error, const char* description)
    {
        OPAL_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    Window::Window(const WindowProps& props)
    {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		glfwInitVulkanLoader(vkGetInstanceProcAddr);

		{
			OPAL_CORE_INFO("Initializing GLFW");
			int success = glfwInit();
			OPAL_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
			m_WindowHandle = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}

		glfwSetWindowUserPointer(m_WindowHandle, &m_Data);

		// CALLBACKS
		{
			glfwSetWindowSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					data.Width = width;
					data.Height = height;

					if (width == 0 || height == 0) data.Minimized = true;

					WindowResizeEvent event(width, height);
					data.EventCallback(event);
				});
            
            glfwSetFramebufferSizeCallback(m_WindowHandle, [](GLFWwindow* window, int width, int height)
                {
                    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                    data.Width = width;
                    data.Height = height;

                    if (width == 0 || height == 0) data.Minimized = true;

                    FramebufferResizeEvent event(width, height);
                    data.EventCallback(event);
                });

			glfwSetWindowCloseCallback(m_WindowHandle, [](GLFWwindow* window)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
					WindowCloseEvent event;
					data.EventCallback(event);
				});

			glfwSetKeyCallback(m_WindowHandle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						KeyPressedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{
						KeyPressedEvent event(key, true);
						data.EventCallback(event);
						break;
					}
					}
				});

			glfwSetCharCallback(m_WindowHandle, [](GLFWwindow* window, unsigned int keycode)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					KeyTypedEvent event(keycode);
					data.EventCallback(event);
				});

			glfwSetMouseButtonCallback(m_WindowHandle, [](GLFWwindow* window, int button, int action, int mods)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					switch (action)
					{
					case GLFW_PRESS:
					{
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
					}
				});

			glfwSetScrollCallback(m_WindowHandle, [](GLFWwindow* window, double xOffset, double yOffset)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseScrolledEvent event((float)xOffset, (float)yOffset);
					data.EventCallback(event);
				});

			glfwSetCursorPosCallback(m_WindowHandle, [](GLFWwindow* window, double xPos, double yPos)
				{
					WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

					MouseMovedEvent event((float)xPos, (float)yPos);
					data.EventCallback(event);
				});
		}
    }

    Window::~Window()
    {
		glfwDestroyWindow(m_WindowHandle);
		glfwTerminate();
    }

    void Window::OnUpdate() const
    {
		glfwPollEvents();
    }

    void Window::GetFramebufferSize(int* width, int* height) const
    {
		glfwGetFramebufferSize(m_WindowHandle, width, height);
    }

    void Window::CreateVulkanSurface(VkInstance instance, VkAllocationCallbacks* allocator, VkSurfaceKHR* surface) const
    {
		VkResult res = glfwCreateWindowSurface(instance, m_WindowHandle, allocator, surface);
		OPAL_CORE_ASSERT(res == VK_SUCCESS,"Failed to create VkSurfaceKHR!");
    }

} // namespace Onyx
