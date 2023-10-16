#pragma once

#include "Renderer.h"

#include "Camera.h"

class Tile
{
  private:
	int    m_TexID;
	int    x, y;
	double rotation;
	bool   m_IsSolid;

  public:
	static const int TILE_SIZE = 100;

	Tile(int x, int y, double rotation, int texID, bool isSolid);
	~Tile();
	void render(float secX, float secY);
	void update();
	bool isSolid() { return m_IsSolid; }
#ifdef DEBUG
	void imGuiRender();
#endif
};