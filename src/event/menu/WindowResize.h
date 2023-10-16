#pragma once

#include "../Event.h"

namespace Event
{
	struct WindowResizeEvent : Event
	{
		int oWidth, oHeight;
		int width, height;

		WindowResizeEvent(int originalWidth, int originalHeight, int newWidth, int newHeight)
			: oWidth(originalWidth), oHeight(originalHeight), width(newWidth), height(newHeight) {}
		virtual EventType const getType() const override { return EventType::WindowResize; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}