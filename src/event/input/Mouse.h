#pragma once

#include "../Event.h"

#include "2dVec.h"

namespace Event
{
	Vec2f getMousePos();

	// Stores all the possible mouse buttons that are supported
	enum class MouseButton : uint8_t
	{
		LeftButton = 0,
		RightButton,
		MiddleButton,
		Button4,
		Button5,
		Button6,
		Button7,
		Button8,
	};

	// Event for when mouse button changes state
	struct MouseClickedEvent : Event
	{
		MouseButton button;   // Stores the button that changed action
		Vec2f       pos;      // Stores the position of the mouse
		Action      action;   // Stores the action of the button
		KeyboardMod mods;     // Stores the modifiers

		MouseClickedEvent(MouseButton button, Vec2f pos, Action action, KeyboardMod mods)
			: button(button), pos(pos), action(action), mods(mods) {}

		virtual EventType const getType() const override { return EventType::MouseClicked; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};

	// Event for scroll wheel changes
	struct ScrollEvent : Event
	{
		// Stores the x and y offsets
		double xoffset, yoffset;

		ScrollEvent(double xoffset, double yoffset)
			: xoffset(xoffset), yoffset(yoffset) {}
		virtual EventType const getType() const override { return EventType::Scroll; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}   // namespace Event