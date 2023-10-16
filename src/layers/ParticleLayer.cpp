#include "ParticleLayer.h"

#include "ObjectEffect.h"
#include "VertexBufferLayout.h"

ParticleLayer::ParticleLayer()
{
	m_Shader = std::make_unique<Shader>("res/shaders/ParticleShader.glsl");

	m_Shader->setUniform4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);

	auto bufferInit = [](VertexBufferLayout &layout) {
		layout.push<float>(2);
	};

	m_Buffer = std::make_unique<Render::SmartBuffer>(3528, bufferInit);

	m_Shader->bind();
}

ParticleLayer::~ParticleLayer() {}

void ParticleLayer::render()
{
	for(ParticleSpawner *s : m_Spawners)
	{
		m_Shader->setUniform4f("u_Colour", s->getColour()->r, s->getColour()->g, s->getColour()->b, s->getColour()->a);

		// Gets the particles from the spawner
		std::vector<Particle> *buffer = s->getParticles();
		if(buffer->size() == 0)   // Checks the buffer is not empty
			continue;

		// Goes through all the objects in the buffer and renders them
		for(Particle obj : *buffer)
		{
			// Checks if the buffer is full or the buffer is too big and draws what there is
			if(!m_Buffer->canStore(4 * sizeof(glm::vec2)))
			{
				m_Buffer->draw();
				m_Buffer->resetBuffer();   // Resets the buffer so it can draw again
			}
			// Adds the current object to the buffer by creating its quad (this is for memory efficiency)
			auto vertices = Render::CreateVecQuad(obj.getX(), obj.getY(), 0.0f, obj.getSize());
			m_Buffer->addToBuffer((void *) &vertices, sizeof(vertices));
		}
	}

	if(!m_Buffer->isEmpty())   // If the buffer is not empty, it empties it
	{
		m_Buffer->draw();
		m_Buffer->resetBuffer();
	}
}

void ParticleLayer::update()
{
	for(ParticleSpawner *s : m_Spawners)
		s->update();
}

bool ParticleLayer::eventCallback(const Application::Event &e)
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
	if(e->getType() == Effect::EffectType::ObjectParticleSpawner)
	{
		Effect::ParticleSpawnerObject *ne = static_cast<Effect::ParticleSpawnerObject *>(e);

		ParticleSpawner *s = ne->getObject();
		m_Spawners.push_back(s);
		ne->setObjectToNull();
		return true;
	}
	return Layer::setEffect(e);
}