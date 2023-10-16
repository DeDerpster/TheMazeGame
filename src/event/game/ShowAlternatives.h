#pragma once

#include "../Event.h"

namespace Event
{
	struct ShowAltTileEvent : Event
	{
		bool showAlt;

		ShowAltTileEvent(bool showAlt)
			: showAlt(showAlt) {}
		virtual EventType const getType() const override { return EventType::ShowAltTile; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}