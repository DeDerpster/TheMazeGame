#pragma once

#include "Renderer.h"

#include "Camera.h"
#include "KeyDefinitions.h"
#include "Level.h"

class Tile
{
  protected:
	int    m_SpriteID;
	float  x, y;
	double rotation;
	bool   m_IsSolid;
	Level *m_Level;

  public:
	Tile();
	Tile(float x, float y, double rotation, int texID, bool isSolid, Level *level);
	virtual ~Tile();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);
#ifdef DEBUG
	virtual void imGuiRender();
#endif

	bool isSolid()
	{
		return m_IsSolid;
	}
};