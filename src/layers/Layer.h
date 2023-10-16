#pragma once

#include <vector>

#include "Event.h"
#include "Renderer.h"
#include "Shader.h"
#include "ShaderEffectsManager.h"

#include <memory>

class Layer
{
  protected:
	std::vector<uint16_t> m_ShaderEffectsIDs;

  public:
	Layer();
	virtual ~Layer();

	virtual void render()                                   = 0;
	virtual void update()                                   = 0;
	virtual bool eventCallback(const Event::Event &e)       = 0;

	virtual bool setEffect(Effect::Effect *e);

	std::vector<uint16_t> &getShaderEffects() { return m_ShaderEffectsIDs; }

#ifdef DEBUG
	virtual void imGuiRender();
#endif
};