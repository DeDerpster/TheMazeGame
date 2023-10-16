#pragma once

#include "../Event.h"

namespace Event
{
	// Event for changing the current menu shown in the stack
	struct ChangeGUIMenuEvent : Event
	{
		enum class Menu : uint8_t
		{   // Different menus while playing the game
			Overlay = 0,
			PlayerInventory,
			ChestInventory,
			NPCInventory,
			NPCInteraction,
			ExitMenu,
			PlayerDeath,
			PlayerWin
		};

		Menu menu;

		ChangeGUIMenuEvent(Menu menu)
			: menu(menu)
		{
		}

		virtual EventType const getType() const override { return EventType::ChangeGUIMenu; }
		virtual bool            ignoreIfPaused() const override { return false; }
	};
}   // namespace Event