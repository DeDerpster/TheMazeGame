#pragma once

#include "Layer.h"

class GUILayer : public Layer
{
  public:
	GUILayer();

	~GUILayer() {}

	virtual void render() override;

	virtual void update() override {}

	virtual bool eventCallback(const Event::Event &e) override { return false; }

#ifdef DEBUG
	virtual void imGuiRender() override
	{
	}
#endif
};