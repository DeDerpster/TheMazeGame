#pragma once


namespace Effect
{
	// This stores all the effect types there could be

	class Effect
	{
	  public:
		enum Type
		{
			shaderEffect,
			removeShaderEffect,
			objectParticleSpawner
		};

	  public:
		Effect()
		{
		}
		virtual ~Effect() {}

		virtual Type getType() const = 0;
	};

}   // namespace Effect