#pragma once

#include "Layer.h"

#include <memory>

#include "Level.h"
#include "MenuObject.h"

class GUILayer : public Layer
{
  private:
	Level *                   m_ConnectedLevel;
	std::vector<MenuObject *> m_Objects;

  public:
	GUILayer();
	GUILayer(Level *connectedLevel);
	virtual ~GUILayer();

	virtual void render() override;
	virtual void update() override;
	virtual bool eventCallback(const Event::Event &e) override;

	void addMenuObject(MenuObject *object);

	void setConnectedLevel(Level *level);

	std::vector<MenuObject *> &getObjects() { return m_Objects; }
	Level *                    getConnectedLevel() { return m_ConnectedLevel; }

#ifdef DEBUG
	virtual void imGuiRender() override
	{
	}
#endif
};