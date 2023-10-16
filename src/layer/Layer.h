#pragma once

#include <vector>

#include "ShaderEffectsManager.h"
#include "event/Event.h"
#include "rendering/Renderer.h"
#include "rendering/glInterface/Shader.h"

#include <memory>

class Layer
{
  protected:
	// Stores the current effect ids that are used when rendering
	std::vector<uint16_t> m_ShaderEffectsIDs;

  public:
	Layer();
	virtual ~Layer();

	virtual void render() = 0;
	virtual void update() = 0;
	virtual bool eventCallback(const Event::Event &e);

	std::vector<uint16_t> &getShaderEffects() { return m_ShaderEffectsIDs; }

#ifdef DEBUG
	virtual void imGuiRender();
#endif
};