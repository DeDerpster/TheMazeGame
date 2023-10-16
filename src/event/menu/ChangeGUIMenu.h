#pragma once

#include "../Event.h"

#include "Utils.h"

namespace Event
{
	struct ChangeGUIActiveLayerEvent : Event
	{
		InGameGUILayer layer;

		ChangeGUIActiveLayerEvent(InGameGUILayer layer)
			: layer(layer)
		{
		}
		virtual EventType const getType() const override { return EventType::ChangeGUILayer; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}   // namespace Event