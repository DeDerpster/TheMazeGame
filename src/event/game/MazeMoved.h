#pragma once

#include "../Event.h"

namespace Event
{
	// Event for when a maze has moved
	struct MazeMovedEvent : Event
	{
		// Stores the change in x and y
		float changeX, changeY;

		MazeMovedEvent(float changeX, float changeY)
			: changeX(changeX), changeY(changeY) {}
		virtual EventType const getType() const override { return EventType::MazeMoved; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}