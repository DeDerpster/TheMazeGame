#pragma once

#include "../Event.h"

#include "Container.h"

namespace Event
{
	// Event for when a chest is opened used to transfer a pointer off its inventory over to the menu for handling that
	struct ChestOpenedEvent : Event
	{
		// Stores the container of the chest
		IContainer *       container;
		int *              activeItem;   // Stores its active item

		ChestOpenedEvent(IContainer *container, int *activeItem)
			: container(container), activeItem(activeItem) {}

		virtual EventType const getType() const override { return EventType::ChestOpened; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}