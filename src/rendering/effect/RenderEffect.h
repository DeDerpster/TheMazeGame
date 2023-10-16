#pragma once

#include <string>

namespace Effect
{
	enum EffectType
	{
		shaderEffect,
		removeShaderEffect,
		objectParticleSpawner
	};

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