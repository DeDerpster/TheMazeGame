#pragma once

#include "../Event.h"

#include "Utils.h"

namespace Event
{
	Vec2f getMousePos();

	enum MouseButton
	{
		leftButton = 0,
		rightButton,
		middleButton,
		button4,
		button5,
		button6,
		button7,
		button8,
	};

	struct MouseClickedEvent : Event
	{
		MouseButton button;
		Vec2f       pos;
		Action      action;
		KeyboardMod mods;

		MouseClickedEvent(MouseButton button, Vec2f pos, Action action, KeyboardMod mods)
			: button(button), pos(pos), action(action), mods(mods) {}

		virtual EventType const getType() const override { return EventType::MouseClicked; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};

	struct ScrollEvent : Event
	{
		double xoffset, yoffset;

		ScrollEvent(double xoffset, double yoffset)
			: xoffset(xoffset), yoffset(yoffset) {}
		virtual EventType const getType() const override { return EventType::Scroll; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}   // namespace Event