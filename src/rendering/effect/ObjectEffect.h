#pragma once

#include "ParticleSpawner.h"
#include "RenderEffect.h"

namespace Effect
{
	// These are the effects which transfer the ownership of an object between the layers, for example
	class ObjectEffect : public RenderEffect
	{
	  public:
		ObjectEffect() {}
		virtual ~ObjectEffect() override {}

		virtual EffectType getType() const = 0;
	};

	class ParticleSpawnerObject : public ObjectEffect
	{
	  protected:
		ParticleSpawner *m_Spawner;
		ParticleSpawnerObject() {}

	  public:
		ParticleSpawnerObject(ParticleSpawner *s)
			: m_Spawner(s)
		{
		}
		virtual ~ParticleSpawnerObject() override
		{
			if(!m_Spawner)
				delete m_Spawner;
		}

		ParticleSpawner *  getObject() { return m_Spawner; }
		void               setObjectToNull() { m_Spawner = nullptr; }
		virtual EffectType getType() const override { return EffectType::objectParticleSpawner; }
	};
}   // namespace Effect