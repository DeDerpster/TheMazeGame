#pragma once

namespace Event
{
	void init();

	enum class EventType
	{
		KeyInput,
		Scroll,
		MouseClicked,
		WindowResize,
		MazeMoved,
		ShowAltTile,
		ItemTransfer,
		ChangeGUILayer,
		ChestOpened,
		PlayerResponse,
		MobDied,
		ApplyShaderEffect,
		RemoveShaderEffect,
	};

	enum class Action
	{
		Release = 0,
		Press,
		Repeat
	};

	enum class KeyboardMod
	{
		Shift           = 1,
		Control         = 2,
		Alt             = 4,
		ControlShift    = 3,
		AltShift        = 5,
		ControlAlt      = 6,
		ControlAltShift = 7,
	};

	enum class CallType
	{
		Normal,
		Overlay,
		All,
	};

	struct Event
	{
		virtual EventType const getType() const        = 0;
		virtual bool            ignoreIfPaused() const = 0;
	};

}   // namespace Event
