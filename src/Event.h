#pragma once

#include "Utils.h"

namespace Application
{
	void eventInit();
	bool isKeyPressed(int key);
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

	enum EventType
	{
		keyInput,
		scroll,
		mouseMove,
		mouseClicked,
		windowResize,
	};

	struct Event
	{
		virtual const EventType getType() const = 0;
	};

	struct KeyboardEvent : Event
	{
		int key, scancode, action, mods;

		KeyboardEvent(int key, int scancode, int action, int mods)
			: key(key), scancode(scancode), action(action), mods(mods) {}
		virtual EventType const getType() const override { return EventType::keyInput; }
	};

	struct ScrollEvent : Event
	{
		double xoffset, yoffset;

		ScrollEvent(double xoffset, double yoffset)
			: xoffset(xoffset), yoffset(yoffset) {}
		virtual EventType const getType() const override { return EventType::scroll; }
	};

	struct WindowResizeEvent : Event
	{
		int oWidth, oHeight;
		int width, height;

		WindowResizeEvent(int originalWidth, int originalHeight, int newWidth, int newHeight)
			: oWidth(originalWidth), oHeight(originalHeight), width(newWidth), height(newHeight) {}
		virtual EventType const getType() const override { return EventType::windowResize; }
	};

	struct MouseClickedEvent : Event
	{
		MouseButton button;
		Vec2f       pos;

		MouseClickedEvent(MouseButton button, Vec2f pos)
			: button(button), pos(pos) {}
		virtual EventType const getType() const override { return EventType::mouseClicked; }
	};
}   // namespace Application
