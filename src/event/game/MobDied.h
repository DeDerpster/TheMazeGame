#pragma once

#include "../Event.h"

#include "entity/movableEntity/mob/Mob.h"

namespace Event
{
	struct MobDiedEvent : Event
	{
		const Mob *mob;

		MobDiedEvent(const Mob *mob)
			: mob(mob) {}

		virtual EventType const getType() const override { return EventType::MobDied; }
		virtual bool            ignoreIfPaused() const override { return true; }
	};
}