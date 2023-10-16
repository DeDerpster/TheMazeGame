#pragma once

#include "Renderer.h"

#include "Camera.h"
#include "KeyDefinitions.h"

class Level;   // FIXME
class Tile
{
  protected:
	Sprite::ID m_SpriteID;
	float  x, y;
	double rotation;
	bool   m_IsSolid;
	Level *m_Level;

  public:
	Tile();
	Tile(float x, float y, double rotation, Sprite::ID id, bool isSolid, Level *level);
	virtual ~Tile();

	virtual void render();
	virtual void update();
	virtual bool eventCallback(const Event::Event &e);
#ifdef DEBUG
	virtual void imGuiRender();
#endif

	virtual bool isSolid()
	{
		return m_IsSolid;
	}
};