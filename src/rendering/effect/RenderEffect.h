#pragma once

#include <string>

namespace Effect
{
	// This stores all the effect types there could be
	enum EffectType
	{
		shaderEffect,
		removeShaderEffect,
		objectParticleSpawner
	};

	// TODO: Change this name its terrible
	class RenderEffect
	{
	  public:
		RenderEffect()
		{
		}
		virtual ~RenderEffect() {}

		virtual EffectType getType() const = 0;
	};

}   // namespace Effect