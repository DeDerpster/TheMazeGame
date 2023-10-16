#pragma once

namespace Event
{
	// Event initialisation
	void init();

	// Stores all the different types of events
	enum class EventType : uint8_t
	{
		KeyInput,
		Scroll,
		MouseClicked,
		WindowResize,
		MazeMoved,
		ShowAltTile,
		ItemTransfer,
		ChangeGUIMenu,
		ChestOpened,
		PlayerResponse,
		MobDied,
		ApplyShaderEffect,
		RemoveShaderEffect,
	};

	// The action of a key being pressed
	enum class Action : uint8_t
	{
		Release = 0,
		Press,
		Repeat
	};

	// The modifier that can happen with an event
	enum class KeyboardMod : uint8_t
	{
		Shift           = 1,
		Control         = 2,
		Alt             = 4,
		ControlShift    = 3,
		AltShift        = 5,
		ControlAlt      = 6,
		ControlAltShift = 7,
	};

	// The type of call type (where the event reaches)
	enum class CallType : uint8_t
	{
		Normal,
		Overlay,
		All,
	};

	// Base struct for the event
	struct Event
	{
		virtual EventType const getType() const        = 0;
		virtual bool            ignoreIfPaused() const = 0;
	};

}   // namespace Event
