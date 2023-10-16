#pragma once

#include "../Event.h"

namespace Event
{
	// Event for locking a room and showing the alternative tiles of switch tiles
	struct ShowAltTileEvent : Event
	{
		bool showAlt;

		ShowAltTileEvent(bool showAlt)
			: showAlt(showAlt) {}
		virtual EventType const getType() const override { return EventType::ShowAltTile; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}