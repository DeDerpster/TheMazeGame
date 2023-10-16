#include "Tile.h"

#include "ImGui.h"

#include "Application.h"
#include "Log.h"
#include "Sprite.h"

Tile::Tile(int x, int y, double rotation, int texID, bool isSolid)
	: m_SpriteID(texID), x(x), y(y), rotation(rotation), m_IsSolid(isSolid)
{
}

Tile::~Tile()
{
}

void Tile::render(float secX, float secY)
{
	float boardX = secX + ((float) x) * TILE_SIZE;   // These are the coordinates relative to the board, not the screen! - that is calculated in the shader
	float boardY = secY + ((float) y) * TILE_SIZE;
	Render::sprite(boardX, boardY, rotation, TILE_SIZE, m_SpriteID);
}

void Tile::update()
{
}

#ifdef DEBUG
void Tile::imGuiRender()
{
}
#endif