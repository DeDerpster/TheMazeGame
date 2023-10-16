#pragma once

#include "../Event.h"

namespace Event
{
	// Event for when the window changes size or when the GUIScale changes
	struct WindowResizeEvent : Event
	{
		// Stores the original width and height
		int oWidth, oHeight;
		// Stores the current width and height
		int width, height;

		WindowResizeEvent(int originalWidth, int originalHeight, int newWidth, int newHeight)
			: oWidth(originalWidth), oHeight(originalHeight), width(newWidth), height(newHeight) {}
		virtual EventType const getType() const override { return EventType::WindowResize; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}