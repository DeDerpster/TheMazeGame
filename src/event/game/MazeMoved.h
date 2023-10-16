#pragma once

#include "../Event.h"

namespace Event
{
	struct MazeMovedEvent : Event
	{
		float changeX, changeY;

		MazeMovedEvent(float changeX, float changeY)
			: changeX(changeX), changeY(changeY) {}
		virtual EventType const getType() const override { return EventType::MazeMoved; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}