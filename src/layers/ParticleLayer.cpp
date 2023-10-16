#include "ParticleLayer.h"

#include "ObjectEffect.h"
#include "VertexBufferLayout.h"

ParticleLayer::ParticleLayer()
{
}

ParticleLayer::~ParticleLayer()
{
	for(ParticleSpawner *s : m_Spawners)
		delete s;
}

void ParticleLayer::render()
{
	for(ParticleSpawner *s : m_Spawners)
		s->render();
}

void ParticleLayer::update()
{
	for(ParticleSpawner *s : m_Spawners)
		s->update();
}

bool ParticleLayer::eventCallback(const Event::Event &e)
{
	for(ParticleSpawner *s : m_Spawners)
	{
		if(s->eventCallback(e))
			return true;
	}
	return false;
}

bool ParticleLayer::setEffect(Effect::RenderEffect *e)
{
	if(e->getType() == Effect::EffectType::objectParticleSpawner)
	{
		Effect::ParticleSpawnerObject *ne = static_cast<Effect::ParticleSpawnerObject *>(e);

		ParticleSpawner *s = ne->getObject();
		m_Spawners.push_back(s);
		ne->setObjectToNull();
		return true;
	}
	return Layer::setEffect(e);
}