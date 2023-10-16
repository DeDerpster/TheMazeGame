#pragma once

#include "Layer.h"
#include "ParticleSpawner.h"

#include "vector"

// TODO: Wipe this from existence
class ParticleLayer : public Layer
{
  private:
	std::vector<ParticleSpawner *> m_Spawners;

  public:
	ParticleLayer();
	virtual ~ParticleLayer();

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
	virtual bool setEffect(Effect::RenderEffect *e) override;

#ifdef DEBUG
	virtual void imGuiRender() override
	{
	}
#endif
};