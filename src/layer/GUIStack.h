#pragma once

#include "Layer.h"

#include <vector>

#include "GUILayer.h"
#include "layer/level/Level.h"

class GUIStack : public Layer
{
  private:
	// Stores the active layer
	int m_ActiveLayer;

	// Stores the gui layers in the stack
	std::vector<GUILayer *> m_Layers;

  public:
	GUIStack(Level *level);
	virtual ~GUIStack() override;

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;
};