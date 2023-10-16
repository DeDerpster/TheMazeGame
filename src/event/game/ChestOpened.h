#pragma once

#include "../Event.h"

#include "Container.h"

namespace Event
{
	struct ChestOpenedEvent : Event
	{
		IContainer *       container;
		int *              activeItem;
		GUIInventoryIDCode id;

		ChestOpenedEvent(IContainer *container, int *activeItem, GUIInventoryIDCode id)
			: container(container), activeItem(activeItem), id(id) {}

		virtual EventType const getType() const override { return EventType::ChestOpened; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}