#include "mxpch.hpp"
#include "Platform/Windows/WindowsWindow.hpp"
#ifdef OPAL_PLATFORM_WINDOWS

#include <stdexcept>

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
        wc.lpfnWndProc = DefWindowProc;
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"OnyxWindowClass";

        if (!RegisterClass(&wc)) 
        {
            throw std::runtime_error("Failed to register window class!");
        }

        m_WindowHandle = CreateWindowEx(
            0, L"OnyxWindowClass", std::wstring(m_Data.Title.begin(), m_Data.Title.end()).c_str(),
            WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_Data.Width, m_Data.Height,
            nullptr, nullptr, GetModuleHandle(nullptr), nullptr);

        if (!m_WindowHandle) 
        {
            throw std::runtime_error("Failed to create window!");
        }

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

    void WindowsWindow::OnUpdate()
    {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

}

#endif
