#pragma once

#include "event/Event.h"

namespace Event
{
	// Event for when a player responded to whether they want a npc to follower them
	struct PlayerResponseEvent : Event
	{
		enum class Response : uint8_t
		{
			Reject,
			Accept
		};

		Response response;

		PlayerResponseEvent(Response response)
			: response(response) {}

		virtual EventType const getType() const override { return EventType::PlayerResponse; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}