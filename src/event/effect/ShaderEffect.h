#pragma once

#include "../Event.h"

namespace Event
{
	// These are for all the different shader effects that could be applied
	struct ApplyShaderEffectEvent : Event
	{
		uint16_t id;

		ApplyShaderEffectEvent(uint16_t id)
			: id(id)
		{
		}

		virtual EventType const getType() const override { return EventType::ApplyShaderEffect; }
		virtual bool            ignoreIfPaused() const override { return true; };
	};

	// This is for removing an effect from a layer
	struct RemoveShaderEffectEvent : ApplyShaderEffectEvent
	{
		RemoveShaderEffectEvent(uint16_t id)
			: ApplyShaderEffectEvent(id)
		{
		}

		virtual EventType const getType() const override { return EventType::RemoveShaderEffect; }
		virtual bool            ignoreIfPaused() const override { return true; };
	};
}   // namespace Event
