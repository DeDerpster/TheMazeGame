#pragma once

#include "../Event.h"

#include "Container.h"

namespace Event
{
	// Event for when an item is starting to be transferred  (used to pass an item to the TransferObject class)
	struct ItemTransferEvent : Event
	{
		uint16_t    index;       // Stores the index of the item
		IContainer *container;   // Stores the container of the item

		ItemTransferEvent(uint16_t index, IContainer *container)
			: index(index), container(container) {}
		virtual EventType const getType() const override { return EventType::ItemTransfer; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}