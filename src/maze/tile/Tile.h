#pragma once

#include "rendering/Renderer.h"

#include "Camera.h"
#include "KeyDefinitions.h"

#include "CollisionBox.h"

class Level;

class Tile
{
  protected:
	Sprite::ID m_SpriteID;   // Stores the sprite ID of the tile
	float      x, y;         // Stores the position of the tile
	double     rotation;     // Stores the rotation of the tile
	bool       m_IsSolid;    // Stores whether it is a solid tile or not
	Level *    m_Level;      // Stores the level it is in

	// This is just an easy wait to get the collision box of a tile
	static CollisionBox getCollisionBox();

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

	virtual bool const isSolid() const
	{
		return m_IsSolid;
	}
};