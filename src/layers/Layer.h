#pragma once

#include "Event.h"
#include "RenderEffect.h"
#include "Renderer.h"

#include <memory>

class Layer
{
  protected:
	std::unique_ptr<Shader> m_Shader;

  public:
	Layer() {}
	virtual ~Layer() {}
	virtual void render()                                   = 0;
	virtual void update()                                   = 0;
	virtual bool eventCallback(const Application::Event &e) = 0;
	virtual bool setEffect(const Effect::RenderEffect &e)   = 0;

	virtual void updateMVP(glm::mat4 &view) {};
#ifdef DEBUG
	virtual void imGuiRender() = 0;
#endif
};