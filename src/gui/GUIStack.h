#pragma once

#include "Layer.h"

#include <vector>

#include "GUILayer.h"
#include "Level.h"

class GUIStack : public Layer
{
  public:
  private:
	int m_ActiveLayer;

	std::vector<GUILayer *> m_Layers;

  public:
	GUIStack(Level *level);
	virtual ~GUIStack() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
};