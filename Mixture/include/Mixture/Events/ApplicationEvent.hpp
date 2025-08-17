#pragma once

#include "Mixture/Events/Event.hpp"

namespace Mixture 
{
    class FramebufferResizeEvent final : public Event
    {
    public:
        FramebufferResizeEvent(const unsigned int width, const unsigned int height) : m_Width(width), m_Height(height) {}

        OPAL_NODISCARD unsigned int GetWidth() const { return m_Width; }
        OPAL_NODISCARD unsigned int GetHeight() const { return m_Height; }

        OPAL_NODISCARD std::string ToString() const override
        {
            std::stringstream ss;
            ss << "FramebufferResizeEvent: " << m_Width << ", " << m_Height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(FramebufferResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

    private:
        unsigned int m_Width, m_Height;
    };

	class WindowResizeEvent final : public Event
    {
	public:
		WindowResizeEvent(const unsigned int width, const unsigned int height) : m_Width(width), m_Height(height) {}

		OPAL_NODISCARD unsigned int GetWidth() const { return m_Width; }
		OPAL_NODISCARD unsigned int GetHeight() const { return m_Height; }

		OPAL_NODISCARD std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)

	private:
		unsigned int m_Width, m_Height;
	};

	class WindowCloseEvent final : public Event 
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppTickEvent final : public Event 
	{
	public:
		AppTickEvent() = default;

		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppUpdateEvent final : public Event 
	{
		AppUpdateEvent() = default;

		EVENT_CLASS_TYPE(AppUpdate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class AppRenderEvent final : public Event 
	{
		AppRenderEvent() = default;

		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}
