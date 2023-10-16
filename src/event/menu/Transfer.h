#pragma once

#include "../Event.h"

#include "Container.h"

namespace Event
{
	struct ItemTransferEvent : Event
	{
		uint16_t    index;
		IContainer *container;

		ItemTransferEvent(uint16_t index, IContainer *container)
			: index(index), container(container) {}
		virtual EventType const getType() const override { return EventType::ItemTransfer; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}