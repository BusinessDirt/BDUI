#include "mxpch.hpp"
#include "Platform/Windows/WindowsWindow.hpp"
#ifdef OPAL_PLATFORM_WINDOWS

#include "Mixture/Events/ApplicationEvent.hpp"
#include "Mixture/Events/KeyEvent.hpp"
#include "Mixture/Events/MouseEvent.hpp"
#include "Mixture/Core/Application.hpp"

#include <vulkan/vulkan_win32.h>

namespace Mixture 
{

    WindowsWindow::WindowsWindow(const WindowProps& props) 
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow() 
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props) 
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowsWindow::WndProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"MixtureWindowClass";

        ATOM regClass = RegisterClass(&wc);
        OPAL_CORE_ASSERT(regClass, "Failed to register window class!");

        m_WindowHandle = CreateWindowEx(
            0, L"MixtureWindowClass", std::wstring(m_Data.Title.begin(), m_Data.Title.end()).c_str(),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Data.Width, m_Data.Height,
            nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

        OPAL_CORE_ASSERT(m_WindowHandle, "Failed to create window!");

        SetWindowLongPtr(m_WindowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        ShowWindow(m_WindowHandle, SW_SHOW);
    }

    void WindowsWindow::Shutdown() 
    {
        if (m_WindowHandle) 
        {
            DestroyWindow(m_WindowHandle);
            m_WindowHandle = nullptr;
        }
    }

    LRESULT CALLBACK WindowsWindow::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        // Retrieve the user data (WindowsWindow pointer) associated with this HWND
        WindowsWindow* window = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if (!window || !window->m_EventCallback) return DefWindowProc(hwnd, uMsg, wParam, lParam);
        
        switch (uMsg)
        {
            case WM_SIZE:
            {
                window->m_Data.Width = LOWORD(lParam);
                window->m_Data.Height = HIWORD(lParam);
                WindowResizeEvent e(window->m_Data.Width, window->m_Data.Height, false);
                window->m_EventCallback(e);
                break;
            }
            case WM_EXITSIZEMOVE:
            {
                WindowResizeEvent e(window->m_Data.Width, window->m_Data.Height, true);
                window->m_EventCallback(e);
                break;
            }
            case WM_CLOSE:
            {
                WindowCloseEvent e;
                window->m_EventCallback(e);
                break;
            }
            default:
            {
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }

        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    void WindowsWindow::OnUpdate()
    {
        MSG msg = {};
        while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            switch (msg.message)
            {
                case WM_KEYDOWN:
                {
                    KeyPressedEvent e(static_cast<int>(msg.wParam), (msg.lParam & 0x40000000) ? 1 : 0); // Check for repeat count
                    m_EventCallback(e);
                    break;
                }
                case WM_KEYUP:
                {
                    KeyReleasedEvent e(static_cast<int>(msg.wParam));
                    m_EventCallback(e);
                    break;
                }
                case WM_MOUSEMOVE:
                {
                    int x = (int)(short)LOWORD(msg.lParam);
                    int y = (int)(short)HIWORD(msg.lParam);
                    MouseMovedEvent e(static_cast<float>(x), static_cast<float>(y));
                    m_EventCallback(e);
                    break;
                }
                case WM_LBUTTONDOWN:
                {
                    MouseButtonPressedEvent e(VK_LBUTTON);
                    m_EventCallback(e);
                    break;
                }
                case WM_RBUTTONDOWN:
                {
                    MouseButtonPressedEvent e(VK_RBUTTON);
                    m_EventCallback(e);
                    break;
                }
                case WM_LBUTTONUP:
                {
                    MouseButtonReleasedEvent e(VK_LBUTTON);
                    m_EventCallback(e);
                    break;
                }
                case WM_RBUTTONUP:
                {
                    MouseButtonReleasedEvent e(VK_RBUTTON);
                    m_EventCallback(e);
                    break;
                }
                case WM_MOUSEWHEEL:
                {
                    float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(msg.wParam)) / WHEEL_DELTA;
                    MouseScrolledEvent e(0.0f, delta); // Vertical scrolling
                    m_EventCallback(e);
                    break;
                }
                case WM_MOUSEHWHEEL:
                {
                    float delta = static_cast<float>(GET_WHEEL_DELTA_WPARAM(msg.wParam)) / WHEEL_DELTA;
                    MouseScrolledEvent e(delta, 0.0f); // Horizontal scrolling
                    m_EventCallback(e);
                    break;
                }
                default:
                    // Handle other messages or pass them to the default window procedure
                    break;
            }
        }
    }

    void WindowsWindow::GetFramebufferSize(int* width, int* height) const
    {
        *width = GetWidth();
        *height = GetHeight();
    }

    VkSurfaceKHR WindowsWindow::CreateVulkanSurface(VkInstance instance) const
    {
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = m_WindowHandle;
        createInfo.hinstance = GetModuleHandle(nullptr);

        VkSurfaceKHR surface;
        if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS) 
        {
            OPAL_CORE_ERROR("Failed to create Vulkan surface for Windows!");
        }

        return surface;
    }

}

#endif
