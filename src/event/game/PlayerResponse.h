#pragma once

#include "event/Event.h"

namespace Event
{
	struct PlayerResponseEvent : Event
	{
		enum Response
		{
			reject,
			accept
		};

		Response response;

		PlayerResponseEvent(Response response)
			: response(response) {}

		virtual EventType const getType() const override { return EventType::PlayerResponse; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}