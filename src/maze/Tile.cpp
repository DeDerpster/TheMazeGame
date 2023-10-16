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
	// TODO: Make this call a function to get rid of the texture if no one is using it
}

void Tile::render(float secX, float secY)
{
	float boardX = secX + ((float) x) * TILE_SIZE;   // These are the coordinates relative to the board, not the screen! - that is calculated in the shader
	float boardY = secY + ((float) y) * TILE_SIZE;
	if(Application::isInFrame(boardX, boardY))
		Render::Sprite::getSprite(m_SpriteID)->render(boardX, boardY, rotation, TILE_SIZE);
}

void Tile::update()
{
	//rotation += 0.01f;
}

#ifdef DEBUG
void Tile::imGuiRender()
{
	//ImGui::SliderFloat("Rotation", (float *) &rotation, 0.0f, 360.f);
}
#endif